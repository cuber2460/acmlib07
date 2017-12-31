import datetime
import formatter
import math
import os
import pdf_pages
import pygments
import pygments.lexers

from latex_generator import GeneratePdfDocumentForTex

# Returns black or white RGB color of text on the given background color.
def _AntiColor(color):
  if (color[0] + color[1] + color[2] > 1.5):
    return (0, 0, 0)
  else:
    return (1, 1, 1)

LONG_LINE_LETTERS = 0
LONG_LINE_TEX_DOCUMENT = 1

class LongLine(object):
  def __init__(self, line_type, letters=None, tex_filename=None):
    if letters is None:
      letters = []
    self.line_type_ = line_type
    self.letters_ = letters
    self.tex_filename_ = tex_filename

  def Type(self):
    return self.line_type_

  def Letters(self):
    assert self.line_type_ == LONG_LINE_LETTERS
    return self.letters_

  def TexFilename(self):
    assert self.line_type_ == LONG_LINE_TEX_DOCUMENT
    return self.tex_filename_

SHORT_LINE_LETTERS = 0
SHORT_LINE_PDF_PAGE = 1

class ShortLine(object):
  def __init__(self, line_type, height=1, letters=None, pdf_page=None):
    if letters is None:
      letters = []
    self.line_type_ = line_type
    self.height_ = height
    self.letters_ = letters
    self.pdf_page_ = pdf_page

  def Type(self):
    return self.line_type_

  def Height(self):
    return self.height_

  def Letters(self):
    assert self.line_type_ == SHORT_LINE_LETTERS
    return self.letters_

  def PdfPage(self):
    assert self.line_type_ == SHORT_LINE_PDF_PAGE
    return self.pdf_page_


class CodeParser(object):
  """List of options:
       * lang      (eg. 'C++')
       * style     (eg. 'colorful')
       * expandtab (eg. 2)
       * number_of_columns (eg. 2)
       * width             (in millimeters; eg. 279)
       * height            (in millimeters; eg. 216)
       * margin_top, margin_bottom, margin_left, margin_middle, margin_right
                           (in millimeters; eg. 10)
       * header_decorations
       * right_wrap_decorations
       * left_wrap_decorations
  """
  def __init__(self, options):
    self.cols = options['characters_in_a_row']
    self.long_lines = []

  def _GenerateTitleLine(self, title, options):
    style = options['header_decorations']
    title_text = [pdf_pages.Character(c, **style) for c in title]
    space = pdf_pages.Character(' ', **style)
    while len(title_text) < self.cols:
      if len(title_text) % 2 == 0:
        title_text = [space] + title_text
      else:
        title_text = title_text + [space]
    for c in title_text:
      c.SetTitle(title)
    return LongLine(LONG_LINE_LETTERS, letters=title_text)

  def _AddOutputLines(self, title, output, options):
    next_line = LongLine(LONG_LINE_LETTERS)
    for character in output:
      character.SetTitle(title)
      if character.GetCharacter() == '\n':
        self.long_lines.append(next_line)
        next_line = LongLine(LONG_LINE_LETTERS)
      elif character.GetCharacter() == '\r':
        pass
      elif character.GetCharacter() == '\t':
        character.SetCharacter(' ')
        for i in range(options.get('expandtab', 2)):
          next_line.Letters().append(character)
      else:
        next_line.Letters().append(character)
    self.long_lines.append(next_line)
    while self.long_lines and not self.long_lines[-1].Letters():
      del self.long_lines[-1]

  def _GetShortLines(self, options, row_height, column_width):
    right_arrow = pdf_pages.Character('↲', **options['right_wrap_decorations'])
    right_arrows = [right_arrow]
    left_arrow = pdf_pages.Character('↳', **options['left_wrap_decorations'])
    left_arrows = [left_arrow]
    for line in self.long_lines:
      if line.Type() == LONG_LINE_LETTERS:
        l = line.Letters()
        while len(l) > self.cols:
          yield ShortLine(
              SHORT_LINE_LETTERS,
              letters=l[:self.cols - len(right_arrows)] + right_arrows)
          l = left_arrows + l[self.cols - len(right_arrows):]
        yield ShortLine(SHORT_LINE_LETTERS, letters=l)
      elif line.Type() == LONG_LINE_TEX_DOCUMENT:
        latex_options = options['latex_options']
        latex_options['input_tex'] = line.TexFilename()
        latex_options['width'] = column_width
        document = GeneratePdfDocumentForTex(latex_options)
        for page_id in range(document.get_n_pages()):
          page = document.get_page(page_id)
          rect = page.get_crop_box()
          width = rect.x2 - rect.x1
          scale = column_width / width
          height = (rect.y2 - rect.y1) * scale
          cnt_rows = math.ceil(height / row_height)
          yield ShortLine(SHORT_LINE_PDF_PAGE, height=cnt_rows, pdf_page=page)
      else:
        assert False

  def _SplitToColumns(self, options, short_lines, rows_per_column):
    def FillColumn(column, size, rows_per_column):
      while size < rows_per_column:
        column.append(ShortLine(SHORT_LINE_LETTERS))
        size += 1
    size = 0
    next_column = []
    for line in short_lines:
      h = line.Height()
      if h + size > rows_per_column:
        FillColumn(next_column, size, rows_per_column)
        yield next_column
        size = h
        next_column = [line]
      else:
        size += h
        next_column.append(line)
    yield next_column

  def AddSource(self, title, code, options):
    code_lexer = pygments.lexers.get_lexer_by_name(options.get('lang', 'raw'))
    if 'style' in options:
      code_formatter = formatter.CharacterFormatter(style=options['style'])
    else:
      code_formatter = formatter.CharacterFormatter()
    pygments.highlight(code, code_lexer, code_formatter)
    self._AddOutputLines(title, code_formatter.GetOutput(), options)

  def AddCodeFile(self, filename, options):
    title = os.path.relpath(filename, options["code_dir"])
    self.long_lines.append(self._GenerateTitleLine(title, options))
    if options['lang'] == 'LaTeX':
      self.long_lines.append(
          LongLine(LONG_LINE_TEX_DOCUMENT, tex_filename=filename))
    else:
      f = open(filename)
      self.AddSource(title, f.read(), options)

  def GeneratePdf(self, options):
    pages = pdf_pages.PdfPages(options)
    rows_per_column = pages.GetRowsPerColumn()
    number_of_columns = pages.GetNumberOfColumns()
    row_height = pages.GetRowHeight()
    column_width = pages.GetColumnWidth()
    columns = tuple(self._SplitToColumns(options, self._GetShortLines(
        options, row_height, column_width), rows_per_column))
    pages.SetDate(datetime.datetime.today())
    pages.SetTotalNumberOfPages(
        (len(columns) + number_of_columns - 1) // number_of_columns)
    for column in columns:
      for short_line in column:
        if short_line.Type() == SHORT_LINE_LETTERS:
          assert short_line.Height() == 1
          pages.AddLine(short_line.Letters())
        elif short_line.Type() == SHORT_LINE_PDF_PAGE:
          pages.AddPdfPage(short_line.Height(), short_line.PdfPage())
        else:
          assert False

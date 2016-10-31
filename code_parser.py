import datetime
import formatter
import pdf_pages
import pygments
import pygments.lexers

# Returns black or white RGB color of text on the given background color.
def _AntiColor(color):
  if (color[0] + color[1] + color[2] > 1.5):
    return (0, 0, 0)
  else:
    return (1, 1, 1)

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
    self.long_lines = [[]]

  def _GenerateTitleLine(self, title, options):
    style = options['header_decorations']
    title_text = [pdf_pages.Character(c, **style) for c in title]
    space = pdf_pages.Character(' ', **style)
    while len(title_text) < self.cols:
      if len(title_text) % 2 == 0:
        title_text = [space] + title_text
      else:
        title_text = title_text + [space]
    return title_text

  def _AddOutputLines(self, title, output, options):
    if self.long_lines[-1]:
      self.long_lines.append([])
    self.long_lines[-1] = self._GenerateTitleLine(title, options)
    self.long_lines.append([])
    for character in output:
      if character.GetCharacter() == '\n':
        self.long_lines.append([])
      elif character.GetCharacter() == '\r':
        pass
      elif character.GetCharacter() == '\t':
        character.SetCharacter(' ')
        for i in range(options.get('expandtab', 2)):
          self.long_lines[-1].append(character)
      else:
        self.long_lines[-1].append(character)

  def _GetShortLines(self, options):
    long_lines = list(self.long_lines)
    if not long_lines[-1]:
      long_lines = long_lines[:-1]
    right_arrow = pdf_pages.Character('↲', **options['right_wrap_decorations'])
    right_arrows = [right_arrow]
    left_arrow = pdf_pages.Character('↳', **options['left_wrap_decorations'])
    left_arrows = [left_arrow]
    for line in long_lines:
      while len(line) > self.cols:
        yield line[:self.cols - len(right_arrows)] + right_arrows
        line = left_arrows + line[self.cols - len(right_arrows):]
      yield line

  def AddSource(self, title, code, options):
    code_lexer = pygments.lexers.get_lexer_by_name(options.get('lang', 'raw'))
    if 'style' in options:
      code_formatter = formatter.CharacterFormatter(style=options['style'])
    else:
      code_formatter = formatter.CharacterFormatter()
    pygments.highlight(code, code_lexer, code_formatter)
    self._AddOutputLines(title, code_formatter.GetOutput(), options)

  def AddCodeFile(self, filename, options):
    with open(filename) as f:
      self.AddSource(filename, f.read(), options)

  def GeneratePdf(self, options):
    pages = pdf_pages.PdfPages(options)
    rows_per_page = pages.GetRowsPerPage()
    short_lines = tuple(self._GetShortLines(options))
    pages.SetDate(datetime.datetime.today())
    pages.SetTotalNumberOfPages(
        (len(short_lines) + rows_per_page - 1) // rows_per_page)
    for short_line in short_lines:
      pages.AddLine(short_line)

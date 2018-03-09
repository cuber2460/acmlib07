import cairo
import math
import os

def GetAlgorithmFromFilename(filename):
  algorithm = os.path.basename(filename)
  if algorithm.endswith('.cpp'):
    algorithm = algorithm[:-4]
  elif algorithm.endswith('.tex'):
    algorithm = algorithm[:-4]
  return algorithm

def GetNameFromFilename(filename):
  dirname = os.path.dirname(filename)
  if dirname == "":
    return "C"
  elif dirname == "kamil":
    return "K"
  elif dirname == "marek":
    return "M"
  elif dirname == "mateusz":
    return "R"
  elif dirname == "blazej":
    return "B"
  else:
    assert False

def MakeSummaryText(summary_data):
  algorithms = []
  for title in summary_data:
    algorithms.append(GetAlgorithmFromFilename(title))
  return ', '.join(algorithms)

def MakeSummaryLetters(summary_data):
  letters = set()
  for title in summary_data:
    letters.add(GetNameFromFilename(title))
  return letters

class PdfPagesError(Exception): pass

class Character(object):
  def __init__(self, character='', is_bold=False, is_italic=False,
               color=(0, 0, 0, 1), bg_color=(1, 1, 1, 0),
               border_color=(0, 0, 0, 0)):
    self.SetCharacter(character)
    self.SetIsBold(is_bold)
    self.SetIsItalic(is_italic)
    self.SetColor(color)
    self.SetBgColor(bg_color)
    self.SetBorderColor(border_color)
    self.SetTitle(None)

  def __repr__(self):
    return self.character

  def SetCharacter(self, character):
    if len(character) > 1:
      raise PdfPagesError("Character's character must be a string of length 1.")
    self.character = character

  def SetIsBold(self, is_bold):
    self.is_bold = is_bold

  def SetIsItalic(self, is_italic):
    self.is_italic = is_italic

  def SetColor(self, color):
    self.color = color

  def SetBgColor(self, bg_color):
    self.bg_color = bg_color

  def SetBorderColor(self, border_color):
    self.border_color = border_color

  def SetTitle(self, title):
    self.title = title

  def GetCharacter(self):
    return self.character

  def EqualStyle(self, character):
    return self.is_bold == character.is_bold and \
           self.is_italic == character.is_italic and \
           self.color == character.color and \
           self.bg_color == character.bg_color

  def HasTitle(self):
    return self.title is not None

  def GetTitle(self):
    assert self.HasTitle()
    return self.title

  def _CairoFontSlant(self):
    if self.is_italic:
      return cairo.FONT_SLANT_ITALIC
    else:
      return cairo.FONT_SLANT_NORMAL

  def _CairoFontWeight(self):
    if self.is_bold:
      return cairo.FONT_WEIGHT_BOLD
    else:
      return cairo.FONT_WEIGHT_NORMAL

  def ApplyStyleToContext(self, context, fonts):
    context.set_source_rgba(*self.color)
    context.set_font_face(
        fonts[self._CairoFontSlant()][self._CairoFontWeight()])

  def ApplyBackgroundStyleToContext(self, context):
    context.set_source_rgba(*self.bg_color)

  def ApplyBorderStyleToContext(self, context):
    context.set_source_rgba(*self.border_color)

class PdfPages(object):
  POINTS_TO_MILLIMETERS_RATIO = 2.83464567
  FONT_SLANTS = [cairo.FONT_SLANT_NORMAL, cairo.FONT_SLANT_ITALIC,
                 cairo.FONT_SLANT_OBLIQUE]
  FONT_WEIGHTS = [cairo.FONT_WEIGHT_NORMAL, cairo.FONT_WEIGHT_BOLD]

  def __init__(self, options):
    """Options:
         * number_of_columns    :: int
         * characters_in_a_row  :: int
         * width                :: double, in millimeters
         * height               :: double, in millimeters
         * margin_top           :: double, in millimeters
         * margin_bottom        :: double, in millimeters
         * margin_left          :: double, in millimeters
         * margin_middle        :: double, in millimeters
         * margin_right         :: double, in millimeters

       +---------------------------------------------+     ---  ---
       |                                             |      |    | top margin
       |  +-----------------+   +-----------------+  |      |   ---
       |  |                 |   |                 |  |      |
       |  |      Col 1      |   |      Col 2      |  |      | height
       |  |                 |   |                 |  |      |
       |  +-----------------+   +-----------------+  |      |   ---
       |                                             |      |    | bottom margin
       +---------------------------------------------+     ---  ---

       |---------------------------------------------|
                            width

       |--|                 |---|                 |--|
         left margin          middle marign         right margin

       All widths should be given in millimeters.
    """
    self.options = options
    for option in {'number_of_columns', 'characters_in_a_row', 'width',
                   'height', 'margin_top', 'margin_bottom', 'margin_left',
                   'margin_middle', 'margin_right'}:
      setattr(self, option, options[option])
    self._ComputeMeasurements()
    self._InitCairo()
    self._InitLineData()
    self._ComputeFooterMeasurements()
    self._ComputeHeaderMeasurements()
    self._ComputeRightMeasurements()
    self.summary_data = []

  def __del__(self):
    self._NextPage()

  def _ComputeMeasurements(self):
    self.width_without_margins = \
        self.width - self.margin_left - self.margin_right \
            - self.margin_middle * (self.number_of_columns - 1)
    self.height_without_margins = \
        self.height - self.margin_top - self.margin_bottom
    self.column_width = self.width_without_margins / self.number_of_columns
    self.column_height = self.height_without_margins
    self.column_y = self.margin_top
    self.column_xs = [self.margin_left]
    for column_id in range(1, self.number_of_columns):
      self.column_xs.append(
          self.column_xs[-1] + self.column_width + self.margin_middle)

  def _ComputeFooterMeasurements(self):
    self.footer_y = self.column_y + self.column_height + \
        self.margin_bottom / 2 - self.font_height / 2
    self.footer_x = self.margin_left
    self.footer_width = self.width - self.margin_left - self.margin_right

  def _ComputeHeaderMeasurements(self):
    self.header_y = self.margin_top / 2 - self.font_height / 2
    self.header_x = self.margin_left
    self.header_width = self.width - self.margin_left - self.margin_right

  def _ComputeRightMeasurements(self):
    big_letter_size_percent = self.options['big_letter_box_percent']
    self.right_x = self.width - self.margin_right * big_letter_size_percent
    self.right_y = self.margin_top
    self.right_width = self.margin_right * big_letter_size_percent

  def _InitCairo(self):
    self.surface = cairo.PDFSurface(
        self.options['output_pdf'],
        self.width * self.POINTS_TO_MILLIMETERS_RATIO,
        self.height * self.POINTS_TO_MILLIMETERS_RATIO)
    self.fonts = dict()
    for slant in self.FONT_SLANTS:
      self.fonts[slant] = dict()
      for weight in self.FONT_WEIGHTS:
        self.fonts[slant][weight] = \
            cairo.ToyFontFace("Monospace", slant, weight)
    self.context = cairo.Context(self.surface)
    self.context.scale(self.POINTS_TO_MILLIMETERS_RATIO,
                       self.POINTS_TO_MILLIMETERS_RATIO)
    self.context.set_font_face(
        self.fonts[cairo.FONT_SLANT_NORMAL][cairo.FONT_WEIGHT_NORMAL])
    self.context.set_font_size(1);
    self.font_ascent, self.font_descent, self.font_height, \
        self.font_max_x_advance, self.font_max_y_advance = \
            self.context.font_extents()
    font_scale_factor = \
        self.column_width / (self.font_max_x_advance * self.characters_in_a_row)
    self.context.set_font_size(font_scale_factor)
    self.font_ascent, self.font_descent, self.font_height, \
        self.font_max_x_advance, self.font_max_y_advance = \
            self.context.font_extents()
    self.context.set_line_width(0.05)

  def _InitLineData(self):
    self.pages_printed = 0
    self.columns_printed = 0
    self.lines_printed = 0
    self.row_limit = math.floor(self.column_height / self.font_height)
    self.total_rows = self.row_limit
    if self.row_limit < 1:
      raise PdfPagesError(
          "The page configuration doesn't leave any space for the text.")
    vertical_space_left = \
        self.column_height - self.total_rows * self.font_height
    self.margin_top += vertical_space_left / 2
    self.margin_bottom += vertical_space_left / 2
    self._ComputeMeasurements()

  def _PrintFrames(self):
    self.context.save()
    for column_id in range(self.columns_printed):
      self.context.save()
      self.context.rectangle(self.column_xs[column_id], self.column_y,
                             self.column_width, self.column_height)
      self.context.stroke()
      self.context.restore()
    if self.columns_printed != self.number_of_columns:
      self.context.save()
      self.context.rectangle(
          self.column_xs[self.columns_printed], self.column_y,
          self.column_width, (self.lines_printed) * self.font_height)
      self.context.stroke()
      self.context.restore()
    self.context.restore()

  def _PrintPageNumber(self):
    if hasattr(self, 'total_number_of_pages'):
      page_text = "Page {page:{padding}} / {all}".format(
          page=self.pages_printed + 1,
          padding=len(str(self.total_number_of_pages)),
          all=self.total_number_of_pages)
    else:
      page_text = "Page {}".format(self.pages_printed + 1)
    self._PrintMonoStyleTextXY(
        Character(**self.options['page_number_decorations']),
        page_text,
        self.footer_x + self.footer_width \
            - len(page_text) * self.font_max_x_advance,
        self.footer_y)

  def _PrintDate(self):
    if not hasattr(self, 'date'):
      return
    self._PrintMonoStyleTextXY(
        Character(**self.options['date_decorations']),
        self.date.strftime('%Y-%m-%d'), self.footer_x, self.footer_y)

  def _PrintSummaryLetter(self, index, letter, is_highlighted):
    width = self.right_width
    height = width
    margin = 0.1 * width
    x = self.right_x
    y = self.right_y + (self.right_width + margin) * index
    # Background.
    if is_highlighted:
      self.context.save()
      self.context.rectangle(x, y, width, width)
      self.context.set_source_rgba(*self.options['big_letter_colors'][letter])
      self.context.fill()
      self.context.restore()
    # Big letter.
    self.context.save()
    style = Character('', **self.options['big_letter_font'])
    if not is_highlighted:
      style.SetColor(self.options['big_letter_colors'][letter])
    style.ApplyStyleToContext(self.context, self.fonts)
    self.context.translate(x, y)
    te_xbearing, te_ybearing, te_width, te_height, te_xadvance, te_yadvance = \
        self.context.text_extents(letter)
    scale = (height / te_height) * self.options['big_letter_size_percent']
    new_height = height / scale
    border = (new_height - te_height) / 2
    self.context.scale(scale, scale)
    te_xbearing, te_ybearing, te_width, te_height, te_xadvance, te_yadvance = \
        self.context.text_extents(letter)
    y_space = new_height - te_height
    self.context.move_to(border + te_xbearing, y_space / 2 - te_ybearing)
    self.context.show_text(letter)
    self.context.restore()

  def _PrintPageSummary(self):
    summary_text = MakeSummaryText(self.summary_data)
    self._PrintMonoStyleTextXY(
        Character(**self.options['page_summary_decorations']),
        summary_text,
        self.header_x + self.header_width \
            - len(summary_text) * self.font_max_x_advance,
        self.header_y)
    summary_letters = MakeSummaryLetters(self.summary_data)
    letters = "KMRB"
    for i in range(len(letters)):
      self._PrintSummaryLetter(i, letters[i], letters[i] in summary_letters)

  def _NextPage(self):
    if self.pages_printed == self.total_number_of_pages:
      return
    self._PrintFrames()
    self._PrintPageNumber()
    self._PrintDate()
    self._PrintPageSummary()
    self.context.show_page()
    self.pages_printed += 1
    self.summary_data = []

  def _NextColumn(self):
    self.columns_printed += 1
    if self.columns_printed == self.number_of_columns:
      self._NextPage()
      self.columns_printed = 0

  def _NextLine(self, jump=1):
    self.lines_printed += jump
    if self.lines_printed >= self.row_limit:
      self._NextColumn()
      self.lines_printed = 0

  def _PrintMonoStyleTextXY(self, style, text, x, y):
    # Background.
    self.context.save()
    style.ApplyBackgroundStyleToContext(self.context)
    self.context.rectangle(
        x, y, len(text) * self.font_max_x_advance, self.font_height)
    self.context.fill()
    self.context.restore()
    # Border.
    self.context.save()
    style.ApplyBorderStyleToContext(self.context)
    self.context.rectangle(
        x, y, len(text) * self.font_max_x_advance, self.font_height)
    self.context.stroke()
    self.context.restore()
    # Text.
    self.context.save()
    style.ApplyStyleToContext(self.context, self.fonts)
    for index in range(len(text)):
      self.context.move_to(x + self.font_max_x_advance * index,
                           y + self.font_ascent)
      self.context.show_text(text[index])
    self.context.restore()

  def _PrintMonoStyleText(self, style, text, column, line, char):
    self._PrintMonoStyleTextXY(
        style,
        text,
        self.column_xs[column] + char * self.font_max_x_advance,
        self.column_y + line * self.font_height)

  def _AddSummaryData(self, title):
    if self.summary_data and self.summary_data[-1] == title:
      pass
    else:
      self.summary_data.append(title)

  def _AddLine(self, line):
    style_fragments = []
    for character in line:
      if character.HasTitle():
        self._AddSummaryData(character.GetTitle())
      if style_fragments and style_fragments[-1][-1].EqualStyle(character):
        style_fragments[-1].append(character)
      else:
        style_fragments.append([character])
    characters_processed = 0
    for style_fragment in style_fragments:
      text = ""
      for character in style_fragment:
        text += character.GetCharacter()
      self._PrintMonoStyleText(style_fragment[0], text, self.columns_printed,
                               self.lines_printed, characters_processed)
      characters_processed += len(text)
    self._NextLine()

  def GetRowsPerColumn(self):
    return self.row_limit

  def GetNumberOfColumns(self):
    return self.number_of_columns

  def GetRowHeight(self):
    return self.font_height

  def GetColumnWidth(self):
    return self.column_width

  def SetDate(self, date):
    self.date = date

  def SetTotalNumberOfPages(self, total_number_of_pages):
    self.total_number_of_pages = total_number_of_pages

  def AddLine(self, line):
    if len(line) <= self.characters_in_a_row:
      self._AddLine(line)
    else:
      self._AddLine(line[:self.characters_in_a_row])
      self.AddLine(line[self.characters_in_a_row:])

  def AddPdfPage(self, rowspan, pdf_page):
    if rowspan > self.row_limit:
      rowspan = self.row_limit
    self.context.save()
    x = self.column_xs[self.columns_printed]
    y = self.column_y + self.lines_printed * self.font_height
    width = self.GetColumnWidth()
    height = rowspan * self.GetRowHeight()
    rect = pdf_page.get_crop_box()
    x2 = rect.x1
    y2 = rect.y1
    width2 = rect.x2 - x2
    height2 = rect.y2 - y2
    extra_height = height - height2 * width / width2
    self.context.rectangle(x, y, width, height)
    self.context.clip()
    self.context.translate(x, y + max(0, extra_height / 2))
    self.context.scale(width, width)
    self.context.save()
    self.context.scale(1 / width2, 1 / width2)
    self.context.translate(-x2, -y2)
    pdf_page.render(self.context)
    self.context.restore()
    if extra_height < -0.01:
      message = "UCIĘTY FRAGMENT"
      self.context.save()
      self.context.set_source_rgba(1, 0, 0, 0.5)
      self.context.paint()
      self.context.set_source_rgba(1, 0, 0, 1)
      te_xbearing, te_ybearing, te_width, te_height, te_xadvance, te_yadvance = \
          self.context.text_extents(message)
      scale = 1 / te_width
      new_width = 1 / scale
      self.context.scale(scale, scale)
      te_xbearing, te_ybearing, te_width, te_height, te_xadvance, te_yadvance = \
          self.context.text_extents(message)
      W = new_width
      H = height / width / scale
      self.context.translate(W / 2, H / 2)
      self.context.rotate(-math.pi / 4);
      self.context.translate(-W / 2, -H / 2)
      self.context.move_to((W - te_width) / 2 - te_xbearing,
                           (H - te_height) / 2 - te_ybearing)
      self.context.show_text(message)
      self.context.restore()
    self.context.restore()
    self._NextLine(rowspan)

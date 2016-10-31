from pdf_pages import Character
from pygments.formatter import Formatter

def _HexColorToRgb256(hex_color):
  i = int(hex_color, 16)
  return (i // 2**16, (i // 2**8) % 2**8, i % 2**8)

def _HexColorToRgb(hex_color):
  return tuple([i / 255 for i in _HexColorToRgb256(hex_color)])

class CharacterFormatter(Formatter):
  def __init__(self, **options):
    Formatter.__init__(self, **options)
    self.styles = {}
    for token, style in self.style:
      self.styles[token] = style
    self.output = []

  def format(self, tokensource, outfile):
    for ttype, value in tokensource:
      for c in value:
        character = Character(str(c))
        for key, value in self.styles.get(ttype, dict()).items():
          if key == 'color':
            if value:
              character.SetColor(_HexColorToRgb(value))
          elif key == 'bold':
            character.SetIsBold(value)
          elif key == 'italic':
            character.SetIsItalic(value)
          elif key == 'bgcolor':
            if value:
              character.SetBorderColor(_HexColorToRgb(value))
          elif key == 'border':
            if value:
              character.SetBorderColor(_HexColorToRgb(value))
        self.output.append(character)

  def GetOutput(self):
    return self.output

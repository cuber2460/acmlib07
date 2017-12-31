import code_parser
import os
import sys

options = {
  'number_of_columns': 2,
  'characters_in_a_row': 80,
  'expandtab': 2,
  'width': 297,
  'height': 210,
  'margin_top': 8,
  'margin_bottom': 11,
  'margin_left': 8,
  'margin_middle': 4,
  'margin_right': 8,

  'big_letter_box_percent': 0.8,
  'big_letter_size_percent': 0.7,
  'big_letter_colors': {
    'C': (0.2, 0.2, 0.2),
    'K': (0.7, 0, 0),
    'M': (0, 0, 0.7),
    'R': (0, 0.7, 0),
    'B': (0, 0, 0),
  },
  'big_letter_font': {
    'is_bold': True,
    'color': (1, 1, 1),
  },

  'header_decorations': {
    'is_bold': True,
    'color': (0, 0, 0),
    'bg_color': (0, 1, 0, 0.5),
    'border_color': (0, 0, 0),
  },

  'right_wrap_decorations': {
    'is_bold': True,
    'color': (1, 1, 1),
    'bg_color': (1, 0, 0),
  },

  'left_wrap_decorations': {
    'is_bold': True,
    'color': (1, 1, 1),
    'bg_color': (0, 0, 1),
  },

  'page_number_decorations': {
    'is_italic': True,
    'color': (0, 0, 0),
  },

  'date_decorations': {
    'is_italic': True,
    'color': (0, 0, 0),
  },

  'page_summary_decorations': {
    'is_italic': True,
    'color': (0, 0, 0),
  },

  'latex_options': {
    'input_tex': "???",
    'width': "???",
    'zoom': 2.5,
    'border': (5,   # Left.
               5,   # Bottom.
               5,   # Right.
               5),  # Top.
  },
}

def CheckFileType(filename):
  if filename.endswith('.cpp'):
    return (filename, 'C++', 'default')
  elif filename.endswith('.tex'):
    return (filename, 'LaTeX', 'default')
  else:
    return ()

def FindAllSourceCode(code_dir):
  sources = []
  for (dirpath, dirnames, filenames) in os.walk(code_dir):
    for filename in filenames:
      path = os.path.join(dirpath, filename)
      file_type = CheckFileType(path)
      if file_type:
        sources.append(file_type)
  return sources

if len(sys.argv) != 3:
  print("Usage: {} code_dir/ output.pdf".format(sys.argv[0]))
  sys.exit(1)

options['code_dir'] = sys.argv[1]
options['output_pdf'] = sys.argv[2]

cp = code_parser.CodeParser(options)

files = []

for filename, lang, style in FindAllSourceCode(options['code_dir']):
  files.append({"filename": filename,
                "lang":     lang,
                "style":    style})

def key_to_compare(f):
  global options
  filename = os.path.relpath(f["filename"], options["code_dir"])
  directory = os.path.dirname(filename)
  basename = os.path.basename(filename)
  dir_weight = -1
  if directory == "":
    dir_weight = 0
    if basename == "makra.cpp":
      basename = "1"
    elif basename == "Rzeczy_Na_Dzien_Probny.cpp":
      basename = "2"
    else:
      assert False
  elif directory == "kamil":
    dir_weight = 1
  elif directory == "marek":
    dir_weight = 2
  elif directory == "mateusz":
    dir_weight = 3
  elif directory == "blazej":
    dir_weight = 4
  else:
    assert False
  return (dir_weight, basename.lower())

files.sort(key=key_to_compare)

for f in files:
  new_options = dict(options)
  new_options['lang'] = f['lang']
  new_options['style'] = f['style']
  cp.AddCodeFile(f['filename'], new_options)

cp.GeneratePdf(options)

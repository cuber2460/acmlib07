#!/usr/bin/python3

import gi
import os
import pathlib
import subprocess
import tempfile

gi.require_version('Poppler', '0.18')

from gi.repository import Poppler

class TexError(Exception): pass

def ReadBlocksFromTexFile(filename):
  f = open(filename, 'r')
  next_block = ''
  for line in f.readlines():
    if line == (80 * '%' + '\n'):
      next_block = next_block.strip()
      if next_block:
        yield next_block
      next_block = ''
    else:
      next_block = next_block + line
  next_block = next_block.strip()
  if next_block:
    yield next_block

def GenerateTexFile(output_filename, options):
  f = open(output_filename, 'w')
  border = "border={{{}pt {}pt {}pt {}pt}}".format(*options['border'])
  f.write(R"\documentclass[multi=minipage," + border + "]{standalone}" + "\n")
  is_header = True
  is_first = True
  for block in ReadBlocksFromTexFile(options['input_tex']):
    if is_header:
      is_header = False
      f.write(block)
      f.write(R"\standaloneenv{my}" + "\n")
      f.write(R"\begin{document}" + "\n")
      continue
    elif is_first:
      is_first = False
    else:
      f.write(R"\newpage" + "\n")
    f.write(R"\begin{my}" + "\n")
    f.write(R"\begin{minipage}{" + \
            str(options['width'] * options['zoom']) + "pt}" + "\n")
    f.write(block + "\n")
    f.write(R"\end{minipage}" + "\n")
    f.write(R"\end{my}" + "\n")
  f.write(R"\end{document}" + "\n")
  if is_header:
    raise TexError("There is no header.")

def GeneratePdfDocumentForTex(options):
  tmp_dir = tempfile.TemporaryDirectory()
  tmp_path = tmp_dir.name
  out_tex = 'out.tex'
  out_pdf = 'out.pdf'
  GenerateTexFile(os.path.join(tmp_path, out_tex), options)
  process = subprocess.run(["pdflatex", out_tex], cwd=tmp_path)
  if process.returncode != 0:
    raise TexError("pdflatex failed.")
  return Poppler.Document.new_from_file(
      pathlib.Path(os.path.join(tmp_path, out_pdf)).as_uri(), None)

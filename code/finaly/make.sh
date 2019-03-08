find code -name '*.cpp' | xargs cat | a2ps -o lib.ps
ps2pdf lib.ps
rm lib.ps*

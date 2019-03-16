(for f in code/*.cpp; do echo -e "\n/*======  ${f#code/} ======*/"; cat $f; done) | sed 's/\t/  /g' | a2ps --center-title='' --header='Warsaw Eagles Team Reference Document' -o lib.ps
ps2pdf lib.ps
rm lib.ps*

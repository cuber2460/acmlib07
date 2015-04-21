ALL=.all
ALLFILE=${ALL}.cpp
ALLPDF=${ALL}.pdf
LIBFILE=lib.pdf

echo -n "" > $ALLFILE

function make_name {
	echo ${1%.*} | sed 's/_/ /g'
}

function print_header {
	NAME=`make_name $1`
	WIDTH=74
	printf "/*****************************************************************************/\n"
	printf "/*                                                                           */\n"
	printf "/* %*s%*s*/\n" $(((${#NAME}+$WIDTH)/2)) "$NAME" $(($WIDTH-((${#NAME}+$WIDTH)/2))) ""
	printf "/*                                                                           */\n"
	printf "/*****************************************************************************/\n"
}

for j in *; do
	if [ -d "$j" ]; then
		cd "$j"
		for i in *.cpp; do
			print_header $i >> ../$ALLFILE
			cat $i >> ../$ALLFILE
		done
		cd ..
	fi
done

TEMP=.tmp.cpp
iconv -f utf-8 -t iso-8859-2 $ALLFILE -o $TEMP
mv $TEMP $ALLFILE

a2ps -P pdf $ALLFILE -L 65 --center-title="University of Warsaw" --footer="" --left-footer="" --right-footer="" --header="" --encoding=latin2

#a2ps $ALLFILE --file-align=fill -L 65  --margin=0 -o $LIBFILE --left-title="University of Warsaw" --center-title="" --right-title="" --header="Page %s./%s#"

rm $ALLFILE
mv $ALLPDF $LIBFILE

#!/bin/bash


expectedArg=1

if [ "$#" -lt "$expectedArg" ]
then
	echo "Need .na4 as input file!"
	exit
else
	na4_file=$1
fi

echo "start automation for <${na4_file}>"

name=${na4_file%.*}
name=${na4_file%.*}
path=${name%/*}
name=${name##*/}

echo "path: <${path}>"
echo "name: <${name}>"

# make map file with maptona4
infile="${na4_file}"
outfile="${path}/${name}.map"
# maptona4 
echo "maptona4 in: <${infile}>"
echo "maptona4 out: <${outfile}>"

maptona4 mapin "${infile}" mapout "${outfile}"

echo "maptona4 conversoin done!"



# sfall HKLOUT "/home/eva/Documents/deed/molecular_db/teapot/Teapot03_4MEZ_111_141_227_reshuffle.mtz" MAPIN "/home/eva/Documents/deed/molecular_db/teapot/Teapot03_4MEZ_111_141_227_reshuffle.map"

# title [No title given]
# labout  FC=FCalc PHIC=PHICalc
# NAME -
#     PROJECT DEED
# MODE SFCALC -
#     MAPIN
# badd 0.0
# vdwr 2.5
# end


 # COMMAND

# sfall HKLOUT "/home/eva/Documents/deed/molecular_db/teapot/Teapot03_4MEZ_111_141_227_reshuffle.mtz" MAPIN "/home/eva/Documents/deed/molecular_db/teapot/Teapot03_4MEZ_111_141_227_reshuffle.map" 

# NEED keyworded input ?

# title [abc] labout  FC=FCalc PHIC=PHICalc MODE SFCALC -      MAPIN NAME -      PROJECT DEED badd 0.0 vdwr 2.5 end


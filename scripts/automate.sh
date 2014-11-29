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
#!/bin/bash

if [ $# -ne 1 ] ; then
	echo "Usage $0 <DirectoryName>"
	exit 1
fi 
# Se non esiste lo creo
if [ ! -e $1 ] ; then
	mkdir $1
elif [ ! -d $1 ] ; then
	echo "Error: Isn't a directory"
	exit 1	
fi

for file in $(ls -a $1) ; do
	if [ "$file" = "." -o "$file" = ".." ] ; then
		continue 
	fi
	new=$(echo "$file"|tr A-Z a-z)
	if [ "$new" = "$file" ] ; then
		continue
	fi
	mv $1/$file $1/$new
done

exit 0

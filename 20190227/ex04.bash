#!/bin/bash
if [ $# -ne 1 ]; then
	echo "usage $0 <dir>"
	exit 1
fi

if [ ! -d $1 ]; then
	echo "is not a directory"
	exit 1
fi

find $1 -type f -exec wc -c \{} \;| cut -d " " -f 1 | sort -n |uniq > size.log

bar=0;
while read size; do
	if [ $(($size/1024 + 1)) -gt $bar ]; then
		if [ $bar -gt 0 ]; then
			echo ""
		fi
		bar=$(($size/1024 +1))
		echo -n -e "$bar\t"
	fi
	echo -n "#"
done < size.log
echo ""

exit 0

#!/bin/bash
if [ $# -ne 2 ]; then
	echo "Usage $0 <User Name> <Directory>"
	exit 1
fi

for file in $(find $2 -type f -user $1 -exec egrep "^***Da modificare" -l \{} \;); do
	cat $file| egrep "^\*\*\*Da modificare" -v > ${file}_mod
	rm $file
done

exit 0



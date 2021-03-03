#!/bin/bash
if [ $# -ne 3 ] ; then
	echo "Insert 3 direcoties "
	exit 1
fi

if [ ! -d $1 ] ; then
	echo "First name direcory isn't a  directory"
	exit 1
fi

if [ ! -d $2 ] ; then
	mkdir $2
else
 	rm -R $2
 	mkdir $2
fi

if [ ! -d $3 ] ; then
	mkdir $3
else
 	rm -R $3
 	mkdir $3
fi

for file in $(ls -a $1); do
	if [ -f $1/$file ];then
		egrep "main" $1/$file --quiet
		if [ $? -eq 0 ]; then
			echo "$file" >> tmp1_$$
			cp $1/$file $2
		else
			echo "$file" >> tmp2_$$
			cp $1/$file $3
		fi
	fi
done
cat	tmp1_$$ tmp2_$$

exit 0

#!/bin/bash

if [ $# -ne 2 ] ; then 
	echo "Usage $0 <NameProcess> <Intevall[s]>"
	exit 1
fi

# numero di volte che è zombie
let z=0

while [ 0 ] ; do
	ps -el > .tmp_$$1
	while read line ; do
		# Rimuvovo eventuali spazi e tabulazioni eccessivi
		line=$(echo "$line"| tr "\t" " "|tr -s " ")
		name=$(echo "$line"|cut -d " " -f 14)
		state=$(echo "$line"|cut -d " " -f 2)
		if [ "$name" = "$1" ] ; then
			if [ "$state" = "Z" ] ; then
				let z=z+1
			else
				let z=0
			fi	
			echo "$line"
		fi
	done < .tmp_$$1
	
	if [ $z -eq 5 ] ; then
		break
	fi
	
	sleep $2
done

rm -f .tmp_$$1

exit 0

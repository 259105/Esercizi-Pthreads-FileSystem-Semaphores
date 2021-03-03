#!/bin/bash

if [ $# -ne 1 ]; then
	echo "usage $0 <graph>"
	exit 1
fi

let pesotot=0
let not=0
read path
let i=0

for v in $path; do
	if [ $i -eq 0 ]; then
		primo=$v
		let i=i+1
		continue
	elif [ $i -eq 1 ]; then
		secondo=$v
	else 
		primo=$secondo
		secondo=$v
	fi
	arco="$primo $secondo"
	echo "passo $i-esimo $arco"
	finded=$(cat $1| egrep "$arco")
	
	if [ $? -eq 0 ]; then
		peso=$(echo $finded|cut -d " " -f 3)
		let pesotot=pesotot+peso
	else
		let not=1
	fi
	
	let i=i+1
done

if [ $not -eq 1 ]; then
	echo "path non valido"
	exit 1
else
	echo "path corretto, peso complessivo $pesotot"
fi
exit 0


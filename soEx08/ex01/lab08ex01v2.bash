#!/bin/bash

if [ $# -ne 3 ] ; then
	echo "Usage: $0 <Dir> <Function> <FileOutput>"
	exit 1
fi

if [ -d $1 ] ; then
	#	Faccio find di tutti i file
	# Per ogni file Faccio una grep che verifichi che ci sia la funzione desiderata
	for line in $(find $1 -type f); do
		# Faccio la grep che ferifichi che sia presente
		egrep -n "$2[ \t]*(.*)" $line > .temp$$2
		if [ $? -eq 0 ] ; then
			# Se è presente potrebbe essere che ci sia anche il suo prototipo, quindi presente più volte nello stesso file, quindi faccio un ciclo linea per linea della grep
			while read found ; do
			 	# Divido il numero della linea in cui è presente
				num=$(echo $found|tr : " "|cut -d " " -f 1)
				# Divido la lqqinea in cui è presente
				phrase=$(echo $found|tr : " "|cut -d " " -f 1 --complement)
				# Faccio la stampa
				# Tolgo il Path del file
				name=$(basename $line)
				# Stampo tutto
				echo -e "Name File: $name\t\t\tRow Number: $num\t\t\tPhrase: $phrase"
			done < .temp$$2
		fi
	done > $3
fi

rm -f .temp_$$2

# Ordinamento del file di uscita

sort -n -k 6 $3 > .temp_$$3
cp .temp_$$3 $3
sort -k 3 $3 > .temp_$$3
cp .temp_$$3 $3

exit 0

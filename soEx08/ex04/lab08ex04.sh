#!/bin/bash

if [ $# -ne 1 ] ; then
	echo "Inserisci nome file da analizzare:"
	read file
	if [ $? -ne 0 ] ; then
		echo "Non hai inserito nulla"
		exit 1
	fi
elif [ ! -f $1 ] ; then
	echo "Il path inserito non è relativo ad un file"
	exit 1
else
	file=$1
fi


# Posso usare un array associativo (Tabella di hash) (Hash Map)
declare -A occ

# Per ogni parola dentro il file
for word in $(cat $file) ; do
	# Verifico che la parola non sia gia stata inserita
	f=0; #indica che non ha trovato la key nella hash
	for w in ${!occ[*]} ; do
		if [ "$word" = "$w" ] ; then
			# Significa che già è stato inserito
			let f=1 # ho trovato l'occorenza
			let occ[$word]=occ[$word]+1
			break;
		fi
	done
	if [ $f -eq 0 ] ; then
		# significa che non era presente nella hash
		let occ[$word]=1 #inizializzo a 1
	fi
	# Se è gia stata inserita allora incremeto
done

for occurence in ${!occ[*]} ; do
	echo "$occurence ${occ[$occurence]}"
done | sort -n -r -k 2	

exit 0



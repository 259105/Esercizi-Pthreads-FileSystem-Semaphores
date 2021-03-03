#!/bin/bash

# Controllo che i parametri inseriti siano ok
if [ $# -ne 1 ] ; then
	echo "Usage $0 <NameFile>"
	exit 1;
fi

# Controllo che il nome del file ricevuto sia veramente un file
if [ -e $1 -a ! -f $1 ] ; then
	echo "Insert file isn't file"
	exit 1;
fi
touch $1
let n=0 #numero righe del file
let max=0 #massimo caratteri riga
while read line ; do
	#incremento le righe del file
	let n=n+1
	#prendo il numero di caratteri della riga
	l=$(echo -n $line|wc -c)
	if [ $l -gt $max ] ; then
		let max=l
	fi
done < $1

echo -e "Numero caratteri della riga più lunga: $max\nNumero righe: $n"

exit 0

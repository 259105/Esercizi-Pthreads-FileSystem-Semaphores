NAMEPRG=prg1
BIN=bin1
SRC=soEx01.c # potrei aggiungerne altri con lo spazio
includedir = ../inc
INCLUDEDIR =  -I. -I$(includedir)
LIB = -lm

install: ../$(BIN) compile
	mv prg1 ../$(BIN)
../$(BIN):
	mkdir ../$(BIN)
compile: soEx01.o
	gcc -Wall -o $(NAMEPRG) $(INCLUDEDIR)  soEx01.o
soEx01.o:
	gcc -Wall $(INCLUDEDIR) -c $(SRC)

# correggere: come fare se io voglio che non canelli nulla se non c'è l'ogetto???
clean: soEx01.o
	# cosa fanno queste cose???
	rm -rf *~
	rm -rf *bak*
	rm -rf core
	rm *.o
	
distclean: clean
	rm -rf ../$(BIN)

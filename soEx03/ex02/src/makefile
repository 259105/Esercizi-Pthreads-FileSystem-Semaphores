NAMEPRG=ex2
BIN=bin
SRC=lab03ex02.c # potrei aggiungerne altri con lo spazio
includedir = ../inc
INCLUDEDIR =  -I. -I$(includedir)
LIB = -lm

install: ../$(BIN) compile
	mv $(NAMEPRG) ../$(BIN)
../$(BIN):
	mkdir ../$(BIN)
compile: 
	gcc -Wall -o $(NAMEPRG) $(INCLUDEDIR) $(SRC) $(LIB)

# correggere: come fare se io voglio che non canelli nulla se non c'è l'ogetto???
clean: soEx01.o
	# cosa fanno queste cose???
	rm -rf *~
	rm -rf *bak*
	rm -rf core
	rm *.o
	
distclean: clean
	rm -rf ../$(BIN)

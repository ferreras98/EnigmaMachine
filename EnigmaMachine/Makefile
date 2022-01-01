
all: enigma_v3.exe

enigma_v3.exe: mainEnigma.o menus.o libroClaves.o interfaz.o auxiliar.o config.o cifrado.o reflector.o rotores.o stecker.o Enigma.o
	gcc.exe -o enigma_v3.exe mainEnigma.o menus.o libroClaves.o interfaz.o auxiliar.o config.o cifrado.o reflector.o rotores.o stecker.o Enigma.o

mainEnigma.o: main/mainEnigma.c
	gcc.exe -c -Wall -ansi -pedantic -o mainEnigma.o main/mainEnigma.c

menus.o: main/menus.c
	gcc.exe -c -Wall -ansi -pedantic -o menus.o main/menus.c

libroClaves.o: LibroClaves/libroClaves.c
	gcc.exe -c -Wall -ansi -pedantic -o libroClaves.o LibroClaves/libroClaves.c

cifrado.o: Maquina/cifrado.c
	gcc.exe -c -Wall -ansi -pedantic -o cifrado.o Maquina/cifrado.c

reflector.o: Maquina/reflector.c
	gcc.exe -c -Wall -ansi -pedantic -o reflector.o Maquina/reflector.c

rotores.o: Maquina/rotores.c
	gcc.exe -c -Wall -ansi -pedantic -o rotores.o Maquina/rotores.c

stecker.o: Maquina/stecker.c
	gcc.exe -c -Wall -ansi -pedantic -o stecker.o Maquina/stecker.c

interfaz.o: Maquina/interfaz.c
	gcc.exe -c -Wall -ansi -pedantic -o interfaz.o Maquina/interfaz.c

config.o: config/config.c
	gcc.exe -c -Wall -ansi -pedantic -o config.o config/config.c

auxiliar.o: auxiliar/auxiliar.c
	gcc.exe -c -Wall -ansi -pedantic -o auxiliar.o auxiliar/auxiliar.c

Enigma.o: Maquina/Enigma.c
	gcc.exe -c -Wall -ansi -pedantic -o Enigma.o Maquina/Enigma.c


clean:
	rm enigma_v3.exe mainEnigma.o menus.o libroClaves.o interfaz.o auxiliar.o \
	config.o cifrado.o reflector.o rotores.o stecker.o Enigma.o

zip:
	zip Enigma_v3.zip main/*.* Maquina/*.* auxiliar/*.* LibroClaves/*.* \
		BASEDATOS/*.* Config/*.* Makefile *.cbp

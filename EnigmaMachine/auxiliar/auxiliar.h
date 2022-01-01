#ifndef AUXILIAR_H_INCLUDED
#define AUXILIAR_H_INCLUDED

#include <stdbool.h>

#include "../Maquina/rotores.h"

unsigned char getPos (char, char[]);

void gotoxy(int, int);

void setConsola(int, int);

void unsetConsola(void);

void genera_numeros(int, unsigned short[], int, int);

void dibuja_caja(int, int, int, int);

void dibuja_caja_doble(char *, int, int, int, int);

void letrasStecker(unsigned short, unsigned short, bool, int);

void letrasRotor(struct sRotor, unsigned short, unsigned short, bool, int);

#endif /* AUXILIAR_H_INCLUDED */


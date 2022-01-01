#ifndef CIFRADO_H_INCLUDED
#define CIFRADO_H_INCLUDED

#include "Enigma.h"

void abreArchivosMensaje(FILE **, FILE **);

void cierraArchivosMensaje(FILE *, FILE *);

void inicCifrado(void);

char leeLetra(unsigned short);

void procesoCifrado(struct sEnigma);

#endif /* CIFRADO_H_INCLUDED */

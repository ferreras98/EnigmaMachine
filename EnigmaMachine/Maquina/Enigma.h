#ifndef ENIGMA_H_INCLUDED
#define ENIGMA_H_INCLUDED

#include "../auxiliar/punto.h"
#include "../Config/config.h"
#include "../Maquina/rotores.h"
#include "../Maquina/reflector.h"

struct sEnigma
{
    struct sConfig config;
    struct sListaRotores listaRotores;
    struct sReflector reflector;
};

void configuracion(struct sEnigma *);

void muestraEsquema(struct sEnigma, struct sPunto []);

#endif /* ENIGMA_H_INCLUDED */


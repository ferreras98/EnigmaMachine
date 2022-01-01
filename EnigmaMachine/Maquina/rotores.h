#ifndef ROTORES_H_INCLUDED
#define ROTORES_H_INCLUDED

#include <stdbool.h> /* Para bool */

#include "../auxiliar/punto.h"
#include "../Config/config.h"

struct sRotor {
    char anillo[27];
    char cableado[27];
    char muesca[3];
    unsigned short num;
    char referencia[27];
};

struct sListaRotores
{
    unsigned short num;
    struct sRotor *rotor;
};

void avanzaRotores(struct sListaRotores *);

void configAnillos(struct sRotor *, int );

void configRotores(struct sListaRotores*, struct sConfig);

void dibujaRotor(struct sRotor, struct sPunto);

void fprintGrundstellung(int, char [], FILE *);

void fprintRingstellung(int, unsigned short [], FILE *);

void fprintWalzenlage(int, unsigned short [], FILE *);

void genGrundstellung(int, char []);

void genRingstellung(int, unsigned short []);

void genWalzenlage(int, unsigned short []);

void leeCfgRotor(struct sRotor *);

unsigned short opRotor(unsigned short, struct sRotor, bool);

void pasoRotor(struct sRotor *pRotor, int );

#endif /* ROTORES_H_INCLUDED */

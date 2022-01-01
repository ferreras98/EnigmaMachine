#ifndef REFLECTOR_H_INCLUDED
#define REFLECTOR_H_INCLUDED

struct sReflector{
    char conexiones[27];
    char modelo;
};

void dibujaReflector(struct sReflector);

void leeCfgReflector(char [], struct sReflector*);

unsigned short opReflector(unsigned short, struct sReflector);

#endif /* REFLECTOR_H_INCLUDED */

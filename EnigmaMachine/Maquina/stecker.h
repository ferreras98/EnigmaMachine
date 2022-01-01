#ifndef STECKER_H_INCLUDED
#define STECKER_H_INCLUDED

#include <stdbool.h>

void dibujaEstator(unsigned short);

void dibujaStecker(char [], unsigned short);

void fprintStecker(char [], FILE *);

void genStecker(char []);

unsigned short opStecker(unsigned short , char []);

#endif /* STECKER_H_INCLUDED */

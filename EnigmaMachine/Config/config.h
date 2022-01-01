#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

struct sConfig
{
    int dia;
    char grund[5];
    char modeloETW;
    unsigned short numRotores;
    unsigned short ringS[4];
    char stecker[27];
    unsigned short walzen[4];
};

void leeConfig(struct sConfig *);

void muestraConfig(struct sConfig);

void selecModelo(struct sConfig *);

#endif /* CONFIG_H_INCLUDED */

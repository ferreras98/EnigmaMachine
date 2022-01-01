#include <stdio.h>
#include <locale.h>
#include <windows.h>

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"

#include "../Maquina/Enigma.h"
#include "../Maquina/interfaz.h"
#include "../Maquina/stecker.h"

/* muestraEsquema() Dibuja los elementos de Enigma que intervienen en el proceso
 * de cifrado, llamando a la función correspondiente de pintado.
 * enigma (in) Registro de tipo struct sEnigma, que contiene todos los parámetros
 * de configuración y cableados de la máquina Enigma
 * coordRotor[] (in) Vector numérico de registros de tipo struct sPunto, que contiene
 * las coordenadas de cada uno de los rotores en la interfaz gráfica.
 * Esta función es necesaria para 'limpiar/refrescar' la interfaz de la ruta de cifrado de
 * una letra, y debe ser invocada cada vez que el usuario teclee una nueva letra.
 */
void muestraEsquema(struct sEnigma enigma, struct sPunto coordRotor[])
{
    unsigned short dx = 0;
    int i;

    /* Desplazamiento de los elemetos a la izquierda de los rotores */
    /* Calculo base cuando hay 3 rotores */
    dx = 8*(enigma.config.numRotores -3);

    /* Dibujamos cada uno de los elementos, de izquierda a derecha */
    dibujaReflector(enigma.reflector);

    for (i=0; i < enigma.config.numRotores; i++)
    {
        dibujaRotor(enigma.listaRotores.rotor[i], coordRotor[i]);
    }

    dibujaEstator(dx);

    dibujaStecker(enigma.config.stecker, dx);

    return;
}

/* configuracion() carga todos los parámetros de configuración y cableado de los
 * diferentes elementos de la máquina
 * pEnigma (in, out) Dirección del registro de tipo struct sEnigma que contiene
 * todos los elementos de la máquina Enigma.
 */
void configuracion(struct sEnigma *pEnigma)
{
    int dia;
    int i, offset = 0;
    char buffer[100], mensaje[200];
    bool valido = false;

    selecModelo(&pEnigma->config);

    /* Incializamos la lista de rotores */
    pEnigma->listaRotores.num = pEnigma->config.numRotores;
    pEnigma->listaRotores.rotor = (struct sRotor*) malloc(pEnigma->listaRotores.num *
                                  sizeof(struct sRotor));

    /* Para que nos quepa en el ancho de la ventana */
    if ((pEnigma->config.numRotores > 6) || (PARES_STECKER > 10))
    {
        setConsola(59, 12);
        offset = 9;
    }
    /* Leemos el dia elegido de por el usuario */
    while(!valido)
    {
        system("cls");  /* Pintamos todo la ventana para evitarnos problemas */

        /* Pintamos la caja triple */
        dibuja_caja(0,0,50+offset,3);
        dibuja_caja(0,2,50+offset,7);
        dibuja_caja(0,8,50+offset,3);
        gotoxy(0,2);  /* Arreglamos las esquinas de las juntas */
        printf("%c", (char)195);   /* ├ */
        gotoxy(49+offset,2);
        printf("%c", (char)180);   /* ┤ */
        gotoxy(0,8);
        printf("%c", (char)195);   /* ├ */
        gotoxy(49+offset,8);
        printf("%c", (char)180);   /* ┤ */

        setlocale(LC_CTYPE, "Spanish");
        gotoxy(2,1);
        printf(TITULO_CONFIG);
        gotoxy(2,9);
        printf(INTRODUCE_DIA);    /* Texto definido en globals.h */
        setlocale(LC_CTYPE, "C");

        gotoxy(37,9);
        gets(buffer);
        if((dia = atoi(buffer)) == 0)  /* Si no es un número entonces error */
        {
            sprintf(mensaje, TEXTO_ERROR2, buffer);
            MessageBox(NULL, mensaje, TITULO_ERROR23, MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
        }
        else if ((dia <1) || (dia>31)) /* Si no es un numero entre 1 y 31 entonces error */
        {
            sprintf(mensaje, TEXTO_ERROR3, dia);
            MessageBox(NULL, mensaje, TITULO_ERROR23, MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
        }
        else
            valido = true;
    }
    pEnigma->config.dia = dia;

    /* Leemos la configuracion contenida e config.cfg */
    leeConfig(&pEnigma->config);

    /* Configuramos los rotores con la información de cableado contenida en cableadoRotores.cfg */
    for (i = 0; i < pEnigma->config.numRotores; i++)
    {
        pEnigma->listaRotores.rotor[i].num = pEnigma->config.walzen[i];
        leeCfgRotor(&pEnigma->listaRotores.rotor[i]);  /* En el 1 ponemos la configuracion del walzen */
    }
    /* Configuracion del reflector del fichero reflector.cfg */
    pEnigma->reflector.modelo = pEnigma->config.modeloETW;
    leeCfgReflector(FILE_REFLECTOR, &pEnigma->reflector);

    /* Presentamos en la ventana la configuración leída */
    muestraConfig(pEnigma->config);

    return;
}

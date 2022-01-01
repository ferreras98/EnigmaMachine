#include <stdio.h>
#include <strings.h>
#include <windows.h>
#include <locale.h>
#include <conio.h>
#include <ctype.h>

#include "../main/globals.h"

#include "../auxiliar/auxiliar.h"
#include "../Config/config.h"
#include "../Maquina/reflector.h"
#include "../Maquina/rotores.h"

/* leeConfig() Lee los parámetros de configuración correspondientes
 * pConfig (in out) Dirección del registro que contiene y devuelve los parámetros de configuración.
 * Hay cuatro posibles archivos de configuración, uno por cada modelo de Enigma
 * El nombre del archivo de datos se genera a partir del modelo de reflector y del número de rotores:
 *          libro_M3_B.bin
 *          libro_M3_C.bin
 *          libro_M4_B.bin
 *          libro_M4_C.bin
 */
void leeConfig(struct sConfig *pConfig)
{
    FILE *ptr;
    int  dia;

    char buffer[100], mensaje[100];

    /* Abrimos el fichero de configuracion */
    sprintf(buffer, "BASEDATOS/libro_M%1d_%c.bin", pConfig->numRotores, pConfig->modeloETW);
    ptr = fopen(buffer, "rb");
    if (ptr == NULL)
    {
        sprintf(mensaje, "No puedo abrir el fichero %s", buffer);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        unsetConsola();
        exit(1);
    }

    /* Leemos las lineas hasta que encontremos el día que nos han pedido */
    dia = pConfig->dia;
    do
    {
        fread(pConfig, sizeof(struct sConfig), 1, ptr);
    }
    while (pConfig->dia != dia);  /* Hasta que llegamos la día correcto */

    /* Cerramos el fichero de configuracion */
    fclose(ptr);
    return;
}

/* selecModelo() Muestra los modelos disponibles de máquina Enigma
 * pConfig (out) Dirección del registro de tipo struct sConfig que contiene los parámetros de configuración.
 * Pide al usuario que seleccione un modelo de máquina Enigma y comprueba si el valor introducido
 * es correcto (corresponde a una de las opciones). Si la opción no es válida vuelve a mostrar el
 * menú, hasta que el usuario selecciona una opción correcta.
 * La función asigna valores a los campos numRotores (3 ó 4) y modeloETW (B ó C) del registro que recibe por dirección.
 */
void selecModelo(struct sConfig *pConfig)
{

    char opcion;
    /* Escribimos en menu con los caracteres en español */
    setConsola(50, 12);

    dibuja_caja_doble("Elija tipo de máquina (1-4)", 0, 0, 50,7);
    dibuja_caja(0,8,50,3); /* Ventana de abajo */

    setlocale(LC_CTYPE, "Spanish");
    gotoxy(2,3);
    printf("1. 3-rotor M3 Kriegsmarine UKW = B");
    gotoxy(2,4);
    printf("2. 3-rotor M3 Kriegsmarine UKW = C");
    gotoxy(2,5);
    printf("3. 4-rotor M4 Kriegsmarine UKW = B");
    gotoxy(2,6);
    printf("4. 4-rotor M4 Kriegsmarine UKW = C");
    gotoxy(2,9);
    printf("Selecciona una opción:");
    setlocale(LC_CTYPE, "C");

    /* Leemos la opcion elegida */
    do
    {
        gotoxy(25,9);
        fflush(stdin); /* Limpia el buffer del teclado */
        opcion = _getch();
    }
    while (opcion != '1' && opcion != '2' && opcion != '3' && opcion != '4');

    switch(opcion)
    {
    case '1':
        pConfig->modeloETW = 'B';
        pConfig->numRotores = 3;
        break;
    case'2':
        pConfig->modeloETW = 'C';
        pConfig->numRotores = 3;
        break;
    case '3':
        pConfig->modeloETW = 'B';
        pConfig->numRotores = 4;
        break;
    case '4':
        pConfig->modeloETW = 'C';
        pConfig->numRotores = 4;
        break;
    }
    return;
}

/* muestraConfig() Muestra la configuración leida del fichero config.cfg en la consola
 * walzen (in) Vector con la información del tipo de rotores (1-8)
 * stecker (in) Información con los pares del stecker
 * ring (in) Información del Ringstellung
 * grund (in) Información del Grundstellung
 */
void muestraConfig(struct sConfig config)
{
    int i;

    setlocale(LC_CTYPE, "Spanish"); /* Sólo para el acento de "Configuración" */

    gotoxy(2,1);
    printf(TITULO_CONFIG);
    gotoxy(2,3);
    printf("UKW           : %c", config.modeloETW);  /* Siempre la B por el momento */
    gotoxy(2,4);
    printf("Walzen        : ");
    for (i=0; i< config.numRotores; i++)   /* Imprimimos en números romanos los tipos de rotores */
    {
        switch(config.walzen[i])
        {
        case 1:
            printf("%s", "I ");
            break;
        case 2:
            printf("%s", "II ");
            break;
        case 3:
            printf("%s", "III ");
            break;
        case 4:
            printf("%s", "IV ");
            break;
        case 5:
            printf("%s", "V ");
            break;
        case 6:
            printf("%s", "VI ");
            break;
        case 7:
            printf("%s", "VII ");
            break;
        case 8:
            printf("%s", "VIII ");
            break;
        case 9:
            printf("%s", "Beta ");
            break;
        case 10:
            printf("%s", "Gamma ");
            break;
        }
    }

    gotoxy(2,5);
    printf("Ringstellung  : ");
    for (i=0; i< config.numRotores; i++)
        printf("%c-%02d ", 'A' + config.ringS[i]-1, config.ringS[i]);

    gotoxy(2,6);
    printf("Stecker       : ");
    for (i=0; i<26; i++)
    {
        if (('A'+i) < config.stecker[i])
            printf("%c%c ", 'A'+i, config.stecker[i]);
    }
    gotoxy(2,7);
    printf("Grundstellung : ");
    for(i=0; i< config.numRotores; i++)
        printf("%c ", config.grund[i]);

    /* Esperamos la salida para continuar */
    gotoxy(2,9);
    printf("Pulsa una tecla para continuar...     ");
    gotoxy(36,9);
    fflush(stdin);
    _getch();
    setlocale(LC_CTYPE, "C");
    return;
}

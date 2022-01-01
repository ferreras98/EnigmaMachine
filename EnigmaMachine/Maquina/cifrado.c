#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>
#include <conio.h>

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"

#include "../Maquina/Enigma.h"
#include "../Maquina/cifrado.h"
#include "../Maquina/interfaz.h"
#include "../Maquina/stecker.h"

/* inicCifrado() Inicia el proceso de cifrado o descifrado cargando la configuración necesaria
 * y llamando a las funciones que simulan el cifrado en cada una de los elementos de la
 * máquina Enigma.
 */
void inicCifrado(void)
{
    struct sEnigma enigma;

    /* Con los parámetros cconfiguramos nuestra máquina*/
    configuracion(&enigma);

    /* Terminamos de configurar los rotores, ajustando el Ringstellung y el Grundstellung */
    configRotores(&enigma.listaRotores, enigma.config);

    /* Dibujamos todo */
    if (enigma.config.numRotores==4)
        dibujaInterfazM4(); /* Esta para los mayores de 4 */
    else
        dibujaInterfazM3(); /* Esta para los menores de 3 */

    /* Comenzamos el cifrado */
    procesoCifrado(enigma);

    /* Liberamos la memoria que se hace en configuracion() */
    free(enigma.listaRotores.rotor);

    return;
}

/* leeLetra() Espera que el operador introduzca una letra, y comprueba que la letra
 * está en el intervalo [A, Z], o que es el caracter elegido para finalizar el mensaje
 * dx (in) Desplazamiento de la posición de la tecla para poner el cursos en M4
 * return El caracter seleccionado en Mayúsculas o 0 si se marca el fin del mensaje
 */
char leeLetra(unsigned short dx)
{
    char caracter;

    while(1)
    {
        /* Colocar el cursor en su sitio */
        gotoxy(40+dx,2);
        fflush(stdin);

        /* Leemos el caracter */
        caracter = _getch();

        /* Si es el caracter devolvemos un 0 */
        if (caracter == FIN_CODIF)
            return (char) 0;

        /* Si no se puede codificar lo saltamos */
        if (!isalpha(caracter))
            continue;

        /* Lo convertimos a mayusculas */
        caracter = toupper(caracter);

        /* Lo imprimimos en la ventana */
        printf("%c", caracter);

        return caracter;
    }
}

/* procesoCifrado() Encargada de implementar y realizar el cifrado/descifrado
 * de un mensaje completo
 * enigma (in) Contiene información sobre todos los elementos de la máquina
 * Utiliza un bucle que va leyendo y cifrando letras hasta que el usuario pulsa una
 * tecla determinada, la tecla ESCAPE (código ASCII 27) para finalizar el cifrado
 */
void procesoCifrado(struct sEnigma enigma)
{
    FILE * pPlano, *pCifrado;

    /* Coordinadas posición de los rotores (x,y) */
    struct sPunto coordRotor[8] = {{7,4},{15,4},{23,4},{31,4},{39,4},{47,4},{55,4},{63,4}};
    unsigned short posicion, der, izq;
    unsigned short dx;
    bool dir;

    int i;
    char caracter;

    /* Calculamos el desplazamiento de los elementos a la derecha de los rotores */
    dx = 8*enigma.config.numRotores;  /* 1 rotor ocupa 8 spacios */

    /* Abrimos archivos para los textos de salida */
    abreArchivosMensaje(&pPlano, &pCifrado);

    /* Rellenamos la Interfaz con todas las letras de los rotores */
    muestraEsquema(enigma, coordRotor);

    /* Colocamos el cursor en su posición para que no esté perdido por ahí */
    gotoxy(40+dx,2);

    /* Ultima ventana antes de comenzar avisando de que no se pase de 250 caracteres */
    MessageBox(NULL, TEXTO_INFO, TITULO_CONSOLA, MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);

    /* Reseteamos los contadores de letras, y le indicamos la posición de la ventana */
    printLetraPlana((char) dx, NULL);
    printLetraCifrada((char) dx, NULL);

    fflush(stdin); /* Para limpiar el buffer del teclado */

    /* Leemos el caracter; si es un 0 terminamos  */
    while ((caracter = leeLetra(dx)) != 0)
    {
        /* Imprimos la letra plana */
        if(printLetraPlana(caracter, pPlano) > MAX_LETRAS)   /*250 */
            return;  /* Finalizamos si hemos llegado al máximo (250) */

        /* ANTES de codificar avanzamos los rotores */
        avanzaRotores(&enigma.listaRotores);

        /* Pintamos la nueva situación */
        muestraEsquema(enigma, coordRotor);

        /* COMENZAMOS LA CODIFICACION */
        /* Viaje de ida */
        /* Posición de entrada al stecker */
        dir = false;

        der = posicion = (unsigned short) caracter - 'A';

        /* Posición de salida del stecker */
        izq = posicion = opStecker(posicion, enigma.config.stecker);
        letrasStecker(izq, der, dir, dx); /* Coloreamos las letras */

        for (i=enigma.config.numRotores; i > 0; i--)
        {
            /* Salida de los rotores */
            der = izq;
            izq = posicion = opRotor(posicion, enigma.listaRotores.rotor[i-1], dir);

            /* Pintamos las letras de colores */
            letrasRotor(enigma.listaRotores.rotor[i-1], izq, der, dir, 7+8*(i-1));

        }

        /* Salida del reflector ; la propia funcion pinta las letras*/
        der = posicion = opReflector(posicion, enigma.reflector);

        /* Viaje de vuelta */
        dir = true;

        for (i=0; i<enigma.config.numRotores; i++)
        {
            /* Salida de los rotores */
            izq = der;
            der = posicion = opRotor(posicion, enigma.listaRotores.rotor[i], dir);

            /* Pintamos las letras de colores */
            letrasRotor(enigma.listaRotores.rotor[i], izq, der, dir, 7+8*i);
        }
        /* Posición de salida del stecker */
        izq = der;
        der = posicion = opStecker(posicion, enigma.config.stecker);

        /* Pintamos las letras de colores */
        letrasStecker(izq, der, dir, dx);

        /* Recuperamos el caracter */
        caracter = 'A' + posicion;

        /* Imprimimos el caracter cifrado */
        printLetraCifrada(caracter, pCifrado);
    }
    /* Cerramos los archivos de los textos de salida */
    cierraArchivosMensaje(pPlano, pCifrado);
    return;
}

/* abreArchivosMensaje() Se encarga de abrir los archivos para almacenar el texto plano
 * y el texto cifrado del proceso de cifrado en curso. Comprueba la correcta
 * apertura de los archivos y muestra un error informativo en caso de error
 * ptPlano (out) Dirección del manipulador del archivo de texto plano
 * ptCifrado (out) Dirección del manipulador del archivo de texto cifrado
 */
void  abreArchivosMensaje (FILE **ptPlano, FILE **ptCifrado)
{
    char mensaje[100];

    /* Chequeamos si no hemos podido abrir el fichero */
    *ptPlano = fopen(FILE_PLANO, "wt");
    if (ptPlano == NULL)
    {
        sprintf(mensaje, "No puedo abrir el fichero %s", FILE_PLANO);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        exit(1);
    }
    *ptCifrado = fopen(FILE_CIFRADO, "wt");
    if (ptCifrado == NULL)
    {
        sprintf(mensaje, "No puedo abrir el fichero %s", FILE_CIFRADO);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        exit(1);
    }
    return;
}

/* cierraArchivosMensaje() Se encarga de cerrar los archivos para almacenar el texto plano y
 * el texto cifrado del proceso de cifrado en curso, una vez que dicho proceso
 * ha finalizado. Además comprueba la correcta clausura de los archivos y muestra
 * un error informativo en caso de error.
 * ptPlano (in) Manipulador del archivo de texto plano
 * ptCifrado	(in) Manipulador del archivo de texto cifrado
 */
void cierraArchivosMensaje(FILE *ptPlano, FILE *ptCifrado)
{
    char mensaje[100] = "No puedo cerrar el fichero %s";

    /* Chequeamos si no hemos podido cerrar el fichero */
    if(fclose(ptPlano) == EOF)
    {
        sprintf(mensaje, FILE_PLANO);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        exit(1);
    }
    if(fclose(ptCifrado) == EOF)
    {
        sprintf(mensaje, FILE_CIFRADO);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        exit(1);
    }
    return;
}

#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"

/* dibujaEstator() Dibuja en la interfaz de la versión 3 una representación esquemática del estator
 * o ETW. A nivel de código, el estator es una cadena de caracteres constante:
 *          ABCDEFGHIJKLMNOPQRSTUVWXYZ
 * Su única función es conectar la salida del stecker con la entrada del rotor derecho.
 */
void dibujaEstator(unsigned short dx)
{
    int i;

    /* Dibujamos la caja del estartor */
    dibuja_caja(31+dx,4,3,28);

    /* Dibujamos las letras */
    for (i=0; i < 26; i++)
    {
        gotoxy(32+dx,5+i);
        printf("%c", 'A'+i);
    }
    return;
}

/* dibujaStecker() Dibuja el stecker en pantalla, dentro de su caja que ya estaba pintada
 * stecker (in) Contiene el cableado del panel de conexiones (steckerbrett)
 * dx (in) Desplazamiento del stecker en la consola para los modelos M4.
 *  La posición del stecker no es la misma en los modelos M3 que en los modelos M4,
 *  ya que en estos últimos hay que dibujar un rotor más, con el consiguiente
 *  desplazamiento de los elementos a la derecha del grupo de rotores. Este parámetro
 *  se puede utilizar para indicar dicho desplazamiento.
 */
void dibujaStecker(char stecker[], unsigned short dx)
{
    int i;
    char c;

    /* Sólo pintamos los dos pares de letras del Stecker */
    for (i=0; i < 26; i++)
    {
        gotoxy(35 + dx,5 + i);
        c = 'A' + i;
        printf("%c%c  %c%c%c  ",(char) 179, c, 'A'+ getPos(c,stecker), c,(char) 179);  /* │A BC│  */
    }
    return;
}

/* opStecker() Implementa el comportamiento del panel de conexiones cuando se teclea una letra.
 * Recibe la posición que ocupa la letra tecleada, busca su conexión en el vector de conexiones,
 * y devuelve la posición de la letra a la que está conectada.
 * in (in) Posición de la entrada al Stecker
 * stecker (in) Vector con los pares de letras conectados
 * return Posición de la salida
 * Para poder pintar las letras del Stecker debemos distinguir si es en IDA o en VUELTA
 * Para no meter más parámetros utilizamos posin. Le sumamos 100 para marcar que es de vuelta
 */
unsigned short opStecker(unsigned short posin, char stecker[])
{
    unsigned short posout;

    /* La operacion es bien sencilla */
    posout = getPos('A' + posin, stecker);

    return posout;
}

/* fprintStecker() Escribe el stecker en el libro de claves
 * conex (in) Array de 26 elementos con las conexiones
 * ptr (in) Puntero a fichero donde escribir los 10 pares
 * Escribe en el archivo del libro de claves la parte correspondiente
 * a las conexiones del panel, siguiendo el formato que aparece en el ejemplo del libro de claves.
 *      | AH CY DE FO GI JU MW PX RS TV |
 */
void fprintStecker(char conex[], FILE *ptr)
{
    int i;

    /* Recorremos todos los pares... */
    for (i=0; i<26; i++)
    {
        /* ... pero de cada pareja diferente solo escribimos sólo uno de ellos */
        /* De esta forma sólo escribimos uno sólo de los pares, y cuando el par es diferente */
        if ((int) conex[i] - 'A' < i)
            fprintf(ptr, "%c%c ", conex[i], i + 'A');
    }
    return;
}

/* genStecker() Genera las 10 conexiones del genstecker
 * conexiones (out) Array de 26 elementos donde almacenar las conexiones
 * Genera una cadena de caracteres que contiene las conexiones establecidas
 * aleatoriamente utilizando la función rand().
 * Se pueden conectar hasta 13 pares de letras, aunque normalmente solo se
 * utilizaban 10 (utilizadas aqui). Las no conectadas apuntan a sí mismas
 */
void genStecker(char conexiones[])
{
    unsigned short numeros[26];
    int i;

    /* Se conectan todas consigo mismas para no dejar ninguna libre */
    for (i=0; i <26; i++)
        conexiones[i] = (char) i + 'A';

    conexiones[26] = '\0';

    /* Buscamos 10 posiciones aleatorias diferentes */
    /* PARES_STECKER = 10  en globals.h */
    genera_numeros(2*PARES_STECKER, numeros, 0, 25);

    /* Las conectamos de 2 en 2 */
    for (i= 0; i<PARES_STECKER; i++)
    {
        conexiones[(int) numeros[2*i]] = numeros[2*i+1] + 'A';
        conexiones[(int) numeros[2*i+1]] = numeros[2*i] + 'A';
    }
    return;
}

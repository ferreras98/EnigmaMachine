#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <unistd.h>   /* Para access() */

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"

/* getPos() Busca la posición de un caracter dentro de una cadena de caracteres
 * c (in) Caracter a buscar
 * y (out) Numero de fila (comienza en 0)
 * return Posición del caracter dentro de la cadena. 0 si no encontrado
 */
unsigned char getPos(char c, char cadena[])
{
    int i=0;

    while(cadena[i] !=  '\0')
    {
        if (cadena[i] == c)
            return (unsigned char) i; /* Si lo encuentra salimos */
        else
            i++;
    }
    return 0;
}

/* gotoxy() Posiciona el cursor en unas coordinadas determinadas de la pantalla
 * x (in) Numero de columna (comienza en 0)
 * y (in) Numero de fila (comienza en 0)
 * return void
 */
void gotoxy(int x, int y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    return;
}

/* setConsola() Cambia la consola al tamano y colores adecuados
 * cols (in) Numero de columnas de la ventana
 * fils (in) Numero de filas de la ventana
 */
void setConsola(int cols, int fils)
{
    char orden[100]; /* Para escribir los comandos completos para el system */

    /* Titulo de la consola */
    sprintf(orden, "title %s", TITULO_CONSOLA);
    system(orden);

    /* Tamaño de la consola */
    sprintf(orden, "mode con cols=%d lines=%d", cols, fils);
    system(orden);

    /* Colores del TEXTO (Negro) y del FONDO (VerdeClaro) */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_NORMAL);

    /* Para que toda la pantalla se pinte de los nuevos colores */
    system("cls");

    return;
}

/* unsetConsola() Deja la consola como nos la encontramos (tamaño y colores)
 */
void unsetConsola()
{
    /* Reajusta el tamaño */
    system("mode con cols=80 lines=25");

    /* Volvemos a poner la pantalla en Blanco y Negro */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    /* Limpia pantalla "clear screen"*/
    system("cls");
    return;
}

/*  dibuja_caja() Dibuja una caja simple, de dimensiones (anchoxalto) en la posición indicada
 *  posx (in) Coordenada x de la esquina superior izquierda
 *  posy (in) Numero de filas de la ventana superior izquierda
 *  ancho (in) Numero de columnas de la ventana
 *  alto (in) Numero de filas de la ventana
 */
void dibuja_caja(int posx, int posy, int ancho, int alto)
{
    char buffer[100];   /* Lo utilizamos para escribir líneas completas */
    int i;

    buffer[ancho] = '\0';  /* Caracter de fin de línea */

    /* Linea de arriba */
    buffer[0] = (char)218;            /* ┌ */
    for(i=1; i< ancho-1; i++)
        buffer[i] = (char)196;        /* ─ */
    buffer[ancho-1] = (char)191;      /* ┐ */
    gotoxy(posx, posy);
    printf(buffer);

    /* Linea de abajo */
    buffer[0] = (char)192;            /* └ */
    buffer[ancho-1] = (char)217;      /* ┘ */
    gotoxy(posx, posy+alto-1);
    printf(buffer);

    /* Lineas del centro */
    buffer[0] = buffer[ancho-1] = (char)179;  /* │ */
    for(i=1; i< ancho-1; i++)
        buffer[i] = ' ';
    for (i=1; i< alto-1; i++)
    {
        gotoxy(posx, posy+i);
        printf(buffer);
    }
}

/* dibuja_caja_doble() Pinta una caja doble, con una cabecera. Estilo de línea simples
 * Titulo (in) de la caja centrado en la primera línea (NULL si es sin título)
 * x (in) Coordenada x de la esquina superior izquierda
 * y (in) Coordenada y de la esquina superior izquierda
 * ancho (in) Ancho total de la caja
 * alto (in) Alto total de la caja
 *
 * Se corresponde con del texto plano y texto cifrado
 */
void dibuja_caja_doble(char *Titulo, int x, int y, int ancho, int alto)
{
    /* Aprovechamos a pintarlo con dos cajas simples */
    dibuja_caja(x, y, ancho, 3);
    dibuja_caja(x, y+2, ancho, alto-1);

    /* Corregimos los esquinas de la línea de unión de las dos cajas */
    gotoxy(x, y+2);
    printf("%c", (char) 195);     /* ├ */
    gotoxy(x+ancho-1,y+2);
    printf("%c", (char) 180);    /* ┤ */

    /* El titulo de la ventana si lo hubiera, en posicion centrada */
    if (Titulo != NULL)
    {
        gotoxy(x+(ancho - strlen(Titulo))/2, y+1);
        setlocale(LC_CTYPE, "Spanish");
        printf(Titulo);
        setlocale(LC_CTYPE, "C");
    }
    return;
}

/* buscar_numeros() Busca un número entero dentro de un vector de tamaño determinado.
 * numero (in) Numero a buscar
 * vector (in) Array de datos donde realilzar la búqueda
 * tamano (in) Numero de elementos que ya están en el array
 * return Si se encuentra devuelve 1. Si no está devuelve un 0.
 * Funcion auxiliar necesria para generar un vector de numeros aleatorios DISTINTOS
 */
int buscar_numeros(unsigned short numero, unsigned short vector[], int tamano)
{
    int i, encontrado = 0;

    /* Hasta que lo encontremos o nos quedemos sin numeros */
    for (i=0; i < tamano && encontrado == 0; i++)
    {
        if(vector[i] == numero)  /* Lo encontramos!! */
            encontrado = 1;
    }
    return encontrado;
}

/* genera_numeros() Genera n numeros aleatorios diferentes de valores entre minimo y maximo incluidos
 * n (in) Numero de elementos a generar
 * vector (out) Array donde guardar los numeros (debe ser superior a n)
 * minimo (in) Valor minimo de los elementos
 * maximo (in) Valor maximo de los elementos (< 127 porque se almacenan en char)
 * Utiliza la funcion buscar_numeros() para descartar los números ya utilizados
 */
void genera_numeros(int n, unsigned short vector[], int minimo, int maximo)
{
    int i, encontrado;
    unsigned short aleatorio;

    for (i=0; i< n; i++)
    {
        do
        {
            aleatorio = (unsigned short) minimo + rand()%(maximo - minimo + 1);
            encontrado = buscar_numeros(aleatorio, vector, i);
        }
        while(encontrado); /* Hasta que aleatorio no esté en la lista */
        /* Si pedimos más números que maximo - minino este bucle no termina */

        vector[i] = aleatorio;
    }
    return;
}


/* letrasStecker() Pinta de colores las letras del stecker segun la entrada y la salida
 * izq (in) Valor del elemento izquierdo del stecker
 * der (in) Valor del elemento derecho del stecker
 * dir (in) Dirección 0 hazia 1zq 1 hacia derecha
 * dx (in) Desplazamiento
 */
void letrasStecker(unsigned short izq, unsigned short der, bool dir, int dx)
{
    /* Cambiamos de color los caracteres */
    if(!dir)  /* Vamos de derecha a izquierda IDA*/
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CHARS_IDA);
    }
    else  /* Vamos de izquierda a derecha  VUELTA*/
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CHARS_VUE);
    }
    /* La letra del Estartor */
    gotoxy(8+dx, 5 + izq);
    printf("%c", 'A' + izq);

    /* La letra de la columna izquierda del Stecker */
    gotoxy(12+dx, 5 + izq);
    printf("%c", 'A' + izq);

    /* Las letras de la columna derecha del Stecker */
    gotoxy(15 + dx, 5 + der);
    printf("%c%c", 'A' + izq, 'A' + der);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_NORMAL);
    return;
}

/* letrasRotor() Pinta de colores las letras del rotor segun la entrada y la salida
 * rotor (in) Rotor
 * izq (in) Valor del elemento izquierdo del stecker
 * der (in) Valor del elemento derecho del stecker
 * dir (in) Dirección 0 hazia 1zq 1 hacia derecha
 * dx (in) Desplazamiento
 */
void letrasRotor(struct sRotor rotor, unsigned short izq, unsigned short der, bool dir, int dx)
{
    if(!dir)  /* Vamos de derecha a izquierda IDA */
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CHARS_IDA);
    }
    else  /* Vamos de izquierda a derecha  VUELTA */
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CHARS_VUE);
    }
    gotoxy(dx+4,der+5);   /* Los dos de la derecha del rotor */
    printf("%c%c",rotor.cableado[der],rotor.referencia[der]);

    gotoxy(dx+1,izq+5);  /* El de la izquierda del rotor */
    printf("%c", rotor.anillo[izq]);

    /* Lo dejamos como estaba */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_NORMAL);

    return;

}

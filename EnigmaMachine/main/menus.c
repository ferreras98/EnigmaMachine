#include <stdio.h>
#include <stdlib.h>     /* para system()*/
#include <conio.h>      /* para _getch()*/
#include <locale.h>     /* para setlocale()*/

#include "../main/globals.h"

#include "../auxiliar/auxiliar.h"
#include "../LibroClaves/libroClaves.h"
#include "../Maquina/cifrado.h"

/* menuPrincipal() Muestra las tres opciones del Menu Principal
 * return Opción seleccionada por el usuario '1' '2' '0'
 * Muestra las opciones generales:
 *      1. Cifrar/Descifrar mensaje
 *      2. Genera libro de claves.
 *      0. Salir del Programa.
 * Pide al usuario que seleccione una opción de las que le muestra y comprueba si el
 * valor introducido es correcto (corresponde a una de las opciones). Si la opción no
 * es válida sigue mostrandor el menú, hasta que el usuario selecciona una opción
 * correcta, o la opción de salida.
 */
char menuPrincipal()
{
    char opcion;

    /* Escribimos en menu con los caracteres en español */
    setConsola(50, 13);

    /* Pintamos las diferentes ventanas del menu principal*/
    dibuja_caja(0,0,50,3); /* Ventana de arriba */
    dibuja_caja(0,3,50,6); /* Ventana de enmedio, más grande */
    dibuja_caja(0,9,50,3); /* Ventana de abajo */

    setlocale(LC_CTYPE, "Spanish");
    gotoxy(2,1);
    printf("                Menú Principal");
    gotoxy(2,4);
    printf("1. Cifrar/descifrar mensaje");
    gotoxy(2,5);
    printf("2. Generar el libro de claves");
/*    gotoxy(2,6);
    printf("3. Mi otra opción"); */
    gotoxy(2,7);
    printf("0. Salir del programa");
    gotoxy(2,10);
    printf("Selecciona una opción:");
    setlocale(LC_CTYPE, "C");

    /* Leemos la opcion elegida */
    do
    {
        gotoxy(25,10);
        fflush(stdin); /* Limpia el buffer del teclado */
        opcion = _getch();
    }
    while (opcion != '1' && opcion != '2' && opcion != '0' /* && opcion != '3' */);

    return opcion;
}

/* gestionMenuPrincipal() Según el valor que elija el usuario, gestiona el Libro o dibuja la Interfaz
 * La función gestionMenuPrincipal()llama a la función menuPrincipal() para que el usuario
 * elija una opción; a partir de esta opción selecciona y llama a la función correspondiente a dicha
 * opción.
 * En la siguiente tabla se indican las funciones asociadas a cada opción:
 *         Opción                   inicCifrado()
 *      1. Cifrar/Descifrar mensaje dibujaInterfaz()
 *      2. Genera libro de claves.  generaLibroClaves()
 *      0. Salir del Programa.      Finaliza el programa
 */
void gestionMenuPrincipal()
{
    system("cls");  /* Clear Screen */
    while (1)
    {
        switch (menuPrincipal())
        {
        case '1':
            inicCifrado();
            continue;
        case '2':
            generaLibroClaves();
            continue;
/*        case '3':
            otrafuncion();
            continue; */
        /* Salida */
        case '0':
        default:
            /* Dejamos la consola como nos la encontramos */
            unsetConsola();
            return;
        }
    }
}

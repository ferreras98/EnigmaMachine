#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../main/menus.h"

/*********************************************************************
*
* Proyecto Integrador de la asignatura de Programaci�n v3.
* Curso 2016/17
*
* Consiste en la simulaci�n de la m�quina Enigma, con las siguientes
* limitaciones debidas al dispositivo utilizado:
*       �La interfaz ser� todo lo amigable y atractiva que permitan las
*        caracter�sticas de la consola MS-DOS (tambi�n denominada s�mbolo
*        del sistema).
*       �Los datos utilizados por la aplicaci�n (en adelante, base de
*        datos) no se almacenan en una base de datos como tal (del tipo
*        MySql, Access, etc.), sino que es un simple conjunto de archivos
*        de datos, tanto binarios como de texto.
* La aplicaci�n se desarrolla en una interfaz en la que el usuario puede
* interaccionar con la aplicaci�n.
*
* En esta tercera versi�n, se incorpora:
* - Creaci�n de varias estructuras de datos
* - Definici�n de varios vectores de registros
* - Utilizaci�n de un vector din�mico para almacenar los rotores
* - Utilizaci�n de archivos binarios para almacenar la configuraci�n diar�a de cada modelo
*
*********************************************************************/

/* main()*/
int main()
{
    /* Inicializamos el generador de n�meros aleatorios */
    srand(time(NULL));

    gestionMenuPrincipal();

    return 0;
}

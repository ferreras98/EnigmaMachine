#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../main/menus.h"

/*********************************************************************
*
* Proyecto Integrador de la asignatura de Programación v3.
* Curso 2016/17
*
* Consiste en la simulación de la máquina Enigma, con las siguientes
* limitaciones debidas al dispositivo utilizado:
*       •La interfaz será todo lo amigable y atractiva que permitan las
*        características de la consola MS-DOS (también denominada símbolo
*        del sistema).
*       •Los datos utilizados por la aplicación (en adelante, base de
*        datos) no se almacenan en una base de datos como tal (del tipo
*        MySql, Access, etc.), sino que es un simple conjunto de archivos
*        de datos, tanto binarios como de texto.
* La aplicación se desarrolla en una interfaz en la que el usuario puede
* interaccionar con la aplicación.
*
* En esta tercera versión, se incorpora:
* - Creación de varias estructuras de datos
* - Definición de varios vectores de registros
* - Utilización de un vector dinámico para almacenar los rotores
* - Utilización de archivos binarios para almacenar la configuración diaría de cada modelo
*
*********************************************************************/

/* main()*/
int main()
{
    /* Inicializamos el generador de números aleatorios */
    srand(time(NULL));

    gestionMenuPrincipal();

    return 0;
}

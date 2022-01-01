#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"

#include "../Maquina/Enigma.h"

/* dibujaInterfazM4() Fija las dimensiones de la consola para ser utilizada como interfaz de la
 * aplicación, y dibuja todas las ventanas necesarias para la interacción del
 * operador con los modelos M4 de la máquina Enigma.
 * Se ha ampliado para que pueda dibujar máquinas de 4 o más rotores (hasta 8), según se indique
 * en el parámetro MAX_ROTORS globals.h
 */
void dibujaInterfazM4(void)
{
    int i;
    int ncol;
    int dx;
    char buffer[200]; /* Pintamos por líneas enteras */

    /* Desplazamiento de los elementos a la derecha de los rotores */
    dx  = 8;

    /* Ponemos el tamaño de la ventana */
    ncol = NCOL + dx;
    setConsola(ncol, NFIL+1);

    /* DIBUJAMOS EL MARCO DOBLE */
    buffer[ncol] = '\0'; /* El caracter de fin de string*/

    /* Linea superior */
    buffer[0] = (char) 201;                   /* ╔ */
    for (i=1; i<ncol-1; i++)
        buffer[i]=(char) 205;                 /* ═ */
    buffer[ncol-1] = (char) 187;              /* ╗ */
    gotoxy(0,0);
    printf(buffer);

    /* Linea inferior, utilizamos los caracteres del medio que ya están puestos */
    buffer[0] = (char) 200;                    /* ╚ */
    buffer[ncol-1] = (char) 188;             /*  ╝ */
    gotoxy(0,NFIL-1);
    printf(buffer);

    /* Linea media */
    buffer[0] = buffer[ncol-1] = (char) 186;  /* ║ */
    for(i=1; i<ncol-1; i++)
        buffer[i] = ' ';        /* Dibujamos el hueco */
    for (i=1; i < NFIL-1; i++)
    {
        gotoxy(0,i);
        printf(buffer);
    }

    /* El titulo de la ventana centrado en el espacio */
    gotoxy((ncol -strlen(TITULO_VENTANA04))/2, 0);  /* String length */
    setlocale(LC_CTYPE, "Spanish");
    printf(TITULO_VENTANA04);
    setlocale(LC_CTYPE, "C");

    /* Colocamos las letras (tanteando) UKW ETW STECKER */
    gotoxy(3,1);
    printf("U");
    gotoxy(3,2);
    printf("K");
    gotoxy(3,3);
    printf("W");
    gotoxy(32+dx,1);
    printf("E");
    gotoxy(32+dx,2);
    printf("T   STE-");
    gotoxy(32+dx,3);
    printf("W   CKER");

    /* Reflector (siempre en el mismo sitio) */
    dibuja_caja(1,4,5,28);
    dibuja_caja(1,32,5,3);

    /* Rotores */
    for (i=0; i < 4; i++)
    {
        dibuja_caja(9+i*8,1,3,3);     /* Caja arriba - Grundstellung */
        dibuja_caja(7+i*8,4,7,28);    /* Caja del Rotor */
        dibuja_caja(7+i*8,32,7,3);    /* Caja abajo - Tipo */
    }
    /* Cajas para el reflector, rotores, estartor y stecker */
    dibuja_caja(31+dx,4,3,28);   /* Estartor */
    dibuja_caja(35+dx,4,7,28);   /* Stecker */

    /* Las dos cajas para la entrada de datos */
    dibuja_caja(45+dx,1,18,3); /* La del mensaje "Introduce letra "*/
    dibuja_caja(63+dx,1,3,3); /* La entrada propiamente dicha */

    /* Las dos ventanas con el texto plano y el cifrado */
    dibuja_caja_doble(TITULO_VENTANA6, 44+dx,  5, 35, 13);
    dibuja_caja_doble(TITULO_VENTANA7, 44+dx, 20, 35, 13);

    /* Pedimos datos de entrada */
    gotoxy(46+dx,2);
    printf("Introduce letra");

    /* Texto con la indicación de Escape para terminar */
    gotoxy(66+dx,2);
    printf(TEXTO_FIN, (char) 175, (char) 175);
    return;

}

/* dibujaInterfazM3() Fija las dimensiones de la consola para ser utilizada como interfaz de la
 * aplicación, y dibuja todas las ventanas necesarias para la interacción del
 * operador con los modelos M3 de la máquina Enigma.
 * Se ha ampliado para que pueda dibujar máquinas de 3 o menos rotores (hasta 0), según se indique
 * en el parámetro MIN_ROTORS de globals.h
 */
void dibujaInterfazM3(void)
{
    int i;
    int ncol;
    int dx;
    char buffer[200]; /* Pintamos por líneas enteras */

    /* Desplazamiento de los elementos a la izquierda de los rotores */
    dx = 0;

    /* Creamos la ventana en su tamaño */
    ncol = NCOL + dx; /* Columnas de la ventana */
    setConsola(ncol, NFIL+1);

    /* DIBUJAMOS EL MARCO DOBLE */
    buffer[ncol] = '\0'; /* El caracter de fin de string*/

    /* Linea superior */
    buffer[0] = (char) 201;                   /* ╔ */
    for (i=1; i<ncol-1; i++)
        buffer[i]=(char) 205;                 /* ═ */
    buffer[ncol-1] = (char) 187;              /* ╗ */
    gotoxy(0,0);
    printf(buffer);

    /* Linea inferior, utilizamos los caracteres del medio que ya están puestos */
    buffer[0] = (char) 200;                    /* ╚ */
    buffer[ncol-1] = (char) 188;             /*  ╝ */
    gotoxy(0,NFIL-1);
    printf(buffer);

    /* Linea media */
    buffer[0] = buffer[ncol-1] = (char) 186;  /* ║ */
    for(i=1; i<ncol-1; i++)
        buffer[i] = ' ';        /* Dibujamos el hueco */
    for (i=1; i < NFIL-1; i++)
    {
        gotoxy(0,i);
        printf(buffer);
    }
    /* El titulo de la ventana centrado en el espacio */
    gotoxy((ncol -strlen(TITULO_VENTANA03))/2, 0);  /* String length */
    setlocale(LC_CTYPE, "Spanish");
    printf(TITULO_VENTANA03);
    setlocale(LC_CTYPE, "C");

    /* Colocamos las letras tanteando */
    gotoxy(3,1);
    printf("U");
    gotoxy(3,2);
    printf("K");
    gotoxy(3,3);
    printf("W");
    gotoxy(32+dx,1);
    printf("E");
    gotoxy(32+dx,2);
    printf("T   STE-");
    gotoxy(32+dx,3);
    printf("W   CKER");

    /* Reflector */
    dibuja_caja(1,4,5,28);    /* Reflector */
    dibuja_caja(1,32,5,3);    /* Reflector */

    for (i=0; i < 3; i++)  /* Para cada uno de los tres rotores */
    {
        dibuja_caja(9+i*8,1,3,3);     /* Cajita para el Grundstellung */
        dibuja_caja(7+i*8,4,7,28);    /* Caja grande del rotor */
        dibuja_caja(7+i*8,32,7,3);    /* Caja pequeña abajo para el tipo de rotor */
    }

    /* Las dos cajas para la entrada de datos */
    dibuja_caja(45+dx,1,18,3); /* La del mensaje "Introduce letra "*/
    dibuja_caja(63+dx,1,3,3); /* La entrada propiamente dicha */

    /* Cajas para el reflector, rotores, estartor y stecker */
    dibuja_caja(31+dx,4,3,28);   /* Estartor */
    dibuja_caja(35+dx,4,7,28);   /* Stecker */

    /* Las dos ventanas con el texto plano y el cifrado */
    dibuja_caja_doble(TITULO_VENTANA6, 44+dx,  5, 35, 13);
    dibuja_caja_doble(TITULO_VENTANA7, 44+dx, 20, 35, 13);

    /* Pedimos datos de entrada */
    gotoxy(46+dx,2);
    printf("Introduce letra");

    /* Texto con la indicación de Escape para terminar */
    gotoxy(66+dx,2);
    printf(TEXTO_FIN, (char) 175, (char) 175);
    return;
}

/* printLetraPlana() Realiza dos operaciones: Muestra la letra que recibe como parámetro
 * en la ventana "Texto plano" de la interfaz.Escribe la letra que recibe como
 * parámetro en el archivo de texto para el texto plano.
 * letra (in) Caracter a escribir
 * ptPlano (in) Puntero a fichero donde escribir las letras
 * Los mensajes cifrados con Enigma tenían una longitud máxima de 250 letras.
 * Esta función tiene que controlar este tamaño. En caso de que se supere,
 * mostrará un mensaje informando de tal circunstancia. Además, las letras se
 * agrupaban en grupos de 5 letras, y en filas de 5 grupos. Si el puntero de
 * fichero es NULL reseteamos el contador de letras
 * Para inicializar el contador, llamamos a la ventana con ptPlano == NULL
 *  en letra le mandamos el desplazamiento para escribir la ventana
 */
unsigned char printLetraPlana(char letra, FILE *ptPlano)
{
    /* Para llevar la cuenta de cuantas letras llevamos */
    static int impresas = 0;
    static unsigned short dx = 0; /* Desplazamiento */

    /* Posicion de la primera letra */
    int posx = 23;
    int posy = 8;

    /* Si el puntero a fichero es NULL reseteamos el contador */
    if (ptPlano == NULL)
    {
        impresas = 0;
        dx = (unsigned short) letra; /* Al resetear nos dicen en que modelo estamos */
        return impresas;
    }

    if (impresas >= MAX_LETRAS)  /* MAX_LETRAS 250 */
    {
        /* Llegamos al límite de letras. Mensaje de error y salida */
        gotoxy(40+dx,2); /* Para que no este el cursor perdidopor ahí */
        MessageBox(NULL, TEXTO_ERROR1, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        return impresas+1;
    }

    posx += dx;

    /* Posicion de la proxima letra, con huecos cada 5 letras (BLOQUE)
    y líneas de 25 letras (LINEA)*/
    gotoxy(posx + (impresas%LINEA) + (impresas%LINEA)/BLOQUE, posy + impresas/LINEA);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXTO_PLANO);
    printf("%c", letra);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_NORMAL);
    fprintf(ptPlano,"%c", letra);
    impresas++;

    if (impresas % LINEA == 0)
    {
        fprintf(ptPlano,"%c", '\n');
    }
    else if (impresas % BLOQUE==0)
    {
        fprintf(ptPlano,"%c", ' ');
    }
    return impresas;
}

/* printLetraCifrada() Realiza dos operaciones: Muestra la letra que recibe como parámetro
 * en la ventana "Texto cifrado" de la interfaz.Escribe la letra que recibe como
 * parámetro en el archivo de texto para el texto cifrado.
 * letra (in) Caracter a escribir
 * ptPlano (in) Puntero a fichero donde escribir las letras
 * Los mensajes cifrados con Enigma tenían una longitud máxima de 250 letras.
 * Esta función tiene que controlar este tamaño. En caso de que se supere,
 * mostrará un mensaje informando de tal circunstancia. Además, las letras se
 * agrupaban en grupos de 5 letras, y en filas de 5 grupos. Si el puntero de
 * fichero es NULL reseteamos el contador de letras, y en letra tenemos el desplazamiento
 */
void printLetraCifrada(char letra, FILE *ptCifrado)
{
    /* Para llevar la cuenta de cuantas letras llevamos */
    static int impresas = 0;
    static int dx = 0;

    /* Posicion de la primera letra */
    int posx = 23;
    int posy = 23;

    /* Mecanismo para resetear el contador de letras */
    if (ptCifrado == NULL)
    {
        impresas = 0;
        dx = (unsigned short) letra;
        return;
    }

    posx += dx;
    /* Posicion de la proxima letra, con huecos cada 5 letras (BLOQUE)
    y líneas de 25 letras (LINEA) */
    gotoxy(posx + (impresas%LINEA) + (impresas%LINEA)/BLOQUE,
           posy + impresas/LINEA);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXTO_CIFRADO);
    printf("%c", letra);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_NORMAL);
    fprintf(ptCifrado,"%c", letra);
    impresas++;
    if (impresas % LINEA == 0)
    {
        fprintf(ptCifrado,"%c", '\n');
    }
    else if (impresas % BLOQUE==0)
    {
        fprintf(ptCifrado,"%c", ' ');
    }
    return;
}

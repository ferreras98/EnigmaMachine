#include <stdio.h>
#include <windows.h>
#include <ctype.h>

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"

#include "../Maquina/reflector.h"

/* leeCfgReflector() Dibuja en la interfaz de la versi�n 3 una representaci�n
 * esquem�tica del reflector de la m�quina Enigma.
 * archivo (in) Nombre del archivo que contiene el cableado del reflector.
 * reflector (in out) Registro de tipo struct sReflector que contiene los datos del reflector utilizado
 * El archivo de configuraci�n contiene el cableado de los modelos B y C:
 *          B YRUHQSLDPXNGOKMIEBFZCWVJAT
 *          C FVPJIAOYEDRZXWGCTKUQSBNMHL
 * La funci�n lee el cableado del modelo de reflector que recibe en el par�metro modelo.
 */
void leeCfgReflector(char archivo[], struct sReflector *pRef)
{
    FILE *ptr;
    char type, mensaje[100];

    /* Por si no pudi�ramos leer del fichero */
    struct sReflector conf_defecto[2] =
    {
        {"YRUHQSLDPXNGOKMIEBFZCWVJAT", 'B'},
        {"FVPJIAOYEDRZXWGCTKUQSBNMHL", 'C'}
    };

    ptr = fopen(archivo, "rt");
    if (ptr == NULL) /* En caso de que no podamos abrir el archivo utilizamos valores por defecto */
    {
        sprintf(mensaje, "No puedo abrir el fichero %s\n Utilizo valores por defecto", archivo);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
        if (pRef->modelo == 'B')  /* Si hubiera m�s con un switch */
        {
            strcpy(pRef->conexiones, conf_defecto[0].conexiones);
        } else {
            strcpy(pRef->conexiones, conf_defecto[1].conexiones);
        }

    }
    else
    {
        do  /* leemos lineas hasta la que tiene el tipo que queremos */
        {
            fscanf(ptr,"%c %s\n",&type, pRef->conexiones);
        }
        while (type != pRef->modelo);
    }
    fclose(ptr);
    return;
}

/* dibujaReflector() Dibuja en la interfaz de la versi�n 2 una representaci�n
 * esquem�tica del reflector de la m�quina Enigma
 * reflector (in) Vector con la configuracion del reflector
 * A nivel de c�digo, un reflector est� compuesto por una cadena de caracteres, que almacena
 * las conexiones de los contactos de dicho elemento:
 *            (cableado): YRUHQSLDPXNGOKMIEBFZCWVJAT
 * Esta cadena relaciona cada elemento con la letra a la que est� conectado. De estaa forma,
 * la 'A' est� conectada con la Y, la B con la R, la C con la U, etc.
 * La funci�n muestra la cadena con las conexiones y otra cadena constante que se utiliza
 * como referencia. Aunque en esta versi�n solo se utiliza el reflector modelo B, tambi�n se
 * indica el modelo debajo de la representaci�n del reflector.
 */
void dibujaReflector(struct sReflector reflector)
{
    int i;

    /* Dibujamos las dos l�neas de caracteres */
    for (i=0; i < 26; i++)
    {
        gotoxy(2,5+i);
        printf(" %c%c%c ", 'A' + i, 'A' + getPos('A'+i,reflector.conexiones), (char) 179);
    }
    /* Ponemos el tipo de reflector en la ventana de abajo */
    gotoxy(3,33);
    printf("%c", reflector.modelo);
    return;
}

/* opReflector() Implementa el cifrado que realiza el reflector.
 * in (in) Posici�n del contacto de entrada (n�mero del intervalo [0, 25])
 * reflector	(in) Registro de tipo struct sReflector que contiene los datos del reflector utilizado
 * return posici�n del contacto de salida (n�mero del intervalo [0, 25])
 * Recibe la posici�n del contacto de entrada, y devuelve la posici�n correspondiente al contacto
 * de salida, que se calcula a partir del vector de conexiones.
 * PINTA LAS LETRAS CORRESPONDIENTES DE COLORES
 */
unsigned short opReflector(unsigned short in, struct sReflector reflector)
{
    unsigned short out;

    /* Operaci�n es muy f�cil */
    out = getPos(in + 'A', reflector.conexiones);

    /* Colores a los contactos de entrada y salida */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CHARS_IDA);
    gotoxy(3,5+in);   /* El de entrada */
    printf("%c%c", 'A' + in, 'A' + getPos('A'+in,reflector.conexiones));

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CHARS_VUE);
    gotoxy(3,5+out);  /* El de salida */
    printf("%c%c", 'A' + out, 'A' + getPos('A'+out,reflector.conexiones));

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_NORMAL);

    return out;
}

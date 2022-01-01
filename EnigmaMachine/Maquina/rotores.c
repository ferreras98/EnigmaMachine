#include <stdio.h>
#include <stdlib.h> /* Para rand() */
#include <string.h> /* Para strcpy() */
#include <windows.h>
#include <ctype.h>

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"
#include "../Maquina/Enigma.h"

/* avanzaRotores() Simula el avance de los rotores antes de pulsar la tecla
 * pListaRotores (in - out) Puntero a direcci�n de una lista de rotores.
 * Tiene en cuenta el avance de los siguientes rotores de acuerdo con las muescas
 * y el avance del "Double Step"
 */
void avanzaRotores(struct sListaRotores *pListaRotores)
{
    int num;

    num = pListaRotores->num;  /* Para no escribir tanto */

    /* Mecanismo de double step: el rotor de 3� por la izq est� en la posicion de arriba */
    if (num > 3)
    {
        if ((pListaRotores->rotor[num-3].anillo[0]==pListaRotores->rotor[num-3].muesca[0])
                || (pListaRotores->rotor[num-3].anillo[0]==pListaRotores->rotor[num-3].muesca[1]))
        {
            pasoRotor(&pListaRotores->rotor[num-3],1);
            pasoRotor(&pListaRotores->rotor[num-4],1);
        }
    }

    if (num > 2) /* Seguimos con el mecanismo de double step ahora para el 2� por la izquierda */
    {
        if ((pListaRotores->rotor[num-2].anillo[0]==pListaRotores->rotor[num-2].muesca[0])
                || (pListaRotores->rotor[num-2].anillo[0]==pListaRotores->rotor[num-2].muesca[1]))
        {
            pasoRotor(&pListaRotores->rotor[num-2],1);
            pasoRotor(&pListaRotores->rotor[num-3],1);
        }
    }

    /* Avanzamos el rotor de la derecha una posici�n  */
    if (num >0)
        pasoRotor(&pListaRotores->rotor[num-1], 1);

    /* Vemos si toca avanzar a los siguientes rotores */
    if ((num > 1) &&
            ((pListaRotores->rotor[num-1].anillo[25]==pListaRotores->rotor[num-1].muesca[0]) ||
             (pListaRotores->rotor[num-1].anillo[25]==pListaRotores->rotor[num-1].muesca[1])))
    {
        pasoRotor(&pListaRotores->rotor[num-2],1);
    }

    /* NOTA: No est� implementado el avance de los rotores > 4 cuando el n�mero es superior a 4 */
    return;
}

/* configRotores() Establece la configuraci�n inicial de los rotores
 * lista (in out) Contiene la direcci�n de una lista de rotores
 * config (in) Registro con la configuraci�n de la m�quina Enigma
 */
void configRotores(struct sListaRotores* lista, struct sConfig config)
{
    int i;

    /* Para cada uno de los rotores */
    for (i=0; i<config.numRotores; i++)
    {
        /* Configuramos el ringenstellung de los rotores */
        configAnillos(&lista->rotor[i], config.ringS[i]);
        /* Les movemos a la posici�n inicial segun el Grundstellung */
        pasoRotor(&lista->rotor[i], config.grund[i] - 'A');
    }
    return;
}

/* dibujaRotor(() Dibuja en la interfaz de la versi�n 2 una representaci�n esquem�tica de
 * un rotor de la m�quina Enigma
 * rotor (in) Registro de tipo struct sRotor que contiene los datos del rotor.
 * coordRotor (in) Registro de tipo struct sPunto que contiene las coordenadas
 * del rotor en la interfaz gr�fica
 *
 * A nivel de c�digo, un rotor est� compuesto por tres cadenas de caracteres:
 *      anillo    : ABCDEFGHIJKLMNOPQRSTUVWXYZ
 *      cableado  : EKMFLGDQVZNTOWYHXUSPAIBRCJ
 *      referencia: ABCDEFGHIJKLMNOPQRSTUVWXYZ
 * La primera cadena (anillo) representa el anillo m�vil del rotor, que conecta
 * al mismo con el elemento que tiene a su izquierda (rotor o reflector).
 * La segunda cadena (cableado) representa el cableado del rotor, que conecta los
 * conectores de la derecha con los del anillo.
 * La tercera cadena (referencia) se utiliza como referencia para establecer la posici�n
 * relativa entre el rotor y su anillo.
 * La funci�n muestra las tres cadenas en vertical, la letra que se muestra en la ventanilla,
 * y un r�tulo con el n�mero (romano) que identifica a cada rotor.
 */
void dibujaRotor(struct sRotor rotor, struct sPunto coordRotor)
{
    int i;

    /* Escribimos la posicion actual en la ventana de arriba */
    gotoxy(coordRotor.x+3, coordRotor.y - 2);
    printf("%c",rotor.anillo[0]);

    /* Escribimos las dos cadenas de caracteres de la derecha*/
    for (i=0; i < 26; i++)
    {
        gotoxy(coordRotor.x, coordRotor.y+i+1);
        printf("%c   %c%c%c ", (char) 179, rotor.cableado[i], rotor.referencia[i], (char) 179);
    }

    /* Escribimos en otro fondo la cadena de caracteres de la izquierda */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_ROTOR);
    for (i=0; i < 26; i++)
    {
        gotoxy(coordRotor.x+1, coordRotor.y+i+1);
        printf("%c", rotor.anillo[i]);
    }

    /* Dibujamos la(s) muesca(s) en su color */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_MUESCA);

    /* Siempre tienen una muesca...  */
    i = getPos(rotor.muesca[0], rotor.anillo);
    gotoxy(coordRotor.x+1, coordRotor.y+i+1);
    if(isalpha(rotor.muesca[0])) /*... Excepto los rotores  gamma y beta */
        printf("%c", rotor.muesca[0]);

    /* Pero algunos rotores tienen dos muescas */
    if (rotor.muesca[1] != '\0')
    {
        i = getPos(rotor.muesca[1], rotor.anillo);
        gotoxy(coordRotor.x+1, coordRotor.y+i+1);
        if (isalpha(rotor.muesca[1]))
            printf("%c", rotor.muesca[1]);
    }

    /* Escribimos el tipo de rotor en la parte de abojo en numeracion romana */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_NORMAL);

    gotoxy(coordRotor.x+1, 33);
    switch(rotor.num)
    {
    case 1:
        printf("%s", "  I  ");
        break;
    case 2:
        printf("%s", " II  ");
        break;
    case 3:
        printf("%s", " III ");
        break;
    case 4:
        printf("%s", " IV  ");
        break;
    case 5:
        printf("%s", "  V  ");
        break;
    case 6:
        printf("%s", " VI  ");
        break;
    case 7:
        printf("%s", " VII ");
        break;
    case 8:
        printf("%s", "VIII ");
        break;
    case 9:
        printf("%s", "Beta ");
        break;
    case 10:
        printf("%s", "Gamma");
        break;
    }
}

/* genWalzenlage() un n�mero de rotores diferentes entre 1 y 8
 * nrotor Numero de rotores a generar (M3:3, M4: 4)
 * rotores Array donde almacenar los n�meros (entre 1 y 8) generados
 * Utiliza la funci�n genera_numeros para seleccionar, aleatoriamente, un n�mero de
 * rotores DIFERENTES entre los 8 disponibles igual al par�metro nrotor que recibe
 * En el caso del Modelo M3,este valor ser� de 3, y en el caso del modelo M4, de 4.
 * Para almacenar los rotores seleccionados se utiliza una cadena de caracteres, almacenando el n�mero de
 * rotor en cada uno de los elementos de la cadena de caracteres. Para el Walzenlage del ejemplo (II IV III),
 * la codificaci�n ser�a:
 *     Elemento: 0 1 2
 *     Valor   : 2 4 3
 */
void genWalzenlage(int nrotor, unsigned short rotores[])
{
    genera_numeros(nrotor, rotores, 1, 8);
    if (nrotor == 4)
        rotores[0] = rand() % 2 + 9;
    return;
}

/* pasoRotor() Implementa el avance paso a paso de un rotor
 * pRotor (in out) Direcci�n de un registro de tipo struct sRotor, que contiene los atributos de un rotor
 * num (in) Numero de pasos a avanzar
 *
 * Implementa el avance paso a paso de un rotor:
 * 1. Operaci�n manual que realiza el operador de Enigma para colocar la letra indicada
 * en el grundstellung en la ventanilla correspondiente.
 * 2. Avance autom�tico que se produce en el proceso de cifrado.
 * A nivel de c�digo, este comportamiento se implementa con un algoritmo de recorrido
 * que desplaza simultaneamente las tres cadenas de caracteres que representan al rotor
 */
void pasoRotor(struct sRotor *pRotor, int num)
{
    char buffer[27]; /* Para copiar las cadenas de caracteres */
    int j;

    strcpy(buffer, pRotor->anillo); /* La guardamos */
    for(j=0; j< 26; j++)
        pRotor->anillo[j] = buffer[(j+num)%26]; /* La desplazamos */

    strcpy(buffer, pRotor->cableado);
    for(j=0; j< 26; j++)
        pRotor->cableado[j] = buffer[(j+num)%26]; /* La desplazamos */

    strcpy(buffer, pRotor->referencia);
    for(j=0; j< 26; j++)
        pRotor->referencia[j] = buffer[(j+num)%26]; /* La desplazamos */

    return;
}

/* configAnillos() Implementa la configuraci�n del anillo de un rotor
 * pRotor (in out) Direcci�n de un registro de tipo struct sRotor, que contiene los atributos de un rotor
 * pos (in) Posici�n del anillo indicado por el ringstellung (n�mero entero del intervalo [0, 25])
 *
 * El proceso se puede hacer en dos etapas:
 * 1. Se desplaza la cadena correspondiente al anillo tantas posiciones como indique el ringstellung, manteniendo
 * las otras dos fijas
 * 2. Se actualizan las conexiones aplicando el algoritmo propuesto.
 */
void configAnillos(struct sRotor *pRotor, int pos)
{
    char buffer[30];
    int i;

    strcpy(buffer, pRotor->cableado);

    pos--;

    /* No lo entiendo muy bien, pero entre la explicacion y el ejemplo del profe ... */
    for (i=0; i < 26; i++)
    {
        pRotor->cableado[i] = 'A' + ((buffer[(i-pos+26)%26] - 'A') + pos)%26;
        pRotor->referencia[i] = 'A' + (i-pos+26)%26;
    }
    return;
}

/* opRotor() Devuelve la posici�n del contacto de salida del rotor
 * in (in) Posici�n del contacto de entrada al rotor
 * rotor (in) Registro que contiene los atributos de un rotor
 * coord (in) Coordenadas del rotor (par�metro a�adido respecto a las especificaciones)
 * return (out) Posici�n de salida
 * Ruta de ida
 * El rotor de la derecha (rotor I) recibe la se�al el�ctrica del estator en la posici�n 0. En ese
 * momento esa posici�n corresponde a la letra Y, que, seg�n la cadena del cableado, est� conectada
 * a la letra F del anillo. Por lo tanto, la funci�n tiene que buscar la letra F en la cadena del
 * anillo, y devolver dicha posici�n, que ser� la entrada para el siguiente rotor.
 * Ruta de vuelta
 * En el camino de vuelta la operaci�n es justamente la contraria de la realizada en el camino de ida.
 * Ahora la posici�n que recibe el rotor es la que le env�a el elemento que tiene a su izquierda (rotor
 * o reflector). En este caso el valor recibido que corresponde a posici�n en la cadena del anillo, y
 * la funci�n tiene que buscar dicha letra en la cadena cableado, y ese es el valor que tiene que
 * devolver la funci�n
 */
unsigned short opRotor(unsigned short in, struct sRotor rotor, bool dir)
{
    unsigned short out;
    char caracter;

    /* La parte de operaci�n es bastante f�cil, depende de la direcci�n */
    if (!dir)   /* Si es a la ida */
    {
        caracter = rotor.cableado[in];
        out = getPos(caracter, rotor.anillo);
    }
    else        /* Si es a la vuelta */
    {
        caracter = rotor.anillo[in];
        out = getPos(caracter, rotor.cableado);
    }
    return out;
}

/* leeCfgRotor() Lee la configuraci�n de cada rotor
 * numRotor (in) Tipo de rotor a leer; walzen (1 a 8)
 * rotor (out) Informaci�n del rotor
 * muesca (out) Posici�n de las muescas
 */
void  leeCfgRotor(struct sRotor *pRotor)
{
    FILE *ptr;
    char mensaje[100];
    int i;

    struct sCfgRotor  /* Configuraciones por defecto de los rotores; por si no abre cableadoRotores.cfg */
    {
        char cableado[27];
        char muesca[3];
    } conf_defecto[10] =
    {
        {"EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q"},
        {"AJDKSIRUXBLHWTMCQGZNPYFVOE", "E"},
        {"BDFHJLCPRTXVZNYEIWGAKMUSQO", "V"},
        {"ESOVPZJAYQUIRHXLNFTGKDCMWB", "J"},
        {"VZBRGITYUPSDNHLXAWMJQOFECK", "Z"},
        {"JPGVOUMFYQBENHZRDKASXLICTW", "ZM"},
        {"NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM"},
        {"FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM"},
        {"LEYJVCNIXWPBQMDRTAKZGFUHOS", "--"},
        {"FSOKANUERHMBTIYCWLQPZXVGJD", "--"}
    };


    ptr = fopen(FILE_ROTORES, "rt");
    if (ptr == NULL)
    {
        sprintf(mensaje, "No puedo abrir el fichero %s\n Utilizo configuraci�n por defecto", FILE_ROTORES);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
        strcpy(pRotor->cableado, conf_defecto[(pRotor->num) - 1].cableado);
        strcpy(pRotor->muesca, conf_defecto[(pRotor->num) - 1].muesca);
    }
    else         /* Leemos todos, pero s�lo nos quedamos con el �ltimo (el numRotor)*/
    {
        for (i=0; i < pRotor->num; i++)
        {
            fscanf(ptr, "%s %s\n", pRotor->cableado, pRotor->muesca);
        }
    }

    strcpy(pRotor->anillo, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");   /* Ponemos configuraci�n estandar en la referencia */
    strcpy(pRotor->referencia, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");   /* Ponemos configuraci�n estandar en la referencia */

    fclose(ptr);
    return;
}

/* fprintRingstellung() Escribe el Ringstellung en los Libros de claves
 * nrotor Numero de rotores a generar (M3:3, M4: 4)
 * ring Array donde almacenar los n�meros (entre 1 y 26) generados
 * ptr Puntero a fichero donde escribir
 * Escribe en los archivos de libros de claves la parte correspondiente a la
 * configuraci�n de los anillos de los rotores (ringstellung), siguiendo el
 * formato: |  11 10 17  |. En esta version siempre tres rotores.
 */
void fprintRingstellung(int nrotor, unsigned short ring[], FILE *ptr)
{
    int i;

    /* El 0 del formato es para que lo rellene con un 0 cuando s�lo tiene un d�gito */
    for (i=0; i< nrotor; i++)
    {
        fprintf(ptr, "%02d ", (int) ring[i]);
    }
    return;
}

/* genRingstellung(() Genera una configuraci�n aletoria para la configuraci�n inicial de los rotores
 * nrotor Numero de rotores
 * ring Vector para almacenar la configuraci�n
 * Utiliza la funci�n de biblioteca rand() para seleccionar, aleatoriamente,
 * la posici�n inicial del anillo cada uno de los rotores. El n�mero de rotores viene dado por el par�metro
 * nrotor. La posici�n debe ser un n�mero entre 1 y 26, ya que esas son las posibles posiciones (una por
 * cada letra) de cada anillo.
 * Para almacenar las posiciones seleccionados se utiliza una cadena de caracteres, almacenando la posici�n
 * en cada uno de los elementos de la cadena de caracteres. Para el Ringstellung, por ejemplo, de 11 10 17,
 * la codificaci�n ser�a:       Elemento:  0  1  2
 *                              Valor   : 11 10 17
 * En esta versi�n siempre de 3 rotores
 */
void genRingstellung(int nrotor, unsigned short ring[])
{
    int i;

    /* Simplemente n�meros aleat�rios, que se pueden repetir de 1 a 26 */
    for (i=0; i<nrotor; i++)
        ring[i] = (unsigned short) (rand()% 26 + 1);
    return;
}

/* genGrundstellung() Genera aleatoriamente la posici�n inicial de los rotores
 * nrotor (in) Numero de Rotores
 * grund (in) Posiciones generadas
 * El Grundstellung es el t�rmino utilizado en la m�quina Enigma para
 * indicar la posici�n inicial de los rotores una vez que est�n montados
 * en la m�quina Enigma. La posici�n viene indicada por una letra, que es
 * la que se debe visualizar en la ventanilla de cada rotor.
 * La funci�n genGrundstellung() utiliza la funci�n de biblioteca rand()
 * para seleccionar, aleatoriamente, la posici�n de cada uno de los rotores.
 * El n�mero de rotores viene dado por el par�metro nrotor. La posici�n debe
 * ser una letra may�scula del intervalo [A, Z].
 * Para almacenar las posiciones seleccionadas se utiliza un vector de 3
 * caracteres almacenando la posici�n en cada uno de los elementos del vector.
 * Para el grundstellung, por ejemplo, de R D K (ver imagen anterior), la
 * codificaci�n debe ser:
 *              Elemento:  0  1  2
 *              Valor   :  R  D  K
 */
void genGrundstellung(int nrotor, char grund[])
{
    int i;

    /* Numeros aletorios que se convierte a caracteres de 'A' a 'Z' */
    for (i=0; i<nrotor; i++)
        grund[i] = rand()% 26 + 'A';

    grund[i] = '\0'; /* Por si acaso*/
    return;
}

/* fprintGrundstellung() Escribe la parte correspondiente a la posici�n inicial de cada uno de los rotores,
 * el grundstellung
 * nrotor (in) Numero de rotores
 * grund (in) Vector con la informacion del grundstellung de los rotores
 * ptr (in) Puntero al fichero libros.txt
 * Escribe en dos archivos: libro.txt: con el formato que aparece en el ejemplo
 * del libro de claves. config.cfg: con el formato que aparece en el ejemplo
 * del archivo de configuraci�n
 */
void fprintGrundstellung(int nrotor, char grund[], FILE *ptr)
{
    int i;
    for (i=0; i < nrotor; i++)
    {
        fprintf(ptr,"%c ",grund[i]); /* Para libros.txt con un espacio */
    }
    return;
}

/* fprintWalzenlage() Escribe en los archivos de claves lel tipo de rotores (Walzenlage)
 * nrotor Numero de rotores a generar (M3:3, M4: 4)
 * walzen Array donde almacenar los n�meros (entre 1 y 8) generados
 * ptr Puntero a fichero libro.txt donde escribir
 * Escribe en los archivos de claves la parte correspondiente a la
 * configuraci�n de los rotores, siguiendo el formato que aparece en el ejemplo
 * del libro de claves
 *  |   Walzenlage   |
 *  ------------------
 *  | II   IV   III  |
 *  | V    III  I    |
 *  | V    I    III  |
 */
void fprintWalzenlage(int nrotor, unsigned short walzen[], FILE *ptr)
{
    int i;

    /* Escribimos los tres par�metros en n�meros romanos */
    for (i=0; i<nrotor; i++)
    {
        switch(walzen[i])
        {
        case 1:
            fprintf(ptr,"I    ");
            break;
        case 2:
            fprintf(ptr,"II   ");
            break;
        case 3:
            fprintf(ptr,"III  ");
            break;
        case 4:
            fprintf(ptr,"IV   ");
            break;
        case 5:
            fprintf(ptr,"V    ");
            break;
        case 6:
            fprintf(ptr,"VI   ");
            break;
        case 7:
            fprintf(ptr,"VII  ");
            break;
        case 8:
            fprintf(ptr,"VIII ");
            break;
        case 9:
            fprintf(ptr,"Beta  ");
            break;
        case 10:
            fprintf(ptr,"Gamma ");
            break;
        default:
            break;
        }
    }
    return;
}

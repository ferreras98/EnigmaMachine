#include <stdio.h>
#include <stdlib.h> /* Para rand() */
#include <string.h> /* Para strcpy() */
#include <windows.h>
#include <ctype.h>

#include "../main/globals.h"
#include "../auxiliar/auxiliar.h"
#include "../Maquina/Enigma.h"

/* avanzaRotores() Simula el avance de los rotores antes de pulsar la tecla
 * pListaRotores (in - out) Puntero a dirección de una lista de rotores.
 * Tiene en cuenta el avance de los siguientes rotores de acuerdo con las muescas
 * y el avance del "Double Step"
 */
void avanzaRotores(struct sListaRotores *pListaRotores)
{
    int num;

    num = pListaRotores->num;  /* Para no escribir tanto */

    /* Mecanismo de double step: el rotor de 3º por la izq está en la posicion de arriba */
    if (num > 3)
    {
        if ((pListaRotores->rotor[num-3].anillo[0]==pListaRotores->rotor[num-3].muesca[0])
                || (pListaRotores->rotor[num-3].anillo[0]==pListaRotores->rotor[num-3].muesca[1]))
        {
            pasoRotor(&pListaRotores->rotor[num-3],1);
            pasoRotor(&pListaRotores->rotor[num-4],1);
        }
    }

    if (num > 2) /* Seguimos con el mecanismo de double step ahora para el 2º por la izquierda */
    {
        if ((pListaRotores->rotor[num-2].anillo[0]==pListaRotores->rotor[num-2].muesca[0])
                || (pListaRotores->rotor[num-2].anillo[0]==pListaRotores->rotor[num-2].muesca[1]))
        {
            pasoRotor(&pListaRotores->rotor[num-2],1);
            pasoRotor(&pListaRotores->rotor[num-3],1);
        }
    }

    /* Avanzamos el rotor de la derecha una posición  */
    if (num >0)
        pasoRotor(&pListaRotores->rotor[num-1], 1);

    /* Vemos si toca avanzar a los siguientes rotores */
    if ((num > 1) &&
            ((pListaRotores->rotor[num-1].anillo[25]==pListaRotores->rotor[num-1].muesca[0]) ||
             (pListaRotores->rotor[num-1].anillo[25]==pListaRotores->rotor[num-1].muesca[1])))
    {
        pasoRotor(&pListaRotores->rotor[num-2],1);
    }

    /* NOTA: No está implementado el avance de los rotores > 4 cuando el número es superior a 4 */
    return;
}

/* configRotores() Establece la configuración inicial de los rotores
 * lista (in out) Contiene la dirección de una lista de rotores
 * config (in) Registro con la configuración de la máquina Enigma
 */
void configRotores(struct sListaRotores* lista, struct sConfig config)
{
    int i;

    /* Para cada uno de los rotores */
    for (i=0; i<config.numRotores; i++)
    {
        /* Configuramos el ringenstellung de los rotores */
        configAnillos(&lista->rotor[i], config.ringS[i]);
        /* Les movemos a la posición inicial segun el Grundstellung */
        pasoRotor(&lista->rotor[i], config.grund[i] - 'A');
    }
    return;
}

/* dibujaRotor(() Dibuja en la interfaz de la versión 2 una representación esquemática de
 * un rotor de la máquina Enigma
 * rotor (in) Registro de tipo struct sRotor que contiene los datos del rotor.
 * coordRotor (in) Registro de tipo struct sPunto que contiene las coordenadas
 * del rotor en la interfaz gráfica
 *
 * A nivel de código, un rotor está compuesto por tres cadenas de caracteres:
 *      anillo    : ABCDEFGHIJKLMNOPQRSTUVWXYZ
 *      cableado  : EKMFLGDQVZNTOWYHXUSPAIBRCJ
 *      referencia: ABCDEFGHIJKLMNOPQRSTUVWXYZ
 * La primera cadena (anillo) representa el anillo móvil del rotor, que conecta
 * al mismo con el elemento que tiene a su izquierda (rotor o reflector).
 * La segunda cadena (cableado) representa el cableado del rotor, que conecta los
 * conectores de la derecha con los del anillo.
 * La tercera cadena (referencia) se utiliza como referencia para establecer la posición
 * relativa entre el rotor y su anillo.
 * La función muestra las tres cadenas en vertical, la letra que se muestra en la ventanilla,
 * y un rótulo con el número (romano) que identifica a cada rotor.
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

/* genWalzenlage() un número de rotores diferentes entre 1 y 8
 * nrotor Numero de rotores a generar (M3:3, M4: 4)
 * rotores Array donde almacenar los números (entre 1 y 8) generados
 * Utiliza la función genera_numeros para seleccionar, aleatoriamente, un número de
 * rotores DIFERENTES entre los 8 disponibles igual al parámetro nrotor que recibe
 * En el caso del Modelo M3,este valor será de 3, y en el caso del modelo M4, de 4.
 * Para almacenar los rotores seleccionados se utiliza una cadena de caracteres, almacenando el número de
 * rotor en cada uno de los elementos de la cadena de caracteres. Para el Walzenlage del ejemplo (II IV III),
 * la codificación sería:
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
 * pRotor (in out) Dirección de un registro de tipo struct sRotor, que contiene los atributos de un rotor
 * num (in) Numero de pasos a avanzar
 *
 * Implementa el avance paso a paso de un rotor:
 * 1. Operación manual que realiza el operador de Enigma para colocar la letra indicada
 * en el grundstellung en la ventanilla correspondiente.
 * 2. Avance automático que se produce en el proceso de cifrado.
 * A nivel de código, este comportamiento se implementa con un algoritmo de recorrido
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

/* configAnillos() Implementa la configuración del anillo de un rotor
 * pRotor (in out) Dirección de un registro de tipo struct sRotor, que contiene los atributos de un rotor
 * pos (in) Posición del anillo indicado por el ringstellung (número entero del intervalo [0, 25])
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

/* opRotor() Devuelve la posición del contacto de salida del rotor
 * in (in) Posición del contacto de entrada al rotor
 * rotor (in) Registro que contiene los atributos de un rotor
 * coord (in) Coordenadas del rotor (parámetro añadido respecto a las especificaciones)
 * return (out) Posición de salida
 * Ruta de ida
 * El rotor de la derecha (rotor I) recibe la señal eléctrica del estator en la posición 0. En ese
 * momento esa posición corresponde a la letra Y, que, según la cadena del cableado, está conectada
 * a la letra F del anillo. Por lo tanto, la función tiene que buscar la letra F en la cadena del
 * anillo, y devolver dicha posición, que será la entrada para el siguiente rotor.
 * Ruta de vuelta
 * En el camino de vuelta la operación es justamente la contraria de la realizada en el camino de ida.
 * Ahora la posición que recibe el rotor es la que le envía el elemento que tiene a su izquierda (rotor
 * o reflector). En este caso el valor recibido que corresponde a posición en la cadena del anillo, y
 * la función tiene que buscar dicha letra en la cadena cableado, y ese es el valor que tiene que
 * devolver la función
 */
unsigned short opRotor(unsigned short in, struct sRotor rotor, bool dir)
{
    unsigned short out;
    char caracter;

    /* La parte de operación es bastante fácil, depende de la dirección */
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

/* leeCfgRotor() Lee la configuración de cada rotor
 * numRotor (in) Tipo de rotor a leer; walzen (1 a 8)
 * rotor (out) Información del rotor
 * muesca (out) Posición de las muescas
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
        sprintf(mensaje, "No puedo abrir el fichero %s\n Utilizo configuración por defecto", FILE_ROTORES);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
        strcpy(pRotor->cableado, conf_defecto[(pRotor->num) - 1].cableado);
        strcpy(pRotor->muesca, conf_defecto[(pRotor->num) - 1].muesca);
    }
    else         /* Leemos todos, pero sólo nos quedamos con el último (el numRotor)*/
    {
        for (i=0; i < pRotor->num; i++)
        {
            fscanf(ptr, "%s %s\n", pRotor->cableado, pRotor->muesca);
        }
    }

    strcpy(pRotor->anillo, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");   /* Ponemos configuración estandar en la referencia */
    strcpy(pRotor->referencia, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");   /* Ponemos configuración estandar en la referencia */

    fclose(ptr);
    return;
}

/* fprintRingstellung() Escribe el Ringstellung en los Libros de claves
 * nrotor Numero de rotores a generar (M3:3, M4: 4)
 * ring Array donde almacenar los números (entre 1 y 26) generados
 * ptr Puntero a fichero donde escribir
 * Escribe en los archivos de libros de claves la parte correspondiente a la
 * configuración de los anillos de los rotores (ringstellung), siguiendo el
 * formato: |  11 10 17  |. En esta version siempre tres rotores.
 */
void fprintRingstellung(int nrotor, unsigned short ring[], FILE *ptr)
{
    int i;

    /* El 0 del formato es para que lo rellene con un 0 cuando sólo tiene un dígito */
    for (i=0; i< nrotor; i++)
    {
        fprintf(ptr, "%02d ", (int) ring[i]);
    }
    return;
}

/* genRingstellung(() Genera una configuración aletoria para la configuración inicial de los rotores
 * nrotor Numero de rotores
 * ring Vector para almacenar la configuración
 * Utiliza la función de biblioteca rand() para seleccionar, aleatoriamente,
 * la posición inicial del anillo cada uno de los rotores. El número de rotores viene dado por el parámetro
 * nrotor. La posición debe ser un número entre 1 y 26, ya que esas son las posibles posiciones (una por
 * cada letra) de cada anillo.
 * Para almacenar las posiciones seleccionados se utiliza una cadena de caracteres, almacenando la posición
 * en cada uno de los elementos de la cadena de caracteres. Para el Ringstellung, por ejemplo, de 11 10 17,
 * la codificación sería:       Elemento:  0  1  2
 *                              Valor   : 11 10 17
 * En esta versión siempre de 3 rotores
 */
void genRingstellung(int nrotor, unsigned short ring[])
{
    int i;

    /* Simplemente números aleatórios, que se pueden repetir de 1 a 26 */
    for (i=0; i<nrotor; i++)
        ring[i] = (unsigned short) (rand()% 26 + 1);
    return;
}

/* genGrundstellung() Genera aleatoriamente la posición inicial de los rotores
 * nrotor (in) Numero de Rotores
 * grund (in) Posiciones generadas
 * El Grundstellung es el término utilizado en la máquina Enigma para
 * indicar la posición inicial de los rotores una vez que están montados
 * en la máquina Enigma. La posición viene indicada por una letra, que es
 * la que se debe visualizar en la ventanilla de cada rotor.
 * La función genGrundstellung() utiliza la función de biblioteca rand()
 * para seleccionar, aleatoriamente, la posición de cada uno de los rotores.
 * El número de rotores viene dado por el parámetro nrotor. La posición debe
 * ser una letra mayúscula del intervalo [A, Z].
 * Para almacenar las posiciones seleccionadas se utiliza un vector de 3
 * caracteres almacenando la posición en cada uno de los elementos del vector.
 * Para el grundstellung, por ejemplo, de R D K (ver imagen anterior), la
 * codificación debe ser:
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

/* fprintGrundstellung() Escribe la parte correspondiente a la posición inicial de cada uno de los rotores,
 * el grundstellung
 * nrotor (in) Numero de rotores
 * grund (in) Vector con la informacion del grundstellung de los rotores
 * ptr (in) Puntero al fichero libros.txt
 * Escribe en dos archivos: libro.txt: con el formato que aparece en el ejemplo
 * del libro de claves. config.cfg: con el formato que aparece en el ejemplo
 * del archivo de configuración
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
 * walzen Array donde almacenar los números (entre 1 y 8) generados
 * ptr Puntero a fichero libro.txt donde escribir
 * Escribe en los archivos de claves la parte correspondiente a la
 * configuración de los rotores, siguiendo el formato que aparece en el ejemplo
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

    /* Escribimos los tres parámetros en números romanos */
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

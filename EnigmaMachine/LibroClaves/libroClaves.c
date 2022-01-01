#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../main/globals.h"

#include "../Config/config.h"
#include "../LibroClaves/libroClaves.h"
#include "../Maquina/rotores.h"
#include "../Maquina/stecker.h"

/* generaLibroClaves() Como su nombre indica, genera el libro de claves diarias para configurar
 * la m�quina Enigma.
 * En la versi�n 3, la funci�n genera un libro de claves distinto para cada uno de los cuatro modelos
 * Enigma disponibles. El modelo lo elige el usuario en la llamada a la funci�n selecModelo(), con lo
 * que se obtiene el n�mero de rotores y el modelo de reflector. Con estos datos se genera el nombre del
 * archivo libro de claves correspondiente.
 * As� mismo, y tal como se muestra en la tabla anterior, tambi�n se genera un archivo BINARIO de
 * configuraci�n para cada modelo de Enigma. cada archivo de configuraci�n contiene 31 registros de
 * tipo struct sConfig. Para una correcta lectura de los archivos de configuraci�n de ejemplo, se ha
 * de utilizar la siguiente declaraci�n, con los atributos en el mismo orden y con las mismas dimensiones
 */
void generaLibroClaves ()
{
    FILE * ptr, *pCfg;
    struct sConfig Config;
    char buffer[100], mensaje[100];

    selecModelo(&Config);

    /* Si no hemos podido devolvemos un fallo y no continuamos */
    sprintf(buffer, "BASEDATOS/libro_M%1d_%c.bin", Config.numRotores, Config.modeloETW);
    pCfg = fopen(buffer, "wb");
    if (pCfg == NULL)
    {
        sprintf(mensaje, "No puedo abrir el fichero %s", buffer);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        exit(1);
    }

    sprintf(buffer, "BASEDATOS/libro_M%1d_%c.txt", Config.numRotores, Config.modeloETW);
    ptr = fopen(buffer, "wt");
    if (ptr == NULL)
    {
        sprintf(mensaje, "No puedo abrir el fichero %s", buffer);
        MessageBox(NULL, mensaje, TITULO_ERROR1, MB_OK | MB_ICONSTOP | MB_SETFOREGROUND);
        exit(1);
    }


    /* Imprimimos la cabecera */
    if (Config.numRotores >= 4)
    {
        fprintf(ptr,"%s\n\n","GEHEIM!                        SS SONDER-PROJEKT ABTEILUNG                        Januar 1942");
        fprintf(ptr,"%s\n", "---------------------------------------------------------------------------------------------");
        fprintf(ptr,"%s\n", "|Tag |      Walzenlage      | Ringstellung  |      Steckerverbindungen      | Grundstellung |");
        fprintf(ptr,"%s\n", "---------------------------------------------------------------------------------------------");
    }
    else
    {
        fprintf(ptr,"%s\n\n","GEHEIM!                    SS SONDER-PROJEKT ABTEILUNG                   Januar 1942");
        fprintf(ptr,"%s\n", "------------------------------------------------------------------------------------");
        fprintf(ptr,"%s\n", "|Tag |   Walzenlage   |Ringstellung|      Steckerverbindungen      | Grundstellung |");
        fprintf(ptr,"%s\n", "------------------------------------------------------------------------------------");
    }

    /* Para cada uno de los 31 d�as empezando por el �ltimo */
    for (Config.dia=31; Config.dia > 0; Config.dia--)
    {
        /* Generamos todos los par�metros que necesitamos */
        genWalzenlage(Config.numRotores, Config.walzen);
        genRingstellung(Config.numRotores, Config.ringS);
        genStecker(Config.stecker);
        genGrundstellung(Config.numRotores, Config.grund);

        /* Imprmimos en el fichero plano */
        fprintf(ptr,"| %2d | ", Config.dia);
        fprintWalzenlage (Config.numRotores, Config.walzen, ptr);
        fprintf(ptr,"|  ");
        fprintRingstellung (Config.numRotores, Config.ringS, ptr);
        fprintf(ptr," | ");
        fprintStecker (Config.stecker, ptr);
        if (Config.numRotores>=4)
            fprintf(ptr,"|    ");
        else
            fprintf(ptr,"|     ");
        fprintGrundstellung (Config.numRotores, Config.grund, ptr);
        if (Config.numRotores>=4)
            fprintf(ptr,"   |\n");
        else
            fprintf(ptr,"    |\n");

        /* Imprimimos en el fichero binario */
        fwrite(&Config, sizeof(struct sConfig), 1, pCfg);
    }

    if (Config.numRotores >= 4)
        fprintf(ptr,"%s\n", "---------------------------------------------------------------------------------------------");
    else
        fprintf(ptr,"%s\n", "------------------------------------------------------------------------------------");

    /* Cerramos los dos ficheros */
    fclose(ptr);
    fclose(pCfg);

    /* Abrimos el fichero con el notepad */
    sprintf(mensaje, "notepad %s", buffer);
    system(mensaje);

    return;
}

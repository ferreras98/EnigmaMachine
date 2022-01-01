#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* TAMAÑOS */
#define NCOL 82  /* Numero de columnas para 3 rotores */
#define NFIL 36  /* Numero de filas de la pantalla */

/* Numero de rotores en ambas opciones */
#define TITULO_VENTANA03 " M Á Q U I N A   E N I G M A   M-3 "
#define TITULO_VENTANA04 " M Á Q U I N A   E N I G M A   M-4 "

/* Número de pares de conexiones del stecker */
#define PARES_STECKER 10  /* Normal 10 - Máximo 13 pares */

#define MAX_LETRAS 250  /* Maximo numero de letras a codificar */
#define LINEA  25 /* Numero de caracteres por linea en las ventanas de texto */
#define BLOQUE 5  /* Numero de caracteres por bloque en las ventanas de texto */
#define TITULO_ERROR1 "Cifrado"
#define TEXTO_ERROR1 "El mensaje excede de 250 letras"

/* NOMBRES DE FICHEROS */
#define FILE_REFLECTOR "BASEDATOS/reflector.cfg"
#define FILE_ROTORES   "BASEDATOS/cableadoRotores.cfg"
#define FILE_PLANO     "BASEDATOS/mensajePlano.txt"
#define FILE_CIFRADO   "BASEDATOS/mensajeCifrado.txt"

/* Colores de la Consola */
enum {Negro=0,Azul,Verde,Aguamarina,Rojo,Purpura,Amarillo,Blanco,Gris,AzulClaro,
      VerdeClaro,AguamarinaClaro,RojoClaro,PurpuraClaro,AmarilloClaro,
      BlancoBrillante
     };

/* COLORES */
#define COLOR_NORMAL (16*BlancoBrillante + AzulClaro)

#define COLOR_ROTOR  (16*Blanco + AzulClaro)
#define COLOR_MUESCA (16*Blanco + RojoClaro)

#define TEXTO_PLANO   (16*BlancoBrillante + Amarillo)
#define TEXTO_CIFRADO (16*BlancoBrillante + RojoClaro)

#define CHARS_IDA (16*Amarillo + BlancoBrillante)
#define CHARS_VUE (16*RojoClaro + BlancoBrillante)

/* TEXTOS */
#define TITULO_CONSOLA "Emulador ENIGMA v3.0"
#define TITULO_VENTANA6 "Texto Plano"
#define TITULO_VENTANA7 "Texto Cifrado"

#define TEXTO_FIN " [ESC %c%c fin]"

#define FIN_CODIF ((char) 27)  /* Caracter de escape */

#define TITULO_ERROR23 "MENSAJE PARA TORPES"
#define TEXTO_ERROR2 "Tiene que introducir un número entre\n1 y 31, y ha introducido... ¡\"%s\"!"
#define TEXTO_ERROR3 "¡No hay ningún mes con %d días!"

#define TEXTO_INFO "Introduzca el mensaje, letra a letra\nPara terminar, pulse la letra [ESC]"
#define TITULO_CONFIG "Configuración de Enigma"
#define INTRODUCE_DIA "Introduce día del libro de claves:"

#endif /* GLOBALS_H_INCLUDED */

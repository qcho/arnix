/**
 * screen.h | Interfaz para manejo de pantalla.
 */

#ifndef SCREEN_H
    #define SCREEN_H
    #include "common.h"

    #define BLACK = 0;
    #define WHITE = 15;

    /**
     * Escribe un caracter en pantalla.
     * @param char c: el caracter a escribir.
     **/
    void screen_put(char c, u8int backColour, u8int foreColour);

    /**
     * Limpia la pantalla
     **/
    void screen_clear();

    /**
     * Escribe la cadena de caracteres en pantalla.
     * @param char *string: cadena de caracteres ASCII a escribir (finalizada en \0)
     **/
    void screen_write(char *string, u8int backColour, u8int foreColour);
#endif

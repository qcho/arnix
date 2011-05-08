/**
 * screen.h | Interfaz para manejo de pantalla.
 */

#ifndef SCREEN_H
    #define SCREEN_H
    #include "common.h"

    enum Colour {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHT_GREY,
        DARK_GREY,
        LIGHT_BLUE,
        LIGHT_GREEN,
        LIGHT_CYAN,
        LIGHT_RED,
        LIGHT_MAGENTA,
        LIGHT_BROWN,
        WHITE
    };

    void screen_setBackColour(enum Colour colour);

    void screen_setForeColour(enum Colour colour);

    void screen_setColours(enum Colour colourBack, enum Colour colourFore);

    /**
     * Escribe un caracter en pantalla.
     * @param char c: el caracter a escribir.
     **/
    void screen_put(char c);

    /**
     * Limpia la pantalla
     **/
    void screen_clear();

    /**
     * Escribe la cadena de caracteres en pantalla.
     * @param char *string: cadena de caracteres ASCII a escribir (finalizada en \0)
     **/
    void screen_write(char *string);


#endif

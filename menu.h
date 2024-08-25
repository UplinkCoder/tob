#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>

// Definition der verschiedenen Zustände des Spiels
typedef enum {
    WAIT_FOR_INPUT,
    MENU,
    NEW_GAME,
    LOAD_GAME,
    SAVE_GAME,
    QUIT_GAME
} GameState;

// Funktion zur Initialisierung des Menüs
void initMenu(SDL_Renderer *renderer, TTF_Font *font);

// Funktion zur Anzeige des Menüs
void renderMenu(SDL_Renderer *renderer, TTF_Font *font, GameState state);

// Funktion zur Handhabung der Tasteneingaben
void handleMenuInput(SDL_Event *event, GameState *state);

#endif

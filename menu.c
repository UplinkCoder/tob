#include "menu.h"
#include <stdio.h>

// Menüoptionen
typedef struct {
    const char *text;
    GameState state;
} MenuOption;

// Menüoptionen definieren
MenuOption menuOptions[] = {
    {"New Game", NEW_GAME},
    {"Load Game", LOAD_GAME},
    {"Save Game", SAVE_GAME},
    {"Quit", QUIT_GAME}
};

int selectedOption = 0;
const int menuOptionCount = sizeof(menuOptions) / sizeof(MenuOption);

// Initialisiert das Menü
void initMenu(SDL_Renderer *renderer, TTF_Font *font) {
    // Hier können spezifische Initialisierungen für das Menü erfolgen
}

// Rendert das Menü basierend auf dem aktuellen Zustand
void renderMenu(SDL_Renderer *renderer, TTF_Font *font, GameState state) {

    for (int i = 0; i < menuOptionCount; i++) {
        SDL_Color color = (i == selectedOption) ? g_Colors.grey : g_Colors.white;
        renderText(renderer, font, menuOptions[i].text, 50, 50 + i * 50, color);
    }
}

// Handhabt die Tasteneingaben im Menü
void handleMenuInput(SDL_Event *event, GameState *state) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                selectedOption--;
                if (selectedOption < 0) selectedOption = menuOptionCount - 1;
                break;
            case SDLK_DOWN:
                selectedOption++;
                if (selectedOption >= menuOptionCount) selectedOption = 0;
                break;
            case SDLK_RETURN: // Wenn Enter gedrückt wird, setze den Zustand
                *state = menuOptions[selectedOption].state;
                break;
            default:
                break;
        }
    }
}


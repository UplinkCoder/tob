#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

// Fensterdimensionen
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* FONT_PATH = "assets/fonts/SansThirteenBlack.ttf";

struct {
    SDL_Color white;
    SDL_Color grey;
} g_Colors = {
    .white = {255, 255, 255, 255},
    .grey = {90, 91, 89, 255}
};

#include "render.c"
#include "menu.c"

// Utility function to log and clean up SDL resources in case of failure
void cleanup(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font) {
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

// Initialize SDL, TTF, Window, and Renderer
bool initSDL(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL konnte nicht initialisiert werden: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        SDL_Log("SDL_TTF konnte nicht initialisiert werden: %s", TTF_GetError());
        SDL_Quit();
        return false;
    }

    *window = SDL_CreateWindow("The Obedient Mind",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        SDL_Log("Fenster konnte nicht erstellt werden: %s", SDL_GetError());
        cleanup(*window, *renderer, *font);
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        SDL_Log("Renderer konnte nicht erstellt werden: %s", SDL_GetError());
        cleanup(*window, *renderer, *font);
        return false;
    }

    *font = TTF_OpenFont(FONT_PATH, 32);
    if (!*font) {
        SDL_Log("Schrift konnte nicht geladen werden: %s", TTF_GetError());
        cleanup(*window, *renderer, *font);
        return false;
    }

    return true;
}

// Main function with updated state handling
int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;

    if (!initSDL(&window, &renderer, &font)) {
        return 1; // Initialization failed
    }

    GameState state = MENU;
    bool running = true;
    SDL_Event event;
    const char *message = NULL;

    initMenu(renderer, font);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (state == MENU) {
                handleMenuInput(&event, &state);
            } else if (state == WAIT_FOR_INPUT) {
                if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                    state = MENU;  // Return to menu after input
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Use a switch-case for better clarity and maintainability
        switch (state) {
            case MENU:
                renderMenu(renderer, font, state);
                break;
            case NEW_GAME:
                message = "Starting a New Game...";
                renderText(renderer, font, message, 50, 50, g_Colors.white);
                state = WAIT_FOR_INPUT;  // Transition to waiting for input state
                break;
            case LOAD_GAME:
                message = "Loading Game...";
                renderText(renderer, font, message, 50, 50, g_Colors.white);
                state = WAIT_FOR_INPUT;  // Transition to waiting for input state
                break;
            case SAVE_GAME:
                message = "Saving Game...";
                renderText(renderer, font, message, 50, 50, g_Colors.white);
                state = WAIT_FOR_INPUT;  // Transition to waiting for input state
                break;
            case QUIT_GAME:
                running = false;
                break;
            case WAIT_FOR_INPUT:
                if (message) {
                    renderText(renderer, font, message, 50, 50, g_Colors.white);
                }
                break;
        }

        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer, font);
    return 0;
}


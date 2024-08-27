#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#define ATLAS_WIDTH 512
#define ATLAS_HEIGHT 512
#define CHAR_START 32
#define CHAR_END 126

typedef struct {
    SDL_Rect srcRect;
    int xOffset;
    int yOffset;
    int advance;
} Glyph;

void generateTextureAtlas(const char* fontPath, int fontSize, const char* atlasPath, const char* metadataPath) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return;
    }

    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("Atlas Generator", 0, 0, ATLAS_WIDTH, ATLAS_HEIGHT, SDL_WINDOW_HIDDEN), -1, 0);
    SDL_Texture* atlasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ATLAS_WIDTH, ATLAS_HEIGHT);
    SDL_SetRenderTarget(renderer, atlasTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    Glyph glyphs[CHAR_END - CHAR_START + 1];
    int x = 0, y = 0, maxRowHeight = 0;

    for (char c = CHAR_START; c <= CHAR_END; c++) {
        SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(font, c, (SDL_Color){255, 255, 255, 255});
        if (!glyphSurface) {
            printf("Failed to render glyph: %s\n", TTF_GetError());
            continue;
        }

        if (x + glyphSurface->w > ATLAS_WIDTH) {
            x = 0;
            y += maxRowHeight;
            maxRowHeight = 0;
        }

        if (glyphSurface->h > maxRowHeight) {
            maxRowHeight = glyphSurface->h;
        }

        SDL_Texture* glyphTexture = SDL_CreateTextureFromSurface(renderer, glyphSurface);
        SDL_Rect dstRect = {x, y, glyphSurface->w, glyphSurface->h};
        SDL_RenderCopy(renderer, glyphTexture, NULL, &dstRect);

        int minx, maxx, miny, maxy, advance;
        TTF_GlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &advance);

        glyphs[c - CHAR_START].srcRect = dstRect;
        glyphs[c - CHAR_START].xOffset = minx;
        glyphs[c - CHAR_START].yOffset = maxy;
        glyphs[c - CHAR_START].advance = advance;

        x += glyphSurface->w;

        SDL_FreeSurface(glyphSurface);
        SDL_DestroyTexture(glyphTexture);
    }

    SDL_SetRenderTarget(renderer, NULL);

    SDL_Surface* atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, ATLAS_WIDTH, ATLAS_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, atlasSurface->pixels, atlasSurface->pitch);
    SDL_SaveBMP(atlasSurface, atlasPath);
    SDL_FreeSurface(atlasSurface);

    FILE* metadataFile = fopen(metadataPath, "wb");
    fwrite(glyphs, sizeof(Glyph), CHAR_END - CHAR_START + 1, metadataFile);
    fclose(metadataFile);

    SDL_DestroyTexture(atlasTexture);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

int main() {
    generateTextureAtlas("assets/fonts/SansThirteenBlack.ttf", 32, "assets/textures/font_atlas.bmp", "assets/textures/font_metadata.dat");
    return 0;
}

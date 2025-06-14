#ifndef RENDERING_H
#define RENDERING_H
#include <SDL2/SDL.h>
/*
 * Config
 */

#define WIDTH 800
#define HEIGHT 600
extern Uint32 *colorPalette;

SDL_Texture *create_texture(SDL_Renderer *renderer);

void render(SDL_Renderer *renderer, SDL_Texture *texture);

void handleInput(SDL_Event *e, int *quit);

#endif // !RENDERING_H

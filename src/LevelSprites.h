#pragma once
#include "SDL.h"
#include "SDL_image.h"

// IMAGE CLASS (LOADING IN IMAGES)
class LevelSprites
{
public:
	LevelSprites(int x, int y, int w, int h);
	~LevelSprites();

	void loadGraphics(const char *filename, SDL_Renderer *ren);
	void render(SDL_Renderer *ren);

	int X, Y, width, height;

private:
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect dstRect;

};


#include "LevelSprites.h"



LevelSprites::LevelSprites(int x, int y, int w, int h)
{
	X = x;
	Y = y;
	width = w;
	height = h;
}


LevelSprites::~LevelSprites()
{
}

void LevelSprites::loadGraphics(const char *filename, SDL_Renderer *ren)
{
	surface = IMG_Load(filename);
	texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
}

void LevelSprites::render(SDL_Renderer *ren)
{
	dstRect = { X, Y, width, height };

	SDL_RenderCopy(ren, texture, NULL, &dstRect);
}
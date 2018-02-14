#pragma once
#include <SDL.h>
#include <SDL_image.h>

class CastleClass
{
public:
	CastleClass(float x, float y, int w, int h);
	~CastleClass();

	void loadGraphics(SDL_Renderer *ren);
	void update();
	void render(SDL_Renderer* ren);

	// Collision Rect
	SDL_Rect r;

	// POS + Size
	float X, Y;
	int width, height;

	// Castle Health
	int health = 500;

private:	
	// Graphics
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect dstRect;
};


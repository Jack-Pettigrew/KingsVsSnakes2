#pragma once
#include <SDL_image.h>
#include <SDL.h>
#include "Vector2D.h"
#include "CastleClass.h"

class EnemySnakes
{
public:
	EnemySnakes(float x, float y, int w, int h);
	~EnemySnakes();

	void init();
	void loadGraphics(SDL_Renderer *ren);
	void update();
	void render(SDL_Renderer *ren);

	// POS + Size
	float X, Y, Speed = -0.07;
	int width, height;
	
	// Hitbox SDL_Rect
	SDL_Rect r;

	// Velocity Object
	Vector2D velocity;

private:
	// Snake Attributes
	int health = 100;
	int attack = 10;

	// Graphic Objects
	SDL_Surface *surface;
	SDL_Texture *texture;

	SDL_Rect dstRect;

	// Movement Animation
	unsigned int ticks = SDL_GetTicks();
	unsigned int changeTimeMS = 1000;
	SDL_Rect spriteAnimation[ 3 ];
	int frame = (ticks/changeTimeMS) % 3;
};


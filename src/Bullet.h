#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include "Vector2D.h"
#include "EnemySnakes.h"

class Bullet
{
public:
	Bullet(float x, float y, int w, int h);
	~Bullet();

	void init();
	void LoadGraphics(SDL_Renderer* ren);
	void update();
	void render(SDL_Renderer* aRenderer);

	SDL_Rect r;

	//object 2d for velocity
	Vector2D velocity;

	int counter = 0;

	//variables for player
	float X, Y;
	int width, height;
};

#include "Bullet.h"

Bullet::Bullet(float x, float y, int w, int h)
{
	r.x = x, r.y = y, r.w = w, r.h = h;
	//POS + Size
	X = x;
	Y = y;
	width = w;
	height = h;
}


Bullet::~Bullet()
{
}

void Bullet::init()
{
}

void Bullet::LoadGraphics(SDL_Renderer * ren)
{
}

void Bullet::update()
{
	X = X + velocity.X;
}

void Bullet::render(SDL_Renderer * aRenderer)
{
	r.x = X; r.y = Y; r.w = width; r.h = height;
	SDL_SetRenderDrawColor(aRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(aRenderer, &r);
}

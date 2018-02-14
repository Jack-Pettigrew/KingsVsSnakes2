#pragma once
#include <SDL.h>
#include <vector>
#include "Bullet.h"
#include "EnemySnakes.h"
#include "Vector2D.h"


class ShootController
{
public:
	ShootController();
	~ShootController();

	void init(float x, float y);
	void add(float x, float y);
	void shoot();
	void update();
	void render(SDL_Renderer *ren);

	float X, Y;

	std::vector<Bullet*> BulletList;

	SDL_Rect r;

	Vector2D velocity;

};


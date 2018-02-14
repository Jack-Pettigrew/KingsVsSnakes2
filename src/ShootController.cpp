#include "ShootController.h"



ShootController::ShootController()
{
}


ShootController::~ShootController()
{
}

void ShootController::init(float x, float y)
{
	X = x; Y = y;
}

void ShootController::add(float x, float y)
{
	Bullet* bulletItem = new Bullet(X, Y, 10, 10);
	bulletItem->velocity.X = 0;
	bulletItem->velocity.Y = 0;
	this->BulletList.push_back(bulletItem);
}

void ShootController::shoot()
{
	int i = 0;
	bool CANTFINDIT = true;

	while ((i < BulletList.size() && CANTFINDIT))
	{
		if (BulletList[i]->velocity.X == 0)
		{
			BulletList[i]->counter = 0;
			BulletList[i]->velocity.X = 0.5;
			CANTFINDIT = false;
		}
		i++;
	}
}

void ShootController::update()
{
	for (auto& element : BulletList)
	{
		element->update();

		if (element->velocity.X > 0)
		{
			element->counter++;
		}
		else if (element->velocity.X == 0)
		{
			element->X = X + 30;
			element->Y = Y + 30;
		}
		if (element->counter > 2300)
		{
			element->counter = 0;
			element->X = X + 30;
			element->Y = Y + 30;
			element->velocity.X = 0;
		}
	}
}

void ShootController::render(SDL_Renderer * ren)
{
	for (auto& element : BulletList)
	{
		element->render(ren);
	}
	r.x = X + 30; r.y = Y + 30; r.w = 10; r.h = 10;

	SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
	SDL_RenderFillRect(ren, &r);
}

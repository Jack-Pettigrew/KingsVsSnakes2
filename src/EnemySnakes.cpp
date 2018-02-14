#include "EnemySnakes.h"

EnemySnakes::EnemySnakes(float x, float y, int w, int h)
{
	r.x = x, r.y = y, r.w = w, r.h = h;
	//POS + Size
	X = x;
	Y = y;
	width = w;
	height = h;
}


EnemySnakes::~EnemySnakes()
{
}

void EnemySnakes::init()
{

}

void EnemySnakes::loadGraphics(SDL_Renderer *ren)
{
	// Loading Graphics
	surface = IMG_Load("../content/snakeWalkAni.png");
	texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);

	spriteAnimation[0].x = 0;
	spriteAnimation[0].y = 0;
	spriteAnimation[0].w = 32;
	spriteAnimation[0].h = 14;

	spriteAnimation[1].x = 32;
	spriteAnimation[1].y = 0;
	spriteAnimation[1].w = 32;
	spriteAnimation[1].h = 14;

	spriteAnimation[2].x = 64;
	spriteAnimation[2].y = 0;
	spriteAnimation[2].w = 32;
	spriteAnimation[2].h = 14;
}

void EnemySnakes::update()
{
	// Enemy Movement, Speed
	velocity.X = Speed;
	X = X + velocity.X;

	++frame;
	
	if (frame / 3 >= 3)
	{
		frame = 0;
	}

	// Sprite dstRect + Conversion
	int iX = int(X + 0.5);
	int iY = int(Y + 0.5);
	dstRect = { iX - 20, iY, 32 * 3, 14 * 3 };
}

void EnemySnakes::render(SDL_Renderer *ren)
{
	// Hitbox POS + Size
	r.x = X; r.y = Y; r.w = width; r.h = height;
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	//SDL_RenderFillRect(ren, &r);

	// Display + Flip Snake Sprite
	SDL_Rect *currentFrame = &spriteAnimation[frame / 3];
	SDL_RenderCopyEx(ren, texture, currentFrame, &dstRect, 0, NULL, SDL_FLIP_HORIZONTAL);
}
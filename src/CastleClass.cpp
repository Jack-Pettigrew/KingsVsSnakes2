#include "CastleClass.h"

CastleClass::CastleClass(float x, float y, int w, int h)
{
	r.x = x, r.y = y, r.w = w, r.h = h;
	// POS + Size
	X = x; Y = y; width = w; height = h;

	SDL_Log("Castle Created");
}


CastleClass::~CastleClass()
{
}

void CastleClass::loadGraphics(SDL_Renderer * ren)
{
	// Loading Graphics
	surface = IMG_Load("../content/castle.png");
	texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
}

void CastleClass::update()
{
	// Sprite dstRect + Conversion Float -> Int
	int iX = int(X + 0.5);
	int iY = int(Y + 0.5);
	dstRect = { iX, iY, 185 * 3, 176 * 3 };
}

void CastleClass::render(SDL_Renderer * ren)
{
	// Render Hitbox
	r.x = X; r.y = Y; r.w = width; r.h = height;
	SDL_SetRenderDrawColor(ren, 0, 0, 255, 0);
	//SDL_RenderFillRect(ren, &r);

	// Render Castle
	SDL_RenderCopy(ren, texture, NULL, &dstRect);
}
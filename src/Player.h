#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<string>
#include<fstream>
#include"json.hpp"
#include "Vector2D.h"

using json = nlohmann::json;

class Player
{
public:
	//player is rect FOR NOW
	Player(float x, float y, int w, int h);
	~Player();

	void init();
	void LoadGraphics(SDL_Renderer* ren);
	void update();
	void render(SDL_Renderer* aRenderer);
	void serialise(const std::string &path) const;
	void deserialise(const std::string &path);

	//bool for key movements
	bool KEY_UP = false;
	bool KEY_DOWN = false;

	SDL_Rect *r = new SDL_Rect();

	//variables for player
	float X, Y;
	int width, height;

	//object 2d for velocity
	Vector2D velocity;
private:

	//graphics
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Rect dstRect;
};


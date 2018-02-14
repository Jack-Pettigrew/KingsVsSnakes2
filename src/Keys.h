#pragma once
#include<SDL.h>
#include<string>
#include<fstream>
#include"json.hpp"

using json = nlohmann::json;

class Key
{
public:
	SDL_Keycode shoot;
	SDL_Keycode moveUp;
	SDL_Keycode moveDown;
	SDL_Keycode menuUp;
	SDL_Keycode menuDown;
	SDL_Keycode menuConfirm;

	void loadKeys(const std::string &path);

	Key();
};
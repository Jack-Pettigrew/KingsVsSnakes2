#include "Keys.h"

Key::Key()
{
}

void Key::loadKeys(const std::string & path)
{
	SDL_Log("Loading Key Bindings");
	std::ifstream inStream;
	inStream.open(path, std::ios::in);
	json jsonObject;

	inStream >> jsonObject;

	this->shoot = SDL_GetKeyFromName(jsonObject["shoot"].get<std::string>().c_str());
	this->moveUp = SDL_GetKeyFromName(jsonObject["moveUp"].get<std::string>().c_str());
	this->moveDown = SDL_GetKeyFromName(jsonObject["moveDown"].get<std::string>().c_str());
	this->menuUp = SDL_GetKeyFromName(jsonObject["menuUp"].get<std::string>().c_str());
	this->menuDown = SDL_GetKeyFromName(jsonObject["menuDown"].get<std::string>().c_str());
	this->menuConfirm = SDL_GetKeyFromName(jsonObject["menuConfirm"].get<std::string>().c_str());

	inStream.close();
}

#include "Player.h"


Player::Player(float x, float y, int w, int h) // Player Constructor
{
	//POS + Size
	X = x;
	Y = y;
	width = w;
	height = h;

	SDL_Log("Player Created");
}


Player::~Player()
{
	delete r;
	r = NULL;
}

void Player::init()
{
}

void Player::LoadGraphics(SDL_Renderer* ren)
{
	// Loading Graphics
	surface = IMG_Load("../content/kingIdle.png");
	texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
}

void Player::update() // Movement and Conditions
{
	// Movement + Velocity
	Y = Y + velocity.Y;
	if (KEY_UP)	velocity.Y = -0.15;
	if (KEY_DOWN) velocity.Y = 0.15;

	if ((KEY_DOWN == false) && (KEY_UP == false))
	{
		velocity.Y = 0;
	}

	// NO OUT-OF-BOUNDS
	if (Y < 100)
		Y++;
	else if (Y > 650)
		Y--;

	// Sprite dstRect + Conversion
	int iX = int(X + 0.5);
	int iY = int(Y + 0.5);
	dstRect = { iX, iY, 15 * 3, 22 * 3 };
}

void Player::render(SDL_Renderer* aRenderer)
{
	// Hitbox POS + Size
	r->x = X; r->y = Y; r->w = width; r->h = height;
	SDL_SetRenderDrawColor(aRenderer, 0, 0, 0, 255);
	//SDL_RenderFillRect(aRenderer, r);

	// Display the Player
	SDL_RenderCopy(aRenderer, texture, NULL, &dstRect);
}

void Player::serialise(const std::string & path) const
{
	SDL_Log("Saving Player Position Y: %f", Y);

	std::ofstream outStream;
	outStream.open(path, std::ios::out);
	json outObject;

	outObject["position"]["Y"] = this->Y;

	outStream << outObject.dump(1);

	outStream.close();
}

void Player::deserialise(const std::string & path)
{
	std::ifstream instream;
	instream.open(path, std::ios::in);
	json j;
	instream >> j;

	this->Y = j["position"]["Y"].get<float>();

	instream.close();

	SDL_Log("Loaded player position Y: %f", Y);
}


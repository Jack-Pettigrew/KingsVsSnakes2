//Jack Pettigrew - 15593452
//UoL - 2017 - 2018
//Kings Vs Snakes 2

#pragma region Header
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <SDL_mixer.h>
#include <time.h>
#include <fstream>
#include "json.hpp"
#include "Player.h"
#include "EnemySnakes.h"
#include "CastleClass.h"
#include "LevelSprites.h"
#include "ShootController.h"
#include "Keys.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "ScreenText.h"

using namespace std;
using json = nlohmann::json;
#pragma endregion

// GLOBAL VARIABLES
bool gRunning = true;
bool multiplayer = false;
bool pause = false;
bool fullScreen = false;
bool menu = true;
bool instructions = false;
bool endGame = false;
int volume = 100;
int score = 0;

// GLOBAL OBJECTS
SDL_Event event; // Event Controller
ShootController* shooting = NULL; // Shooting Controller
ShootController* shooting2 = NULL;

// IMAGE OBJECTS
LevelSprites bckground(0, 0, 1280, 800); // Game Background scaled to window size
LevelSprites menuBckGround(0, 0, 1280, 800); // Menu Background
LevelSprites logo(325, 100, 812 / 1.5 , 466 / 1.5);
LevelSprites insctructionsBckGround(0, 0, 1280, 800); // Instructions Background
LevelSprites splashScreen(0, 0, 1280, 800);
LevelSprites endGameIMG(0, 0, 1280, 800);
LevelSprites bindingsIMG(0, 0, 1280, 800);

// Objects
Key key;
Player player(125, 125, 45, 64);
Player player2(-300, 300, 45, 64);
CastleClass castle(0, 170, 455, 515);
vector<EnemySnakes*> SnakeVector;

SDL_Joystick *GamePadController = NULL;

// Text
ScreenText *singlePlayerText;
ScreenText *multiplayerText;
ScreenText *instructionsText;
ScreenText *instructionsText2;
ScreenText *instructionsText3;
ScreenText *instructionsText4;
ScreenText *quitText;
vector<ScreenText*> menuItems;
int currentMenuIndex = 0;

ScreenText *scoreText;
ScreenText *healthText;
ScreenText *escText;

// GLOBAL SFX
Mix_Music *Sound_BckGrndMusic = NULL;
Mix_Music *Sound_BckGrndFightMusic = NULL;
Mix_Chunk *Chunk_MenuUp = NULL;
Mix_Chunk *Chunk_MenuDown = NULL;
Mix_Chunk *Chunk_confirm = NULL;
Mix_Chunk *Chunk_SnakeHurt = NULL;

// GAME FUNCTIONS
void init(SDL_Window *window) // Initialise Important
{
	IMG_Init(SDL_INIT_EVERYTHING); // Initialise SDL_IMG

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) // Initialise Music
	{
		printf("Warning: Audio has not been found! \n");
	}
	else
	{
		Sound_BckGrndMusic = Mix_LoadMUS("../content/menuMusic.wav"); // Menu BGM
		Sound_BckGrndFightMusic = Mix_LoadMUS("../content/battleMusic.mp3"); // Game BGM
		Chunk_MenuUp = Mix_LoadWAV("../content/menuUp.wav");
		Chunk_MenuDown = Mix_LoadWAV("../content/menuDown.wav");
		Chunk_confirm = Mix_LoadWAV("../content/confirm.mp3");
		Chunk_SnakeHurt = Mix_LoadWAV("../content/snakeHurt.mp3");
		Mix_VolumeChunk(Chunk_MenuUp, volume);
		Mix_VolumeChunk(Chunk_MenuDown, volume);
		Mix_VolumeChunk(Chunk_confirm, volume);
		Mix_VolumeChunk(Chunk_SnakeHurt, volume);
		Mix_VolumeMusic(volume - 20); // The volume for the music (0-255)
	}

	Mix_PlayMusic(Sound_BckGrndMusic, -1);

	if (SDL_Init(SDL_INIT_JOYSTICK) < 0) // Initialising Gamepad functions
	{ 
		printf("SDL couldn't access Joystick! SDL Error: %s\n", SDL_GetError()); 
	}

	if (SDL_NumJoysticks() < 1) // Check for connected Gamepads
	{
		printf("Warning: No GameControllers connected!\n");
	}
	else
	{
		GamePadController = SDL_JoystickOpen(0);
		printf("GameController connected!\n");
		if (GamePadController == NULL)
		{
			printf("Error - %s \n", SDL_GetError());
		}
	}

	shooting = new ShootController(); // Assigning shooting object
	shooting2 = new ShootController();

	// Attaching the Shoot Controller onto the Player's POS
	shooting->init(player.X, player.Y);
	shooting2->init(player2.X, player2.Y);

	// Inputting the bullets into the vector (the amount needed (3))
	shooting->add(shooting->X, shooting->Y);
	shooting->add(shooting->X, shooting->Y);
	shooting->add(shooting->X, shooting->Y);

	// Adding Snakes to the 'SnakeVector'
	SnakeVector.push_back(new EnemySnakes(2000, 150, 50, 50));
	SnakeVector.push_back(new EnemySnakes(1300, 250, 50, 50));
	SnakeVector.push_back(new EnemySnakes(1750, 350, 50, 50));
	SnakeVector.push_back(new EnemySnakes(1500, 450, 50, 50));
	SnakeVector.push_back(new EnemySnakes(1600, 550, 50, 50));
	SnakeVector.push_back(new EnemySnakes(1950, 650, 50, 50));

	// Setting Text
	scoreText = new ScreenText(window, "../content/Mont_Royal.ttf");
	scoreText->setColor(255, 255, 255);
	scoreText->setText("Score: 0", 75, 740, 32);

	healthText = new ScreenText(window, "../content/Mont_Royal.ttf");
	healthText->setColor(255, 255, 255);
	healthText->setText("Protect the Castle!", 425, 725, 48);

	escText = new ScreenText(window, "../content/Mont_Royal.ttf");
	escText->setColor(255, 255, 225);
	escText->setText("ESC to Quit", 1000, 740, 32);

	singlePlayerText = new ScreenText(window, "../content/Mont_Royal.ttf");
	singlePlayerText->setColor(255, 255, 255);
	singlePlayerText->setText("Single-Player", 500, 500, 32);
	menuItems.push_back(singlePlayerText);

	multiplayerText = new ScreenText(window, "../content/Mont_Royal.ttf");
	multiplayerText->setColor(255, 255, 255);
	multiplayerText->setText("Multiplayer", 500, 550, 32);
	menuItems.push_back(multiplayerText);

	instructionsText = new ScreenText(window, "../content/Mont_Royal.ttf");
	instructionsText->setColor(255, 255, 255);
	instructionsText->setText("Instructions", 500, 600, 32);
	menuItems.push_back(instructionsText);

	quitText = new ScreenText(window, "../content/Mont_Royal.ttf");
	quitText->setColor(255, 255, 255);
	quitText->setText("Quit", 500, 650, 32);
	menuItems.push_back(quitText);

	instructionsText = new ScreenText(window, "../content/Mont_Royal.ttf");
	instructionsText->setColor(255, 255, 255);
	instructionsText->setText("Welcome to Kings Vs Snakes!", 410, 200, 32);

	instructionsText2 = new ScreenText(window, "../content/Mont_Royal.ttf");
	instructionsText2->setColor(255, 255, 255);
	instructionsText2->setText("How long can you save the kingdom from the Snakes?", 275, 300, 32);

	instructionsText3 = new ScreenText(window, "../content/Mont_Royal.ttf");
	instructionsText3->setColor(255, 255, 255);
	instructionsText3->setText("Ensure the enemy Snakes do not reach the castle or face elimination!", 150, 400, 32);

	instructionsText4 = new ScreenText(window, "../content/Mont_Royal.ttf");
	instructionsText4->setColor(255, 255, 255);
	instructionsText4->setText("Change Key Bindings in the JSON file within the content folder", 150, 500, 32);
}

void LoadGraphics(SDL_Renderer* ren) // Obtains Graphical Assets
{
	player.LoadGraphics(ren);
	player2.LoadGraphics(ren);

	for (auto& snake : SnakeVector)
		snake->loadGraphics(ren);

	castle.loadGraphics(ren);
	menuBckGround.loadGraphics("../content/morning.jpg", ren);
	bckground.loadGraphics("../content/field.jpg", ren);
	logo.loadGraphics("../content/KingsVsSnakes2.png", ren);
	insctructionsBckGround.loadGraphics("../content/temple.png", ren);
	endGameIMG.loadGraphics("../content/gameOver.png", ren);
	bindingsIMG.loadGraphics("../content/bindings.png", ren);
}

void input(SDL_Window *win) // Identifies Game Changes
{
	Vector2D aJoystick; // Create the Joystick object
	aJoystick.Y = (SDL_JoystickGetAxis(GamePadController, 1) / 32768.f); // Slow down movement
	//SDL_Log("\rY: %f", aJoystick.Y); // Print out the current Y axis of controller

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) // Close Button works
		{
			gRunning = false;
			menu = false;
		}

		switch (event.type) // Controller Button Press Assigning
		{
		case SDL_JOYAXISMOTION:
			if (event.jaxis.which == 0)
			{
				

				if (event.jaxis.axis == 1)
				{
					player.velocity.Y = aJoystick.Y; // Attaching player Y velocity to Joystick Y
				}
			}
			break;
		case SDL_JOYBUTTONDOWN:
			if (event.jbutton.button == 0)
			{
				shooting->shoot();
				SDL_Log("Shotting Controller");
			}
			if (event.jbutton.button == 7)
			{
				Mix_PlayChannel(-1, Chunk_confirm, 0);
				Mix_PlayMusic(Sound_BckGrndMusic, -1);
				menu = true;
			}
			break;
			case SDL_JOYHATMOTION:
				if (event.jhat.value == SDL_HAT_UP)
				{
					player.KEY_UP = true;
					SDL_Log("'UP' Key Pressed");
				}
				if (event.jhat.value == SDL_HAT_DOWN)
				{
					player.KEY_DOWN = true;
					SDL_Log("'DOWN' Key Pressed");
				}
				if (aJoystick.Y < 0.5f && aJoystick.Y > -0.5f) // Do as long as axis not used
				{
					if (event.jhat.value == SDL_HAT_CENTERED)
					{
						player.KEY_DOWN = false;
						player.KEY_UP = false;
					}
				}
				break;
		}

		// Activate Keys on Press
		if (event.type == SDL_KEYDOWN && event.key.repeat == NULL)
		{
			// Player Movement
			if (event.key.keysym.sym == key.moveUp)
			{
				player.KEY_UP = true;
				SDL_Log("'UP' Key Pressed");
			}
			if (event.key.keysym.sym == key.moveDown)
			{
				player.KEY_DOWN = true;
				SDL_Log("'DOWN' Key Pressed");
			}
			if (event.key.keysym.sym == key.shoot && event.key.repeat == NULL)
			{
				shooting->shoot();
				SDL_Log("Shooting");
			}

			if (multiplayer)
			{
				if (event.key.keysym.sym == SDLK_UP)
				{
					player2.KEY_UP = true;
				}
				if (event.key.keysym.sym == SDLK_DOWN)
				{
					player2.KEY_DOWN = true;
				}
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					shooting2->shoot();
				}
			}

			if (event.key.keysym.sym == SDLK_p)
			{
				if (!pause)
				{
					pause = true;
					Mix_PauseMusic();
					SDL_Log("Game Paused");
				}
				else
				{
					pause = false;
					Mix_ResumeMusic();
					SDL_Log("Game Unpaused");
				}
			}
			if (event.key.keysym.sym == SDLK_f)
			{
				if (!fullScreen)
				{
					SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
					SDL_Log("Fullscreen Mode");
				}
			}

			if (event.key.keysym.sym == SDLK_m) // Music Button
			{
				Mix_PlayChannel(-1, Chunk_confirm, 0);
				if (!Mix_PlayingMusic())
				{
					Mix_PlayMusic(Sound_BckGrndFightMusic, -1);
					SDL_Log("Music Playing");
				}
				else
				{
					Mix_HaltMusic();
					SDL_Log("Music Stopped");
				}
			}
			if (event.key.keysym.sym == SDLK_F1)
			{
				if (volume >= 10)
				{
					volume -= 10;
					Mix_VolumeMusic(volume);
					Mix_VolumeChunk(Chunk_confirm, volume); Mix_VolumeChunk(Chunk_MenuDown, volume); Mix_VolumeChunk(Chunk_MenuUp, volume);
					SDL_Log("Volume: %i", volume);
				}
				else
				{
					SDL_Log("Volume at Minimum: %i", volume);
				}
			}
			if (event.key.keysym.sym == SDLK_F2)
			{
				if (volume <= 240)
				{
					volume += 10;
					Mix_VolumeMusic(volume);
					Mix_VolumeChunk(Chunk_confirm, volume); Mix_VolumeChunk(Chunk_MenuDown, volume); Mix_VolumeChunk(Chunk_MenuUp, volume);
					SDL_Log("Volume: %i", volume);
				}
				else
				{
					SDL_Log("Volume at Maximum: %i", volume);
				}
			}
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				Mix_PlayChannel(-1, Chunk_confirm, 0);
				Mix_PlayMusic(Sound_BckGrndMusic, -1);
				multiplayer = false;
				menu = true;
			}
		}

		// Stop Keys on Release
		if (event.type == SDL_KEYUP && event.key.repeat == NULL)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				player.KEY_UP = false;
				break;
			case SDLK_s:
				player.KEY_DOWN = false;
				break;
			case SDLK_UP:
				player2.KEY_UP = false;
				break;
			case SDLK_DOWN:
				player2.KEY_DOWN = false;
				break;
			}
		}
	}
}

bool checkCollision(SDL_Rect r1, SDL_Rect r2) // Global Collision Check (reference: http://lazyfoo.net/SDL_tutorials/lesson17/index.php)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = r1.x;
	rightA = r1.x + r1.w;
	topA = r1.y;
	bottomA = r1.y + r1.h;

	leftB = r2.x;
	rightB = r2.x + r2.w;
	topB = r2.y;
	bottomB = r2.y + r2.h;

		
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	return true;
}

void EnemyBulletCollision(ShootController *s) // Collision for BULLET and SNAKE
{
	for (auto& bullet : s->BulletList)
	{
		for (auto& snake : SnakeVector)
		{
			if (checkCollision(bullet->r, snake->r))
			{
				// Generate Random Number
				float spawn = rand() % 700 + 1300;
				snake->X = spawn;

				score += 10; // Score Increment
				string scoreChange = "Score: " + to_string(score); // Forming String version of score
				scoreText->setText(scoreChange.c_str(), 75, 740, 32); // String to Char for output
				
				bullet->velocity.X = 0;

				Mix_PlayChannel(-1, Chunk_SnakeHurt, 0);

				SDL_Log("COLLISION: Snake x Castle");
			}
		}
	}
}

void EnemyCastleCollision() // Collision for SNAKE and CASTLE
{
	for (auto& snake : SnakeVector)
	{
		if (checkCollision(snake->r, castle.r))
		{
			snake->X = snake->X + 10;
			castle.health--;

			SDL_Log("COLLISION: Snake x Bullet");

			if (castle.health == 0)
			{
				// END THE GAME -> endGame = true;
				Mix_PlayMusic(Sound_BckGrndMusic, -1);
				endGame = true;
			}
			else
			{
				snake->Speed = -0.05;
			}
		}
	}
}

void Serialise() // Saves Player's last postion before quitting program
{
	player.serialise("../content/player.json");
}

void Deserialise() // Loads Player's last position
{
	player.deserialise("../content/player.json");

	key.loadKeys("../content/keys.json");
}

void update() // Updates Per Frame Changes (Movement, Collision, etc)
{
	if (!pause || endGame) // Pauses Game State
	{
		// Update Functions
		player.update();
		player2.update();

		shooting->update();
		shooting2->update();

		for (auto& snake : SnakeVector)
			snake->update();

		castle.update();

		// Attach shooting controller to player
		shooting->X = player.X;
		shooting->Y = player.Y;

		shooting2->X = player2.X;
		shooting2->Y = player2.Y;

		// Collision Testing
		EnemyBulletCollision(shooting);
		EnemyBulletCollision(shooting2);
		EnemyCastleCollision();
	}
}

void render(SDL_Renderer* ren) // Renders Graphics
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // Background Colour
	SDL_RenderClear(ren);

	bckground.render(ren);

	castle.render(ren);

	shooting->render(ren);
	shooting2->render(ren);
	
	for (auto& snake : SnakeVector)
		snake->render(ren);

	player.render(ren);
	player2.render(ren);

	scoreText->render();
	healthText->render();
	escText->render();
}

void cleanUp() // Program Clean Up
{
	// Clean up Snake and Bullets
	for (auto& snake : SnakeVector)
		delete snake;

	// DELETE MUSIC
	Mix_FreeMusic(Sound_BckGrndMusic); Mix_FreeMusic(Sound_BckGrndFightMusic);
	Mix_FreeChunk(Chunk_confirm); Mix_FreeChunk(Chunk_MenuDown); Mix_FreeChunk(Chunk_MenuUp);
	Mix_CloseAudio();

	// NULLING SOUNDS
	Mix_Music *Sound_BckGrndMusic = NULL;
	Mix_Music *Sound_BckGrndFightMusic = NULL;
	Mix_Chunk *Chunk_MenuUp = NULL;
	Mix_Chunk *Chunk_MenuDown = NULL;
	Mix_Chunk *Chunk_confirm = NULL;
	Mix_Chunk *Chunk_SnakeHurt = NULL;

	// DELETE GAMECONTROLLERS
	SDL_JoystickClose(GamePadController);
	GamePadController = NULL;

	// NULLING SHOOTCONTROLLER
	ShootController* shooting = NULL; 
	ShootController* shooting2 = NULL;

	// DELETE TEXT
	delete scoreText, healthText, escText, singlePlayerText, multiplayerText, instructionsText, instructionsText2, instructionsText3, quitText, menuItems;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	// Create Window + Renderer
	SDL_Window* window = SDL_CreateWindow("Kings Vs Snakes 2 - Jack Pettigrew - ID: 15593452", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	SDL_Renderer* aRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Splash Screen While Loading in Assets
	splashScreen.loadGraphics("../content/SplashScreen.png", aRenderer);
	splashScreen.render(aRenderer);
	SDL_RenderPresent(aRenderer);

	// Loading Assets
	init(window);
	LoadGraphics(aRenderer);

	Deserialise();

	SDL_Delay(1000);

	// GAME LOOP
	while (gRunning)
	{
		while (menu) // Menu Loop within Main Loop
		{
			endGame = false;

			// MENU INPUT
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT) {
					gRunning = false;
					menu = false;
				}

				switch (event.type)
				{
				case SDL_JOYHATMOTION:
					if (event.jhat.value == SDL_HAT_UP)
					{
						if ((currentMenuIndex - 1) < 0)
							currentMenuIndex = menuItems.size() - 1;
						else
							currentMenuIndex--;

						Mix_PlayChannel(-1, Chunk_MenuUp, 0);
					}
					if (event.jhat.value == SDL_HAT_DOWN)
					{
						currentMenuIndex = (currentMenuIndex + 1) % menuItems.size();
						Mix_PlayChannel(-1, Chunk_MenuDown, 0);
					}
					break;
				case SDL_JOYBUTTONDOWN:
					if (event.jbutton.button == 0 && currentMenuIndex == 0)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						SDL_Log("============ Single Player ============");
						score = 0;
						castle.health = 500;
						SnakeVector[0]->X = 2000;
						SnakeVector[1]->X = 1300;
						SnakeVector[2]->X = 1750;
						SnakeVector[3]->X = 1500;
						SnakeVector[4]->X = 1600;
						SnakeVector[5]->X = 1950;
						Mix_HaltMusic();
						Mix_PlayMusic(Sound_BckGrndFightMusic, -1);
						menu = false;
					}
					if (event.jbutton.button == 0 && currentMenuIndex == 1)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						SDL_Log("============ Multiplayer ============");
						menu = false;
					}
					if (event.jbutton.button == 0 && currentMenuIndex == 2)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						instructions = true;
						menu = false;
					}
					if (event.jbutton.button == 0 && currentMenuIndex == 3)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						Mix_HaltMusic();
						SDL_Delay(2000);
						gRunning = false;
						menu = false;
					}
					break;
				}

				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == key.menuDown)
					{
						currentMenuIndex = (currentMenuIndex + 1) % menuItems.size();
						Mix_PlayChannel(-1, Chunk_MenuDown, 0);
					}
					if (event.key.keysym.sym == key.menuUp)
					{
						if ((currentMenuIndex - 1) < 0)
							currentMenuIndex = menuItems.size() - 1;
						else
							currentMenuIndex--;

						Mix_PlayChannel(-1, Chunk_MenuUp, 0);
					}
					if (event.key.keysym.sym == SDLK_f)
					{
						if (!fullScreen)
						{
							SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
							SDL_Log("Fullscreen Mode");
						}
					}
					if (event.key.keysym.sym == key.menuConfirm && currentMenuIndex == 0)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						SDL_Log("============ Single Player ============");
						player2.X = -300;
						score = 0;
						castle.health = 500;
						SnakeVector[0]->X = 2000;
						SnakeVector[1]->X = 1300;
						SnakeVector[2]->X = 1750;
						SnakeVector[3]->X = 1500;
						SnakeVector[4]->X = 1600;
						SnakeVector[5]->X = 1950;
						Mix_HaltMusic();
						Mix_PlayMusic(Sound_BckGrndFightMusic, -1);
						menu = false;
					}
					if (event.key.keysym.sym == key.menuConfirm && currentMenuIndex == 1)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						SDL_Log("============ Multiplayer ============");
						player2.X = 125;
						score = 0;
						castle.health = 500;
						SnakeVector[0]->X = 2000;
						SnakeVector[1]->X = 1300;
						SnakeVector[2]->X = 1750;
						SnakeVector[3]->X = 1500;
						SnakeVector[4]->X = 1600;
						SnakeVector[5]->X = 1950;
						Mix_HaltMusic();
						Mix_PlayMusic(Sound_BckGrndFightMusic, -1);
						multiplayer = true;
						menu = false;
					}
					if (event.key.keysym.sym == key.menuConfirm && currentMenuIndex == 2)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						instructions = true;
						menu = false;
					}
					if (event.key.keysym.sym == key.menuConfirm && currentMenuIndex == 3)
					{
						Mix_PlayChannel(-1, Chunk_confirm, 0);
						Mix_HaltMusic();
						SDL_Delay(2000);
						gRunning = false;
						menu = false;
					}
					if (event.key.keysym.sym == SDLK_F1) // Music Increase
					{
						if (volume >= 10)
						{
							volume -= 10;
							Mix_VolumeMusic(volume);
							Mix_VolumeChunk(Chunk_confirm, volume); Mix_VolumeChunk(Chunk_MenuDown, volume); Mix_VolumeChunk(Chunk_MenuUp, volume);
							SDL_Log("Volume: %i", volume);
						}
						else
						{
							SDL_Log("Volume at Minimum: %i", volume);
						}
					}
					if (event.key.keysym.sym == SDLK_F2)
					{
						if (volume <= 240)
						{
							volume += 10;
							Mix_VolumeMusic(volume);
							Mix_VolumeChunk(Chunk_confirm, volume); Mix_VolumeChunk(Chunk_MenuDown, volume); Mix_VolumeChunk(Chunk_MenuUp, volume);
							SDL_Log("Volume: %i", volume);
						}
						else
						{
							SDL_Log("Volume at Maximum: %i", volume);
						}
					}
					if (event.key.keysym.sym == SDLK_m)
					{
						if (Mix_PlayingMusic() == 0)
						{
							Mix_PlayMusic(Sound_BckGrndMusic, -1);
							SDL_Log("Music Playing");
						}
						else
						{
							Mix_HaltMusic();
							SDL_Log("Music Stopped");
						}
					}
				}
			}

			menuBckGround.render(aRenderer);
			logo.render(aRenderer);

			// MENU UPDATE
			for (int i = 0; i < menuItems.size(); i++)
			{
				if (i == currentMenuIndex)
					menuItems[i]->setColor(255, 0, 0);
				else
					menuItems[i]->setColor(255, 255, 255);

				menuItems[i]->render();
			}

			// MENU RENDER
			SDL_RenderPresent(aRenderer);

			// INSTRUCTIONS LOOP
			while (instructions)
			{
				// INSTRUCTIONS INPUT
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT) // Close Button works
					{
						gRunning = false;
						menu = false;
						instructions = false;
					}

					switch (event.type)
					{
					case SDL_JOYBUTTONDOWN:
						if (event.jbutton.button == 1)
						{
							Mix_PlayChannel(-1, Chunk_confirm, 0);
							menu = true;
							instructions = false;
						}
					}

					if (event.type == SDL_KEYDOWN && event.key.repeat == NULL)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_f:
							if (!fullScreen)
							{
								SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
								SDL_Log("Fullscreen Mode");
							}
							break;
						case SDLK_m: // Music Button
							if (Mix_PlayingMusic() == 0)
							{
								Mix_PlayMusic(Sound_BckGrndMusic, -1);
								SDL_Log("Music Playing");
							}
							else
							{
								Mix_HaltMusic();
								SDL_Log("Music Stopped");
							}
							break;
						case SDLK_F1:
							if (volume >= 10)
							{
								volume -= 10;
								Mix_VolumeMusic(volume);
								SDL_Log("Volume: %i", volume);
							}
							else
							{
								SDL_Log("Volume at Minimum: %i", volume);
							}
							break;
						case SDLK_F2:
							if (volume <= 240)
							{
								volume += 10;
								Mix_VolumeMusic(volume);
								SDL_Log("Volume: %i", volume);
							}
							else
							{
								SDL_Log("Volume at Maximum: %i", volume);
							}
							break;
						case SDLK_ESCAPE:
							instructions = false;
							menu = true;
							break;
						}
					}
				}

				// INSTRUCTIONS RENDER
				insctructionsBckGround.render(aRenderer);
				bindingsIMG.render(aRenderer);
				instructionsText->render();
				instructionsText2->render();
				instructionsText3->render();
				instructionsText4->render();
				escText->render();
				SDL_RenderPresent(aRenderer);
			}
		}

		// Game Functions
		input(window);
		update();
		render(aRenderer);
		SDL_RenderPresent(aRenderer);

		while (endGame) // Upon losing Game
		{
			endGameIMG.render(aRenderer);
			SDL_RenderPresent(aRenderer);
			
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					gRunning = false;
					menu = false;
					instructions = false;
					endGame = false;
				}
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					Mix_PlayChannel(-1, Chunk_confirm, 0);
					player2.Y = -1000;
					menu = true;
					endGame = false;
				}
			}
		}
	}

	Serialise();

	cleanUp();

	return 0;
}
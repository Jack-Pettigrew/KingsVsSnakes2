#include "CollisionController.h"

CollisionController::CollisionController()
{
}

CollisionController::~CollisionController()
{
}

bool CollisionController::CastleVsEnemyCollision(const EnemySnakes &snake, const CastleClass &castle)
{
	// Obj 1
	float snakeLeft = snake.X;
	float snakeRight = snake.X + snake.width;
	float snakeTop = snake.Y;
	float snakeBottom = snake.Y + snake.height;

	// Obj 2
	float castleLeft = castle.X;
	float castleRight = castle.X + castle.width;
	float castleTop = castle.Y;
	float castleBottom = castle.Y + castle.height;

	// Check for Collision
	if (snakeLeft > castleRight)
		return false;

	if (snakeRight < castleLeft)
		return false;

	if (snakeTop > castleBottom)
		return false;

	if (snakeBottom < castleTop)
		return false;

	return true;
}

bool CollisionController::ShootVsSnake(const EnemySnakes & snake, const Bullet & bullet)
{
	// Obj 1
	float snakeLeft = snake.X;
	float snakeRight = snake.X + snake.width;
	float snakeTop = snake.Y;
	float snakeBottom = snake.Y + snake.height;

	// Obj 2
	float bulletLeft = bullet.X;
	float bulletRight = bullet.X + bullet.width;
	float bulletTop = bullet.Y;
	float bulletBottom = bullet.Y + bullet.height;

	// Check for Collision
	if (snakeLeft > bulletRight)
		return false;

	if (snakeRight < bulletLeft)
		return false;

	if (snakeTop > bulletBottom)
		return false;

	if (snakeBottom < bulletTop)
		return false;

	return true;
}

#pragma once
#include "EnemySnakes.h"
#include "CastleClass.h"
#include "Bullet.h"

class CollisionController
{
public:
	CollisionController();
	~CollisionController();

	// ALL POSSIBLE VARIATIONS OF COLLISION
	bool CastleVsEnemyCollision(const EnemySnakes &snake, const CastleClass &castle);

	// Shooting Bullets <-> Enemy Collision (Health decrease, Knock-back?, SFX)
	bool ShootVsSnake(const EnemySnakes &snake, const Bullet &bullet);

	// Walls <-> Enemy Collision
};


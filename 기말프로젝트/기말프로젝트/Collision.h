#pragma once
#include "Player.h"
#include "CMonster.h"
class Collision
{
public:
	RECT BoundngBox;

	bool IsCollided(Player player, CMonster monster);
	bool check_collision(RECT& A, RECT& B);
};


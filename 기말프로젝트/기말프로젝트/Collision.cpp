#include "Collision.h"
#include "global.h"


bool check_collision(RECT& A, RECT& B)
{
	//if (A.left < B.top) return false;
	//if (A.top > B.bottom) return false;
	//if (A.right < B.left) return false;
	//if (A.left > B.right) return false;
	//return true;

	if (A.bottom < B.top) return false;
	if (A.right < B.left) return false;
	if (A.left > B.right) return false;
	if (A.top > B.bottom) return false;
	return true;
}

bool IsCollided(Player player, CMonster monster)
{
	RECT pBox = player.GetAABB();
	RECT mBox = monster.GetAABB();

	check_collision(pBox, mBox);
}
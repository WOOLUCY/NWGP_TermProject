#include "Collision.h"
#include "global.h"

bool check_collision(RECT& A, RECT& B)
{


	if (A.left < B.top) return false;
	if (A.top > B.bottom) return false;
	if (A.right < B.left) return false;
	if (A.left > B.right) return false;
	return true;
}



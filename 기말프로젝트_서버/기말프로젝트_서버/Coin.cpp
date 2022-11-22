#include "Coin.h"

Coin::Coin()
	:iXpos(0), iYpos(0)
{
		aabb.bottom = iYpos + (iHeight / 2);
		aabb.left = iXpos;
		aabb.right = iXpos + (iWidth / 2);
		aabb.top = iYpos;


		send.iXpos = iXpos;
		send.iYpos = iYpos;
		send.bIsCrush = bIsCrush;
}



Coin::Coin(int x, int y)
	:iXpos(x), iYpos(y)
{

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;

	send.iXpos = iXpos;
	send.iYpos = iYpos;
	send.bIsCrush = bIsCrush;
}

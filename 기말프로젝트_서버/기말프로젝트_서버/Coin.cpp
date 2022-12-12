#include "Coin.h"

Coin::Coin()
	:iXpos(0), iYpos(0)
{
		//aabb.bottom = iYpos + (iHeight / 2);
		//aabb.left = iXpos;
		//aabb.right = iXpos + (iWidth / 2);
		//aabb.top = iYpos;

		send.aabb.bottom = iYpos + (iHeight / 1.5);
		send.aabb.left = iXpos;
		send.aabb.right =iXpos + (iWidth / 1.5);
		send.aabb.top = iYpos;


		send.bIsCrush = bIsCrush;
}

Coin::Coin(int x, int y)
	:iXpos(x), iYpos(y)
{

	send.aabb.bottom = iYpos + (iHeight / 1.5);
	send.aabb.left = iXpos;
	send.aabb.right = iXpos + (iWidth / 1.5);
	send.aabb.top = iYpos;
	send.bIsCrush = bIsCrush;
}

void Coin::CoinUpdate() {
	send.aabb.bottom = iYpos + (iHeight / 1.5);
	send.aabb.left = iXpos;
	send.aabb.right = iXpos + (iWidth / 1.5);
	send.aabb.top =iYpos;
}
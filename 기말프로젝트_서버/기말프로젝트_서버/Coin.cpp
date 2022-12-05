#include "Coin.h"

Coin::Coin()
	:iXpos(0), iYpos(0)
{
		//aabb.bottom = iYpos + (iHeight / 2);
		//aabb.left = iXpos;
		//aabb.right = iXpos + (iWidth / 2);
		//aabb.top = iYpos;

		send.aabb.bottom = send.iYpos + (iHeight / 1.5);
		send.aabb.left = send.iXpos;
		send.aabb.right = send.iXpos + (iWidth / 1.5);
		send.aabb.top = send.iYpos;


		send.iXpos = iXpos;
		send.iYpos = iYpos;
		send.bIsCrush = bIsCrush;
}

Coin::Coin(int x, int y)
	:iXpos(x), iYpos(y)
{

	send.aabb.bottom = iYpos + (iHeight / 1.5);
	send.aabb.left = iXpos;
	send.aabb.right = iXpos + (iWidth / 1.5);
	send.aabb.top = iYpos;

	send.iXpos = iXpos;
	send.iYpos = iYpos;
	send.bIsCrush = bIsCrush;
}

void Coin::CoinUpdate() {
	send.aabb.bottom = send.iYpos + (iHeight / 1.5);
	send.aabb.left = send.iXpos;
	send.aabb.right = send.iXpos + (iWidth / 1.5);
	send.aabb.top = send.iYpos;
}
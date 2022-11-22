#include "Coin.h"


#include "global.h"

Coin::Coin()
	:uSpriteX(0), uSpriteY(0),  iXpos(340), iYpos(500), iRange(60)

{
	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//�ϴ� ���� 0���� �ʱ�ȭ�� 

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}



Coin::Coin(int xpos, int ypos, CImage* img)
	:uSpriteX(0), uSpriteY(0)
{
	myImage = img;

	iXpos = xpos;
	iYpos = ypos;

	
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;

}



Coin::~Coin()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}


void Coin::UpdateCoin()
{
	if (bIsCrush) return;

	

}

void Coin::ChangeSprite()
{
	if (uSpriteX ==12) {
		uSpriteX = 0;
	}
	++uSpriteX;
}

#include "CMonster.h"
#include "global.h"
#include <stdlib.h>

CMonster::CMonster()
	: send(), uCharnum(0), fVel(3.f), iRange(60), dir(-1), uMonNum(0)

{
	SetSpriteY(0);
	iMaxX = send.iXpos + iRange;
	iMinX = send.iXpos - iRange;

	//일단 걍다 0으로 초기화함 
	send.aabb.bottom = send.iYpos + (iHeight / 2);
	send.aabb.left = send.iXpos;
	send.aabb.right = send.iXpos + (iWidth / 2);
	send.aabb.top = send.iYpos;


	//send.iXpos = iXpos;
	//send.iYpos = iYpos;
	//send.uSpriteX = uSpriteX;
	//send.uSpriteY = uSpriteY;

}

void CMonster::updateRange() {
	iMaxX = send.iXpos + iRange;
	iMinX = send.iXpos - iRange;
}

CMonster::CMonster(USHORT sprite, USHORT charnum, POS position, float Vel, int range, USHORT in)
	: uCharnum(charnum), fVel(Vel), iRange(range), uMonNum(in)
{
	//이미지 설정하기 

	send.uSpriteX = sprite;
	send.iXpos = position.x;
	send.iYpos = position.y;


}

CMonster::CMonster(int x, int y, int in)
{

	send.iXpos = x;
	send.iYpos = y;

	iRange = 10;

	iMaxX = send.iXpos + iRange;
	iMinX = send.iXpos - iRange;

	uMonNum = in;
}

CMonster::~CMonster()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}



void CMonster::UpdateMonsterLocation(SendData* d)
{
	// dir = std::clamp(-1, (int)velocity.x, 1);
	send.iXpos += dir * fVel;
	if (send.iXpos == iMinX)
	{
		dir = 1;
		SetSpriteY(2);
	}
	if (send.iXpos == iMaxX)
	{
		dir = -1;
		SetSpriteY(0);
	}

	// AABB update
	send.aabb.bottom = send.iYpos + (iHeight / 2);
	send.aabb.left = send.iXpos;
	send.aabb.right = send.iXpos + (iWidth / 2);
	send.aabb.top = send.iYpos;

	//d->iXpos = iXpos;
	//d->iYpos = iYpos;
	//d->uSpriteX = uSpriteX;
	//send.uSpriteY = uSpriteY;

}

void CMonster::ChangeSprite(int* count)
{
	if (*count == 3) {
		send.uSpriteX = (send.uSpriteX + 1) % 8;
		*count = 0;
	}
	*count += 1;
}

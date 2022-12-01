#include "CMonster.h"
#include "global.h"
#include <stdlib.h>

CMonster::CMonster()
	:uSpriteX(0), uSpriteY(0), uCharnum(0), iYpos(625), fVel(3.f), iRange(60), dir(-1), uMonNum(0)

{
	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//일단 걍다 0으로 초기화함 
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;

	send.iXpos = iXpos;
	send.iYpos = iYpos;
	send.uSpriteX = uSpriteX;
	send.uSpriteY = uSpriteY;

}

CMonster::CMonster(USHORT sprite, USHORT charnum, POS position, float Vel, int range, USHORT in)
	:uSpriteX(sprite), uCharnum(charnum), iXpos(position.x), iYpos(position.y), fVel(Vel), iRange(range), uMonNum(in)
{
	//이미지 설정하기 
	send.iXpos = iXpos;
	send.iYpos = iYpos;
	send.uSpriteX = uSpriteX;
	send.uSpriteY = uSpriteY;



}

CMonster::CMonster(int x, int y)
	:uSpriteX(0), uSpriteY(0), uCharnum(0), iXpos(x),iYpos(y), fVel(3.f), iRange(60), dir(-1), uMonNum(0)
{

	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//일단 걍다 0으로 초기화함 
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;


	send.iXpos = iXpos;
	send.iYpos = iYpos;
	send.uSpriteX = uSpriteX;
	send.uSpriteY = uSpriteY;



}

CMonster::~CMonster()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}


void CMonster::UpdateMonsterLocation(SendData* d)
{
	// dir = std::clamp(-1, (int)velocity.x, 1);
	iXpos += dir * fVel;
	if (iXpos == iMinX)
	{
		dir = 1;
		SetSpriteY(1);
	}
	if (iXpos == iMaxX)
	{
		dir = -1;
		SetSpriteY(0);
	}

	// AABB update
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;

	d->iXpos = iXpos;
	d->iYpos = iYpos;
	d->uSpriteX = uSpriteX;
	d->uSpriteY = uSpriteY;
	


}

void CMonster::ChangeSprite(int* count)
{
	if (*count == 3) {
		uSpriteX = (uSpriteX + 1) % 4;
		*count = 0;
	}
	*count += 1;
}

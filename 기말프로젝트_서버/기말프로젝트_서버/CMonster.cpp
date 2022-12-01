#include "CMonster.h"
#include "global.h"
#include <stdlib.h>

CMonster::CMonster()
	:uSpriteX(0), uSpriteY(0), uCharnum(0), iYpos(625), fVel(3.f), iRange(60), dir(-1), uMonNum(0)

{
	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//�ϴ� ���� 0���� �ʱ�ȭ�� 
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
	//�̹��� �����ϱ� 
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

	//�ϴ� ���� 0���� �ʱ�ȭ�� 
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
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
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

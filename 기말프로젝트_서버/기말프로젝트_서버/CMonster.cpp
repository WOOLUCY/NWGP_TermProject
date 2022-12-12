#include "CMonster.h"
#include "global.h"
#include <stdlib.h>

CMonster::CMonster()
	: send(), uCharnum(0), fVel(3.f), iRange(60), dir(-1), uMonNum(0)

{
	SetSpriteY(0);
	iMaxX = send.iXpos + iRange;
	iMinX = send.iXpos - iRange;

	//�ϴ� ���� 0���� �ʱ�ȭ�� 
	send.aabb.bottom = send.iYpos + (iHeight / 2);
	send.aabb.left = send.iXpos;
	send.aabb.right = send.iXpos + (iWidth / 2);
	send.aabb.top = send.iYpos;

}

void CMonster::updateRange(int range) {
	//�⺻ range �ϴ� 10
	//iRange = range;
	iMaxX = send.iXpos + iRange;
	iMinX = send.iXpos - iRange;
}

CMonster::CMonster(USHORT sprite, USHORT charnum, POS position, float Vel, int range, USHORT in)
	: uCharnum(charnum), fVel(Vel), iRange(range), uMonNum(in)
{
	//�̹��� �����ϱ� 

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
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
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
	if (send.isDeath == FALSE) {
		send.aabb.bottom = send.iYpos + (iHeight / 2);
		send.aabb.left = send.iXpos + iWidth / 8;
		send.aabb.right = send.iXpos + (iWidth / 4);
		send.aabb.top = send.iYpos;
	}
	else {
		send.aabb.bottom = 4000;
		send.aabb.top = 4000;
		send.aabb.right = 4000;
		send.aabb.left = 4000;
	}

	
}

void CMonster::DeathMonster()
{
	send.uSpriteY = 1;
}

void CMonster::ChangeSprite(int* count)
{
	if (*count == 3) {
		send.uSpriteX = (send.uSpriteX + 1) % 8;
		if (send.isDeath == FALSE) {
			*count = 0;
		}
		else {
			if (send.uSpriteX >= 6) {
				send.iXpos = 4000;
				send.iYpos = 4000;
			}
		}
	}
	*count += 1;
}

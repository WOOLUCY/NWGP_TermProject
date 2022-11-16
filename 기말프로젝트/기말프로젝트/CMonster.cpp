#include "CMonster.h"
#include "global.h"

CMonster::CMonster()
	:uSpriteX(0), uSpriteY(0), uCharnum(0), iXpos(340), iYpos(625), fVel(3.f), iRange(60), dir(-1)

{
	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//�ϴ� ���� 0���� �ʱ�ȭ�� 
	myImage[0] = nullptr;
	myImage[1] = nullptr;
	myImage[2] = nullptr;
	myImage[3] = nullptr;
	aabb.bottom = 0;
	aabb.left = 0;
	aabb.right = 0;
	aabb.top = 0;
}

CMonster::CMonster(USHORT sprite, USHORT charnum, POS position, float Vel, int range)
	:uSpriteX(sprite), uCharnum(charnum), iXpos(position.x), iYpos(position.y), fVel(Vel), iRange(range)
{
	//�̹��� �����ϱ� 

	switch (uCharnum)
	{
	case 0:

		break;
	case 1:

		break;
	case 2:

		break;

	default:
		break;
	}
}

CMonster::~CMonster()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}


void CMonster::UpdateMonsterLocation()
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
}

void CMonster::ChangeSprite(int* count)
{
	if (*count == 3) {
		uSpriteX = (uSpriteX + 1) % 4;
		*count = 0;
	}
	*count += 1;
}

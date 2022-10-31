#pragma once
#include "global.h"

class Monster
{
private:
	POS		pPosition;
	POS		pVel;
	USHORT	uSprite;

	UINT	iHp;

	bool	bAlive;

	RECT	aabb;

	//�̰� �ʿ����� �𸣰ڴµ� �ϴ� �־�� Monster�� �Ѱ�->�����ڼҸ��ڿ��� �����ϰ���
	static UINT iTotal;

public:
	CImage* myImage[2];

public:
	
	Monster();
	Monster(POS, POS ,USHORT, UINT, bool, RECT);
	~Monster() { --iTotal; };


	POS GetPosition() { return pPosition; }
	void SetPosition(POS in) { pPosition = in; }

	POS GetVel() { return pVel; }
	void SetVel(POS in) { pVel = in; }

	USHORT GetSprite() { return uSprite; }
	void SetSprite(USHORT in) { uSprite = in; }

	UINT GetHp() { return iHp; }
	void SetHp(UINT in) { iHp = in; }

	bool GetAlive() { return bAlive; }
	void SetAlive(bool in) { bAlive = in; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }


	void update();

};
UINT Monster::iTotal{ 0 };	// ���� �� ����
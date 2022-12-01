#pragma once
#include "global.h"
#include <atlImage.h>

class CMonster
{
private:
	int		iWidth = 144;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 138;	// ���� ũ��� sprite ������ ���̶� ������ ��
	int		iRange;			// range�� ��ȸ ������ ������ �� ����
	USHORT	uMonNum;

	RECT	aabb;
	float	fVel;

	int		iDir;
	int		iMaxX;
	int		iMinX;

public:

	struct SendData {
		int		iXpos;
		int		iYpos;
		USHORT	uSpriteX;
		USHORT	uSpriteY;

	};

	SendData send;

	USHORT	uCharnum;
	USHORT	uSpriteX;
	USHORT	uSpriteY;
	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;
	int		dir;
	POS		velocity = { 0, 0 };

public:
	USHORT GetMonNum() { return uMonNum; }
	void SetMonNum(USHORT in) { uMonNum = in; }

	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }

	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }

	float GetVel() { return fVel; }
	void SetVel(float in) { fVel = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

	int GetRange() { return iRange; }
	void SetRange(int in) { iRange = in; }

public:
	//������
	CMonster();
	CMonster(USHORT sprite, USHORT charnum, POS position, float Vel, int range, USHORT in);
	CMonster(int x, int y);
	~CMonster();

	void ChangeSprite(int* count);
	void UpdateMonsterLocation();
};





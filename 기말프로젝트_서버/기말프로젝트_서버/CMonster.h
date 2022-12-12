#pragma once
#include "global.h"
#include <atlImage.h>

class CMonster
{
private:
	int		iWidth = 144;	// draw 에 필요한 width, height 추가
	int		iHeight = 138;	// 고정 크기로 sprite 제작할 것이라서 고정해 둠
	int		iRange;			// range로 배회 범위를 지정할 수 있음
	USHORT	uMonNum;

	POS		pVel;
	float	fVel;

	int		iDir;

public:

	struct SendData {
		int		iXpos = 700;
		int		iYpos = 625;
		USHORT	uSpriteX = 0;
		USHORT	uSpriteY = 0;
		RECT	aabb;
		bool	isDeath = FALSE;

	};

	SendData send;

	CImage* myImage; //이거로 스프라이트 가리키면 될것같은데 

	USHORT	uCharnum;
	int		dir;
	POS		velocity = { 0, 0 };
	int		iMaxX;
	int		iMinX;

public:
	USHORT GetMonNum() { return uMonNum; }
	void SetMonNum(USHORT in) { uMonNum = in; }

	USHORT GetSpriteX() { return send.uSpriteX; }
	void SetSpriteX(USHORT in) { send.uSpriteX = in; }
	void AddSpriteX() { send.uSpriteX++; }

	USHORT GetSpriteY() { return send.uSpriteY; }
	void SetSpriteY(USHORT in) { send.uSpriteY = in; }

	int GetXPos() { return send.iXpos; }
	void SetXPos(int in) { send.iXpos = in; }

	int GetYPos() { return send.iYpos; }
	void SetYPos(int in) { send.iYpos = in; }

	float GetVel() { return fVel; }
	void SetVel(float in) { fVel = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	RECT GetAABB() { return send.aabb; }
	void SetAABB(RECT in) { send.aabb = in; }

	int GetRange() { return iRange; }
	void SetRange(int in) { iRange = in; }

	void updateRange(int range);

public:
	//생성자
	CMonster();
	CMonster(USHORT sprite, USHORT charnum, POS position, float Vel, int range, USHORT in);
	CMonster(int x, int y, int in);
	~CMonster();

	void ChangeSprite(int* count);
	void UpdateMonsterLocation(SendData* d);
	void DeathMonster();
};





#pragma once
#include "global.h"
#include "CMonster.h"
// #include "Coin.h"
// #include <atlImage.h>

struct KeyInput {
	bool            bRight;
	bool            bLeft;
	bool            bSpace;
};

class Player
{
private:
	LPCWCHAR wId;

	int		iWidth = 170;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 148;	// ���� ũ��� sprite ������ ���̶� ������ ��

	POS		pVel;
	USHORT	MaxJump;		// ��Ű�� �ִ� ������

	USHORT	sHeart; 
	USHORT	uCoin;

	bool	bFind;
	RECT	aabb;

	float	RUN_SPEED_PPS;
	float	fJumpPower;


		
public:
	CImage*	myImage[4]; //�̰ŷ� ��������Ʈ ����Ű�� �ɰͰ����� 
	// ���� ������ �� ���� ���� ���ص� ��� ���� �ֵ��� public ���� ��
	USHORT	uCharnum;
	USHORT	uSpriteX;
	USHORT	uSpriteY;
	int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	int		iYpos;
	int		dir;
	POS		velocity;
	bool	bJumpKeyPressed = FALSE;
	float	fJumpTime;
	float	JumpHeight;
	KeyInput input;

public:
	LPCWCHAR GetId() { return wId; }
	void SetId(LPCWCHAR in) { wId = in; }

	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }

	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }

	POS GetVel() { return pVel; }
	void SetVel(POS in) { pVel = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	USHORT GetHeart() { return sHeart; }
	void SetHeart(USHORT in) { sHeart = in; }

	USHORT GetCoin() { return uCoin; }
	void SetCoin(USHORT in) { uCoin = in; }

	bool GetFind() { return bFind; }
	void SetFind(bool in) { bFind = in; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

	wchar_t GetMaxJump() { return MaxJump; }
	void SetMaxJump(USHORT in) { MaxJump = in; }

	float GetRunSpeed() { return RUN_SPEED_PPS; }
	void SetRunSpeed(float in) { RUN_SPEED_PPS = in; }


public: 
	//������
	Player();
	Player(LPCWCHAR id, USHORT sprite,USHORT charnum,POS position, POS Vel, USHORT heart, USHORT coin, bool find);
	
	~Player();
	
	void Jump(USHORT spriteCnt);
	void ChangeSprite(int* count);
	void UpdatePlayerLocation();

	// W collsion
	bool IsCollidedMonster(CMonster monster);
	// bool IsCollidedCoin(Coin coin);
};




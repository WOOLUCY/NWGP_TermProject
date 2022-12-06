#pragma once
#include "global.h"
#include "CMonster.h"
#include "Coin.h"
#include "Key.h"
#include "Portal.h"
#include <atlImage.h>

struct KeyInput {
	bool            bRight = 0;
	bool            bLeft = 0;
	bool            bSpace = 0;
};

class Player
{
private:
	LPCWCHAR wId = 0;

	bool	IsReady = FALSE;	// W ĳ���͸� �����ߴ��� ����
	USHORT	uCharNum = 0;
	bool	HasKey = FALSE;		// W ĳ���Ͱ� Ű�� �����ߴ��� ����

	int		iWidth = 170;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 148;	// ���� ũ��� sprite ������ ���̶� ������ ��

	POS		pVel;
	USHORT	MaxJump;		// ��Ű�� �ִ� ������


	//USHORT	uCoin;

	bool	bFind;

	float	RUN_SPEED_PPS;
	float	fJumpPower;

	USHORT	uRecCollidedMon;

	RECT	aabb;

public:
	struct SendData {
		int		charNum = 999;
		int		iXpos;
		int		iYpos;
		USHORT	uSpriteX;
		USHORT	uSpriteY;
		int		iBgMove;
		wchar_t wID[21] = { '\0' };
		RECT	aabb;
		USHORT	uHeart;
		USHORT	uScore;

	};

	SendData Send;


	int		iBgMove;
	CImage*	myImage; //�̰ŷ� ��������Ʈ ����Ű�� �ɰͰ����� 
	// ���� ������ �� ���� ���� ���ص� ��� ���� �ֵ��� public ���� ��
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
	KeyInput up;

public:
	LPCWCHAR GetId() { return wId; }
	void SetId(LPCWCHAR in) { wId = in; }

	bool GetIsReady() { return IsReady; }
	void SetIsReady(bool in) { IsReady = in; }

	bool GetHasKey() { return HasKey; }
	void SetHasKey(bool in) { HasKey = in; }

	int GetCharNum() {return uCharNum;}
	void SetCharNum(USHORT in) { uCharNum = in; }

	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }

	USHORT GetRecCollidedMon() { return uRecCollidedMon; }
	void SetRecCollidedMon(USHORT in) { uRecCollidedMon = in; }

	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }

	POS GetVel() { return pVel; }
	void SetVel(POS in) { pVel = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	//USHORT GetHeart() { return sHeart; }
	//void SetHeart(USHORT in) { sHeart = in; }

	USHORT GetCoin() { return Send.uScore; }
	void SetCoin(USHORT in) { Send.uScore = in; }

	bool GetFind() { return bFind; }
	void SetFind(bool in) { bFind = in; }

	RECT GetAABB() { return Send.aabb; }
	void SetAABB(RECT in) { Send.aabb = in; }

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
	int IsCollidedMonster(CMonster monster);
	bool IsCollidedCoin(Coin coin);
	bool IsCollidedKey(Key key);
	bool IsCollidedPortal(Portal portal);
};




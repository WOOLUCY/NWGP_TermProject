#pragma once
#include "global.h"
#include "CMonster.h"
#include "Coin.h"
#include "Platform.h"
#include "Key.h"
#include <atlImage.h>
#include "Portal.h"

struct KeyInput {
	bool            bRight = 0;
	bool            bLeft = 0;
	bool            bSpace = 0;
};

class Player
{
private:
	LPCWCHAR wId = 0;

	POS		pVel;

	bool	IsReady = FALSE;	// W ĳ���͸� �����ߴ��� ����
	USHORT	uCharNum = 999;

	int		iWidth = 170;	// draw �� �ʿ��� width, height �߰�
	int		iHeight = 148;	// ���� ũ��� sprite ������ ���̶� ������ ��

	USHORT	MaxJump;		// ��Ű�� �ִ� ������


	bool	bFind;

	float	RUN_SPEED_PPS;
	float	fJumpPower;

	USHORT	uRecCollidedMon;

public:
	bool	bHasKey;
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
		USHORT	uRank;
		bool	timeRecord = FALSE; // ���� �ð� ��� �����ߴ���
		bool	bHasKey;
	};

	SendData Send;


	int		iBgMove =  0;
	USHORT	uSpriteX;
	USHORT	uSpriteY;
	//int		iXpos;	// POS ���� draw �Լ��� �� ���� ��� int, int �� ��������
	//int		iYpos;
	int		dir;
	POS		velocity;
	bool	bJumpKeyPressed = FALSE;
	float	fJumpTime;
	float	JumpHeight;
	KeyInput input;
	KeyInput up;
	bool	killMonster = FALSE;	// ���� �׿��� �� ƨ�ܳ����� ���� ��
	int curSpriteCnt;
	double	invincibleTime	= 3.f; // ���� �ð�
	time_t	iTimeStart;		// ���� �ð� ��� ���� �ð�
	time_t	iTimePre;		// ���� �ð� ��� ���� �ð�


public:
	LPCWCHAR GetId() { return wId; }
	void SetId(LPCWCHAR in) { wId = in; }

	bool GetIsReady() { return IsReady; }
	void SetIsReady(bool in) { IsReady = in; }

	int GetCharNum() { return Send.charNum; }
	void SetCharNum(USHORT in) { Send.charNum= in; }

	USHORT GetSpriteX() { return uSpriteX; }
	void SetSpriteX(USHORT in) { uSpriteX = in; }
	void AddSpriteX() { uSpriteX++; }

	USHORT GetSpriteY() { return uSpriteY; }
	void SetSpriteY(USHORT in) { uSpriteY = in; }

	USHORT GetRecCollidedMon() { return uRecCollidedMon; }
	void SetRecCollidedMon(USHORT in) { uRecCollidedMon = in; }

	int GetXPos() { return Send.iXpos; }
	void SetXPos(int in) { Send.iXpos = in; }

	int GetYPos() { return Send.iYpos; }
	void SetYPos(int in) { Send.iYpos = in; }

	POS GetVel() { return pVel; }
	void SetVel(POS in) { pVel = in; }

	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	//USHORT GetHeart() { return uHeart; }
	//void SetHeart(USHORT in) { uHeart = in; }

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
	Player(LPCWCHAR id, USHORT sprite, USHORT charnum, POS position, POS Vel, USHORT heart, USHORT coin, bool find);

	~Player();

	void Jump(USHORT spriteCnt);
	void ChangeSprite(int* count);
	void UpdatePlayerLocation();

	// W collsion, semin collision
	int IsCollidedMonster(CMonster* monster);
	bool IsCollidedKey(Key key);
	bool IsCollidedPortal(Portal portal);
	bool IsCollidedCoin(Coin* coin);
	bool IsCollidedPlatform(Platform platform);
	bool IsNotCollidedPlatform(Platform platform);
	void CheckLocationCollideMonster(CMonster* monster);


};




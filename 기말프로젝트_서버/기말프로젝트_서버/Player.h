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

	bool	IsReady = FALSE;	// W 캐릭터를 선택했는지 여부
	USHORT	uCharNum = 999;

	int		iWidth = 170;	// draw 에 필요한 width, height 추가
	int		iHeight = 148;	// 고정 크기로 sprite 제작할 것이라서 고정해 둠

	USHORT	MaxJump;		// 쿠키의 최대 점프력


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
		bool	timeRecord = FALSE; // 무적 시간 기록 시작했는지
		bool	bHasKey;
	};

	SendData Send;


	int		iBgMove =  0;
	USHORT	uSpriteX;
	USHORT	uSpriteY;
	//int		iXpos;	// POS 형을 draw 함수에 쓸 수가 없어서 int, int 로 나누었음
	//int		iYpos;
	int		dir;
	POS		velocity;
	bool	bJumpKeyPressed = FALSE;
	float	fJumpTime;
	float	JumpHeight;
	KeyInput input;
	KeyInput up;
	bool	killMonster = FALSE;	// 몬스터 죽였을 때 튕겨나가기 위해 둠
	int curSpriteCnt;
	double	invincibleTime	= 3.f; // 무적 시간
	time_t	iTimeStart;		// 무적 시간 재는 시작 시간
	time_t	iTimePre;		// 무적 시간 재는 현재 시간


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
	//생성자
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




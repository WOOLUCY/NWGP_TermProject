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

	int		iWidth = 170;	// draw 에 필요한 width, height 추가
	int		iHeight = 148;	// 고정 크기로 sprite 제작할 것이라서 고정해 둠

	POS		pVel;
	USHORT	MaxJump;		// 쿠키의 최대 점프력

	USHORT	sHeart; 
	USHORT	uCoin;

	bool	bFind;
	RECT	aabb;

	float	RUN_SPEED_PPS;
	float	fJumpPower;


		
public:
	CImage*	myImage[4]; //이거로 스프라이트 가리키면 될것같은데 
	// 자주 참조할 것 같고 값이 변해도 상관 없는 애들은 public 으로 뺌
	USHORT	uCharnum;
	USHORT	uSpriteX;
	USHORT	uSpriteY;
	int		iXpos;	// POS 형을 draw 함수에 쓸 수가 없어서 int, int 로 나누었음
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
	//생성자
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




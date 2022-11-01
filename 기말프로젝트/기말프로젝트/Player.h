#pragma once
#include "global.h"

#include <atlImage.h>

class Player
{
private:
	wchar_t wId;
	USHORT	uSpriteX;
	USHORT	uSpriteY;

	int		iWidth = 170;	// draw 에 필요한 width, height 추가
	int		iHeight = 148;	// 고정 크기로 sprite 제작할 것이라서 고정해 둠

	POS		pVel;
	USHORT	MaxJump;		// 쿠키의 최대 점프력

	USHORT	sHeart; 
	USHORT	uCoin;

	bool	bFind;
	RECT	aabb;

		
public:
	CImage*	myImage[4]; //이거로 스프라이트 가리키면 될것같은데 
	// 자주 참조할 것 같고 값이 변해도 상관 없는 애들은 public 으로 뺌
	USHORT	uCharnum;
	int		iXpos;	// POS 형을 draw 함수에 쓸 수가 없어서 int, int 로 나누었음
	int		iYpos;
	USHORT	JumpHeight;	// 지금 점프하고 있는 위치로 쓰려고 했는데... AABB 쓰면 필요 없을 것 같음
	
public:
	wchar_t GetId() { return wId; }
	void SetId(wchar_t in) { wId = in; }

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

public: 
	//생성자
	Player();
	Player(wchar_t id, USHORT sprite,USHORT charnum,POS position, POS Vel, USHORT heart, USHORT coin, bool find);
	
	~Player();

	void Move(POS Direction); //무브,rect도 같이 움직는거로 할까??
	void Jump();
	void ChangeSprite();
};




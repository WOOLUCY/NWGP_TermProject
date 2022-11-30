#include "Player.h"
#include "global.h"
#include <iostream>

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharNum(0), iXpos(640), iYpos(620), pVel(0, 0), sHeart(0), uCoin(0), bFind(0), MaxJump(4), JumpHeight(0), uRecCollidedMon(0)

{
	//일단 걍다 0으로 초기화함 


	// W AABB initialization
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
	RUN_SPEED_PPS = (10.0 / 60.0) * (10.0 / 0.3);		// 플레이어 기본 속도는 이것

	velocity = { 0, 0 };
	dir = 0;
	input = { 0 };

	bJumpKeyPressed = FALSE;
	fJumpPower = 4.f;
	fJumpTime = 0.01;
	JumpHeight = 0;

	Send.charNum = uCharNum-1;
	Send.iXpos = iXpos;
	Send.iYpos = iYpos;
	Send.uSpriteX = uSpriteX;
	Send.uSpriteY = uSpriteY;

}

Player::Player(LPCWCHAR id, USHORT sprite, USHORT charnum, POS position, POS Vel, USHORT heart, USHORT coin, bool find)
	:wId(id), uSpriteX(sprite), uCharNum(charnum), iXpos(position.x), iYpos(position.y), pVel(Vel), sHeart(heart), uCoin(coin), bFind(find)
{
	//이미지 설정하기 

	Send.charNum = uCharNum;

	Send.iXpos = iXpos;
	Send.iYpos = iYpos;
	Send.uSpriteX = uSpriteX;
	Send.uSpriteY = uSpriteY;
}

Player::~Player()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}


void Player::Jump(USHORT spriteCnt)
{
	if (!bJumpKeyPressed) {
		return;
	}
	
	SetSpriteY(2);
	velocity.y = (fJumpTime * fJumpTime - fJumpPower * fJumpTime) * 4; //	4로 나눈이유는, 너무 높이 뛰어서. 값을 낮추기 위해.
	fJumpTime += 0.2f;		//	시간의 흐름을 표현하기 위해서.

	if (fJumpTime > fJumpPower * 2)	//	착지했다면의 의미로, 각 변수 초기화.
	{
		SetSpriteY(spriteCnt);
		fJumpTime = 0;
		velocity.y = 0;
		bJumpKeyPressed = false;
		input.bSpace = false;



	}

	if (iYpos > 620) { //이거 바닥?
		SetSpriteY(spriteCnt);
		fJumpTime = 0;
		velocity.y = 0;
		iYpos = 620;
		input.bSpace = false;

		bJumpKeyPressed = false;


	}
}

void Player::UpdatePlayerLocation()
{
	// dir = std::clamp(-1, (int)velocity.x, 1);
	iXpos += velocity.x;
	iYpos += velocity.y;

	// W update collision box
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;

	Send.iXpos = iXpos;
	Send.iYpos = iYpos;
	Send.uSpriteX = uSpriteX;
	Send.uSpriteY = uSpriteY;

}

void Player::ChangeSprite(int* count)
{
	// count 는 timer 의 시간에 따라 1 씩 올라감
	// 밑의 조건문에 count == "숫자" 숫자 올릴수록 스프라이트는 천천히 돌아갈 것
	if (*count == 3) {
		uSpriteX = (uSpriteX + 1) % 4;
		*count = 0;
	}
	*count += 1;
}

// 충돌 시 플레이어와 충돌한 몬스터의 일련번호 반환. 0이면 충돌하지 않았다는 뜻.
int Player::IsCollidedMonster(CMonster monster)
{
	RECT A = aabb;
	RECT B = monster.GetAABB();

	if (A.bottom < B.top) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	uRecCollidedMon = monster.GetMonNum();
	return uRecCollidedMon;
}

bool Player::IsCollidedCoin(Coin coin)
{
	RECT A = aabb;
	RECT B = coin.GetAABB();

	if (A.bottom < B.top) return false;
	if (A.right < B.left) return false;
	if (A.left > B.right) return false;
	if (A.top > B.bottom) return false;
	return true;
}


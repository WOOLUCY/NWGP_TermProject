#include "Player.h"
#include "global.h"
#include <iostream>
#include <algorithm>

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharNum(999), pVel(0, 0), bFind(0), MaxJump(4), JumpHeight(0), uRecCollidedMon(0)
{
	//일단 걍다 0으로 초기화함 
	Send.iXpos = 640;
	Send.iYpos = 620;
	Send.uScore = 0;

	// W AABB initialization
	Send.aabb.bottom = Send.iYpos + (iHeight / 2);
	Send.aabb.left = Send.iXpos;
	Send.aabb.right = Send.iXpos + (iWidth / 2);
	Send.aabb.top = Send.iYpos;
	RUN_SPEED_PPS = (10.0 / 60.0) * (10.0 / 0.3);		// 플레이어 기본 속도는 이것

	velocity = { 0, 0 };
	dir = 0;
	input = { 0 };

	bJumpKeyPressed = FALSE;
	fJumpPower = 4.f;
	fJumpTime = 0.01;
	JumpHeight = 0;

	Send.charNum = uCharNum-1;
	Send.uSpriteX = uSpriteX;
	Send.uSpriteY = uSpriteY;

}

Player::Player(LPCWCHAR id, USHORT sprite, USHORT charnum, POS position, POS Vel, USHORT heart, USHORT coin, bool find)
	:wId(id), uSpriteX(sprite), uCharNum(charnum), pVel(Vel), bFind(find)
{
	//이미지 설정하기 

	Send.charNum = uCharNum;
	Send.uScore = coin;
	Send.iXpos = position.x;
	Send.iYpos = position.y;
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

	if (spriteCnt == 1)
		SetSpriteY(2);
	else if (spriteCnt == 3)
		SetSpriteY(5);
	else if (spriteCnt == 0)
		SetSpriteY(2);
	else if (spriteCnt == 4)
		SetSpriteY(5);
	
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

	if (Send.iYpos > 620) { //이거 바닥?
		SetSpriteY(spriteCnt);
		fJumpTime = 0;
		velocity.y = 0;
		Send.iYpos = 620;
		input.bSpace = false;

		bJumpKeyPressed = false;


	}
}

void Player::UpdatePlayerLocation()
{
	// dir = std::clamp(-1, (int)velocity.x, 1);
	Send.iXpos += velocity.x;
	Send.iYpos += velocity.y;


	if (!(input.bLeft) && (up.bLeft)) {
		//왼쪽으로 이동
		velocity.x = 0;
		SetSpriteY(4);
	}

	if (!(input.bRight) && (up.bRight)) {
		//왼쪽으로 이동
		velocity.x = 0;
		SetSpriteY(0);
	}

	// 캐릭터의 위치가 이동할 때는 배경화면이 끝에 갔을 때 ( 왼쪽 끝 )
	if (Send.iBgMove <= -200) {
		if (input.bLeft) {
			curSpriteCnt = 3;
			Send.iXpos += -GetRunSpeed();
			SetSpriteY(3);

		}

		if (input.bRight) {
			curSpriteCnt = 1;
			Send.iXpos += GetRunSpeed();
			SetSpriteY(1);
			if (Send.iXpos >= 640)
				Send.iBgMove = -195;

		}
	}

	// 캐릭터의 위치가 이동할 때는 배경화면이 끝에 갔을 때 ( 오른쪽 끝 )
	else if (Send.iBgMove >= 1200) {
		if (input.bLeft) {
			curSpriteCnt = 3;
			Send.iXpos += -GetRunSpeed();
			SetSpriteY(3);
			if (Send.iXpos <= 640)
				Send.iBgMove = 1195;
		}

		if (input.bRight) {
			curSpriteCnt = 1;
			Send.iXpos += GetRunSpeed();
			SetSpriteY(1);
		}
	}

	/* 그 외에는 배경화면이 이동 */
	else {
		if (input.bLeft) {
			//왼쪽으로 이동
			curSpriteCnt = 3;
			//p->velocity.x = -p->GetRunSpeed();
			Send.iBgMove += -GetRunSpeed();
			Send.iBgMove = std::clamp(Send.iBgMove, -200, 1200);
			SetSpriteY(3);
		}

		if (input.bRight) {
			//오른쪽으로 이동
			curSpriteCnt = 1;

			//p->velocity.x = p->GetRunSpeed();
			Send.iBgMove += GetRunSpeed();
			Send.iBgMove = std::clamp(Send.iBgMove, -200, 1200);
			SetSpriteY(1);
		}
	}
	if (input.bSpace) {
		//왼쪽 점프
		bJumpKeyPressed = TRUE;
		Jump(curSpriteCnt);
	}

	//Send.iXpos = Send.iXpos;
	//Send.iYpos = Send.iYpos;
	Send.uSpriteX = uSpriteX;
	Send.uSpriteY = uSpriteY;

	// W update collision box
	Send.aabb.bottom = Send.iYpos + (iHeight / 2);
	Send.aabb.left = Send.iXpos;
	Send.aabb.right = Send.iXpos + (iWidth / 2);
	Send.aabb.top = Send.iYpos;

	//printf("player: %d %d %d %d\n", Send.aabb.bottom, Send.aabb.left, Send.aabb.right, Send.aabb.left);
	//printf("%d %d \n", Send.iXpos, Send.iYpos);
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
	RECT A = Send.aabb;
	A.left += Send.iBgMove /2;
	A.right += Send.iBgMove /2;
	RECT B = monster.GetAABB();

	if (A.bottom < B.top) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	uRecCollidedMon = monster.GetMonNum();
	return uRecCollidedMon;
}

bool Player::IsCollidedCoin(Coin* coin)
{
	RECT A = Send.aabb;
	A.left += Send.iBgMove / 2;
	A.right += Send.iBgMove / 2;
	RECT B = coin->GetAABB();

	if (A.bottom < B.top) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	//coin->send.iXpos = 4000;
	//coin->send.iYpos = 4000;
	if (coin->send.bIsCrush == FALSE) {
		Send.uScore += 100;
		coin->SetIsCrush(TRUE);
		coin->send.bIsCrush = TRUE;
		printf("\n코인 먹었당 %d\n", Send.uScore);

	}
	return true;
}


#include "Player.h"
#include "global.h"

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharnum(0), iXpos(640), iYpos(620), pVel(0,0), sHeart(0), uCoin(0), bFind(0), MaxJump(4), JumpHeight(0)

{
	//일단 걍다 0으로 초기화함 
	myImage[0] = nullptr;
	myImage[1] = nullptr;
	myImage[2] = nullptr;
	myImage[3] = nullptr;

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
	fJumpPower = 3.f;
	fJumpTime = 0.1;
	JumpHeight = 0;
}

Player::Player(LPCWCHAR id, USHORT sprite, USHORT charnum, POS position, POS Vel, USHORT heart, USHORT coin, bool find)
	:wId(id), uSpriteX(sprite), uCharnum(charnum), iXpos(position.x), iYpos(position.y), pVel(Vel), sHeart(heart), uCoin(coin), bFind(find)
{
	//이미지 설정하기 
	
	switch (uCharnum)
	{
	case 0:
		
		break;
	case 1:

		break;
	case 2:

		break;

	default:
		break;
	}
}

Player::~Player()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}


void Player::Jump(USHORT spriteCnt)
{
	if (!bJumpKeyPressed) return;
	SetSpriteY(2);
	velocity.y = (fJumpTime * fJumpTime - fJumpPower * fJumpTime) * 6; //	4로 나눈이유는, 너무 높이 뛰어서. 값을 낮추기 위해.
	fJumpTime += 0.2f;		//	시간의 흐름을 표현하기 위해서.

	if (fJumpTime > fJumpPower * 2)	//	착지했다면의 의미로, 각 변수 초기화.
	{
		SetSpriteY(spriteCnt);
		fJumpTime = 0;
		velocity.y = 0;
		bJumpKeyPressed = FALSE;
		input.bSpace = FALSE;
	}
	if (iYpos > 620) {
		SetSpriteY(spriteCnt);
		fJumpTime = 0;
		velocity.y = 0;
		bJumpKeyPressed = FALSE;
		iYpos = 620;
		input.bSpace = FALSE;
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
}

void Player::ChangeSprite(int* count)
{
	if (*count == 3) {
		uSpriteX = (uSpriteX + 1) % 4;
		*count = 0;
	}
	*count += 1;
}

bool Player::IsCollided(CMonster monster)
{
	RECT A = aabb;
	RECT B = monster.GetAABB();

	if (A.bottom < B.top) return false;
	if (A.right < B.left) return false;
	if (A.left > B.right) return false;
	if (A.top > B.bottom) return false;
	return true;
}


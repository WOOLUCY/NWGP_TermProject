#include "Player.h"
#include "global.h"

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharnum(0), iXpos(640), iYpos(620), pVel(0,0), sHeart(0), uCoin(0), bFind(0), MaxJump(4), JumpHeight(0)

{
	//�ϴ� ���� 0���� �ʱ�ȭ�� 
	myImage[0] = nullptr;
	myImage[1] = nullptr;
	myImage[2] = nullptr;
	myImage[3] = nullptr;

	// W AABB initialization
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
	RUN_SPEED_PPS = (10.0 / 60.0) * (10.0 / 0.3);		// �÷��̾� �⺻ �ӵ��� �̰�
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
	//�̹��� �����ϱ� 
	
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
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}


void Player::Jump(USHORT spriteCnt)
{
	if (!bJumpKeyPressed) return;
	SetSpriteY(2);
	velocity.y = (fJumpTime * fJumpTime - fJumpPower * fJumpTime) * 6; //	4�� ����������, �ʹ� ���� �پ. ���� ���߱� ����.
	fJumpTime += 0.2f;		//	�ð��� �帧�� ǥ���ϱ� ���ؼ�.

	if (fJumpTime > fJumpPower * 2)	//	�����ߴٸ��� �ǹ̷�, �� ���� �ʱ�ȭ.
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


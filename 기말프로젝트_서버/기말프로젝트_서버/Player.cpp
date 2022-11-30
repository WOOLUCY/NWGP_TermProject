#include "Player.h"
#include "global.h"
#include <iostream>

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharNum(0), iXpos(640), iYpos(620), pVel(0, 0), sHeart(0), uCoin(0), bFind(0), MaxJump(4), JumpHeight(0), uRecCollidedMon(0)

{
	//�ϴ� ���� 0���� �ʱ�ȭ�� 


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
	//�̹��� �����ϱ� 

	Send.charNum = uCharNum;

	Send.iXpos = iXpos;
	Send.iYpos = iYpos;
	Send.uSpriteX = uSpriteX;
	Send.uSpriteY = uSpriteY;
}

Player::~Player()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}


void Player::Jump(USHORT spriteCnt)
{
	if (!bJumpKeyPressed) {
		return;
	}
	
	SetSpriteY(2);
	velocity.y = (fJumpTime * fJumpTime - fJumpPower * fJumpTime) * 4; //	4�� ����������, �ʹ� ���� �پ. ���� ���߱� ����.
	fJumpTime += 0.2f;		//	�ð��� �帧�� ǥ���ϱ� ���ؼ�.

	if (fJumpTime > fJumpPower * 2)	//	�����ߴٸ��� �ǹ̷�, �� ���� �ʱ�ȭ.
	{
		SetSpriteY(spriteCnt);
		fJumpTime = 0;
		velocity.y = 0;
		bJumpKeyPressed = false;
		input.bSpace = false;



	}

	if (iYpos > 620) { //�̰� �ٴ�?
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
	// count �� timer �� �ð��� ���� 1 �� �ö�
	// ���� ���ǹ��� count == "����" ���� �ø����� ��������Ʈ�� õõ�� ���ư� ��
	if (*count == 3) {
		uSpriteX = (uSpriteX + 1) % 4;
		*count = 0;
	}
	*count += 1;
}

// �浹 �� �÷��̾�� �浹�� ������ �Ϸù�ȣ ��ȯ. 0�̸� �浹���� �ʾҴٴ� ��.
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


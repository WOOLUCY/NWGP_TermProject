#include "Player.h"
#include "global.h"
#include <iostream>
#include <algorithm>

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharNum(999), pVel(0, 0), bFind(0), MaxJump(4), JumpHeight(0), uRecCollidedMon(0)
{
	//�ϴ� ���� 0���� �ʱ�ȭ�� 
	Send.iXpos = 640;
	Send.iYpos = 620;
	Send.uScore = 0;

	// W AABB initialization
	Send.aabb.bottom = Send.iYpos + (iHeight / 2);
	Send.aabb.left = Send.iXpos;
	Send.aabb.right = Send.iXpos + (iWidth / 2);
	Send.aabb.top = Send.iYpos;
	RUN_SPEED_PPS = (10.0 / 60.0) * (10.0 / 0.3);		// �÷��̾� �⺻ �ӵ��� �̰�

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
	//�̹��� �����ϱ� 

	Send.charNum = uCharNum;
	Send.uScore = coin;
	Send.iXpos = position.x;
	Send.iYpos = position.y;
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

	if (spriteCnt == 1)
		SetSpriteY(2);
	else if (spriteCnt == 3)
		SetSpriteY(5);
	else if (spriteCnt == 0)
		SetSpriteY(2);
	else if (spriteCnt == 4)
		SetSpriteY(5);
	
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

	if (Send.iYpos > 620) { //�̰� �ٴ�?
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
		//�������� �̵�
		velocity.x = 0;
		SetSpriteY(4);
	}

	if (!(input.bRight) && (up.bRight)) {
		//�������� �̵�
		velocity.x = 0;
		SetSpriteY(0);
	}

	// ĳ������ ��ġ�� �̵��� ���� ���ȭ���� ���� ���� �� ( ���� �� )
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

	// ĳ������ ��ġ�� �̵��� ���� ���ȭ���� ���� ���� �� ( ������ �� )
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

	/* �� �ܿ��� ���ȭ���� �̵� */
	else {
		if (input.bLeft) {
			//�������� �̵�
			curSpriteCnt = 3;
			//p->velocity.x = -p->GetRunSpeed();
			Send.iBgMove += -GetRunSpeed();
			Send.iBgMove = std::clamp(Send.iBgMove, -200, 1200);
			SetSpriteY(3);
		}

		if (input.bRight) {
			//���������� �̵�
			curSpriteCnt = 1;

			//p->velocity.x = p->GetRunSpeed();
			Send.iBgMove += GetRunSpeed();
			Send.iBgMove = std::clamp(Send.iBgMove, -200, 1200);
			SetSpriteY(1);
		}
	}
	if (input.bSpace) {
		//���� ����
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
		printf("\n���� �Ծ��� %d\n", Send.uScore);

	}
	return true;
}


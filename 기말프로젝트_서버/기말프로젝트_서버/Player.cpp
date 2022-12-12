#include "Player.h"
#include "global.h"
#include <iostream>
#include <algorithm>


Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharNum(999), pVel(0, 0), bFind(0), MaxJump(4), JumpHeight(0), uRecCollidedMon(0), bHasKey(FALSE)
{
	//�ϴ� ���� 0���� �ʱ�ȭ�� 
	Send.iXpos = 640;
	Send.iYpos = 620;
	Send.uScore = 0;
	Send.bHasKey = FALSE;

	// W AABB initialization
	Send.aabb.bottom = Send.iYpos + (iHeight / 2);
	Send.aabb.left = Send.iXpos + iWidth / 8;
	Send.aabb.right = Send.iXpos + (iWidth / 4);
	Send.aabb.top = Send.iYpos;
	RUN_SPEED_PPS = (10.0 / 60.0) * (10.0 / 0.25);		// �÷��̾� �⺻ �ӵ��� �̰�

	velocity = { 0, 0 };
	dir = 0;
	input = { 0 };

	bJumpKeyPressed = FALSE;
	fJumpPower = 4.5f;
	fJumpTime = 0.01;
	JumpHeight = 0;

	Send.charNum = uCharNum-1;
	Send.uSpriteX = uSpriteX;
	Send.uSpriteY = uSpriteY;

}

Player::Player(LPCWCHAR id, USHORT sprite, USHORT charnum, POS position, POS Vel, USHORT heart, USHORT coin, bool find)
	:wId(id), uSpriteX(sprite), uCharNum(charnum), pVel(Vel), bFind(find), bHasKey(FALSE)
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

	// �ʹ� ���� ���� Ŀ���� �÷��� �����ϰ� ������������ �ִ� ���� �� ����
	if (velocity.y >= 20.f) {
		velocity.y = 20.f;
	}

	//if (fJumpTime > fJumpPower * 2)	//	�����ߴٸ��� �ǹ̷�, �� ���� �ʱ�ȭ.
	//{
	//	SetSpriteY(spriteCnt);
	//	fJumpTime = 0;
	//	velocity.y = 0;
	//	bJumpKeyPressed = false;
	//	input.bSpace = false;



	//}

	if (Send.iYpos > 620) { //�̰� �ٴ�?
		SetSpriteY(spriteCnt);
		fJumpTime = 0;
		velocity.y = 0;
		Send.iYpos = 620;
		input.bSpace = false;

		bJumpKeyPressed = false;
		killMonster = FALSE;

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
	Send.aabb.left = Send.iXpos + iWidth / 8;
	Send.aabb.right = Send.iXpos + (iWidth / 8 * 3);
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
int Player::IsCollidedMonster(CMonster* monster)
{
	RECT A = Send.aabb;
	A.left += Send.iBgMove /2;
	A.right += Send.iBgMove /2;
	RECT B = monster->GetAABB();

	if (Send.timeRecord == TRUE) {
		iTimePre = clock();
		if ((double)(iTimePre - iTimeStart) / CLOCKS_PER_SEC >= invincibleTime) {
			// ���� �ð����� �ð��� ������ٸ� ���� ȿ�� ��
			Send.timeRecord = FALSE;
		}
	}

	if (A.bottom < B.top) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	uRecCollidedMon = monster->GetMonNum();
	return uRecCollidedMon;
}

bool Player::IsCollidedKey(Key key)
{
	RECT A = Send.aabb;
	A.left += Send.iBgMove / 2;
	A.right += Send.iBgMove / 2;
	RECT B = key.GetAABB();

	if (A.bottom < B.top) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	if (key.send.bIsVisible)
		return true;
}

bool Player::IsCollidedPortal(Portal portal)
{
	RECT A = Send.aabb;
	A.left += Send.iBgMove / 2;
	A.right += Send.iBgMove / 2;
	RECT B = portal.GetAABB();

	if (A.bottom < B.top) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	return true;
}

void Player::CheckLocationCollideMonster(CMonster* monster)
{
	//����ٰ� ���� �浹 ��¼�� ������ ���ڴµ� ��ٷ���
	RECT A = Send.aabb;
	A.left += Send.iBgMove / 2;
	A.right += Send.iBgMove / 2;
	RECT B = monster->GetAABB();


	if (A.bottom <= B.bottom - GetHeight() / 6) {	// �÷��̾� y ��ġ�� �� ���� ���
		Send.uScore += 2;
		monster->send.isDeath = TRUE;				// ���Ͱ� �״´�
		monster->SetSpriteY(1);
		monster->send.aabb = { 4000, 4000, 4000, 4000 };
		//printf("���� ����\n");
		killMonster = TRUE;
		fJumpTime = 1;
	}

	else {	// �װ� �ƴϸ� �÷��̾� ��Ʈ�� �������.
		if (Send.timeRecord == FALSE) {	// �ð� ��� ���� �ƴ϶�� 
			iTimeStart = clock();	// �ð� ��� ����
			Send.timeRecord = TRUE;
			Send.uHeart -= 1;
			if (Send.uHeart == 0) {
				printf("�÷��̾� ��� [%d]\n", Send.charNum);
				switch (Send.charNum)
				{
				case 0:
					Send.uHeart = 5;

					break;
				case 1:
					Send.uHeart = 4;

					break;
				case 2:
					Send.uHeart = 3;

					break;
				default:
					break;
				}
			}
		}
	}


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

	if (coin->send.bIsCrush == FALSE) {
		coin->SetIsCrush(TRUE);
		coin->send.bIsCrush = TRUE;

	}
	return true;
}


bool Player::IsCollidedPlatform(Platform platform)
{
	RECT A = Send.aabb;
	A.left += Send.iBgMove / 2;
	A.right += Send.iBgMove / 2;
	RECT B = platform.GetAABB();

	if (A.bottom < B.top) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	if (A.bottom <= B.bottom) {	// �÷��̾� y ��ġ�� �� ���� ���
		if (curSpriteCnt == 1)
			SetSpriteY(2);
		else if (curSpriteCnt == 3)
			SetSpriteY(5);
		else if (curSpriteCnt == 0)
			SetSpriteY(2);
		else if (curSpriteCnt == 4)
			SetSpriteY(5);
		fJumpTime = 0;
		velocity.y = 0;
		Send.iYpos = B.top - GetHeight() / 2 - 1;		// �÷��� ���� �ö󰣴�
		//Send.iYpos = 620;
		input.bSpace = false;

		bJumpKeyPressed = false;
	}

	

	return true;
}


bool Player::IsNotCollidedPlatform(Platform platform)
{
	RECT A = Send.aabb;
	A.left += Send.iBgMove / 2;
	A.right += Send.iBgMove / 2;
	RECT B = platform.GetAABB();

	if (A.bottom + 10 < B.top - 10) return 0;
	if (A.right < B.left) return 0;
	if (A.left > B.right) return 0;
	if (A.top > B.bottom) return 0;

	return true;
}
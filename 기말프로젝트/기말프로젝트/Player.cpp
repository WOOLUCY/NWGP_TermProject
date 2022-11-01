#include "Player.h"

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharnum(0), iXpos(640), iYpos(620), pVel(0,0), sHeart(0), uCoin(0), bFind(0), MaxJump(4), JumpHeight(0)

{
	//�ϴ� ���� 0���� �ʱ�ȭ�� 
	myImage[0] = nullptr;
	myImage[1] = nullptr;
	myImage[2] = nullptr;
	myImage[3] = nullptr;
	aabb.bottom = 0;
	aabb.left = 0;
	aabb.right = 0;
	aabb.top = 0;


}

Player::Player(wchar_t id, USHORT sprite, USHORT charnum, POS position, POS Vel, USHORT heart, USHORT coin, bool find)
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

void Player::Move(POS force)
{
	//Ű�Է¿����� �÷��̾��� �̵�
	POS acc;
	acc.x = force.x;
	acc.y = force.y;
	iXpos += acc.x;
	iYpos += acc.y;

	//Ű�Է¿� ���� �÷��̾��� �̵������� RECT�� �̵� 
}

void Player::Jump()
{
	//�����Ҷ� �װ� -���� �����ʿ��ϸ� static int �� �����ص� �ɵ�??
}

void Player::ChangeSprite()
{
	if (uSpriteX == 3)
		uSpriteX = 0;
	else uSpriteX++;
}

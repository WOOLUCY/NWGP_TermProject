#include "Player.h"

Player::Player()
	:wId(0), uSpriteX(0), uSpriteY(0), uCharnum(0), iXpos(640), iYpos(620), pVel(0,0), sHeart(0), uCoin(0), bFind(0), MaxJump(4), JumpHeight(0)

{
	//일단 걍다 0으로 초기화함 
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

void Player::Move(POS force)
{
	//키입력에따른 플레이어의 이동
	POS acc;
	acc.x = force.x;
	acc.y = force.y;
	iXpos += acc.x;
	iYpos += acc.y;

	//키입력에 따른 플레이어의 이동에대한 RECT의 이동 
}

void Player::Jump()
{
	//점프할때 그거 -만약 변수필요하면 static int 로 선언해도 될듯??
}

void Player::ChangeSprite()
{
	if (uSpriteX == 3)
		uSpriteX = 0;
	else uSpriteX++;
}

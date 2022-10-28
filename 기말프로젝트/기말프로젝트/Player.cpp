#include "Player.h"

Player::Player()
	:wId(0), uSprite(0),uCharnum(0), pPosition(0,0), pVel(0,0), sHeart(0), uCoin(0), bFind(0)

{
	//일단 걍다 0으로 초기화함 
	myImage[0] = nullptr;
	myImage[1] = nullptr;
	myImage[2] = nullptr;
	myImage[3] = nullptr;


}

Player::Player(wchar_t id, USHORT sprite, USHORT charnum, POS position, POS Vel, USHORT heart, USHORT coin, bool find)
	:wId(id), uSprite(sprite), uCharnum(charnum), pPosition(position), pVel(Vel), sHeart(heart), uCoin(coin), bFind(find)
{
	//이미지 설정하기 
	
	switch (uCharnum)
	{
	case 0:
		myImage[0] = imgSprite1;
		myImage[1] = imgSprite1_runR;
		myImage[2] = imgSprite1_runL;
		myImage[3] = imgSprite1_jump;
		break;
	case 1:
		myImage[0] = imgSprite2;
		myImage[1] = imgSprite2_runR;
		myImage[2] = imgSprite2_runL;
		myImage[3] = imgSprite2_jump;

		break;
	case 2:
		myImage[0] = imgSprite3;
		myImage[1] = imgSprite3_runR;
		myImage[2] = imgSprite3_runL;
		myImage[3] = imgSprite3_jump;

		break;

	default:
		break;
	}
}

Player::~Player()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}

void Player::Move()
{
	//키입력에따른 플레이어의 이동

	//키입력에 따른 플레이어의 이동에대한 RECT의 이동 
}

void Player::Jump()
{
}


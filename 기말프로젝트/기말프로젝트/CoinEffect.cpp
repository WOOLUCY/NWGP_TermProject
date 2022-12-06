#include "CoinEffect.h"


#include "global.h"

CoinEffect::CoinEffect()
	:iXpos(300), iYpos(600)
{

}

CoinEffect::CoinEffect(int xpos, int ypos, CImage* img)
{
	myImage = img;

	iXpos = xpos;
	iYpos = ypos;
}

CoinEffect::~CoinEffect()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}

void CoinEffect::ChangeSprite(int* count)
{
	//if (uSpriteX == 5) {
	//	uSpriteX = 0;
	//}
	//++uSpriteX;

	if (uSpriteX + 1 == 5) {
		SetDone(TRUE);
	}

	if (*count == 3) {
		uSpriteX = (uSpriteX + 1) % 5;
		*count = 0;
	}
	*count += 1;
}


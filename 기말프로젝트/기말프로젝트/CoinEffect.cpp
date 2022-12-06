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
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
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


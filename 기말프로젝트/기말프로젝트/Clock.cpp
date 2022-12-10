#include "Clock.h"


#include "global.h"

Clock::Clock()
	:iXpos(300), iYpos(600)
{

}

Clock::Clock(int xpos, int ypos, CImage* img)
{
	myImage = img;

	iXpos = xpos;
	iYpos = ypos;
}

Clock::~Clock()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}

void Clock::ChangeSprite(int* count)
{
	//if (uSpriteX == 5) {
	//	uSpriteX = 0;
	//}
	//++uSpriteX;

	if (*count == 6) {
		uSpriteX = (uSpriteX + 1) % 5;
		*count = 0;
	}
	*count += 1;
}


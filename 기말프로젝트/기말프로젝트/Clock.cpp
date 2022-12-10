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
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
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


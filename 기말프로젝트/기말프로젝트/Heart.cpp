#include "Heart.h"


#include "global.h"

Heart::Heart()
	:iXpos(300), iYpos(600)
{

}

Heart::Heart(int xpos, int ypos, CImage* img)
{
	myImage = img;

	iXpos = xpos;
	iYpos = ypos;
}

Heart::~Heart()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}

void Heart::ChangeSprite(int* count)
{
	//if (uSpriteX == 5) {
	//	uSpriteX = 0;
	//}
	//++uSpriteX;

	if (*count == 4) {
		uSpriteX = (uSpriteX + 1) % 5;
		*count = 0;
	}
	*count += 1;
}


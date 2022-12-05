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
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
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


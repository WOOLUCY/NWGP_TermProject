#include "Button.h"


#include "global.h"

Button::Button()
	:iXpos(300), iYpos(600)
{

}

Button::Button(int xpos, int ypos, CImage* img)
{
	myImage = img;

	iXpos = xpos;
	iYpos = ypos;
}

Button::~Button()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}

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
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}

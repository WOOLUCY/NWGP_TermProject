#include "Key.h"


#include "global.h"

Key::Key()
	:iXpos(300), iYpos(600)
{
	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}

Key::Key(int xpos, int ypos, CImage* img)
{
	myImage = img;

	iXpos = xpos;
	iYpos = ypos;

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}

Key::~Key()
{
	//일단 만들어놓음 근데 필요없을것같긴한데,,,,
}

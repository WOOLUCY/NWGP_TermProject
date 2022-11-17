#include "Platform.h"

Platform::Platform()
	: iXpos(340), iYpos(600), iRange(60)

{
	myImage = nullptr;

	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//�ϴ� ���� 0���� �ʱ�ȭ�� 

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}

Platform::Platform(int xpos, int ypos)
{
		myImage = nullptr;

	iXpos = xpos;
	iYpos = ypos;

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}



Platform::~Platform()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}




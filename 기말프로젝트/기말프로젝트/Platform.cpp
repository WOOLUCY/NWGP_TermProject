#include "Platform.h"

Platform::Platform()
	:uSpriteX(0), uSpriteY(0), iXpos(340), iYpos(625), iRange(60)

{
	iMaxX = iXpos + iRange;
	iMinX = iXpos - iRange;

	//�ϴ� ���� 0���� �ʱ�ȭ�� 

	aabb.bottom = iYpos + (iHeight / 2);
	aabb.left = iXpos;
	aabb.right = iXpos + (iWidth / 2);
	aabb.top = iYpos;
}



Platform::~Platform()
{
	//�ϴ� �������� �ٵ� �ʿ�����Ͱ����ѵ�,,,,
}




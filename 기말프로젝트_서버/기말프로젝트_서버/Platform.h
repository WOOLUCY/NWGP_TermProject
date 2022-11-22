#pragma once

#include <windows.h>

struct SendPlatform
{
	int		iXpos;
	int		iYpos;
	bool	bIsCrush;
};


class Platform
{
public://일단 퍼블릭으로 하겠습니다

	struct SendData {
		int		iXpos;
		int		iYpos;
		bool	bIsCrush;
	};

	//위치 
	int		iXpos;
	int		iYpos;

	RECT	aabb; //충돌처리도 서버에서 하니까 일단 넣음

	bool	bIsCrush = false;//충돌되었는지 아닌지 변수 이름 바꿔도됨

	int		iWidth = 600;
	int		iHeight = 60;
	
	Platform::SendData send;

	

public:

	Platform();
	Platform(int x, int y);
};


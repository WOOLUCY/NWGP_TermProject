#pragma once
#include <atlImage.h>

//가온 - 발판을 위한 클래스임니다. 





class Platform
{
private:
	int		iWidth = 600;	// draw 에 필요한 width, height 추가
	int		iHeight = 60;	// 고정 크기로 sprite 제작할 것이라서 고정해 둠
	int		iRange;

	RECT	aabb;

	int		iMaxX;
	int		iMinX;

	
public:
	struct SendData {
		int		iXpos;
		int		iYpos;
		bool	bIsCrush;
		RECT	aabb;
	};

	CImage* myImage;//; //이거로 스프라이트 가리키면 될것같은데 
	// 자주 참조할 것 같고 값이 변해도 상관 없는 애들은 public 으로 뺌
	int		iXpos;	// POS 형을 draw 함수에 쓸 수가 없어서 int, int 로 나누었음
	int		iYpos;

	bool	bIsCrush = false;
	


public:

	int GetXPos() { return iXpos; }
	void SetXPos(int in) { iXpos = in; }

	int GetYPos() { return iYpos; }
	void SetYPos(int in) { iYpos = in; }


	int GetWidth() { return iWidth; }
	int GetHeight() { return iHeight; }

	RECT GetAABB() { return aabb; }
	void SetAABB(RECT in) { aabb = in; }

	int GetRange() { return iRange; }
	void SetRange(int in) { iRange = in; }

public:
	//생성자
	Platform();
	Platform(int xpos, int ypos,CImage* img);

	~Platform();

};




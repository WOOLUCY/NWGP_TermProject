#pragma once
#include <windows.h>
#include <atlImage.h>



struct POS {
	float x = 0, y = 0;

	POS() : x(0),y(0) {}

	POS(float inX, float inY): x(inX),y(inY){}

	POS operator =(const POS p) {
		return POS(x + p.x, y + p.y);
	}
};

struct IMAGE {
	static CImage Image;
	int width = 0, height = 0;
};

//int CLAMP(int minimum, int x, int maximum) {
//	return max(minimum, min(x, maximum));
//}
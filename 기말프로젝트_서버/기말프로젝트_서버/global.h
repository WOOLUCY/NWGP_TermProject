#pragma once
// #include <windows.h>

#define PLATFORMNUM 10
#define COINNUM 10
#define MONSTERNUM 10


struct POS {
	float x = 0, y = 0;

	POS() : x(0),y(0) {}

	POS(float inX, float inY): x(inX),y(inY){}

	POS operator =(const POS p) {
		return POS(x + p.x, y + p.y);
	}
};



//int CLAMP(int minimum, int x, int maximum) {
//	return max(minimum, min(x, maximum));
//}



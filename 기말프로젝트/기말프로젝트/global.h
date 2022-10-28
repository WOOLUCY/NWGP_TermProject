#include <windows.h>



struct POS {
	float x;
	float y;

	POS() : x(0),y(0) {}

	POS(float inX, float inY): x(inX),y(inY){}

	POS operator =(const POS p) {
		return POS(x + p.x, y + p.y);
	}
};


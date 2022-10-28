#include <windows.h>
#include <atlImage.h>

CImage imgSprite1[4], imgSprite1_runR[4], imgSprite1_runL[4], imgSprite1_jump[4];
CImage imgSprite2[4], imgSprite2_runR[4], imgSprite2_runL[4], imgSprite2_jump[4];
CImage imgSprite3[4], imgSprite3_runR[4], imgSprite3_runL[4], imgSprite3_jump[4];


struct POS {
	float x;
	float y;

	POS() : x(0),y(0) {}

	POS(float inX, float inY): x(inX),y(inY){}

	POS operator =(const POS p) {
		return POS(x + p.x, y + p.y);
	}
};


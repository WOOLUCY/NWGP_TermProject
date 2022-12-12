#pragma once
#define PLATFORMNUM 10
#define COINNUM 50
#define MONSTERNUM 10

struct POS {
	float x = 0, y = 0;

	POS() : x(0), y(0) {}

	POS(float inX, float inY) : x(inX), y(inY) {}

	POS operator =(const POS p) {
		return POS(x + p.x, y + p.y);
	}
};


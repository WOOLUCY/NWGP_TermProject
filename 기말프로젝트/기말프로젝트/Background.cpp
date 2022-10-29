#include "Background.h"
#include <algorithm>

Background::Background() : canvas_width(0), canvas_height(0), window_bottom(0), window_left(0)
{
	Image = nullptr;
}


Background::~Background()
{
}


void Background::Update()
{
	// window_left = std::clamp(0, 620, width-1);
	// window_bottom = std::clamp(0, 640, height-1);
}
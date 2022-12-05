#include "global.h"
#include <atlimage.h>

class Background
{
private:
	int width, height;

public:
	int canvas_width;
	int canvas_height;
	int window_left;
	int window_bottom;

	CImage* Image;

	Background();
	~Background();

	void Update();
	void SetWindow(int x, int y) { window_left = x, window_bottom = y; }
	void SetWidth(int w) { width = w; }
	void setHeight(int h) { height = h; }
};


#pragma once

#include "G2D.h"

struct Camera2D
{
	V2 pos;
	int winWidth;
	int winHeight;

	int offsetX;
	int offsetY;

	Camera2D(V2 cPos, int width, int height)
	{
		pos = cPos;
		winWidth = width;
		winHeight = height;

		offsetX = winWidth / 2 - pos.x;
		offsetY = winHeight / 2 - pos.y;
	}

	V2 renderWcamera(V2 ePos)
	{
		return V2(ePos.x + offsetX, ePos.y + offsetY);
	}

	void update(V2 nPos)
	{
		pos = nPos;
		offsetX = winWidth / 2 - pos.x;
		offsetY = winHeight / 2 - pos.y;
	}
};
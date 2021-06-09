#pragma once
#include "PaddleObject.h"


PaddleObject::PaddleObject(Vertex* vertices, bool isP1, bool initialize) : RectangleObject(vertices, 0, (PIXEL_HEIGHT / 2) + 4, 1, 7, initialize)
{
	if (isP1)
		RectangleObject::incrementHPos(13);
	else
		RectangleObject::incrementHPos(PIXEL_WIDTH - 14);
	
	_isP1 = isP1;
};

void PaddleObject::initialize()
{
	RectangleObject::initialize();
}

void PaddleObject::update()
{
	//move();
	RectangleObject::update();
}

void PaddleObject::clean()
{
	RectangleObject::clean();
}

void PaddleObject::move(bool upHold, bool downHold)
{
	if (upHold && !downHold)
		incrementVPos(-4);
	else if (downHold && !upHold)
		incrementVPos(4);
}
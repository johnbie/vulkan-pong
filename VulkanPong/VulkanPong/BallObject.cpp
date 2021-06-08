#pragma once
#include "BallObject.h"


BallObject::BallObject(Vertex* vertices, bool initialize) : RectangleObject(vertices, (PIXEL_WIDTH / 2) - 1, (PIXEL_HEIGHT / 2) - 1, 1, 1, initialize)
{
	//RectangleObject::update((PIXEL_WIDTH / 2) - 1, (PIXEL_HEIGHT / 2) - 1, 2, 2);
	_goUp = false;
	_goRight = false;
};

void BallObject::initialize()
{
	RectangleObject::initialize();
}

void BallObject::update()
{
	bounceBall();
	RectangleObject::update();
}

void BallObject::clean()
{
	RectangleObject::clean();
}

void BallObject::bounceBall()
{
	//bool bounced;
	//if (_goUp)
	//{
	//	bounced = incrementVPos(-1);
	//	incrementHPos(-1);
	//}
	//else
	//{
	//	bounced = incrementVPos(1);
	//	incrementHPos(1);
	//}
	//bool bounced = _goUp ? incrementVPos(-2) : incrementVPos(2);
	bool wallBounced = _goUp ? incrementVPos(-2) : incrementVPos(2);
	bool endBounced = _goRight ? incrementHPos(-2) : incrementHPos(2);
	
	if (wallBounced)
		_goUp = !_goUp;

	if (endBounced)
		_goRight = !_goRight;
} 
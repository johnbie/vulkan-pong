#pragma once
#include "RectangleObject.h"


RectangleObject::RectangleObject(Vertex* vertices, int topLeftHPos, int topLeftVPos, int width, int height, bool initialize)
{
	_vertices = vertices;
	_topLeftHPos = topLeftHPos;
	_topLeftVPos = topLeftVPos;
	_width = width;
	_height = height;
	_isVisible = initialize;

	if (initialize)
		this->initialize();
}
void RectangleObject::initialize()
{
	updateVertices();
}

void RectangleObject::update()
{
	updateVertices();
}

void RectangleObject::clean()
{
}


void RectangleObject::setVisible(bool isVisible)
{
	_isVisible = isVisible;
}

void RectangleObject::update(int topLeftHPos, int topLeftVPos, int width, int height)
{
	_topLeftHPos = topLeftHPos;
	_topLeftVPos = topLeftVPos;
	_width = width;
	_height = height;
}

void RectangleObject::updatePos(int topLeftHPos, int topLeftVPos)
{
	_topLeftHPos = topLeftHPos;
	_topLeftVPos = topLeftVPos;
}

void RectangleObject::updateSize(int width, int height)
{
	_width = width;
	_height = height;
}

void RectangleObject::updateWidth(int width)
{
	_width = width;
}

void RectangleObject::updateHeight(int height)
{
	_height = height;
}

bool RectangleObject::incrementHPos(int increment)
{
	_topLeftHPos += increment;

	if (_topLeftHPos < 0)
	{
		_topLeftHPos = 0;
		return true;
	}
	else if (_topLeftHPos > (WIDTH / PIXEL_SIZE) - _width)
	{
		_topLeftHPos = (WIDTH / PIXEL_SIZE) - _width;
		return true;
	}
	else return false;
}

bool RectangleObject::incrementVPos(int increment)
{
	_topLeftVPos += increment;

	if (_topLeftVPos < 0)
	{
		_topLeftVPos = 0;
		return true;
	}
	else if (_topLeftVPos > (HEIGHT / PIXEL_SIZE) - _height)
	{
		_topLeftVPos = (HEIGHT / PIXEL_SIZE) - _height;
		return true;
	}
	else return false;
}

void RectangleObject::updateVertices()
{
	if (_isVisible) 
	{
		float pixelWidth = (float)WIDTH / PIXEL_SIZE;
		float pixelHeight = (float)HEIGHT / PIXEL_SIZE;

		_vertices[0].pos.x = (2.0f * _topLeftHPos / pixelWidth) - 1.0f;
		_vertices[0].pos.y = (2.0f * _topLeftVPos / pixelHeight) - 1.0f;

		_vertices[1].pos.x = _vertices[0].pos.x + (2.0f * _width / pixelWidth);
		_vertices[1].pos.y = _vertices[0].pos.y;

		_vertices[2].pos.x = _vertices[1].pos.x;
		_vertices[2].pos.y = _vertices[0].pos.y + (2.0f * _height / pixelHeight);

		_vertices[3].pos.x = _vertices[0].pos.x;
		_vertices[3].pos.y = _vertices[2].pos.y;
	}
	else
	{
		_vertices[0].pos.x = 0;
		_vertices[0].pos.y = 0;

		_vertices[1].pos.x = 0;
		_vertices[1].pos.y = 0;

		_vertices[2].pos.x = 0;
		_vertices[2].pos.y = 0;

		_vertices[3].pos.x = 0;
		_vertices[3].pos.y = 0;
	}
}
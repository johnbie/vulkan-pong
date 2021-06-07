#pragma once
#include "RectangleObject.h"


RectangleObject::RectangleObject(Vertex* vertices, int topLeftHPos, int topLeftVPos, int width, int height, bool initialize)
{
	this->vertices = vertices;
	this->topLeftHPos = topLeftHPos;
	this->topLeftVPos = topLeftVPos;
	this->width = width;
	this->height = height;

	if (initialize)
		this->Initialize();
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
	;
}

void RectangleObject::update(int topLeftHPos, int topLeftVPos, int width, int height)
{
	;
}

void RectangleObject::updatePos(int topLeftHPos, int topLeftVPos)
{
	;
}

void RectangleObject::updateSize(int width, int height)
{
	;
}

void RectangleObject::updateWidth(int width)
{
	;
}

void RectangleObject::updateHeight(int height)
{
	;
}

void RectangleObject::updateVertices()
{
	float pixelWidth = (float)WIDTH / PIXEL_SIZE;
	float pixelHeight = (float)HEIGHT / PIXEL_SIZE;
	
	vertices[0].pos.x = (2.0f * topLeftHPos / pixelWidth) - 1.0f;
	vertices[0].pos.y = (2.0f * topLeftVPos / pixelHeight) - 1.0f;

	vertices[1].pos.x = vertices[0].pos.x + (2.0f * width / pixelWidth);
	vertices[1].pos.y = vertices[0].pos.y;

	vertices[2].pos.x = vertices[1].pos.x;
	vertices[2].pos.y = vertices[0].pos.y + (2.0f * height / pixelHeight);

	vertices[3].pos.x = vertices[0].pos.x;
	vertices[3].pos.y = vertices[2].pos.y;
}
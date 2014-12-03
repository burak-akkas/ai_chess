#include "Input.h"

bool Input::intersects(int objectX, int objectY, int mouseX, int mouseY)
{
	if (mouseX < objectX || mouseY < objectY)
	{
		return false;
	}
	if (mouseX > objectX + dimensions || mouseY > objectY + dimensions)
	{
		return false;
	}

	return true;
}

bool Input::isDragging()
{
	return dragging;
}

Uint8 Input::getButton()
{
	return Button;
}

int Input::getOffSetX()
{
	return offsetX;
}

int Input::getOffSetY()
{
	return offsetY;
}

int Input::getDimensions()
{
	return dimensions;
}

void Input::setDragging(bool b)
{
	dragging = b;
}

void Input::setOffsetX(int x)
{
	offsetX = x;
}

void Input::setOffsetY(int y)
{
	offsetY = y;
}
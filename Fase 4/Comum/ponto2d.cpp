#include "Headers/ponto2d.h"

Ponto2D Ponto2D::operator*(const float& num)
{
	return Ponto2D(x*num, y*num);
}

Ponto2D Ponto2D::operator+(const Ponto2D p2)
{
	return Ponto2D(x+p2.x, y+p2.y);
}

Ponto2D Ponto2D::operator-(const Ponto2D vec) // calculate vector from 2 points
{
	return Ponto2D(vec.x-x, vec.y-y);
}

float Ponto2D::getX()
{
	return this->x;
}

float Ponto2D::getY()
{
	return this->y;
}

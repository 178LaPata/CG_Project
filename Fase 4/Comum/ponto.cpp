#include "Headers/ponto.h"

Ponto Ponto::operator/(const float& num)
{
    return Ponto(x/num, y/num, z/num);
}

Ponto Ponto::operator*(const float& num)
{
	return Ponto(x*num, y*num, z*num);
}

Ponto Ponto::operator+(const Ponto& p2)
{
	return Ponto(x+p2.x, y+p2.y, z+p2.z);
}

Ponto Ponto::operator-(const Ponto& vec) // calcular vetor de 2 pontos
{
	return Ponto(x-vec.x, y-vec.y, z-vec.z);
}

Ponto Ponto::operator*(const Ponto& vec) // produto cruzado de vetores
{
	return Ponto(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}

void Ponto::normalizar()
{
	float tam = sqrt(x*x + y*y + z*z);
	if (tam <= 0.0000001f && tam >= -0.0000001f)
	{
		x = 0;
		y = 1;
		z = 0;
	}
	else
	{
		x = x / tam;
		y = y / tam;
		z = z / tam;
	}
}

float Ponto::getX() const
{
	return this->x;
}

float Ponto::getY() const
{
	return this->y;
}

float Ponto::getZ() const
{
	return this->z;
}
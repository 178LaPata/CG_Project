#pragma once
#include <math.h>

class Ponto2D
{
private:
	float x, y;

public:
	Ponto2D() : x(0.0f), y(0.0f){};
	Ponto2D(float p_x, float p_y) : x(p_x), y(p_y){};
	Ponto2D(const Ponto2D &p) : x(p.x), y(p.y){};
	~Ponto2D(){};

	Ponto2D operator*(const float &num);
	Ponto2D operator+(const Ponto2D vec); // sum coordenadas pontos
	Ponto2D operator-(const Ponto2D vec); // calcular vetor de 2 pontos

	float getX();
	float getY();
};

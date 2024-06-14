#pragma once    // inclui o ficheiro só uma vez
// é um ponto

class Ponto
{
private:
	float x, y, z;

public:
	Ponto() :x(0.0f), y(0.0f), z(0.0f) {};
	Ponto(float p_x, float p_y, float p_z) :x(p_x), y(p_y), z(p_z) {};
	~Ponto() {};

	float getX() const;
	float getY() const;
	float getZ() const;
};

#pragma once

#include <math.h>
#include "group.h"
#include "../../Comum/Headers/ponto.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class CatmullRom : public Transformation
{
private:
	vector<Ponto> curvePoints;
	bool alinhar;
	float time;
	float eixoYAntigo[3];

public:
	CatmullRom(float time, vector<Ponto> points) : time(time), alinhar(false), curvePoints(points)
	{
		eixoYAntigo[0] = 0.0f;
		eixoYAntigo[1] = 1.0f;
		eixoYAntigo[2] = 0.0f;
	};
	CatmullRom(float time, bool alinhar, vector<Ponto> points) : time(time), alinhar(alinhar), curvePoints(points)
	{
		eixoYAntigo[0] = 0.0f;
		eixoYAntigo[1] = 1.0f;
		eixoYAntigo[2] = 0.0f;
	};

	void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv);
	void apply(float time);

	bool getAlinhar() { return alinhar; };
	float getTime() { return time; }
};

void buildRotMatrix(float *x, float *y, float *z, float *m);
void cross(float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
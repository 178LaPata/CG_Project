
#include <string>
#include <vector>
#include <sstream>

#include "Headers/catmullRom.h"

void buildRotMatrix(float *x, float *y, float *z, float *m)
{

	m[0] = x[0];
	m[1] = x[1];
	m[2] = x[2];
	m[3] = 0;
	m[4] = y[0];
	m[5] = y[1];
	m[6] = y[2];
	m[7] = 0;
	m[8] = z[0];
	m[9] = z[1];
	m[10] = z[2];
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

void cross(float *a, float *b, float *res)
{

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a)
{

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

float length(float *v)
{

	float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return res;
}

void multMatrixVector(float *m, float *v, float *res)
{

	for (int j = 0; j < 4; ++j)
	{
		res[j] = 0;
		for (int k = 0; k < 4; ++k)
		{
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv)
{

	// catmull-rom matrix
	float m[4][4] = {{-0.5f, 1.5f, -1.5f, 0.5f},
					 {1.0f, -2.5f, 2.0f, -0.5f},
					 {-0.5f, 0.0f, 0.5f, 0.0f},
					 {0.0f, 1.0f, 0.0f, 0.0f}};
	float M[16], T[4] = {t * t * t, t * t, t, 1}, dT[4] = {3 * t * t, 2 * t, 1, 0};

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M[i * 4 + j] = m[i][j];

	for (int i = 0; i < 3; i++)
	{
		float A[4], P[4] = {p0[i], p1[i], p2[i], p3[i]};
		// Compute A = M * P
		multMatrixVector(M, P, A);
		// Compute pos = T * A
		pos[i] = T[0] * A[0] + T[1] * A[1] + T[2] * A[2] + T[3] * A[3];
		// compute deriv = T' * A
		deriv[i] = dT[0] * A[0] + dT[1] * A[1] + dT[2] * A[2] + dT[3] * A[3];
	}
}

// given  global t, returns the point in the curve
void CatmullRom::getGlobalCatmullRomPoint(float gt, float *pos, float *deriv)
{

	int POINT_COUNT = this->curvePoints.size();
	float t = (fmod(gt, this->getTime()) / this->getTime()) * POINT_COUNT;
	int index = floor(t);
	t = t - index;

	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	float p[4][3];
	for (int i = 0; i < 4; i++)
	{
		Ponto pt = this->curvePoints.at(indices[i]);
		float aux[3] = {pt.getX(), pt.getY(), pt.getZ()};
		memcpy(p[i], aux, 3 * sizeof(float));
	}

	getCatmullRomPoint(t, p[0], p[1], p[2], p[3], pos, deriv);
}

void renderCatmullRomCurve(CatmullRom *c)
{
    int NUM_SEG = 100;
    float t = 0.0f, inc = 1.0f / NUM_SEG;
    float pos[3], deriv[3];

    c->getGlobalCatmullRomPoint(t, pos, deriv);
    // draw curve using line segments with GL_LINE_LOOP

    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_SEG; i++, t += inc, c->getGlobalCatmullRomPoint(t * c->getTime(), pos, deriv))
        glVertex3f(pos[0], pos[1], pos[2]);
    glEnd();
    glColor3f(currentColor[0], currentColor[1], currentColor[2]);
}

void CatmullRom::apply(float time) 
{
	float pos[3], deriv[3];
	this->getGlobalCatmullRomPoint(time, pos, deriv);
	renderCatmullRomCurve(this);
	glTranslatef(pos[0], pos[1], pos[2]);
	if (this->getAlinhar())
	{
		float *xAxis=deriv, yAxis[3],zAxis[3], *y0 = this->eixoYAntigo;
		normalize(xAxis);
		cross(xAxis, y0, zAxis);
		normalize(zAxis);
		cross(zAxis,xAxis,yAxis);
		normalize(yAxis);
		eixoYAntigo[0] = yAxis[0];
		eixoYAntigo[1] = yAxis[1];
		eixoYAntigo[2] = yAxis[2];
		float m[16];
		buildRotMatrix(xAxis, yAxis, zAxis, m);
		glMultMatrixf(m);
	}
}

#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "../../Comum/Headers/forma.h"

using namespace std;

class Transformation
{
public:
    virtual void vf() {};
    virtual void apply() {};
};

class Translate : public Transformation
{
private:
    float x, y, z;

public:
    float getX() { return this->x; };
    float getY() { return this->y; };
    float getZ() { return this->z; };
    Translate();
    Translate(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void apply()
    {
        glTranslatef(x, y, z);
    }
};

class Rotate : public Transformation
{
private:
    float angle, x, y, z;

public:
    float getAngle() { return this->angle; };
    float getX() { return this->x; };
    float getY() { return this->y; };
    float getZ() { return this->z; };
    Rotate();
    Rotate(float angle, float x, float y, float z)
    {
        this->angle = angle;
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void apply()
    {
        glRotatef(angle, x, y, z);
    }
};

class Scale : public Transformation
{
private:
    float x, y, z;

public:
    float getX() { return this->x; };
    float getY() { return this->y; };
    float getZ() { return this->z; };
    Scale();
    Scale(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void apply()
    {
        glScalef(x, y, z);
    }
};

class Color
{
private:
    float r, g, b;

public:
    float getR() { return this->r; };
    float getG() { return this->g; };
    float getB() { return this->b; };
    Color();
    Color(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class Group
{
private:
    vector<Transformation *> transformations;
    Color *color;
    vector<Forma*> models;
    vector<Group> groups;

public:
    Group();
    void addTranslate(float x, float y, float z);
    void addRotate(float angle, float x, float y, float z);
    void addScale(float x, float y, float z);
    vector<Transformation *> getTransformations();
    Color *getColor();
    vector<Forma* > getModels();
    vector<Group> getGroups();
    void addModel(Forma* f);
    void addGroup(Group g);
    void addColor(float r, float g, float b);
};

#endif

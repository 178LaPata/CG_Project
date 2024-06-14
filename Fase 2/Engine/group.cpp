#include <string>
#include <vector>
#include <sstream>

#include "Headers/group.h"
#include "../Comum/Headers/forma.h"

Group::Group()
{
    transformations = {};
    color = nullptr;
    models = {};
    groups = {};
}

void Group::addTranslate(float x, float y, float z)
{
    Translate *t = new Translate(x, y, z);
    transformations.push_back(t);
}

void Group::addRotate(float angle, float x, float y, float z)
{
    Rotate *r = new Rotate(angle, x, y, z);
    transformations.push_back(r);
}

void Group::addScale(float x, float y, float z)
{
    Scale *s = new Scale(x, y, z);
    transformations.push_back(s);
}

vector<Transformation *> Group::getTransformations()
{
    return this->transformations;
}

Color *Group::getColor()
{
    return this->color;
}

vector<Forma *> Group::getModels()
{
    return this->models;
}

vector<Group> Group::getGroups()
{
    return this->groups;
}

void Group::addModel(Forma *f)
{
    this->models.push_back(f);
}

void Group::addGroup(Group g)
{
    this->groups.push_back(g);
}

void Group::addColor(float r, float g, float b)
{
    this->color = new Color(r, g, b);
}
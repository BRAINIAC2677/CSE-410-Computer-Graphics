#ifndef LIBGG_HPP
#define LIBGG_HPP

#include <iostream>
using namespace std;

#include "libggutil.hpp"

void ggInit(int _height, int _width);
void ggDrawTriangle(Point _v1, Point _v2, Point _v3);
void ggTranslate(float _x, float _y, float _z);
void ggScale(float _x, float _y, float _z);
void ggRotate(float _angle, float _x, float _y, float _z);
void ggPushMatrix();
void ggPopMatrix();
void ggPrintTriangles(int _stage, ostream& _out = cout);

#endif

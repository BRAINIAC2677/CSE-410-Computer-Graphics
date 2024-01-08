#ifndef LIBGG_HPP
#define LIBGG_HPP

#include <iostream>
using namespace std;

#include "libggutil.hpp"

void ggInit(int _height, int _width);
void ggLookAt(float _eye_x, float _eye_y, float _eye_z,
              float _look_x, float _look_y, float _look_z,
              float _up_x, float _up_y, float _up_z);
void ggPerspective(float _fovy, float _aspect, float _znear, float _zfar);
void ggDrawTriangle(Point3d _v1, Point3d _v2, Point3d _v3);
void ggTranslate(float _x, float _y, float _z);
void ggScale(float _x, float _y, float _z);
void ggRotate(float _angle, float _x, float _y, float _z);
void ggPushMatrix();
void ggPopMatrix();
void ggPrintTriangles(int _stage, ostream& _out = cout);
void ggPrintModelMatrix(ostream& _out = cout);
void ggPrintViewMatrix(ostream& _out = cout);
void ggPrintProjectionMatrix(ostream& _out = cout);
void ggEnd();

#endif

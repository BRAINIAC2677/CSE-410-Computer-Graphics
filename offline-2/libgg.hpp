#ifndef LIBGG_HPP
#define LIBGG_HPP

#include <iostream>
using namespace std;

#include "libggutil.hpp"

void ggInit(int _height, int _width);
void ggLookAt(double _eye_x, double _eye_y, double _eye_z,
              double _look_x, double _look_y, double _look_z,
              double _up_x, double _up_y, double _up_z);
void ggPerspective(double _fovy, double _aspect, double _znear, double _zfar);
void ggDrawTriangle(Point3d _v1, Point3d _v2, Point3d _v3);
void ggTranslate(double _x, double _y, double _z);
void ggScale(double _x, double _y, double _z);
void ggRotate(double _angle, double _x, double _y, double _z);
void ggPushMatrix();
void ggPopMatrix();
void ggPrintTriangles(int _stage, ostream &_out = cout);
void ggPrintModelMatrix(ostream &_out = cout);
void ggPrintViewMatrix(ostream &_out = cout);
void ggPrintProjectionMatrix(ostream &_out = cout);
void ggPrintZBuffer(ostream &_out = cout);
void ggPrintColorBuffer(ostream &_out = cout);
void ggSaveImage(string _filename);
void ggEnd();

#endif

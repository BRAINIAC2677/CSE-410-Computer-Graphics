/*
Date: 2023/15/12
Author: Asif Azad
Description: helper library for openGL
*/

#ifndef GGLIB_HPP
#define GGLIB_HPP

#include <bits/stdc++.h>
using namespace std;

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <glut.h>
#include <windows.h>
#endif

class GGvector
{
public:
    GLdouble x, y, z;

    GGvector();
    GGvector(GLdouble _x, GLdouble _y, GLdouble _z);

    GGvector operator+(const GGvector &p) const;
    GGvector operator+(const GLdouble &d) const;
    GGvector operator+=(const GGvector &p);
    GGvector operator+=(const GLdouble &d);
    GGvector operator-(const GGvector &p) const;
    GGvector operator-(const GLdouble &d) const;
    GGvector operator-=(const GGvector &p);
    GGvector operator-=(const GLdouble &d);
    GGvector operator*(const GGvector &p) const;
    GGvector operator*(const GLdouble &d) const;
    GGvector operator*=(const GGvector &p);
    GGvector operator*=(const GLdouble &d);
    friend ostream &operator<<(ostream &output, const GGvector &p);
    GGvector normalize();
    GGvector operator^(const GGvector &p) const;
};

void ggDrawAxes();
void ggDrawCheckerBoard(GLint _tile_count, GLint _tile_size);
GLdouble ggDeg2Rad(GLdouble _deg);

#endif

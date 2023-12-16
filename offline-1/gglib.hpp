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
    GGvector rotate(GLdouble _angle, GGvector _axis);
    GGvector operator^(const GGvector &p) const;
};

void ggDrawAxes();
void ggDrawCheckerBoard(GLint _tile_count, GLint _tile_size);
GLdouble ggDeg2Rad(GLdouble _deg);

class GGsphere
{
private:
    GLdouble total_rolling_angle;
    vector<GGvector> vertices;
    void __generate_vertices();
    void __draw_up();
    void __draw_forward();
    void __draw_vertices();

public:
    GGvector position, up, forward;
    GLdouble radius, delta_rolling_angle;
    GLint sectors, stacks;
    vector<GGvector> colors;

    GGsphere();
    GGsphere(GGvector _center, GLdouble _radius, GLint _sectors, GLint _stacks);
    void draw();
    void roll_forward();
    void roll_backward();
    void change_forward_direction(GLdouble _angle);
};

#endif

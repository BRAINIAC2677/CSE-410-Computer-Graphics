#ifndef LIBGGUTIL_HPP
#define LIBGGUTIL_HPP

#include <iostream>
#include <vector>
using namespace std;


class Matrix 
{
  int nrow, ncol;
  public:
    vector<vector<float>> values;
    Matrix(int _nrow, int _ncol);
    Matrix(int _nrow, int _ncol, float _initial);

    int get_nrow() const;
    int get_ncol() const ;

    Matrix matmul(Matrix _m);
    friend ostream& operator<<(ostream& _os, const Matrix& _m);
};

class SquareMatrix: public Matrix
{
  int ndim;
  public:
    SquareMatrix(int _ndim=4);
    SquareMatrix(Matrix _m);
    SquareMatrix(int _ndim, float _initial);

    int get_ndim() const;
    void set_identity();

    SquareMatrix transpose();
    float determinant();
    SquareMatrix inverse();

    Matrix matmul(Matrix _m);
    SquareMatrix matmul(SquareMatrix _m);
};

class Vector;

class Point 
{
  public:
    float x, y, z;
    Point(float _x, float _y, float _z);
    
    Point transform(SquareMatrix _m);
    Point project(SquareMatrix _m);
    
    Vector operator-(Point _p);
    friend ostream& operator<<(ostream& _os, const Point& _p);
};

class Vector
{
  public:
    float x, y, z;
    Vector(Point _p);
    Vector(float _x, float _y, float _z);
    
    float magnitude();
    void normalize();
    Vector cross(Vector _v);
    float dot(Vector _v);

    friend ostream& operator<<(ostream& _os, const Vector& _v);
};

class Triangle
{
  public:
    Point p1, p2, p3;
    Triangle(Point _p1, Point _p2, Point _p3);
    
    Triangle transform(SquareMatrix _m);
    Triangle project(SquareMatrix _m);

    friend ostream& operator<<(ostream& _os, const Triangle& _t);
};


// standard transformation matrices

SquareMatrix get_translation_matrix(float _tx, float _ty, float _tz);
SquareMatrix get_scaling_matrix(float _sx, float _sy, float _sz);
SquareMatrix get_rotation_matrix_x(float _angle);
SquareMatrix get_rotation_matrix_y(float _angle);
SquareMatrix get_rotation_matrix_z(float _angle);
SquareMatrix get_alignment_matrix_z(Vector _v);
SquareMatrix get_rotation_matrix(float _angle, Vector _axis);

#endif

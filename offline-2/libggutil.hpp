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
    Matrix operator-(Matrix _m);
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
class Vector3d;
class Vector4d;

class Point2d
{
  public:
    float x, y;
    Point2d(float _x, float _y);

    void round();
    friend ostream& operator<<(ostream& _os, const Point2d& _p);
};

class Point3d 
{
  public:
    float x, y, z;
    Point3d(float _x, float _y, float _z);
    
    void round();
    Point3d transform(SquareMatrix _m);
    Point3d project(SquareMatrix _m);
    Vector4d homogenize();    
    friend ostream& operator<<(ostream& _os, const Point3d& _p);
};

class Line 
{
  public:
    float m, c;
    Line(float _m, float _c);
    Line(Point2d _p1, Point2d _p2);

    bool is_parallel(Line _l);
    Point2d get_intersection(Line _l);
    
    friend ostream& operator<<(ostream& _os, const Line& _l);
};

class Vector: public Matrix
{
    int ndim; 
  public:
    Vector(int _ndim=4);
    Vector(Matrix _m);
    Vector(initializer_list<float> _l);

    int get_ndim() const;

    float magnitude();
    void normalize();
    float dot(Vector _v);
};

class Vector3d: public Vector
{
  public:
    Vector3d(Point3d _p);
    Vector3d(Matrix _m);
    Vector3d(float _x, float _y, float _z);

    float x() const;
    float y() const;
    float z() const;
    
    Vector3d operator-(Vector3d _v);
    Vector3d cross(Vector3d _v);
};

class Vector4d: public Vector
{
  public:
    Vector4d(Point3d _p);
    Vector4d(Matrix _m);
    Vector4d(float _x, float _y, float _z, float _w);

    float x() const;
    float y() const;
    float z() const;
    float w() const;

    Point3d dehomogenize();
};

class Triangle
{
  public:
    vector<Point3d> vertices;
    Triangle(Point3d _p1, Point3d _p2, Point3d _p3);

    void round();
    vector<Line> get_edges2d();
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
SquareMatrix get_alignment_matrix_z(Vector3d _v);
SquareMatrix get_rotation_matrix(float _angle, Vector3d _axis);

#endif

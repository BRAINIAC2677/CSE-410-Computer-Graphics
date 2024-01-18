#ifndef LIBGGUTIL_HPP
#define LIBGGUTIL_HPP

#include <iostream>
#include <vector>
using namespace std;

class Vector;
class Vector3d;
class Vector4d;

class Matrix
{
  int nrow, ncol;
public:
  vector<vector<double>> values;
  Matrix(int _nrow, int _ncol);
  Matrix(int _nrow, int _ncol, double _initial);

  int get_nrow() const;
  int get_ncol() const;

  Matrix matmul(Matrix _m);
  Matrix operator-(Matrix _m);
  friend ostream &operator<<(ostream &_os, const Matrix &_m);
};

class SquareMatrix : public Matrix
{
  int ndim;
public:

  SquareMatrix(Matrix _m);
  SquareMatrix(int _ndim = 4);
  SquareMatrix(int _ndim, double _initial);

  int get_ndim() const;
  void set_identity();

  double determinant();
  SquareMatrix transpose();
  SquareMatrix inverse();

  Matrix matmul(Matrix _m);
  SquareMatrix matmul(SquareMatrix _m);
};

class Point3d
{
public:
  double x, y, z;
  Point3d(double _x, double _y, double _z);
  Point3d(initializer_list<double> _l);

  Point3d transform(SquareMatrix _m);
  Vector4d homogenize();
  friend ostream &operator<<(ostream &_os, const Point3d &_p);
};

class Vector : public Matrix
{
  int ndim;
public:
  Vector(Matrix _m);
  Vector(int _ndim = 4);
  Vector(initializer_list<double> _l);

  int get_ndim() const;
  double magnitude();
  void normalize();
  double dot(Vector _v);
};

class Vector3d : public Vector
{
public:
  Vector3d(Point3d _p);
  Vector3d(Matrix _m);
  Vector3d(double _x, double _y, double _z);

  double x() const;
  double y() const;
  double z() const;

  Vector3d operator-(Vector3d _v);
  Vector3d cross(Vector3d _v);
};

class Vector4d : public Vector
{
public:
  Vector4d(Point3d _p);
  Vector4d(Matrix _m);
  Vector4d(double _x, double _y, double _z, double _w);

  double x() const;
  double y() const;
  double z() const;
  double w() const;

  Point3d dehomogenize();
};

class Triangle
{
public:
  vector<Point3d> vertices;
  Triangle(Point3d _p1, Point3d _p2, Point3d _p3);

  Triangle transform(SquareMatrix _m);

  friend ostream &operator<<(ostream &_os, const Triangle &_t);
};

// standard transformation matrices

SquareMatrix get_translation_matrix(double _tx, double _ty, double _tz);
SquareMatrix get_scaling_matrix(double _sx, double _sy, double _sz);
SquareMatrix get_rotation_matrix_x(double _angle);
SquareMatrix get_rotation_matrix_y(double _angle);
SquareMatrix get_rotation_matrix_z(double _angle);
SquareMatrix get_alignment_matrix_z(Vector3d _v);
SquareMatrix get_rotation_matrix(double _angle, Vector3d _axis);

#endif

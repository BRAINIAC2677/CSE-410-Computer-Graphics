#ifndef LIBGGUTIL_HPP
#define LIBGGUTIL_HPP

#include <iostream>
#include <vector>
using namespace std;

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
  SquareMatrix(int _ndim = 4);
  SquareMatrix(Matrix _m);
  SquareMatrix(int _ndim, double _initial);

  int get_ndim() const;
  void set_identity();

  SquareMatrix transpose();
  double determinant();
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
  double x, y;
  Point2d(double _x, double _y);

  void round();
  friend ostream &operator<<(ostream &_os, const Point2d &_p);
};

class Point3d
{
public:
  double x, y, z;
  Point3d(double _x, double _y, double _z);

  void round();
  Point3d transform(SquareMatrix _m);
  Point3d project(SquareMatrix _m);
  Vector4d homogenize();
  friend ostream &operator<<(ostream &_os, const Point3d &_p);
};

class Line
{
public:
  double m, c;
  Line(double _m, double _c);
  Line(Point2d _p1, Point2d _p2);

  bool is_parallel(Line _l);
  Point2d get_intersection(Line _l);

  friend ostream &operator<<(ostream &_os, const Line &_l);
};

class LineSegment : public Line
{
public:
  vector<Point2d> endpoints;
  LineSegment(Point2d _p1, Point2d _p2);

  Point2d get_intersection(Line _l);
  Point2d get_intersection(LineSegment _ls);

  friend ostream &operator<<(ostream &_os, const LineSegment &_ls);
};

class Plane
{
  double a, b, c, d;

public:
  Plane(Point3d _p1, Point3d _p2, Point3d _p3);

  double z_at(double _x, double _y);

  friend ostream &operator<<(ostream &_os, const Plane &_p);
};

class Vector : public Matrix
{
  int ndim;

public:
  Vector(int _ndim = 4);
  Vector(Matrix _m);
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

  void round();
  void sort_vertices();
  vector<LineSegment> get_edges2d();
  Triangle transform(SquareMatrix _m);
  Triangle project(SquareMatrix _m);

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

#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <initializer_list>
using namespace std;

#include "libggutil.hpp"

Matrix::Matrix(int _nrow, int _ncol) : nrow(_nrow), ncol(_ncol)
{
  values.resize(nrow);
  for (int i = 0; i < nrow; i++)
  {
    values[i].resize(ncol);
  }
}

Matrix::Matrix(int _nrow, int _ncol, double _initial) : nrow(_nrow), ncol(_ncol)
{
  values.resize(nrow);
  for (int i = 0; i < nrow; i++)
  {
    values[i].resize(ncol, _initial);
  }
}

int Matrix::get_nrow() const
{
  return nrow;
}

int Matrix::get_ncol() const
{
  return ncol;
}

Matrix Matrix::matmul(Matrix _m)
{
  assert(ncol == _m.get_nrow());
  Matrix result(nrow, _m.get_ncol(), 0.0);
  for (int i = 0; i < nrow; i++)
  {
    for (int j = 0; j < _m.get_ncol(); j++)
    {
      for (int k = 0; k < ncol; k++)
      {
        result.values[i][j] += values[i][k] * _m.values[k][j];
      }
    }
  }
  return result;
}

Matrix Matrix::operator-(Matrix _m)
{
  assert(nrow == _m.get_nrow() && ncol == _m.get_ncol());
  Matrix result(nrow, ncol, 0.0);
  for (int i = 0; i < nrow; i++)
  {
    for (int j = 0; j < ncol; j++)
    {
      result.values[i][j] = values[i][j] - _m.values[i][j];
    }
  }
  return result;
}

ostream &operator<<(ostream &_os, const Matrix &_m)
{
  _os << fixed << setprecision(7);
  for (int i = 0; i < _m.get_nrow(); i++)
  {
    for (int j = 0; j < _m.get_ncol(); j++)
    {
      _os << _m.values[i][j] << " ";
    }
    _os << "\n";
  }
  _os.unsetf(ios::fixed);
  return _os;
}

SquareMatrix::SquareMatrix(int _ndim) : Matrix(_ndim, _ndim), ndim(_ndim) {}

SquareMatrix::SquareMatrix(Matrix _m) : Matrix(_m), ndim(_m.get_nrow())
{
  assert(ndim == _m.get_ncol());
}

SquareMatrix::SquareMatrix(int _ndim, double _initial) : Matrix(_ndim, _ndim, _initial), ndim(_ndim) {}

int SquareMatrix::get_ndim() const
{
  return ndim;
}

void SquareMatrix::set_identity()
{
  for (int i = 0; i < ndim; i++)
  {
    for (int j = 0; j < ndim; j++)
    {
      values[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }
}

SquareMatrix SquareMatrix::transpose()
{
  SquareMatrix result(ndim);
  for (int i = 0; i < ndim; i++)
  {
    for (int j = 0; j < ndim; j++)
    {
      result.values[i][j] = values[j][i];
    }
  }
  return result;
}

double SquareMatrix::determinant()
{
  // cofactor expansion for n*n matrix
  if (ndim == 1)
  {
    return values[0][0];
  }
  double result = 0.0;
  for (int i = 0; i < ndim; i++)
  {
    SquareMatrix submatrix(ndim - 1);
    for (int j = 1; j < ndim; j++)
    {
      for (int k = 0; k < ndim; k++)
      {
        if (k < i)
        {
          submatrix.values[j - 1][k] = values[j][k];
        }
        else if (k > i)
        {
          submatrix.values[j - 1][k - 1] = values[j][k];
        }
      }
    }
    result += pow(-1.0, i) * values[0][i] * submatrix.determinant();
  }
  return result;
}

SquareMatrix SquareMatrix::inverse()
{
  // Gauss-Jordan elimination
  assert(this->determinant() != 0.0);
  SquareMatrix result(ndim, 0.0);
  result.set_identity();
  SquareMatrix temp(*this);
  for (int i = 0; i < ndim; i++)
  {
    double pivot = temp.values[i][i];
    for (int j = 0; j < ndim; j++)
    {
      temp.values[i][j] /= pivot;
      result.values[i][j] /= pivot;
    }
    for (int j = 0; j < ndim; j++)
    {
      if (j != i)
      {
        double multiplier = temp.values[j][i];
        for (int k = 0; k < ndim; k++)
        {
          temp.values[j][k] -= multiplier * temp.values[i][k];
          result.values[j][k] -= multiplier * result.values[i][k];
        }
      }
    }
  }
  return result;
}

Matrix SquareMatrix::matmul(Matrix _m)
{
  return Matrix::matmul(_m);
}

SquareMatrix SquareMatrix::matmul(SquareMatrix _m)
{
  return SquareMatrix(Matrix::matmul(_m));
}

Point2d::Point2d(double _x, double _y) : x(_x), y(_y) {}

void Point2d::round()
{
  x = std::round(x);
  y = std::round(y);
}

ostream &operator<<(ostream &_os, const Point2d &_p)
{
  _os << "(" << _p.x << ", " << _p.y << ")";
  return _os;
}

Point3d::Point3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

void Point3d::round()
{
  x = std::round(x);
  y = std::round(y);
  z = std::round(z);
}

Point3d Point3d::transform(SquareMatrix _m)
{
  assert(_m.get_ndim() == 4);
  Vector4d temp = homogenize();
  Vector4d result = Vector4d(_m.matmul(temp));
  return result.dehomogenize();
}

Point3d Point3d::project(SquareMatrix _m)
{
  assert(_m.get_ndim() == 4);
  Matrix temp(4, 1);
  temp.values[0][0] = x;
  temp.values[1][0] = y;
  temp.values[2][0] = z;
  temp.values[3][0] = 1.0;
  Matrix result = _m.matmul(temp);
  return Point3d(result.values[0][0] / result.values[3][0], result.values[1][0] / result.values[3][0], result.values[2][0] / result.values[3][0]);
}

Line::Line(double _m, double _c) : m(_m), c(_c) {}

Line::Line(Point2d _p1, Point2d _p2) : m((_p2.y - _p1.y) / (_p2.x - _p1.x)), c(_p1.y - m * _p1.x) {}

bool Line::is_parallel(Line _l)
{
  return (m == _l.m);
}

Point2d Line::get_intersection(Line _l)
{
  assert(!is_parallel(_l));
  double x = (_l.c - c) / (m - _l.m);
  double y = m * x + c;
  return Point2d(x, y);
}

ostream &operator<<(ostream &_os, const Line &_l)
{
  _os << "y = " << _l.m << "x + " << _l.c;
  return _os;
}

LineSegment::LineSegment(Point2d _p1, Point2d _p2) : Line(_p1, _p2), endpoints({_p1, _p2}) {}

Point2d LineSegment::get_intersection(Line _l)
{
  Point2d result = Line::get_intersection(_l);
  if (result.x < min(endpoints[0].x, endpoints[1].x) || result.x > max(endpoints[0].x, endpoints[1].x))
  {
    return Point2d(NAN, NAN);
  }
  if (result.y < min(endpoints[0].y, endpoints[1].y) || result.y > max(endpoints[0].y, endpoints[1].y))
  {
    return Point2d(NAN, NAN);
  }
  return result;
}

Point2d LineSegment::get_intersection(LineSegment _ls)
{
  Point2d result = Line::get_intersection(_ls);
  if (result.x < min(endpoints[0].x, endpoints[1].x) || result.x > max(endpoints[0].x, endpoints[1].x))
  {
    return Point2d(NAN, NAN);
  }
  if (result.y < min(endpoints[0].y, endpoints[1].y) || result.y > max(endpoints[0].y, endpoints[1].y))
  {
    return Point2d(NAN, NAN);
  }
  if (result.x < min(_ls.endpoints[0].x, _ls.endpoints[1].x) || result.x > max(_ls.endpoints[0].x, _ls.endpoints[1].x))
  {
    return Point2d(NAN, NAN);
  }
  if (result.y < min(_ls.endpoints[0].y, _ls.endpoints[1].y) || result.y > max(_ls.endpoints[0].y, _ls.endpoints[1].y))
  {
    return Point2d(NAN, NAN);
  }
  return result;
}

ostream &operator<<(ostream &_os, const LineSegment &_ls)
{
  _os << _ls.endpoints[0] << " - " << _ls.endpoints[1];
  return _os;
}

Vector4d Point3d::homogenize()
{
  return Vector4d(x, y, z, 1.0);
}

ostream &operator<<(ostream &_os, const Point3d &_p)
{
  _os << "(" << _p.x << ", " << _p.y << ", " << _p.z << ")";
  return _os;
}

Plane::Plane(Point3d _p1, Point3d _p2, Point3d _p3)
{
  Vector3d v1(Vector3d(_p2) - Vector3d(_p1));
  Vector3d v2(Vector3d(_p3) - Vector3d(_p1));
  Vector3d normal(v1.cross(v2));
  a = normal.x();
  b = normal.y();
  c = normal.z();
  d = -a * _p1.x - b * _p1.y - c * _p1.z;
}

double Plane::z_at(double _x, double _y)
{
  return (-a * _x - b * _y - d) / c;
}

ostream &operator<<(ostream &_os, const Plane &_p)
{
  _os << _p.a << "x + " << _p.b << "y + " << _p.c << "z + " << _p.d << " = 0";
  return _os;
}

Vector::Vector(int _ndim) : Matrix(_ndim, 1), ndim(_ndim) {}

Vector::Vector(Matrix _m) : Matrix(_m), ndim(_m.get_nrow())
{
  assert(_m.get_ncol() == 1);
}

Vector::Vector(initializer_list<double> _l) : Matrix(_l.size(), 1), ndim(_l.size())
{
  int i = 0;
  for (auto it = _l.begin(); it != _l.end(); it++)
  {
    values[i][0] = *it;
    i++;
  }
}

int Vector::get_ndim() const
{
  return ndim;
}

double Vector::magnitude()
{
  double result = 0.0;
  for (int i = 0; i < ndim; i++)
  {
    result += values[i][0] * values[i][0];
  }
  return sqrt(result);
}

void Vector::normalize()
{
  double mag = magnitude();
  for (int i = 0; i < ndim; i++)
  {
    values[i][0] /= mag;
  }
}

double Vector::dot(Vector _v)
{
  assert(ndim == _v.get_ndim());
  double result = 0.0;
  for (int i = 0; i < ndim; i++)
  {
    result += values[i][0] * _v.values[i][0];
  }
  return result;
}

Vector3d::Vector3d(Point3d _p) : Vector({_p.x, _p.y, _p.z}) {}

Vector3d::Vector3d(Matrix _m) : Vector(_m)
{
  assert(_m.get_nrow() == 3);
}

Vector3d::Vector3d(double _x, double _y, double _z) : Vector({_x, _y, _z}) {}

double Vector3d::x() const
{
  return values[0][0];
}

double Vector3d::y() const
{
  return values[1][0];
}

double Vector3d::z() const
{
  return values[2][0];
}

Vector3d Vector3d::operator-(Vector3d _v)
{
  return Vector3d(Matrix::operator-(_v));
}

Vector3d Vector3d::cross(Vector3d _v)
{
  return Vector3d(values[1][0] * _v.values[2][0] - values[2][0] * _v.values[1][0], values[2][0] * _v.values[0][0] - values[0][0] * _v.values[2][0], values[0][0] * _v.values[1][0] - values[1][0] * _v.values[0][0]);
}

Vector4d::Vector4d(Point3d _p) : Vector({_p.x, _p.y, _p.z, 1.0}) {}

Vector4d::Vector4d(Matrix _m) : Vector(_m)
{
  assert(_m.get_nrow() == 4);
}

Vector4d::Vector4d(double _x, double _y, double _z, double _w) : Vector({_x, _y, _z, _w}) {}

double Vector4d::x() const
{
  return values[0][0];
}

double Vector4d::y() const
{
  return values[1][0];
}

double Vector4d::z() const
{
  return values[2][0];
}

double Vector4d::w() const
{
  return values[3][0];
}

Point3d Vector4d::dehomogenize()
{
  return Point3d(values[0][0] / values[3][0], values[1][0] / values[3][0], values[2][0] / values[3][0]);
}

Triangle::Triangle(Point3d _p1, Point3d _p2, Point3d _p3) : vertices({_p1, _p2, _p3}) {}

void Triangle::round()
{
  for (int i = 0; i < 3; i++)
  {
    vertices[i].round();
  }
}

void Triangle::sort_vertices()
{
  // keep the top vertex at index 0 and then sort the other in anticlockwise order
  if (vertices[1].y > vertices[0].y)
  {
    swap(vertices[0], vertices[1]);
  }
  if (vertices[2].y > vertices[0].y)
  {
    swap(vertices[0], vertices[2]);
  }
  if (vertices[1].x > vertices[2].x)
  {
    swap(vertices[1], vertices[2]);
  }
}

vector<LineSegment> Triangle::get_edges2d()
{
  vector<Point2d> vertices2d;
  vector<LineSegment> edges2d;
  for (int i = 0; i < 3; i++)
  {
    vertices2d.push_back(Point2d(vertices[i].x, vertices[i].y));
  }
  for (int i = 0; i < 3; i++)
  {
    edges2d.push_back(LineSegment(vertices2d[i], vertices2d[(i + 1) % 3]));
  }
  return edges2d;
}

Triangle Triangle::transform(SquareMatrix _m)
{
  return Triangle(vertices[0].transform(_m), vertices[1].transform(_m), vertices[2].transform(_m));
}

Triangle Triangle::project(SquareMatrix _m)
{
  return Triangle(vertices[0].project(_m), vertices[1].project(_m), vertices[2].project(_m));
}

ostream &operator<<(ostream &_os, const Triangle &_t)
{
  _os << fixed << setprecision(7);
  _os << _t.vertices[0] << "\n"
      << _t.vertices[1] << "\n"
      << _t.vertices[2];
  _os.unsetf(ios::fixed);
  return _os;
}

SquareMatrix get_translation_matrix(double _tx, double _ty, double _tz)
{
  SquareMatrix result(4);
  result.set_identity();
  result.values[0][3] = _tx;
  result.values[1][3] = _ty;
  result.values[2][3] = _tz;
  return result;
}

SquareMatrix get_scaling_matrix(double _sx, double _sy, double _sz)
{
  SquareMatrix result(4);
  result.set_identity();
  result.values[0][0] = _sx;
  result.values[1][1] = _sy;
  result.values[2][2] = _sz;
  return result;
}

SquareMatrix get_rotation_matrix_x(double _angle)
{
  SquareMatrix result(4);
  result.set_identity();
  _angle = _angle * M_PI / 180.0;
  result.values[1][1] = cos(_angle);
  result.values[1][2] = -sin(_angle);
  result.values[2][1] = sin(_angle);
  result.values[2][2] = cos(_angle);
  return result;
}

SquareMatrix get_rotation_matrix_y(double _angle)
{
  SquareMatrix result(4);
  result.set_identity();
  _angle = _angle * M_PI / 180.0;
  result.values[0][0] = cos(_angle);
  result.values[0][2] = sin(_angle);
  result.values[2][0] = -sin(_angle);
  result.values[2][2] = cos(_angle);
  return result;
}

SquareMatrix get_rotation_matrix_z(double _angle)
{
  SquareMatrix result(4);
  result.set_identity();
  _angle = _angle * M_PI / 180.0;
  result.values[0][0] = cos(_angle);
  result.values[0][1] = -sin(_angle);
  result.values[1][0] = sin(_angle);
  result.values[1][1] = cos(_angle);
  return result;
}

SquareMatrix get_alignment_matrix_z(Vector3d _v)
{
  SquareMatrix result(4);
  result.set_identity();
  double lambda = sqrt(_v.y() * _v.y() + _v.z() * _v.z());
  result.values[0][0] = lambda / _v.magnitude();
  result.values[0][1] = -_v.x() * _v.y() / (_v.magnitude() * lambda);
  result.values[0][2] = -_v.x() * _v.z() / (_v.magnitude() * lambda);
  result.values[1][1] = _v.z() / lambda;
  result.values[1][2] = -_v.y() / lambda;
  result.values[2][0] = _v.x() / _v.magnitude();
  result.values[2][1] = _v.y() / _v.magnitude();
  result.values[2][2] = _v.z() / _v.magnitude();
  return result;
}

SquareMatrix get_rotation_matrix(double _angle, Vector3d _axis)
{
  SquareMatrix av = get_alignment_matrix_z(_axis);
  SquareMatrix r = get_rotation_matrix_z(_angle);
  SquareMatrix avt = av.transpose();
  return avt.matmul(r.matmul(av));
}

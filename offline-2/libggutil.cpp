#include<cmath>
#include<cassert>
#include<iostream>
#include<iomanip>
#include<initializer_list>
using namespace std;

#include "libggutil.hpp"

Matrix::Matrix(int _nrow, int _ncol): nrow(_nrow), ncol(_ncol)
{
  values.resize(nrow);
  for (int i = 0; i < nrow; i++)
  {
    values[i].resize(ncol);
  }
}

Matrix::Matrix(int _nrow, int _ncol, float _initial): nrow(_nrow), ncol(_ncol)
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

ostream& operator<<(ostream& _os,  const Matrix& _m)
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

SquareMatrix::SquareMatrix(int _ndim): Matrix(_ndim, _ndim), ndim(_ndim) {}

SquareMatrix::SquareMatrix(Matrix _m): Matrix(_m), ndim(_m.get_nrow())
{
  assert(ndim == _m.get_ncol());
}

SquareMatrix::SquareMatrix(int _ndim, float _initial): Matrix(_ndim, _ndim, _initial), ndim(_ndim) {}

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

float SquareMatrix::determinant()
{
  // cofactor expansion for n*n matrix
  if (ndim == 1)
  {
    return values[0][0];
  }
  float result = 0.0;
  for (int i = 0; i < ndim; i++)
  {
    SquareMatrix submatrix(ndim-1);
    for (int j = 1; j < ndim; j++)
    {
      for (int k = 0; k < ndim; k++)
      {
        if (k < i)
        {
          submatrix.values[j-1][k] = values[j][k];
        }
        else if (k > i)
        {
          submatrix.values[j-1][k-1] = values[j][k];
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
  assert (this->determinant() != 0.0);
  SquareMatrix result(ndim, 0.0);
  result.set_identity();
  SquareMatrix temp(*this);
  for (int i = 0; i < ndim; i++)
  {
    float pivot = temp.values[i][i];
    for (int j = 0; j < ndim; j++)
    {
      temp.values[i][j] /= pivot;
      result.values[i][j] /= pivot;
    }
    for (int j = 0; j < ndim; j++)
    {
      if (j != i)
      {
        float multiplier = temp.values[j][i];
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

Point::Point(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

Point Point::transform(SquareMatrix _m)
{
  assert(_m.get_ndim() == 4);
  Vector4d temp = homogenize();
  Vector4d result = Vector4d(_m.matmul(temp));
  return result.dehomogenize();  
}

Point Point::project(SquareMatrix _m)
{
  assert(_m.get_ndim() == 4);
  Matrix temp(4, 1);
  temp.values[0][0] = x;
  temp.values[1][0] = y;
  temp.values[2][0] = z;
  temp.values[3][0] = 1.0;
  Matrix result = _m.matmul(temp);
  return Point(result.values[0][0]/result.values[3][0], result.values[1][0]/result.values[3][0], result.values[2][0]/result.values[3][0]);
}

Vector4d Point::homogenize()
{
  return Vector4d(x, y, z, 1.0);
}

ostream& operator<<(ostream& _os, const Point& _p)
{
  _os << "(" << _p.x << ", " << _p.y << ", " << _p.z << ")";
  return _os;
}

Vector::Vector(int _ndim): Matrix(_ndim, 1), ndim(_ndim) {}

Vector::Vector(Matrix _m): Matrix(_m), ndim(_m.get_nrow())
{
  assert(_m.get_ncol() == 1);
}

Vector::Vector(initializer_list<float> _l): Matrix(_l.size(), 1), ndim(_l.size())
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

float Vector::magnitude()
{
  float result = 0.0;
  for (int i = 0; i < ndim; i++)
  {
    result += values[i][0] * values[i][0];
  }
  return sqrt(result);
}

void Vector::normalize()
{
  float mag = magnitude();
  for (int i = 0; i < ndim; i++)
  {
    values[i][0] /= mag;
  }
}

float Vector::dot(Vector _v)
{
  assert(ndim == _v.get_ndim());
  float result = 0.0;
  for (int i = 0; i < ndim; i++)
  {
    result += values[i][0] * _v.values[i][0];
  }
  return result;
}

Vector3d::Vector3d(Point _p): Vector({_p.x, _p.y, _p.z}) {}

Vector3d::Vector3d(Matrix _m): Vector(_m) {
  assert(_m.get_nrow() == 3);
}

Vector3d::Vector3d(float _x, float _y, float _z): Vector({_x, _y, _z}) {}

float Vector3d::x() const
{
  return values[0][0];
}

float Vector3d::y() const
{
  return values[1][0];
}

float Vector3d::z() const
{
  return values[2][0];
}

Vector3d Vector3d::operator-(Vector3d _v)
{
  return Vector3d(Matrix::operator-(_v));
}

Vector3d Vector3d::cross(Vector3d _v)
{
  return Vector3d(values[1][0]*_v.values[2][0] - values[2][0]*_v.values[1][0], values[2][0]*_v.values[0][0] - values[0][0]*_v.values[2][0], values[0][0]*_v.values[1][0] - values[1][0]*_v.values[0][0]);
}

Vector4d::Vector4d(Point _p): Vector({_p.x, _p.y, _p.z, 1.0}) {}

Vector4d::Vector4d(Matrix _m): Vector(_m) {
  assert(_m.get_nrow() == 4);
}

Vector4d::Vector4d(float _x, float _y, float _z, float _w): Vector({_x, _y, _z, _w}) {}

float Vector4d::x() const
{
  return values[0][0];
}

float Vector4d::y() const
{
  return values[1][0];
}

float Vector4d::z() const
{
  return values[2][0];
}

float Vector4d::w() const
{
  return values[3][0];
}

Point Vector4d::dehomogenize()
{
  return Point(values[0][0]/values[3][0], values[1][0]/values[3][0], values[2][0]/values[3][0]);
}

Triangle::Triangle(Point _p1, Point _p2, Point _p3): p1(_p1), p2(_p2), p3(_p3) {}

Triangle Triangle::transform(SquareMatrix _m)
{
  return Triangle(p1.transform(_m), p2.transform(_m), p3.transform(_m));
}

Triangle Triangle::project(SquareMatrix _m)
{
  return Triangle(p1.project(_m), p2.project(_m), p3.project(_m));
}

ostream& operator<<(ostream& _os, const Triangle& _t)
{
  _os << fixed << setprecision(7); 
  _os << _t.p1 << "\n" << _t.p2 << "\n" << _t.p3;
  _os.unsetf(ios::fixed);
  return _os;
}

SquareMatrix get_translation_matrix(float _tx, float _ty, float _tz)
{
  SquareMatrix result(4 );
  result.set_identity();
  result.values[0][3] = _tx;
  result.values[1][3] = _ty;
  result.values[2][3] = _tz;
  return result;
}

SquareMatrix get_scaling_matrix(float _sx, float _sy, float _sz)
{
  SquareMatrix result(4);
  result.set_identity();
  result.values[0][0] = _sx;
  result.values[1][1] = _sy;
  result.values[2][2] = _sz;
  return result;
}

SquareMatrix get_rotation_matrix_x(float _angle)
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

SquareMatrix get_rotation_matrix_y(float _angle)
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

SquareMatrix get_rotation_matrix_z(float _angle)
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
  float lambda = sqrt(_v.y()*_v.y() + _v.z()*_v.z());
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

SquareMatrix get_rotation_matrix(float _angle, Vector3d _axis)
{
  SquareMatrix av = get_alignment_matrix_z(_axis);
  SquareMatrix r = get_rotation_matrix_z(_angle);
  SquareMatrix avt = av.transpose();
  return avt.matmul(r.matmul(av));
}







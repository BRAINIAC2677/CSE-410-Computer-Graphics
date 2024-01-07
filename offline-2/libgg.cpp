#include<stack>
#include<vector>
#include<cassert>
#include<cmath>
using namespace std;

#include "libgg.hpp"

SquareMatrix model_matrix, view_matrix, projection_matrix;
stack<SquareMatrix> model_matrix_stack;
vector<vector<Triangle>> triangles;

void ggInit(int _height, int _width) {
  model_matrix.set_identity();
  view_matrix.set_identity();
  projection_matrix.set_identity();
  model_matrix_stack = stack<SquareMatrix>();
  triangles = vector<vector<Triangle>>(3, vector<Triangle>());
}

void ggLookAt(float _eye_x, float _eye_y, float _eye_z,
              float _look_x, float _look_y, float _look_z,
              float _up_x, float _up_y, float _up_z) 
{
  Point eye = Point(_eye_x, _eye_y, _eye_z);
  Point look = Point(_look_x, _look_y, _look_z);
  Vector up = Vector(_up_x, _up_y, _up_z);

  Vector l = look - eye;
  l.normalize();
  Vector r = l.cross(up);
  r.normalize();
  Vector u = r.cross(l);

  SquareMatrix T = get_translation_matrix(-eye.x, -eye.y, -eye.z);
  SquareMatrix R;
  R.set_identity();
  R.values = {
    {r.x, r.y, r.z, 0},
    {u.x, u.y, u.z, 0},
    {-l.x, -l.y, -l.z, 0},
    {0, 0, 0, 1}
  };
  view_matrix = R.matmul(T);
}

void ggPerspective(float _fovy, float _aspect, float _znear, float _zfar) {
  _fovy = _fovy * M_PI / 180;
  float fovx = _fovy * _aspect;
  float t = _znear * tan(_fovy / 2);
  float r = _znear * tan(fovx / 2);
  projection_matrix.values = {
    {_znear / r, 0, 0, 0},
    {0, _znear / t, 0, 0},
    {0, 0, -(_zfar + _znear) / (_zfar - _znear), -2 * _zfar * _znear / (_zfar - _znear)},
    {0, 0, -1, 0}
  };
}

void ggDrawTriangle(Point _v1, Point _v2, Point _v3) {
  Triangle t = Triangle(_v1, _v2, _v3);
  t = t.transform(model_matrix);
  triangles[0].push_back(t);
}

void ggTranslate(float _tx, float _ty, float _tz) {
  SquareMatrix T = get_translation_matrix(_tx, _ty, _tz);
  model_matrix = model_matrix.matmul(T);
}

void ggScale(float _sx, float _sy, float _sz) {
  SquareMatrix S = get_scaling_matrix(_sx, _sy, _sz);
  model_matrix = model_matrix.matmul(S);
}

void ggRotate(float _angle, float _x, float _y, float _z) {
  SquareMatrix R = get_rotation_matrix(_angle,Vector( _x, _y, _z));
  model_matrix = model_matrix.matmul(R);
}

void ggPushMatrix() {
  model_matrix_stack.push(model_matrix);
}

void ggPopMatrix() {
  assert(!model_matrix_stack.empty());
  model_matrix = model_matrix_stack.top();
  model_matrix_stack.pop();
}

void ggPrintTriangles(int _stage, ostream& _out) {
  assert(_stage >= 0 && _stage <= 2);
  _out<<"stage "<<_stage+1<<"\n";
  for (Triangle t : triangles[_stage])
  {
    _out << t << "\n\n";
  }
}

void ggPrintModelMatrix(ostream& _out) {
  _out << model_matrix << "\n";
}

void ggPrintViewMatrix(ostream& _out) {
  _out << view_matrix << "\n";
}

void ggPrintProjectionMatrix(ostream& _out) {
  _out << projection_matrix << "\n";
}

void ggEnd() {
  for(auto t: triangles[0]) {
    triangles[1].push_back(t.transform(view_matrix));
  }
  for (auto t : triangles[1]) {
    triangles[2].push_back(t.project(projection_matrix));
  }
}


#include<stack>
#include<vector>
#include<cassert>
using namespace std;

#include "libgg.hpp"

SquareMatrix model_matrix;
stack<SquareMatrix> model_matrix_stack;
vector<vector<Triangle>> triangles;

void ggInit(int _height, int _width) {
  model_matrix.set_identity();
  model_matrix_stack = stack<SquareMatrix>();
  triangles = vector<vector<Triangle>>(3, vector<Triangle>());
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
  _out<<"stage "<<_stage<<"\n";
  for (Triangle t : triangles[_stage])
  {
    _out << t << "\n\n";
  }
}


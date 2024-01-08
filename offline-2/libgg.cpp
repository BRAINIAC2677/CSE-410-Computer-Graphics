#include<stack>
#include<vector>
#include<cassert>
#include<cmath>
using namespace std;

#include "bitmap_image.hpp"
#include "libgg.hpp"

static unsigned long int g_seed = 1;
int screen_height, screen_width;
SquareMatrix model_matrix, view_matrix, projection_matrix, viewport_matrix;
stack<SquareMatrix> model_matrix_stack;
vector<rgb_t> triangle_colors;
vector<vector<Triangle>> triangles;
vector<float> z_buffer;
vector<vector<rgb_t>> color_buffer;

inline int get_random_color() {
  g_seed = (214013 * g_seed + 2531011);
  return (g_seed >> 16) & 0x7FFF;
}

void assign_triangle_colors() {
  triangle_colors.clear();
  for (int i = 0; i < triangles[3].size(); i++) {
    triangle_colors.push_back({(unsigned char)get_random_color(), (unsigned char)get_random_color(), (unsigned char)get_random_color()});
  }
}

void init_viewport_matrix() {
  viewport_matrix.set_identity();
  viewport_matrix.values = {
    {(float)screen_width / 2, 0, 0, (float)(screen_width - 1) / 2},
    {0, (float)screen_height / 2, 0, (float)(screen_height - 1) / 2},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };
}

void z_buffer_algorithm() {
  for(auto triangle: triangles[3]){
    triangle.round();
    vector<Line> edges = triangle.get_edges2d();

    int top_y = max({triangle.vertices[0].y, triangle.vertices[1].y, triangle.vertices[2].y});
    int bottom_y = min({triangle.vertices[0].y, triangle.vertices[1].y, triangle.vertices[2].y}); 
    //clipping
    top_y = min(top_y, screen_height - 1);
    bottom_y = max(bottom_y, 0);

    for(int y = top_y; y >= bottom_y; y--) 
    {
      // find left and right intersecting column
      int left_x = screen_width - 1;
      int right_x = 0;
      Line y_scanline = Line(0, y);

      for(Line edge: edges){
        if(edge.is_parallel(y_scanline)){
          continue;
        }
        Point2d intersection = edge.get_intersection(y_scanline);
        intersection.round();
        left_x = min(left_x, (int)intersection.x);
        right_x = max(right_x, (int)intersection.x);
      } 
      //clipping
      left_x = max(left_x, 0);
      right_x = min(right_x, screen_width - 1);

      for(int x = left_x; x <= right_x; x++) {

      }

    }
  }
}

void ggInit(int _height, int _width) {
  screen_height = _height;
  screen_width = _width;
  model_matrix.set_identity();
  view_matrix.set_identity();
  projection_matrix.set_identity();
  init_viewport_matrix();
  model_matrix_stack = stack<SquareMatrix>();
  triangles = vector<vector<Triangle>>(4, vector<Triangle>());
  z_buffer = vector<float>(screen_height * screen_width, 2);
  color_buffer = vector<vector<rgb_t>>(screen_width, vector<rgb_t>(screen_height, {0, 0, 0}));
}

void ggLookAt(float _eye_x, float _eye_y, float _eye_z,
              float _look_x, float _look_y, float _look_z,
              float _up_x, float _up_y, float _up_z) 
{
  Vector3d eye = Vector3d(_eye_x, _eye_y, _eye_z);
  Vector3d look = Vector3d(_look_x, _look_y, _look_z);
  Vector3d up = Vector3d(_up_x, _up_y, _up_z);

  Vector3d l = look - eye;
  l.normalize();
  Vector3d r = l.cross(up);
  r.normalize();
  Vector3d u = r.cross(l);

  SquareMatrix T = get_translation_matrix(-eye.x(), -eye.y(), -eye.z());
  SquareMatrix R;
  R.set_identity();
  R.values = {
    {r.x(), r.y(), r.z(), 0},
    {u.x(), u.y(), u.z(), 0},
    {-l.x(), -l.y(), -l.z(), 0},
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

void ggDrawTriangle(Point3d _v1, Point3d _v2, Point3d _v3) {
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
  SquareMatrix R = get_rotation_matrix(_angle,Vector3d( _x, _y, _z));
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
  assert(_stage >= 0 && _stage <= triangles.size() - 1);
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
    triangles[2].push_back(t.transform(projection_matrix));
  }
  for (auto t : triangles[2]) {
    triangles[3].push_back(t.transform(viewport_matrix));
  } 
}


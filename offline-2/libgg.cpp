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
vector<vector<double>> z_buffer;
vector<vector<rgb_t>> color_buffer;

inline int get_random_color() {
  g_seed = (214013 * g_seed + 2531011);
  return (g_seed >> 16) & 0x7FFF;
}

void assign_triangle_colors() {
  for (int i = 0; i < triangles[0].size(); i++) {
    if (i < triangle_colors.size()) {
      continue;
    }
    triangle_colors.push_back({(unsigned char)get_random_color(), (unsigned char)get_random_color(), (unsigned char)get_random_color()});
  }
}

void init_viewport_matrix() {
  viewport_matrix.set_identity();
  viewport_matrix.values = {
    {(double)screen_width / 2, 0, 0, (double)(screen_width - 1) / 2},
    {0, (double)screen_height / 2, 0, (double)(screen_height - 1) / 2},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  };
}

int get_top_index(Triangle t) {
  int top_index = 0;
  for (int i = 1; i < 3; i++) {
    if (t.vertices[i].y > t.vertices[top_index].y) {
      top_index = i;
    }
  }
  return top_index;
}

int get_bottom_index(Triangle t) {
  int bottom_index = 0;
  for (int i = 1; i < 3; i++) {
    if (t.vertices[i].y < t.vertices[bottom_index].y) {
      bottom_index = i;
    }
  }
  return bottom_index;
}

int get_left_index(Triangle t) {
  int left_index = 0;
  for (int i = 1; i < 3; i++) {
    if (t.vertices[i].x < t.vertices[left_index].x) {
      left_index = i;
    }
  }
  return left_index;
}

int get_right_index(Triangle t) {
  int right_index = 0;
  for (int i = 1; i < 3; i++) {
    if (t.vertices[i].x > t.vertices[right_index].x) {
      right_index = i;
    }
  }
  return right_index;
}

void z_buffer_algorithm() {
  z_buffer = vector<vector<double>>(screen_width, vector<double>(screen_height, 2));
  color_buffer = vector<vector<rgb_t>>(screen_width, vector<rgb_t>(screen_height, {0, 0, 0}));

  cout << "Number of triangles[0]: " << triangles[0].size() << "\n";
  cout << "Number of triangles[1]: " << triangles[1].size() << "\n";
  cout<<"Number of triangles[2]: "<<triangles[2].size()<<"\n";

  for (int i = 0; i< triangles[2].size(); i++){
    Triangle triangle = triangles[2][i];
    double dy = 2.0/screen_height; 
    double dx = 2.0/screen_width;
    double top_y = 1.0 - dy/2;
    double left_x = -1.0 + dx/2;

    int top_index = get_top_index(triangle);
    int bottom_index = get_bottom_index(triangle);
    int left_index = get_left_index(triangle);
    int right_index = get_right_index(triangle);

    int top_row_n = round((top_y - triangle.vertices[top_index].y)/dy); 
    int bottom_row_n = round((top_y - triangle.vertices[bottom_index].y)/dy);
    top_row_n = max(top_row_n, 0);
    bottom_row_n = min(bottom_row_n, screen_height - 1);
    for(int row_n = top_row_n; row_n <= bottom_row_n; row_n++){
      double ys = top_y - row_n*dy;
      double xa, xb, za, zb;
      if (ys >= triangle.vertices[left_index].y){
        za = triangle.vertices[top_index].z + (triangle.vertices[left_index].z - triangle.vertices[top_index].z)*(ys - triangle.vertices[top_index].y)/(triangle.vertices[left_index].y - triangle.vertices[top_index].y);
        xa = triangle.vertices[top_index].x + (triangle.vertices[left_index].x - triangle.vertices[top_index].x)*(ys - triangle.vertices[top_index].y)/(triangle.vertices[left_index].y - triangle.vertices[top_index].y);
      }  
      else{
        za = triangle.vertices[bottom_index].z + (triangle.vertices[left_index].z - triangle.vertices[bottom_index].z)*(ys - triangle.vertices[bottom_index].y)/(triangle.vertices[left_index].y - triangle.vertices[bottom_index].y);
        xa = triangle.vertices[bottom_index].x + (triangle.vertices[left_index].x - triangle.vertices[bottom_index].x)*(ys - triangle.vertices[bottom_index].y)/(triangle.vertices[left_index].y - triangle.vertices[bottom_index].y);
      }
      if(ys >= triangle.vertices[right_index].y){
        zb = triangle.vertices[top_index].z + (triangle.vertices[right_index].z - triangle.vertices[top_index].z)*(ys - triangle.vertices[top_index].y)/(triangle.vertices[right_index].y - triangle.vertices[top_index].y);
        xb = triangle.vertices[top_index].x + (triangle.vertices[right_index].x - triangle.vertices[top_index].x)*(ys - triangle.vertices[top_index].y)/(triangle.vertices[right_index].y - triangle.vertices[top_index].y);
      }
      else{
        zb = triangle.vertices[bottom_index].z + (triangle.vertices[right_index].z - triangle.vertices[bottom_index].z)*(ys - triangle.vertices[bottom_index].y)/(triangle.vertices[right_index].y - triangle.vertices[bottom_index].y);
        xb = triangle.vertices[bottom_index].x + (triangle.vertices[right_index].x - triangle.vertices[bottom_index].x)*(ys - triangle.vertices[bottom_index].y)/(triangle.vertices[right_index].y - triangle.vertices[bottom_index].y);
      }
      int left_col_n = round((xa - left_x)/dx);
      int right_col_n = round((xb - left_x)/dx);
      left_col_n = max(left_col_n, 0);
      right_col_n = min(right_col_n, screen_width - 1);
      for(int col_n = left_col_n; col_n <= right_col_n; col_n++){
        double xp = left_x + col_n*dx;
        double zp = zb - (zb - za)*(xb - xp)/(xb - xa);
        if(zp >= -1 && zp <= 1 && zp < z_buffer[col_n][row_n]){
          z_buffer[col_n][row_n] = zp;
          color_buffer[col_n][row_n] = triangle_colors[i];
        }
      }
    }
  }
}

void apply_transformations(){
  triangles[1].clear();
  triangles[2].clear();
  triangles[3].clear();
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

void ggSaveImage(string _filename) {
  assign_triangle_colors();
  apply_transformations();
  z_buffer_algorithm();
  bitmap_image image(screen_width, screen_height);
  for (int y = 0; y < screen_height; y++) {
    for (int x = 0; x < screen_width; x++) {
      image.set_pixel(x, y, color_buffer[x][y].red, color_buffer[x][y].green,
                      color_buffer[x][y].blue);
    }
  }
  image.save_image(_filename);
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
}

void ggLookAt(double _eye_x, double _eye_y, double _eye_z,
              double _look_x, double _look_y, double _look_z,
              double _up_x, double _up_y, double _up_z) {
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

void ggPerspective(double _fovy, double _aspect, double _znear, double _zfar) {
  _fovy = _fovy * M_PI / 180;
  double fovx = _fovy * _aspect;
  double t = _znear * tan(_fovy / 2);
  double r = _znear * tan(fovx / 2);
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

void ggTranslate(double _tx, double _ty, double _tz) {
  SquareMatrix T = get_translation_matrix(_tx, _ty, _tz);
  model_matrix = model_matrix.matmul(T);
}

void ggScale(double _sx, double _sy, double _sz) {
  SquareMatrix S = get_scaling_matrix(_sx, _sy, _sz);
  model_matrix = model_matrix.matmul(S);
}

void ggRotate(double _angle, double _x, double _y, double _z) {
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

void ggPrintZBuffer(ostream& _out) {
  _out << "Z Buffer\n";
  for (int y = 0; y < screen_height; y++) {
    for (int x = 0; x < screen_width; x++) {
      _out << z_buffer[x][y] << " ";
    }
    _out << "\n";
  }
}

void ggPrintColorBuffer(ostream& _out) {
  _out << "Color Buffer\n";
  for (int y = 0; y < screen_height; y++) {
    for (int x = 0; x < screen_width; x++) {
      _out << color_buffer[x][y].red << " " << color_buffer[x][y].green << " "
           << color_buffer[x][y].blue << " ";
    }
    _out << "\n";
  }
}

void ggEnd() {
  for(auto t: triangles[0]) {
    triangles[1].clear();
    triangles[1].push_back(t.transform(view_matrix));
  }
  for (auto t : triangles[1]) {
    triangles[2].clear();
    triangles[2].push_back(t.transform(projection_matrix));
  }
  for (auto t : triangles[2]) {
    triangles[3].clear();
    triangles[3].push_back(t.transform(viewport_matrix));
  } 
}


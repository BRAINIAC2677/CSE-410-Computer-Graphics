#pragma once

#include <iostream>
#include <vector>
using namespace std;

struct Color
{
    double r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(double _r, double _g, double _b) : r(_r), g(_g), b(_b) {}

    friend ostream &operator<<(ostream &_out, const Color &_c);
};

struct Coefficients
{
    double ambient, diffuse, specular, reflection;
    Coefficients() : ambient(0), diffuse(0), specular(0), reflection(0) {}
    Coefficients(double _ambient, double _diffuse, double _specular, double _reflection) : ambient(_ambient), diffuse(_diffuse), specular(_specular), reflection(_reflection) {}
};

class Vector3D
{
public:
    double x, y, z;
    Vector3D();
    Vector3D(double _x, double _y, double _z);

    Vector3D normalize();
    double magnitude() const;
    Vector3D operator+(const Vector3D &v) const;
    Vector3D operator+(const double &d) const;
    Vector3D operator+=(const Vector3D &v);
    Vector3D operator+=(const double &d);
    Vector3D operator-(const Vector3D &v) const;
    Vector3D operator-(const double &d) const;
    Vector3D operator-=(const Vector3D &v);
    Vector3D operator-=(const double &d);
    double operator*(const Vector3D &v) const;
    Vector3D operator*(const double &d) const;
    Vector3D operator*=(const double &d);
    Vector3D operator^(const Vector3D &v) const;
    friend ostream &operator<<(ostream &_out, const Vector3D &_v);
};

class Ray
{
public:
    Vector3D origin, direction;
    Ray();
    Ray(Vector3D _origin, Vector3D _direction);
    Ray set_origin(Vector3D _origin);
    Ray set_direction(Vector3D _direction);
    friend ostream &operator<<(ostream &_out, const Ray &_r);
};

class Object
{
protected:
    Vector3D reference_point;
    double height, width, length;
    Color color;
    Coefficients coefficents;
    int shine;

public:
    Object();
    Object set_color(Color _color);
    Object set_shine(int _shine);
    Object set_coefficients(Coefficients _coefficients);
    Coefficients get_coefficients();
    virtual Color get_color_at(Vector3D _point);
    virtual Vector3D get_normal_at(Vector3D _point);
    virtual void draw();
    virtual double intersect(Ray *_ray);
    void phong_lighting(Ray *_ray, Color *_color, int _level);
    friend ostream &operator<<(ostream &_out, const Object &_o);
};

class Sphere : public Object
{
public:
    Sphere(Vector3D _center, double _radius);
    void draw();
    Vector3D get_normal_at(Vector3D _point);
    double intersect(Ray *_ray);
};

class Floor : public Object
{
    double tile_count, tile_size;

public:
    Floor(double _tile_count, double _tile_size, double _height = 0);
    void draw();
    Vector3D get_normal_at(Vector3D _point);
    double intersect(Ray *_ray);
    Color get_color_at(Vector3D _point);
};

class Triangle : public Object
{
    Vector3D a, b, c;
    public:
    Triangle(Vector3D _a, Vector3D _b, Vector3D _c);
    void draw();
    Vector3D get_normal_at(Vector3D _point);
    double intersect(Ray *_ray);
};


// todo: triangle class, object shape having general quadratic equation

class PointLight
{
    Vector3D light_position;
    Color color;

public:
    PointLight();
    PointLight set_light_position(double _x, double _y, double _z);
    PointLight set_color(Color _color);
    Vector3D get_light_position() const;
    Color get_color() const;
    void draw();
    friend ostream &operator<<(ostream &_out, const PointLight &_p);
};

class SpotLight
{
    PointLight point_light;
    Vector3D light_direction;
    double cutoff_angle;

public:
    SpotLight();
    SpotLight set_light_position(double _x, double _y, double _z);
    SpotLight set_color(Color _color);
    SpotLight set_light_direction(double _x, double _y, double _z);
    SpotLight set_cutoff_angle(double _cutoff_angle);

    Vector3D get_light_position() const;
    Color get_color() const;
    Vector3D get_light_direction() const;
    double get_cutoff_angle() const;
    void draw();
    friend ostream &operator<<(ostream &_out, const SpotLight &_s);
};

extern double epsilon;
extern double recursion_level;
extern vector<Object *> objects;
extern vector<PointLight *> pointlights;
extern vector<SpotLight *> spotlights;

double degree_to_radian(double _degree);
double radian_to_degree(double _radian);
#include <iostream>
#include <cmath>
using namespace std;

#include "1905004_classes.hpp"

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <glut.h>
#include <windows.h>
#endif

double epsilon = 1e-6;

ostream &operator<<(ostream &_out, const Color &_c)
{
    _out << "Color: " << _c.r << " " << _c.g << " " << _c.b;
    return _out;
}

Vector3D::Vector3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

Vector3D Vector3D::normalize()
{
    double _magnitude = magnitude();
    x /= _magnitude;
    y /= _magnitude;
    z /= _magnitude;
    return *this;
}

double Vector3D::magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::operator+(const Vector3D &v) const
{
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::operator+(const double &d) const
{
    return Vector3D(x + d, y + d, z + d);
}

Vector3D Vector3D::operator+=(const Vector3D &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3D Vector3D::operator+=(const double &d)
{
    x += d;
    y += d;
    z += d;
    return *this;
}

Vector3D Vector3D::operator-(const Vector3D &v) const
{
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator-(const double &d) const
{
    return Vector3D(x - d, y - d, z - d);
}

Vector3D Vector3D::operator-=(const Vector3D &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3D Vector3D::operator-=(const double &d)
{
    x -= d;
    y -= d;
    z -= d;
    return *this;
}

double Vector3D::operator*(const Vector3D &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::operator*(const double &d) const
{
    return Vector3D(x * d, y * d, z * d);
}

Vector3D Vector3D::operator*=(const double &d)
{
    x *= d;
    y *= d;
    z *= d;
    return *this;
}

Vector3D Vector3D::operator^(const Vector3D &v) const
{
    return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

ostream &operator<<(ostream &_out, const Vector3D &_v)
{
    _out << "< " << _v.x << " " << _v.y << " " << _v.z << " >";
    return _out;
}

Ray::Ray() : origin(0, 0, 0), direction(1, 1, 1) {}

Ray::Ray(Vector3D _origin, Vector3D _direction) : origin(_origin), direction(_direction) {}

Ray Ray::set_origin(Vector3D _origin)
{
    origin = _origin;
    return *this;
}

Ray Ray::set_direction(Vector3D _direction)
{
    direction = _direction;
    return *this;
}

ostream &operator<<(ostream &_out, const Ray &_r)
{
    _out << "Origin: " << _r.origin << " Direction: " << _r.direction;
    return _out;
}

Object::Object() : reference_point(0, 0, 0), height(0), width(0), length(0), shine(0)
{
    color = Color(0, 0, 0);
    coefficents = Coefficients(0, 0, 0, 0);
}

Object Object::set_color(Color _color)
{
    color = _color;
    return *this;
}

Object Object::set_shine(int _shine)
{
    shine = _shine;
    return *this;
}

Object Object::set_coefficients(Coefficients _coefficients)
{
    coefficents = _coefficients;
    return *this;
}

Color Object::get_color_at(Vector3D _point)
{
    return color;
}

Coefficients Object::get_coefficients()
{
    return coefficents;
}

Vector3D Object::get_normal_at(Vector3D _point)
{
    return Vector3D(-1.0, -1.0, -1.0);
}

void Object::draw()
{
    cout << "Object " << *this << endl;
}

double Object::intersect(Ray *_ray, Color *_color, int _level)
{
    return -1.0;
}

// int obs_count = 0, nobs_count = 0;

double Object::phong_lighting(Ray *_ray, Color *_color, int _level)
{
    double tmin = intersect(_ray, _color, _level);
    if (tmin < 0)
    {
        return -1;
    }
    if (_level == 0)
    {
        return 0;
    }
    Vector3D intersection_point = _ray->origin + _ray->direction * tmin;
    Color intersection_point_color = get_color_at(intersection_point);

    _color->r = intersection_point_color.r * coefficents.ambient;
    _color->g = intersection_point_color.g * coefficents.ambient;
    _color->b = intersection_point_color.b * coefficents.ambient;

    for (PointLight *point_light : pointlights)
    {
        Ray light_ray = Ray(point_light->get_light_position(), intersection_point - point_light->get_light_position());
        Ray normal_ray = Ray(intersection_point, get_normal_at(intersection_point));
        Ray reflected_ray = Ray(intersection_point, normal_ray.direction * (2 * (light_ray.direction * normal_ray.direction)) - light_ray.direction);
        Ray view_ray = *_ray;
        light_ray.direction.normalize();
        normal_ray.direction.normalize();
        reflected_ray.direction.normalize();
        view_ray.direction.normalize();

        double distance = (point_light->get_light_position() - intersection_point).magnitude();
        if (distance < epsilon)
        {
            continue;
        }

        bool obscured = false;
        for (Object *object : objects)
        {
            double t = object->intersect(&light_ray, &intersection_point_color, 0);
            if (t > 0 && t + epsilon < distance)
            {
                obscured = true;
                break;
            }
        }
        cout << "obscured: " << obscured << endl;
        // obs_count += obscured;
        // nobs_count += (!obscured);

        if (!obscured)
        {
            double diffuse = max(light_ray.direction * normal_ray.direction, 0.0);
            double specular = max(reflected_ray.direction * view_ray.direction, 0.0);

            // cout << "after ambient reflection: " << (*_color) << endl;
            // diffused reflection

            // cout << "point_light->get_color().r: " << point_light->get_color().r << endl;
            // cout << "intersection_point_color.r: " << intersection_point_color.r << endl;
            // cout << "coefficents.diffuse: " << coefficents.diffuse << endl;
            // cout << "diffuse: " << diffuse << endl;
            // cout << "point_light->get_color().r * intersection_point_color.r * (coefficents.diffuse * diffuse): " << point_light->get_color().r * intersection_point_color.r * (coefficents.diffuse * diffuse) << endl;

            // cout << "point_light->get_color().g: " << point_light->get_color().g << endl;
            // cout << "intersection_point_color.g: " << intersection_point_color.g << endl;
            // cout << "coefficents.diffuse: " << coefficents.diffuse << endl;
            // cout << "diffuse: " << diffuse << endl;
            // cout << "multiplication: " << point_light->get_color().g * intersection_point_color.g * (coefficents.diffuse * diffuse) << endl;

            _color->r += point_light->get_color().r * intersection_point_color.r * (coefficents.diffuse * diffuse);
            _color->g += point_light->get_color().g * intersection_point_color.g * (coefficents.diffuse * diffuse);
            _color->b += point_light->get_color().b * intersection_point_color.b * (coefficents.diffuse * diffuse);

            // cout << "after diffused reflection: " << (*_color) << endl;

            // specular reflection
            // _color->r += point_light->get_color().r * intersection_point_color.r * (coefficents.specular * pow(specular, shine));
            // _color->g += point_light->get_color().g * intersection_point_color.g * (coefficents.specular * pow(specular, shine));
            // _color->b += point_light->get_color().b * intersection_point_color.b * (coefficents.specular * pow(specular, shine));

            // cout << "after specular reflection: " << (*_color) << endl;

            _color->r = min(1.0, _color->r);
            _color->g = min(1.0, _color->g);
            _color->b = min(1.0, _color->b);
            _color->r = max(0.0, _color->r);
            _color->g = max(0.0, _color->g);
            _color->b = max(0.0, _color->b);
        }
    }
    // cout << "obs_count: " << obs_count << endl;
    // cout << "nobs_count: " << nobs_count << endl;
    return tmin;
}

ostream &operator<<(ostream &_out, const Object &_o)
{
    _out << "Reference Point: " << _o.reference_point << " Height: " << _o.height << " Width: " << _o.width << " Length: " << _o.length << " Color: " << _o.color.r << " " << _o.color.g << " " << _o.color.b << " Coefficients: " << _o.coefficents.ambient << " " << _o.coefficents.diffuse << " " << _o.coefficents.reflection << " " << _o.coefficents.specular << " Shine: " << _o.shine;
    return _out;
}

Sphere::Sphere(Vector3D _center, double _radius) : Object()
{
    reference_point = _center;
    height = _radius;
    width = _radius;
    length = _radius;
}

void Sphere::draw()
{
    glPushMatrix();
    {
        glColor3f(color.r, color.g, color.b);
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glutSolidSphere(length, 200, 200);
    }
    glPopMatrix();
}

Vector3D Sphere::get_normal_at(Vector3D _point)
{
    Vector3D normal = _point - reference_point;
    normal.normalize();
    return normal;
}

double Sphere::intersect(Ray *_ray, Color *_color, int _level)
{
    double a = 1;
    double b = 2 * (_ray->direction * _ray->origin);
    double c = _ray->origin * _ray->origin - length * length;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        double t1 = (-b + sqrt(discriminant)) / 2 * a;
        double t2 = (-b - sqrt(discriminant)) / 2 * a;
        if (t1 > 0 && t2 > 0)
        {
            return min(t1, t2);
        }
        else if (t1 > 0)
        {
            return t1;
        }
        else if (t2 > 0)
        {
            return t2;
        }
        else
        {
            return -1.0;
        }
    }
}

Floor::Floor(double _tile_count, double _tile_size) : Object(), tile_count(_tile_count), tile_size(_tile_size)
{
    reference_point = Vector3D(-_tile_count * _tile_size / 2, -_tile_count * _tile_size / 2, 0);
    height = 0;
    width = _tile_count * _tile_size;
    length = _tile_count * _tile_size;
}

void Floor::draw()
{
    int start_x = -tile_count * tile_size / 2;
    int start_y = -tile_count * tile_size / 2;
    glBegin(GL_QUADS);
    {
        for (int i = 0; i < tile_count; i++)
        {
            for (int j = 0; j < tile_count; j++)
            {
                if ((i + j) % 2 == 0)
                {
                    glColor3f(1.0f, 1.0f, 1.0f);
                }
                else
                {
                    glColor3f(0.0f, 0.0f, 0.0f);
                }

                glVertex3f(start_x + i * tile_size, start_y + j * tile_size, 0);
                glVertex3f(start_x + (i + 1) * tile_size, start_y + j * tile_size, 0);
                glVertex3f(start_x + (i + 1) * tile_size, start_y + (j + 1) * tile_size, 0);
                glVertex3f(start_x + i * tile_size, start_y + (j + 1) * tile_size, 0);
            }
        }
    }
    glEnd();
}

PointLight::PointLight() : light_position(0, 0, 0)
{
    color = Color(0, 0, 0);
}

PointLight PointLight::set_light_position(double _x, double _y, double _z)
{
    light_position.x = _x;
    light_position.y = _y;
    light_position.z = _z;
    return *this;
}

PointLight PointLight::set_color(Color _color)
{
    color = _color;
    return *this;
}

Vector3D PointLight::get_light_position() const
{
    return light_position;
}

Color PointLight::get_color() const
{
    return color;
}

ostream &operator<<(ostream &_out, const PointLight &_p)
{
    _out << "Light Position: " << _p.light_position << " Color: " << _p.color.r << " " << _p.color.g << " " << _p.color.b;
    return _out;
}

SpotLight::SpotLight() : light_direction(0, 0, 0), cutoff_angle(0) {}

SpotLight SpotLight::set_light_position(double _x, double _y, double _z)
{
    point_light.set_light_position(_x, _y, _z);
    return *this;
}

SpotLight SpotLight::set_color(Color _color)
{
    point_light.set_color(_color);
    return *this;
}

SpotLight SpotLight::set_light_direction(double _x, double _y, double _z)
{
    light_direction.x = _x;
    light_direction.y = _y;
    light_direction.z = _z;
    light_direction.normalize();
    return *this;
}

SpotLight SpotLight::set_cutoff_angle(double _angle)
{
    cutoff_angle = _angle;
    return *this;
}

Vector3D SpotLight::get_light_position() const
{
    return point_light.get_light_position();
}

Color SpotLight::get_color() const
{
    return point_light.get_color();
}

Vector3D SpotLight::get_light_direction() const
{
    return light_direction;
}

double SpotLight::get_cutoff_angle() const
{
    return cutoff_angle;
}

ostream &operator<<(ostream &_out, const SpotLight &_s)
{
    cout << _s.point_light << " Direction: " << _s.light_direction << " Cutoff Angle: " << _s.cutoff_angle;
    return _out;
}

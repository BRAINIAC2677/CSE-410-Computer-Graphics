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

Vector3D::Vector3D() : x(0), y(0), z(0) {}

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

Ray::Ray() : origin(0, 0, 0), direction(1, 1, 1)
{
    direction.normalize();
}

Ray::Ray(Vector3D _origin, Vector3D _direction) : origin(_origin), direction(_direction)
{
    direction.normalize();
}

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

double Object::intersect(Ray *_ray)
{
    return -1.0;
}

void Object::phong_lighting(Ray *_ray, Color *_color, int _level)
{
    double tmin = intersect(_ray);
    if (tmin < 0)
    {
        return;
    }
    Vector3D intersection_point = _ray->origin + _ray->direction * tmin;
    Color intersection_point_color = get_color_at(intersection_point);

    _color->r = intersection_point_color.r * coefficents.ambient;
    _color->g = intersection_point_color.g * coefficents.ambient;
    _color->b = intersection_point_color.b * coefficents.ambient;

    Ray normal_ray = Ray(intersection_point, get_normal_at(intersection_point));
    Ray view_ray = *_ray;

    // pointlights
    for (PointLight *point_light : pointlights)
    {
        Ray light_ray = Ray(point_light->get_light_position(), intersection_point - point_light->get_light_position());
        Ray reflected_light_ray = Ray(intersection_point, light_ray.direction - normal_ray.direction * (2 * (light_ray.direction * normal_ray.direction)));

        double distance = (point_light->get_light_position() - intersection_point).magnitude();
        if (distance < epsilon)
        {
            continue;
        }

        bool obscured = false;
        for (Object *object : objects)
        {
            double t = object->intersect(&light_ray);
            if (t > 0 && t + epsilon < distance)
            {
                obscured = true;
                break;
            }
        }

        if (!obscured)
        {
            double diffuse = max(-(light_ray.direction * normal_ray.direction), 0.0);
            double specular = max(-(reflected_light_ray.direction * view_ray.direction), 0.0);

            // diffuse reflection
            _color->r += point_light->get_color().r * intersection_point_color.r * (coefficents.diffuse * diffuse);
            _color->g += point_light->get_color().g * intersection_point_color.g * (coefficents.diffuse * diffuse);
            _color->b += point_light->get_color().b * intersection_point_color.b * (coefficents.diffuse * diffuse);

            // specular reflection
            _color->r += point_light->get_color().r * intersection_point_color.r * (coefficents.specular * pow(specular, shine));
            _color->g += point_light->get_color().g * intersection_point_color.g * (coefficents.specular * pow(specular, shine));
            _color->b += point_light->get_color().b * intersection_point_color.b * (coefficents.specular * pow(specular, shine));
        }
    }

    // spotlights
    for (SpotLight *spot_light : spotlights)
    {
        Ray light_ray = Ray(spot_light->get_light_position(), intersection_point - spot_light->get_light_position());
        Ray reflected_light_ray = Ray(intersection_point, light_ray.direction - normal_ray.direction * (2 * (light_ray.direction * normal_ray.direction)));

        double distance = (spot_light->get_light_position() - intersection_point).magnitude();
        if (distance < epsilon)
        {
            continue;
        }

        double beta = acos(light_ray.direction * spot_light->get_light_direction());
        beta = radian_to_degree(beta);
        if (fabs(beta) > spot_light->get_cutoff_angle())
        {
            continue;
        }

        bool obscured = false;
        for (Object *object : objects)
        {
            double t = object->intersect(&light_ray);
            if (t > 0 && t + epsilon < distance)
            {
                obscured = true;
                break;
            }
        }

        if (!obscured)
        {
            double diffuse = max(-(light_ray.direction * normal_ray.direction), 0.0);
            double specular = max(-(reflected_light_ray.direction * view_ray.direction), 0.0);

            // diffuse reflection
            _color->r += spot_light->get_color().r * intersection_point_color.r * (coefficents.diffuse * diffuse);
            _color->g += spot_light->get_color().g * intersection_point_color.g * (coefficents.diffuse * diffuse);
            _color->b += spot_light->get_color().b * intersection_point_color.b * (coefficents.diffuse * diffuse);

            // specular reflection
            _color->r += spot_light->get_color().r * intersection_point_color.r * (coefficents.specular * pow(specular, shine));
            _color->g += spot_light->get_color().g * intersection_point_color.g * (coefficents.specular * pow(specular, shine));
            _color->b += spot_light->get_color().b * intersection_point_color.b * (coefficents.specular * pow(specular, shine));
        }
    }

    // recursive reflection
    if (_level >= recursion_level)
    {
        return;
    }
 
    Ray reflected_view_ray = Ray(intersection_point, view_ray.direction - normal_ray.direction * (2 * (view_ray.direction * normal_ray.direction)));
    reflected_view_ray.origin += reflected_view_ray.direction * epsilon;

    Color *reflected_color = new Color(0, 0, 0);
    double tmin2 = -1;
    Object *nearest_object = nullptr;

    for (auto obj : objects)
    {
        double t = obj->intersect(&reflected_view_ray);
        if ((t > 0) && (t < tmin2 || (nearest_object == nullptr)))
        {
            tmin2 = t;
            nearest_object = obj;
        }
    }

    if (nearest_object != nullptr)
    {
        nearest_object->phong_lighting(&reflected_view_ray, reflected_color, _level + 1);
        _color->r += reflected_color->r * coefficents.reflection;
        _color->g += reflected_color->g * coefficents.reflection;
        _color->b += reflected_color->b * coefficents.reflection;
    }

    _color->r = min(1.0, _color->r);
    _color->g = min(1.0, _color->g);
    _color->b = min(1.0, _color->b);
    _color->r = max(0.0, _color->r);
    _color->g = max(0.0, _color->g);
    _color->b = max(0.0, _color->b);
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

double Sphere::intersect(Ray *_ray)
{
    _ray->origin -= reference_point;
    double a = 1;
    double b = 2 * (_ray->direction * _ray->origin);
    double c = _ray->origin * _ray->origin - length * length;
    double discriminant = b * b - 4 * a * c;
    _ray->origin += reference_point;

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

Floor::Floor(double _tile_count, double _tile_size, double _height) : Object(), tile_count(_tile_count), tile_size(_tile_size)
{
    reference_point = Vector3D(-_tile_count * _tile_size / 2, -_tile_count * _tile_size / 2, 0);
    height = _height;
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

                glVertex3f(start_x + i * tile_size, start_y + j * tile_size,height);
                glVertex3f(start_x + (i + 1) * tile_size, start_y + j * tile_size, height);
                glVertex3f(start_x + (i + 1) * tile_size, start_y + (j + 1) * tile_size, height);   
                glVertex3f(start_x + i * tile_size, start_y + (j + 1) * tile_size, height);
            }
        }
    }
    glEnd();
}

Vector3D Floor::get_normal_at(Vector3D _point)
{
    return Vector3D(0, 0, 1);
}

double Floor::intersect(Ray *_ray)
{
    double t = (height - _ray->origin.z) / _ray->direction.z;
    if (t < 0)
    {
        return -1.0;
    }
    Vector3D intersection_point = _ray->origin + _ray->direction * t;
    if (intersection_point.x < reference_point.x || intersection_point.x > reference_point.x + width || intersection_point.y < reference_point.y || intersection_point.y > reference_point.y + length)
    {
        return -1.0;
    }
    return t;
}

Color Floor::get_color_at(Vector3D _point)
{
    int x = (_point.x - reference_point.x) / tile_size;
    int y = (_point.y - reference_point.y) / tile_size;
    if ((x + y) % 2 == 0)
    {
        return Color(1, 1, 1);
    }
    else
    {
        return Color(0, 0, 0);
    }
}

Triangle::Triangle(Vector3D _a, Vector3D _b, Vector3D _c) : Object(), a(_a), b(_b), c(_c)
{
    Vector3D normal = (b - a) ^ (c - a);
    normal.normalize();
    reference_point = a;
    height = normal.x;
    width = normal.y;
    length = normal.z;
}

void Triangle::draw()
{
    glBegin(GL_TRIANGLES);
    {
        glColor3f(color.r, color.g, color.b);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
    }
    glEnd();
}

Vector3D Triangle::get_normal_at(Vector3D _point)
{
    Vector3D normal = (b - a) ^ (c - a);
    normal.normalize();
    return normal;
}

double Triangle::intersect(Ray *_ray)
{
    Vector3D normal = get_normal_at(reference_point);
    double denominator = normal * _ray->direction;
    if (fabs(denominator) < epsilon)
    {
        return -1.0;
    }
    double t = ((reference_point - _ray->origin) * normal) / denominator;
    if (t < 0)
    {
        return -1.0;
    }
    Vector3D intersection_point = _ray->origin + _ray->direction * t;
    Vector3D c0 = (b - a) ^ (intersection_point - a);
    Vector3D c1 = (c - b) ^ (intersection_point - b);
    Vector3D c2 = (a - c) ^ (intersection_point - c);
    if ((c0 * c1 >= 0) && (c1 * c2 >= 0))
    {
        return t;
    }
    return -1.0;
}

bool General::inside_bounding_box(Vector3D _point)
{
    if(fabs(width)> epsilon) 
    {
        if(_point.x < reference_point.x || _point.x > reference_point.x + width) 
        {
            return false;
        }
    }
    if(fabs(height) > epsilon) 
    {
        if(_point.y < reference_point.y || _point.y > reference_point.y + height) 
        {
            return false;
        }
    }
    if(fabs(length) > epsilon) 
    {
        if(_point.z < reference_point.z || _point.z > reference_point.z + length) 
        {
            return false;
        }
    }
    return true;
}

General::General(double _a, double _b, double _c, double _d, double _e, double _f, double _g, double _h, double _i, double _j) : Object(), a(_a), b(_b), c(_c), d(_d), e(_e), f(_f), g(_g), h(_h), i(_i), j(_j)
{
}

void General::set_bounding_box(Vector3D _reference_point, double _height, double _width, double _length)
{
    reference_point = _reference_point;
    height = _height;
    width = _width;
    length = _length;
}

void General::draw()
{
    return;
}

Vector3D General::get_normal_at(Vector3D _point)
{
    double x = 2 * a * _point.x + d * _point.y + e * _point.z + g;
    double y = 2 * b * _point.y + d * _point.x + f * _point.z + h;
    double z = 2 * c * _point.z + e * _point.x + f * _point.y + i;
    return Vector3D(x, y, z);
}

double General::intersect(Ray *_ray)
{
    double A = a * _ray->direction.x * _ray->direction.x + b * _ray->direction.y * _ray->direction.y + c * _ray->direction.z * _ray->direction.z + d * _ray->direction.x * _ray->direction.y + e * _ray->direction.y * _ray->direction.z + f * _ray->direction.z * _ray->direction.x;
    double B = 2 * (a * _ray->direction.x * _ray->origin.x + b * _ray->direction.y * _ray->origin.y + c * _ray->direction.z * _ray->origin.z) + d * (_ray->direction.x * _ray->origin.y + _ray->direction.y * _ray->origin.x) + e * (_ray->direction.y * _ray->origin.z + _ray->direction.z * _ray-> origin.y) + f * (_ray->direction.z * _ray->origin.x + _ray->direction.x * _ray->origin.z) + g * _ray->direction.x + h * _ray->direction.y + i * _ray->direction.z;
    double C = a * _ray->origin.x * _ray->origin.x + b * _ray->origin.y * _ray->origin.y + c * _ray->origin.z * _ray->origin.z + d * _ray->origin.x * _ray->origin.y + e * _ray->origin.y * _ray->origin.z + f * _ray->origin.z * _ray->origin.x + g * _ray->origin.x + h * _ray->origin.y + i * _ray->origin.z + j;
    double discriminant = B * B - 4 * A * C;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        double t1 = (-B + sqrt(discriminant)) / (2 * A);
        double t2 = (-B - sqrt(discriminant)) / (2 * A);
        if(t1 > t2)
        {
            swap(t1, t2);
        }
        Vector3D intersection_point1 = _ray->origin + _ray->direction * t1;
        Vector3D intersection_point2 = _ray->origin + _ray->direction * t2;
        // t1 < 0 and t2 < 0
        if(t2 < 0)
        {
            return -1.0;
        }
        // t1 < 0 and t2 > 0
        else if(t1 < 0)
        {
            return t2;
        }
        // t1 > 0 and t2 > 0
        else
        {
            if(inside_bounding_box(intersection_point1))
            {
                return t1;
            }
            if(inside_bounding_box(intersection_point2))
            {
                return t2;
            }
        }
        return -1.0;
    }
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

void PointLight::draw()
{
    glPushMatrix();
    {
        glColor3f(color.r, color.g, color.b);
        glTranslatef(light_position.x, light_position.y, light_position.z);
        glutSolidSphere(5, 200, 200);
    }
    glPopMatrix();
}

ostream &operator<<(ostream &_out, const PointLight &_p)
{
    _out << "Light Position: " << _p.light_position << " Color: " << _p.color.r << " " << _p.color.g << " " << _p.color.b;
    return _out;
}

SpotLight::SpotLight() : light_direction(1, 1, 1), cutoff_angle(0)
{
    light_direction.normalize();
}

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

void SpotLight::draw()
{
    point_light.draw();
}

ostream &operator<<(ostream &_out, const SpotLight &_s)
{
    cout << _s.point_light << " Direction: " << _s.light_direction << " Cutoff Angle: " << _s.cutoff_angle;
    return _out;
}

double degree_to_radian(double _degree)
{
    return _degree * M_PI / 180;
}

double radian_to_degree(double _radian)
{
    return _radian * 180 / M_PI;
}
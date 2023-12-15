/*
Date: 2023/15/12
Author: Asif Azad
Description: helper library for openGL
*/

#include "gglib.hpp"

GGvector::GGvector() {}

GGvector::GGvector(GLdouble _x, GLdouble _y, GLdouble _z)
{
    x = _x, y = _y, z = _z;
}

GGvector GGvector::operator+(const GGvector &p) const
{
    return GGvector(x + p.x, y + p.y, z + p.z);
}

GGvector GGvector::operator+(const GLdouble &d) const
{
    return GGvector(x + d, y + d, z + d);
}

GGvector GGvector::operator+=(const GGvector &p)
{
    x += p.x, y += p.y, z += p.z;
    return *this;
}

GGvector GGvector::operator+=(const GLdouble &d)
{
    x += d, y += d, z += d;
    return *this;
}

GGvector GGvector::operator-(const GGvector &p) const
{
    return GGvector(x - p.x, y - p.y, z - p.z);
}

GGvector GGvector::operator-(const GLdouble &d) const
{
    return GGvector(x - d, y - d, z - d);
}

GGvector GGvector::operator-=(const GGvector &p)
{
    x -= p.x, y -= p.y, z -= p.z;
    return *this;
}

GGvector GGvector::operator-=(const GLdouble &d)
{
    x -= d, y -= d, z -= d;
    return *this;
}

GGvector GGvector::operator*(const GGvector &p) const
{
    return GGvector(x * p.x, y * p.y, z * p.z);
}

GGvector GGvector::operator*(const GLdouble &d) const
{
    return GGvector(x * d, y * d, z * d);
}

GGvector GGvector::operator*=(const GGvector &p)
{
    x *= p.x, y *= p.y, z *= p.z;
    return *this;
}

GGvector GGvector::operator*=(const GLdouble &d)
{
    x *= d, y *= d, z *= d;
    return *this;
}

ostream &operator<<(ostream &output, const GGvector &p)
{
    output << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return output;
}

GGvector GGvector::normalize()
{
    GLdouble length = sqrt(x * x + y * y + z * z);
    x /= length, y /= length, z /= length;
    return *this;
}

GGvector GGvector::operator^(const GGvector &p) const
{
    return GGvector(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

void ggDrawAxes()
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
    }
    glEnd();
}

void ggDrawCheckerBoard(GLint _tile_count, GLint _tile_size)
{
    GLint start_x = -_tile_count * _tile_size / 2;
    GLint start_y = -_tile_count * _tile_size / 2;

    glBegin(GL_QUADS);
    {
        for (GLint i = 0; i < _tile_count; i++)
        {
            for (GLint j = 0; j < _tile_count; j++)
            {
                if ((i + j) % 2 == 0)
                {
                    glColor3f(1.0f, 1.0f, 1.0f);
                }
                else
                {
                    glColor3f(0.0f, 0.0f, 0.0f);
                }

                glVertex3f(start_x + i * _tile_size, start_y + j * _tile_size, 0);
                glVertex3f(start_x + (i + 1) * _tile_size, start_y + j * _tile_size, 0);
                glVertex3f(start_x + (i + 1) * _tile_size, start_y + (j + 1) * _tile_size, 0);
                glVertex3f(start_x + i * _tile_size, start_y + (j + 1) * _tile_size, 0);
            }
        }
    }
    glEnd();
}

GLdouble ggDeg2Rad(GLdouble _deg)
{
    return _deg * acos(-1) / 180.0;
}

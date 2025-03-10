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

GGvector GGvector::rotate(GLdouble _angle, GGvector _axis)
{
    _axis = _axis.normalize();
    GGvector rotated = (*this) * cos(ggDeg2Rad(_angle)) + (_axis ^ (*this)) * sin(ggDeg2Rad(_angle));
    return rotated;
}

GGvector GGvector::operator^(const GGvector &p) const
{
    return GGvector(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

void ggDrawAxes(GLint _axis_length)
{
    glLineWidth(5);
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(_axis_length / 2, 0, 0);
        glVertex3f(-_axis_length / 2, 0, 0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0, _axis_length / 2, 0);
        glVertex3f(0, -_axis_length / 2, 0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0, 0, _axis_length / 2);
        glVertex3f(0, 0, -_axis_length / 2);
    }
    glEnd();
    glLineWidth(1);
}

void ggDrawGridPlane(GLint _grid_length, GLint _grid_count)
{
    GLint start_x = -_grid_length * _grid_count / 2;
    GLint start_y = -_grid_length * _grid_count / 2;

    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        for (GLint i = 0; i <= _grid_count; i++)
        {
            glVertex3f(start_x + i * _grid_length, start_y, 0);
            glVertex3f(start_x + i * _grid_length, -start_y, 0);

            glVertex3f(start_x, start_y + i * _grid_length, 0);
            glVertex3f(-start_x, start_y + i * _grid_length, 0);
        }
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

void ggDrawSquareWall(GLint _wall_length, GLint _wall_height)
{
    GLint start_x = -_wall_length / 2;
    GLint start_y = -_wall_length / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(start_x, start_y, 0);
        glVertex3f(start_x + _wall_length, start_y, 0);
        glVertex3f(start_x + _wall_length, start_y, _wall_height);
        glVertex3f(start_x, start_y, _wall_height);

        glVertex3f(start_x + _wall_length, start_y, 0);
        glVertex3f(start_x + _wall_length, start_y + _wall_length, 0);
        glVertex3f(start_x + _wall_length, start_y + _wall_length, _wall_height);
        glVertex3f(start_x + _wall_length, start_y, _wall_height);

        glVertex3f(start_x + _wall_length, start_y + _wall_length, 0);
        glVertex3f(start_x, start_y + _wall_length, 0);
        glVertex3f(start_x, start_y + _wall_length, _wall_height);
        glVertex3f(start_x + _wall_length, start_y + _wall_length, _wall_height);

        glVertex3f(start_x, start_y + _wall_length, 0);
        glVertex3f(start_x, start_y, 0);
        glVertex3f(start_x, start_y, _wall_height);
        glVertex3f(start_x, start_y + _wall_length, _wall_height);
    }
    glEnd();
}

void ggDrawUnitTriangle()
{
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    }
    glEnd();
}

void ggDrawSphereFace(GLdouble _radius, GLint _divisions)
{
    vector<vector<GGvector>> vertices(_divisions + 1, vector<GGvector>(_divisions + 1));

    for (GLsizei i = 0; i <= _divisions; ++i)
    {
        GLdouble latitude_angle = M_PI / 4 - i * M_PI / 2 / _divisions;
        GGvector n2 = GGvector(-sin(latitude_angle), cos(latitude_angle), 0);
        for (GLsizei j = 0; j <= _divisions; ++j)
        {
            GLdouble longitude_angle = -M_PI / 4 + j * M_PI / 2 / _divisions;
            GGvector n1 = GGvector(-sin(longitude_angle), 0, -cos(longitude_angle));
            GGvector v = n1 ^ n2;
            vertices[i][j] = v.normalize();
            vertices[i][j] = vertices[i][j] * _radius;
        }
    }

    for (GLsizei i = 0; i < _divisions; ++i)
    {
        for (GLsizei j = 0; j < _divisions; ++j)
        {
            glBegin(GL_QUADS);
            {
                glVertex3f(vertices[i][j].x, vertices[i][j].y, vertices[i][j].z);
                glVertex3f(vertices[i][j + 1].x, vertices[i][j + 1].y, vertices[i][j + 1].z);
                glVertex3f(vertices[i + 1][j + 1].x, vertices[i + 1][j + 1].y, vertices[i + 1][j + 1].z);
                glVertex3f(vertices[i + 1][j].x, vertices[i + 1][j].y, vertices[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void ggDrawCylinderSegment(GLdouble _radius, GLdouble _height, GLint _divisions)
{
    vector<GGvector> base_vertices;
    vector<GGvector> top_vertices;

    GLdouble delta_angle = ggDeg2Rad(70.5287794) / _divisions;
    GLdouble start_angle = -(delta_angle * _divisions) / 2;
    for (int i = 0; i < _divisions + 1; i++)
    {
        GLdouble angle = start_angle + i * delta_angle;
        base_vertices.push_back(GGvector(_radius * cos(angle), _radius * sin(angle), 0));
        top_vertices.push_back(GGvector(_radius * cos(angle), _radius * sin(angle), _height));
    }

    glBegin(GL_QUADS);
    {
        for (int i = 0; i < _divisions; i++)
        {
            glVertex3f(base_vertices[i].x, base_vertices[i].y, base_vertices[i].z);
            glVertex3f(base_vertices[i + 1].x, base_vertices[i + 1].y, base_vertices[i + 1].z);
            glVertex3f(top_vertices[i + 1].x, top_vertices[i + 1].y, top_vertices[i + 1].z);
            glVertex3f(top_vertices[i].x, top_vertices[i].y, top_vertices[i].z);
        }
    }
    glEnd();
}

GLdouble ggDeg2Rad(GLdouble _deg)
{
    return _deg * acos(-1) / 180.0;
}

void GGsphere::__generate_vertices()
{
    GLdouble stack_step = M_PI / this->stacks;
    GLdouble sector_step = 2 * M_PI / this->sectors;

    for (int i = 0; i <= this->stacks; ++i)
    {
        GLdouble stack_angle = M_PI / 2 - i * stack_step;
        GLdouble xy = radius * cosf(stack_angle);
        GLdouble z = radius * sinf(stack_angle);

        for (int j = 0; j <= this->sectors; ++j)
        {
            float sector_angle = j * sector_step;

            GLdouble x = xy * cosf(sector_angle);
            GLdouble y = xy * sinf(sector_angle);
            this->vertices.push_back(GGvector(x, y, z));
        }
    }
}

// void GGsphere::__draw_up()
// {
//     assert(this->colors.size() >= 1);
//     glColor3f(this->colors[0].x, this->colors[0].y, this->colors[0].z);

//     GGvector scaled_up = this->up * (2 * this->radius);
//     GGvector arrow_tip = this->position + scaled_up;

//     glLineWidth(5);
//     glBegin(GL_LINES);
//     {
//         glVertex3f(this->position.x, this->position.y, this->position.z);
//         glVertex3f(arrow_tip.x, arrow_tip.y, arrow_tip.z);

//         // Draw the arrowhead
//         float arrowhead_size = 0.1; // You can adjust the size as needed
//         GGvector arrowhead_direction = scaled_up.normalize(); // Normalize the direction
//         GGvector arrowhead_right = GGvector(0.0, 1.0, 0.0) ^ (arrowhead_direction).normalize(); // Calculate a right vector
//         GGvector arrowhead_up = arrowhead_direction ^ (arrowhead_right).normalize(); // Calculate an up vector

//         // Draw arrowhead lines
//         GGvector arrowhead1 = arrow_tip - (arrowhead_direction * arrowhead_size) + (arrowhead_right * (arrowhead_size / 2.0));
//         GGvector arrowhead2 = arrow_tip - (arrowhead_direction * arrowhead_size) - (arrowhead_right * (arrowhead_size / 2.0));
//         glVertex3f(arrow_tip.x, arrow_tip.y, arrow_tip.z);
//         glVertex3f(arrowhead1.x, arrowhead1.y, arrowhead1.z);

//         glVertex3f(arrow_tip.x, arrow_tip.y, arrow_tip.z);
//         glVertex3f(arrowhead2.x, arrowhead2.y, arrowhead2.z);
//     }
//     glEnd();
//     glLineWidth(1);
// }

void GGsphere::__draw_up()
{
    assert(this->colors.size() >= 1);
    glColor3f(this->colors[0].x, this->colors[0].y, this->colors[0].z);

    GGvector scaled_up = this->up * (2 * this->radius);

    glLineWidth(5);
    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(scaled_up.x, scaled_up.y, scaled_up.z);
    }
    glEnd();
    glLineWidth(1);
}

void GGsphere::__draw_forward()
{
    assert(this->colors.size() >= 2);
    glColor3f(this->colors[1].x, this->colors[1].y, this->colors[1].z);

    GGvector scaled_forward = this->forward * (2 * this->radius);

    glLineWidth(5);
    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(scaled_forward.x, scaled_forward.y, scaled_forward.z);
    }
    glLineWidth(1);
    glEnd();
}

GGsphere::GGsphere()
{
    this->position = GGvector(0, 0, 0);
    this->up = GGvector(0, 0, 1);
    this->forward = GGvector(0, 1, 0);
    this->radius = 1;
    this->delta_rolling_angle = 30;
    this->sectors = 20;
    this->stacks = 20;
    this->colors = {{1, 0, 0}, {0, 1, 0}};
    this->wall_length = -1;
    this->__generate_vertices();
}

GGsphere::GGsphere(GGvector _center, GLdouble _radius, GLint _sectors, GLint _stacks)
{
    this->position = _center;
    this->up = GGvector(0, 0, 1);
    this->forward = GGvector(0, 1, 0);
    this->radius = _radius;
    this->delta_rolling_angle = 30;
    this->sectors = _sectors;
    this->stacks = _stacks;
    this->colors = {{1, 0, 0}, {0, 1, 0}};
    this->wall_length = -1;
    this->__generate_vertices();
}

void GGsphere::__draw_vertices()
{
    GLint current_color_init_index = 0;
    for (int i = 0; i < this->stacks; ++i)
    {
        GLint k1 = i * (this->sectors + 1);
        GLint k2 = k1 + this->sectors + 1;

        GLint current_color_index = current_color_init_index;
        current_color_init_index = (current_color_init_index + 1) % this->colors.size();

        for (int j = 0; j < this->sectors; ++j, ++k1, ++k2)
        {
            glColor3f(this->colors[current_color_index].x, this->colors[current_color_index].y, this->colors[current_color_index].z);
            current_color_index = (current_color_index + 1) % this->colors.size();

            if (i != 0)
            {
                glBegin(GL_TRIANGLES);
                glVertex3f(vertices[k1].x, vertices[k1].y, vertices[k1].z);
                glVertex3f(vertices[k2].x, vertices[k2].y, vertices[k2].z);
                glVertex3f(vertices[k1 + 1].x, vertices[k1 + 1].y, vertices[k1 + 1].z);
                glEnd();
            }

            // k1+1 => k2 => k2+1
            if (i != (this->sectors - 1))
            {
                glBegin(GL_TRIANGLES);
                glVertex3f(vertices[k1 + 1].x, vertices[k1 + 1].y, vertices[k1 + 1].z);
                glVertex3f(vertices[k2].x, vertices[k2].y, vertices[k2].z);
                glVertex3f(vertices[k2 + 1].x, vertices[k2 + 1].y, vertices[k2 + 1].z);
                glEnd();
            }
        }
    }
}

void GGsphere::draw()
{
    glPushMatrix();
    {
        GGvector right = this->forward ^ this->up;
        glTranslatef(this->position.x, this->position.y, this->position.z);
        this->__draw_up();
        this->__draw_forward();
        glRotatef(this->total_rolling_angle, right.x, right.y, right.z);
        this->__draw_vertices();
    }
    glPopMatrix();
}

void GGsphere::change_forward_direction(GLdouble _angle)
{
    GGvector right = this->forward ^ this->up;
    GGvector orthogonal_up = right ^ this->forward;
    this->forward = this->forward.rotate(_angle, orthogonal_up);
}

void GGsphere::roll_forward()
{

    GLdouble small_angle = 0.1;
    for (GLdouble angle = 0; angle < this->delta_rolling_angle; angle += small_angle)
    {
        GLdouble delta_position_mag = (small_angle / 360.0) * 2 * M_PI * this->radius;
        this->position = this->position + this->forward.normalize() * delta_position_mag;
        this->deflect_after_wall_collision();
    }

    this->total_rolling_angle -= this->delta_rolling_angle;
    this->total_rolling_angle = fmod(this->total_rolling_angle, 360);
}

void GGsphere::roll_backward()
{
    GLdouble small_angle = 0.1;
    for (GLdouble angle = 0; angle < this->delta_rolling_angle; angle += small_angle)
    {
        GLdouble delta_position_mag = (small_angle / 360.0) * 2 * M_PI * this->radius;
        this->position = this->position - this->forward.normalize() * delta_position_mag;
        this->deflect_after_wall_collision();
    }

    this->total_rolling_angle += this->delta_rolling_angle;
    this->total_rolling_angle = fmod(this->total_rolling_angle, 360);

    this->deflect_after_wall_collision();
}

vector<GGvector> GGsphere::get_farthest_surface_point()
{
    GGvector farthest_point = this->position + this->forward.normalize() * this->radius;
    GGvector back_farthest_point = this->position - this->forward.normalize() * this->radius;
    return {farthest_point, back_farthest_point};
}

void GGsphere::deflect_after_wall_collision()
{
    if (this->wall_length == -1)
    {
        return;
    }

    GGvector farthest_point = this->get_farthest_surface_point()[0];
    GGvector back_farthest_point = this->get_farthest_surface_point()[1];

    // check for collision with each of the four walls and deflect accordingly
    if (farthest_point.x >= this->wall_length / 2)
    {
        this->forward.x *= -1;
    }
    else if (farthest_point.x <= -this->wall_length / 2)
    {
        this->forward.x *= -1;
    }
    else if (farthest_point.y >= this->wall_length / 2)
    {
        this->forward.y *= -1;
    }
    else if (farthest_point.y <= -this->wall_length / 2)
    {
        this->forward.y *= -1;
    }

    if (back_farthest_point.x >= this->wall_length / 2)
    {
        this->forward.x *= -1;
    }
    else if (back_farthest_point.x <= -this->wall_length / 2)
    {
        this->forward.x *= -1;
    }
    else if (back_farthest_point.y >= this->wall_length / 2)
    {
        this->forward.y *= -1;
    }
    else if (back_farthest_point.y <= -this->wall_length / 2)
    {
        this->forward.y *= -1;
    }
}

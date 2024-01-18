/*
Date: 2023/15/12
Author: Asif Azad
Description: Controlling the camera view with keyboard in openGL
*/

#include <bits/stdc++.h>
using namespace std;
#include "gglib.hpp"

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <glut.h>
#include <windows.h>
#endif

GLsizei GGwindow_position_x = 100, GGwindow_position_y = 100;
GLsizei GGwindow_width = 1920, GGwindow_height = 1080;
GLint GG_AXIS_LENGTH = 400;
GLint GG_GRID_LENGTH = 5;
GLint GG_GRID_COUNT = 20;

GLdouble GG_FOVY = 45, GG_ZNEAR = 1, GG_ZFAR = 500;

GLdouble GG_CAMERA_CHANGE = 5;
GLdouble GG_CAMERA_ANGLE_CHANGE = 10;
GLdouble GG_CAMERA_PARAMETER = 100;

GGvector GGcamera_position(GG_CAMERA_PARAMETER, GG_CAMERA_PARAMETER, GG_CAMERA_PARAMETER);
GGvector GGcamera_view(-GG_CAMERA_PARAMETER, -GG_CAMERA_PARAMETER, -GG_CAMERA_PARAMETER);
GGvector GGcamera_up(0, 0, 1);

GLdouble GGtriangle_current_side_length = 20, GGtriangle_max_side_length = 20;
GLdouble GGsphere_current_radius = 0, GGsphere_max_radius = GGtriangle_max_side_length / sqrt(3);
GLdouble GGsphere_radius_change = GGsphere_max_radius / 20;

GLdouble GGoverall_rotation_angle = 0;

void ggInit()
{
    glClearColor(0.0f, 0.0f, 0.0f,
                 1.0f); // Set background color to black and opaque

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = (GLdouble)GGwindow_width / (GLdouble)GGwindow_height;
    gluPerspective(GG_FOVY, aspect, GG_ZNEAR, GG_ZFAR);
    glMatrixMode(GL_MODELVIEW);
}

void ggReshape(GLsizei width, GLsizei height)
{
    if (height == 0)
    {
        height = 1; // To prevent divide by 0
    }

    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(GG_FOVY, aspect, GG_ZNEAR, GG_ZFAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ggDrawTriangularFaces()
{
    GLdouble difference = (GGtriangle_max_side_length - GGtriangle_current_side_length) / 3;
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            if (i % 2 == 0)
            {
                glColor3f(0.0f, 1.0f, 1.0f);
            }
            else
            {
                glColor3f(1.0f, 0.0f, 1.0f);
            }
            glRotatef(i * 90, 0, 0, 1);
            glTranslatef(difference, difference, difference);
            glScaled(GGtriangle_current_side_length, GGtriangle_current_side_length, GGtriangle_current_side_length);
            ggDrawUnitTriangle();
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            if (i % 2 == 0)
            {
                glColor3f(1.0f, 0.0f, 1.0f);
            }
            else
            {
                glColor3f(0.0f, 1.0f, 1.0f);
            }
            // lower faces
            glRotatef(i * 90, 0, 0, 1);
            glRotatef(180, 1, 1, 0);
            glTranslatef(difference, difference, difference);
            glScaled(GGtriangle_current_side_length, GGtriangle_current_side_length, GGtriangle_current_side_length);
            ggDrawUnitTriangle();
        }
        glPopMatrix();
    }
}

void ggDrawSphericalVertices()
{
    // upper vertex and lower vertex
    for (int i = 0; i < 2; i++)
    {
        glPushMatrix();
        {
            if (i == 0)
            {
                glColor3f(1.0f, 0.0f, 0.0f);
            }
            else
            {
                glColor3f(0.0f, 1.0f, 0.0f);
            }
            glRotatef(i * 180, 1, 0, 0);
            glTranslatef(0, 0, GGtriangle_current_side_length);
            glRotatef(-90, 0, 1, 0);
            ggDrawSphereFace(GGsphere_current_radius, 150);
        }
        glPopMatrix();
    }

    // lateral vertices
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            if (i % 2 == 0)
            {
                glColor3f(0.0f, 0.0f, 1.0f);
            }
            else
            {
                glColor3f(1.0f, 1.0f, 0.0f);
            }
            glRotatef(i * 90, 0, 0, 1);
            glTranslatef(GGtriangle_current_side_length, 0, 0);
            ggDrawSphereFace(GGsphere_current_radius, 150);
        }
        glPopMatrix();
    }
}

void ggDrawCylindricalEdges()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    GLdouble cylinder_height = GGtriangle_current_side_length * sqrt(2);
    GLdouble cyliner_radius = GGsphere_current_radius;

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotatef(i * 90 + 45, 0, 1, 0);
            glTranslatef(cylinder_height / 2, 0, -cylinder_height / 2);
            ggDrawCylinderSegment(cyliner_radius, cylinder_height, 150);
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotatef(i * 90 + 45, 1, 0, 0);
            glTranslatef(0, cylinder_height / 2, -cylinder_height / 2);
            glRotatef(90, 0, 0, 1);
            ggDrawCylinderSegment(cyliner_radius, cylinder_height, 150);
        }
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotatef(i * 90 + 45, 0, 0, 1);
            glTranslatef(cylinder_height / 2, cylinder_height / 2, 0);
            glRotatef(90, 1, 0, 0);
            ggDrawCylinderSegment(cyliner_radius, cylinder_height, 150);
        }
        glPopMatrix();
    }
}

void ggDisplay()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(GGcamera_position.x, GGcamera_position.y, GGcamera_position.z, GGcamera_position.x + GGcamera_view.x, GGcamera_position.y + GGcamera_view.y, GGcamera_position.z + GGcamera_view.z, GGcamera_up.x, GGcamera_up.y, GGcamera_up.z);

    glRotatef(GGoverall_rotation_angle, 0, 0, 1);
    ggDrawAxes(GG_AXIS_LENGTH);
    ggDrawGridPlane(GG_GRID_LENGTH, GG_GRID_COUNT);

    ggDrawTriangularFaces();
    ggDrawCylindricalEdges();
    ggDrawSphericalVertices();

    glutSwapBuffers();
}

void ggKeyboardListener(unsigned char key, int x, int y)
{
    GGvector right_unit = (GGcamera_view ^ GGcamera_up).normalize();
    switch (key)
    {
    case '1':
        // rotate/ look left
        GGcamera_view = GGcamera_view * cos(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE)) + (GGcamera_up.normalize() ^ GGcamera_view) * sin(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE));
        break;
    case '2':
        // rotate/ look right
        GGcamera_view = GGcamera_view * cos(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE)) + (GGcamera_up.normalize() ^ GGcamera_view) * sin(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE));
        break;
    case '3':
        // look up
        GGcamera_view = GGcamera_view * cos(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE)) + (right_unit ^ GGcamera_view) * sin(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE));
        GGcamera_up = GGcamera_up * cos(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE)) + (right_unit ^ GGcamera_up) * sin(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE));
        break;
    case '4':
        // look down
        GGcamera_view = GGcamera_view * cos(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE)) + (right_unit ^ GGcamera_view) * sin(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE));
        GGcamera_up = GGcamera_up * cos(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE)) + (right_unit ^ GGcamera_up) * sin(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE));
        break;
    case '5':
        // tilt counterclockwise
        GGcamera_up = GGcamera_up * cos(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE)) + (GGcamera_view.normalize() ^ GGcamera_up) * sin(ggDeg2Rad(GG_CAMERA_ANGLE_CHANGE));
        break;
    case '6':
        // tilt clockwise
        GGcamera_up = GGcamera_up * cos(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE)) + (GGcamera_view.normalize() ^ GGcamera_up) * sin(ggDeg2Rad(-GG_CAMERA_ANGLE_CHANGE));
        break;
    case '.':
        // sphere to octahedron
        GGtriangle_current_side_length += 1;
        GGsphere_current_radius -= GGsphere_radius_change;
        if (GGtriangle_current_side_length > GGtriangle_max_side_length)
        {
            GGtriangle_current_side_length = GGtriangle_max_side_length;
            GGsphere_current_radius = 0;
        }
        break;
    case ',':
        // octahedron to sphere
        GGtriangle_current_side_length -= 1;
        GGsphere_current_radius += GGsphere_radius_change;
        if (GGtriangle_current_side_length < 0)
        {
            GGtriangle_current_side_length = 0;
            GGsphere_current_radius = GGsphere_max_radius;
        }
        break;
    case 'a':
        // rotate left
        GGoverall_rotation_angle += 5;
        break;
    case 'd':
        // rotate right
        GGoverall_rotation_angle -= 5;
        break;
    case 'q':
        // exit
        exit(0);
        break;
    default:
        break;
    }
}

void ggKeyboardSpecialListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // move forward
        GGcamera_position += GGcamera_view.normalize() * GG_CAMERA_CHANGE;
        break;
    case GLUT_KEY_DOWN:
        // move backward
        GGcamera_position -= GGcamera_view.normalize() * GG_CAMERA_CHANGE;
        break;
    case GLUT_KEY_RIGHT:
        // move right
        GGcamera_position += (GGcamera_view ^ GGcamera_up).normalize() * GG_CAMERA_CHANGE;
        break;
    case GLUT_KEY_LEFT:
        // move left
        GGcamera_position -= (GGcamera_view ^ GGcamera_up).normalize() * GG_CAMERA_CHANGE;
        break;
    case GLUT_KEY_PAGE_UP:
        // move up
        GGcamera_position += GGcamera_up.normalize() * GG_CAMERA_CHANGE;
        break;
    case GLUT_KEY_PAGE_DOWN:
        // move down
        GGcamera_position -= GGcamera_up.normalize() * GG_CAMERA_CHANGE;
        break;
    default:
        break;
    }
}

void ggIdle()
{
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(GGwindow_width, GGwindow_height);
    glutInitWindowPosition(GGwindow_position_x, GGwindow_position_y);

    glutCreateWindow("It's Magic!");

    glutDisplayFunc(ggDisplay);
    glutReshapeFunc(ggReshape);
    glutKeyboardFunc(ggKeyboardListener);
    glutSpecialFunc(ggKeyboardSpecialListener);
    glutIdleFunc(ggIdle);

    ggInit();

    glutMainLoop();

    return 0;
}

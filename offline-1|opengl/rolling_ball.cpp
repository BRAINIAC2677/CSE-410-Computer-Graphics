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
GLdouble GG_FOVY = 45, GG_ZNEAR = 1, GG_ZFAR = 500;

GLint GG_TILE_COUNT = 100, GG_TILE_SIZE = 10;

GLdouble GG_CAMERA_CHANGE = 5;
GLdouble GG_CAMERA_ANGLE_CHANGE = 10;
GLdouble GG_CAMERA_PARAMETER = 40;

GGvector GGcamera_position(GG_CAMERA_PARAMETER, GG_CAMERA_PARAMETER, GG_CAMERA_PARAMETER);
GGvector GGcamera_view(-GG_CAMERA_PARAMETER, -GG_CAMERA_PARAMETER, -GG_CAMERA_PARAMETER);
GGvector GGcamera_up(0, 0, 1);

GLdouble GG_SPHERE_RADIUS = 2;
GLdouble GG_WALL_LENGTH = 50, GG_WALL_HEIGHT = 2 * GG_SPHERE_RADIUS;
GGsphere sphere(GGvector(0, 0, GG_SPHERE_RADIUS), GG_SPHERE_RADIUS, 8, 8);
GLdouble GG_SPHERE_DIRECTION_ANGLE_CHANGE = 10;

bool GGis_simulation_on = false;

void ggInit()
{
    glClearColor(0.0f, 0.0f, 0.0f,
                 1.0f); // Set background color to black and opaque

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = (GLdouble)GGwindow_width / (GLdouble)GGwindow_height;
    gluPerspective(GG_FOVY, aspect, GG_ZNEAR, GG_ZFAR);

    sphere.wall_length = GG_WALL_LENGTH;

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

void ggDisplay()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(GGcamera_position.x, GGcamera_position.y, GGcamera_position.z, GGcamera_position.x + GGcamera_view.x, GGcamera_position.y + GGcamera_view.y, GGcamera_position.z + GGcamera_view.z, GGcamera_up.x, GGcamera_up.y, GGcamera_up.z);
    ggDrawCheckerBoard(GG_TILE_COUNT, GG_TILE_SIZE);

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    {
        ggDrawSquareWall(GG_WALL_LENGTH, GG_WALL_HEIGHT);
        sphere.draw();
    }
    glPopMatrix();

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

    case 'j':
        // change forward direction counterclockwise
        sphere.change_forward_direction(GG_SPHERE_DIRECTION_ANGLE_CHANGE);
        break;
    case 'l':
        // change forward direction clockwise
        sphere.change_forward_direction(-GG_SPHERE_DIRECTION_ANGLE_CHANGE);
        break;
    case 'i':
        // roll forward
        sphere.roll_forward();
        break;
    case 'k':
        // roll backward
        sphere.roll_backward();
        break;

    // spacebar to toggle simulation
    case ' ':
        GGis_simulation_on = !GGis_simulation_on;
        break;

    case 'q':
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
    if (GGis_simulation_on)
    {
        sphere.roll_forward();
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(GGwindow_width, GGwindow_height);
    glutInitWindowPosition(GGwindow_position_x, GGwindow_position_y);

    glutCreateWindow("Camera View Controller");

    glutDisplayFunc(ggDisplay);
    glutReshapeFunc(ggReshape);
    glutKeyboardFunc(ggKeyboardListener);
    glutSpecialFunc(ggKeyboardSpecialListener);
    glutIdleFunc(ggIdle);

    ggInit();

    glutMainLoop();

    return 0;
}

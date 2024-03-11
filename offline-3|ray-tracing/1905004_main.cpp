#include <bits/stdc++.h>
using namespace std;
#include "1905004_classes.hpp"
#include "bitmap_image.hpp"

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <glut.h>
#include <windows.h>
#endif

double window_width, window_height;
double image_width, image_height;
double window_position_x = 100, window_position_y = 100;

double tile_count = 100, tile_size = 20;

int capture_count = 0;
string output_file_dir;

void capture();

void load_objects(ifstream &_in)
{
    Floor *checkered_floor = new Floor(tile_count, tile_size);
    checkered_floor->set_coefficients(Coefficients(0.4, 0.2, 0.2, 0.2));
    checkered_floor->height = 0;
    checkered_floor->tile_color1 = Color(1, 1, 1);
    checkered_floor->tile_color2 = Color(0, 0, 0);
    objects.push_back(checkered_floor);

    int n_objects;
    _in >> n_objects;
    for (int i = 0; i < n_objects; i++)
    {
        string type;
        _in >> type;
        Object *obj;
        if (type == "sphere")
        {
            double x, y, z, radius;
            _in >> x >> y >> z >> radius;
            Sphere *s = new Sphere(Vector3D(x, y, z), radius);
            obj = s;
        }
        else if (type == "triangle")
        {
            Vector3D a, b, c;
            _in >> a.x >> a.y >> a.z >> b.x >> b.y >> b.z >> c.x >> c.y >> c.z;
            Triangle *t = new Triangle(a, b, c);
            obj = t;
        }
        else if (type == "general")
        {
            double a, b, c, d, e, f, g, h, i, j;
            _in >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;
            General *general = new General(a, b, c, d, e, f, g, h, i, j);
            Vector3D reference_point;
            double height, width, length;
            _in >> reference_point.x >> reference_point.y >> reference_point.z >> height >> width >> length;
            general->set_bounding_box(reference_point, height, width, length);
            obj = general;
        }

        double r, g, b;
        _in >> r >> g >> b;
        obj->set_color(Color(r, g, b));

        double ambient, diffuse, specular, reflection;
        _in >> ambient >> diffuse >> specular >> reflection;
        obj->set_coefficients(Coefficients(ambient, diffuse, specular, reflection));

        int shine;
        _in >> shine;
        obj->set_shine(shine);

        objects.push_back(obj);
    }
}

void load_pointlights(ifstream &_in)
{
    int n_pointlights;
    _in >> n_pointlights;
    for (int i = 0; i < n_pointlights; i++)
    {
        double x, y, z;
        _in >> x >> y >> z;
        PointLight *pl = new PointLight();
        pl->set_light_position(x, y, z);

        double r, g, b;
        _in >> r >> g >> b;
        pl->set_color(Color(r, g, b));

        pointlights.push_back(pl);
    }
}

void load_spotlights(ifstream &_in)
{
    int n_spotlights;
    _in >> n_spotlights;
    for (int i = 0; i < n_spotlights; i++)
    {
        double x, y, z;
        _in >> x >> y >> z;
        SpotLight *sl = new SpotLight();
        sl->set_light_position(x, y, z);

        double r, g, b;
        _in >> r >> g >> b;
        sl->set_color(Color(r, g, b));

        double dx, dy, dz;
        _in >> dx >> dy >> dz;
        sl->set_light_direction(dx, dy, dz);

        double cutoff_angle;
        _in >> cutoff_angle;
        sl->set_cutoff_angle(cutoff_angle);

        spotlights.push_back(sl);
    }
}

void load_data(string _file_name)
{
    ifstream in(_file_name);
    double dimension;
    in >> recursion_level >> dimension;
    window_height = window_width = dimension;
    image_height = image_width = dimension;
    load_objects(in);
    load_pointlights(in);
    load_spotlights(in);
}

void print_inputs()
{
    for (int i = 0; i < objects.size(); i++)
    {
        cout << *objects[i] << endl;
    }
    for (int i = 0; i < pointlights.size(); i++)
    {
        cout << *pointlights[i] << endl;
    }
    for (int i = 0; i < spotlights.size(); i++)
    {
        cout << *spotlights[i] << endl;
    }
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f,
                 1.0f); // Set background color to black and opaque

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = (GLdouble)window_width / (GLdouble)window_height;
    gluPerspective(fovy, aspect, znear, zfar);
    glMatrixMode(GL_MODELVIEW);

    camera_look.normalize();
    camera_up.normalize();
    camera_right = camera_look ^ camera_up;
    camera_right.normalize();
}

void draw_objects()
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects[i]->draw();
    }
}

void draw_lights()
{
    for (int i = 0; i < pointlights.size(); i++)
    {
        pointlights[i]->draw();
    }
    for (int i = 0; i < spotlights.size(); i++)
    {
        spotlights[i]->draw();
    }
}

void display()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, camera_pos.x + camera_look.x, camera_pos.y + camera_look.y, camera_pos.z + camera_look.z, camera_up.x, camera_up.y, camera_up.z);

    draw_objects();
    draw_lights();

    glutSwapBuffers();
}

void reshape(int _width, int _height)
{
    if (_height == 0)
    {
        _height = 1; // To prevent divide by 0
    }

    GLfloat aspect = (GLfloat)_width / (GLfloat)_height;
    glViewport(0, 0, _width, _height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fovy, aspect, znear, zfar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard_listener(unsigned char _key, int _x, int _y)
{
    switch (_key)
    {
    case '0':
        capture();
        break;
    case '1':
        // rotate/ look left
        rotate3D(camera_look, camera_up, camera_angle_change);
        rotate3D(camera_right, camera_up, camera_angle_change);
        break;
    case '2':
        // rotate/ look right
        rotate3D(camera_look, camera_up, -camera_angle_change);
        rotate3D(camera_right, camera_up, -camera_angle_change);
        break;
    case '3':
        // look up
        rotate3D(camera_look, camera_right, camera_angle_change);
        rotate3D(camera_up, camera_right, camera_angle_change);
        break;
    case '4':
        // look down
        rotate3D(camera_look, camera_right, -camera_angle_change);
        rotate3D(camera_up, camera_right, -camera_angle_change);
        break;
    case '5':
        // tilt counterclockwise
        rotate3D(camera_up, camera_look, camera_angle_change);
        rotate3D(camera_right, camera_look, camera_angle_change);
        break;
    case '6':
        // tilt clockwise
        rotate3D(camera_up, camera_look, -camera_angle_change);
        rotate3D(camera_right, camera_look, -camera_angle_change);
        break;
    case 'q':
        // quit
        exit(0);
    default:
        break;
    }
}

void keyboard_special_listener(int _key, int _x, int _y)
{
    switch (_key)
    {
    case GLUT_KEY_UP:
        // move forward
        camera_pos += camera_look.normalize() * camera_change;
        break;
    case GLUT_KEY_DOWN:
        // move backward
        camera_pos -= camera_look.normalize() * camera_change;
        break;
    case GLUT_KEY_RIGHT:
        // move right
        camera_pos += camera_right.normalize() * camera_change;
        break;
    case GLUT_KEY_LEFT:
        // move left
        camera_pos -= camera_right.normalize() * camera_change;
        break;
    case GLUT_KEY_PAGE_UP:
        // move up
        camera_pos += camera_up.normalize() * camera_change;
        break;
    case GLUT_KEY_PAGE_DOWN:
        // move down
        camera_pos -= camera_up.normalize() * camera_change;
        break;
    default:
        break;
    }
}

void idle()
{
    glutPostRedisplay();
}

void free_memory()
{
    for (int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }
    for (int i = 0; i < pointlights.size(); i++)
    {
        delete pointlights[i];
    }
    for (int i = 0; i < spotlights.size(); i++)
    {
        delete spotlights[i];
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cout << "usage: <exe_file_name> <input_file_path> <output_file_dir>" << endl;
        return 0;
    }

    string input_file_path = argv[1];
    output_file_dir = argv[2];
    load_data(input_file_path);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(window_position_x, window_position_y);

    glutCreateWindow("Ray Tracing - 1905004");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard_listener);
    glutSpecialFunc(keyboard_special_listener);
    glutIdleFunc(idle);

    init();
    glutMainLoop();
    free_memory();

    return 0;
}

void capture()
{
    cout << "Capturing..." << endl;
    bitmap_image image(image_width, image_height);
    image.clear();

    double plane_distance = (window_height / 2.0) / tan(degree_to_radian(fovy / 2.0));

    Vector3D top_left = camera_pos + (camera_look * plane_distance) + (camera_up * (window_height / 2.0)) - (camera_right * (window_width / 2.0));

    double du = window_width / image_width, dv = window_height / image_height;

    top_left = top_left + (camera_right * du * 0.5) - (camera_up * dv * 0.5);

    for (int i = 0; i < image_width; i++)
    {
        for (int j = 0; j < image_height; j++)
        {
            Vector3D current_pixel = top_left + (camera_right * i * du) - (camera_up * j * dv);

            Vector3D ray_vector = (current_pixel - camera_pos).normalize();

            Ray *ray = new Ray(camera_pos, ray_vector);

            Color *color = new Color(0, 0, 0);
            double tmin = -1;
            Object *nearest_object = nullptr;

            for (auto obj : objects)
            {
                double t = obj->intersect(ray);
                if ((t > 0) && (t < tmin || (nearest_object == nullptr)))
                {
                    tmin = t;
                    nearest_object = obj;
                }
            }

            // check if the intersection point is inside zfar
            Vector3D intersection_point = camera_pos + (ray_vector * tmin);
            double t_along_look = (intersection_point - camera_pos) * camera_look;

            if (t_along_look > zfar || t_along_look < znear)
            {
                nearest_object = nullptr;
            }

            if (nearest_object != nullptr)
            {
                nearest_object->phong_lighting(ray, color, 0);
                color->r = round(color->r * 255);
                color->g = round(color->g * 255);
                color->b = round(color->b * 255);
                image.set_pixel(i, j, color->r, color->g, color->b);
            }
            delete ray;
            delete color;
        }
    }

    capture_count++;
    image.save_image(output_file_dir + "/capture-" + to_string(capture_count) + ".bmp");
    cout << "Capture saved as capture-" << capture_count << ".bmp" << endl;
}
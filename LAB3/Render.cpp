#include "OpenGL.h"
#include <cmath>

void task1() {
    double old_width;
    glGetDoublev(GL_LINE_WIDTH, &old_width);

    glLineWidth(4);

    int samples = 180;
    double arg_min = 0;
    double arg_max = 10;
    double step = (arg_max - arg_min) / (samples - 1);

    glColor3d(1, 0, 1);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < samples; ++i) {
        double x = arg_min + i * step;
        double z = 2 * std::sin(2 * x);
        glVertex3d(x, 0, z);
    }
    glEnd();

    glLineWidth(old_width);

    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3d(1, 0, 0);

    double point_x = arg_min + 31 * step;
    double point_z = 2 * std::sin(2 * point_x);
    glVertex3d(point_x, 0, point_z);

    glEnd();
}

void task3() {
    glLineWidth(4);

    double t_min = 0;
    double t_max = 2 * M_PI;
    int samples = 100;
    double step = (t_max - t_min) / (samples - 1);

    glBegin(GL_LINE_STRIP); // ✅ исправлено
    for (int i = 0; i < samples; ++i) {
        double t = t_min + step * i;
        double x = 16 * std::pow(sin(t), 3);
        double y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);
        glVertex3d(x, y, 0);
    }
    glEnd();

    glLineWidth(1);
}

void task5() {
    double old_width;
    glGetDoublev(GL_LINE_WIDTH, &old_width);
    glLineWidth(4);

    int samples_x = 21;
    int samples_y = 21;

    double x_min = -2.0, x_max = 2.0;
    double y_min = -2.0, y_max = 2.0;

    double step_x = (x_max - x_min) / (samples_x - 1);
    double step_y = (y_max - y_min) / (samples_y - 1);

    glColor3d(1, 0, 1);

    for (int j = 0; j < samples_y; ++j) {
        double y = y_min + j * step_y;
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < samples_x; ++i) {
            double x = x_min + i * step_x;
            double z = x * x - y * y;
            glVertex3d(x, y, z);
        }
        glEnd();
    }

    for (int i = 0; i < samples_x; ++i) {
        double x = x_min + i * step_x;
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < samples_y; ++j) {
            double y = y_min + j * step_y;
            double z = x * x - y * y;
            glVertex3d(x, y, z);
        }
        glEnd();
    }

    glLineWidth(old_width);
}

void task4() {
    double old_width;
    glGetDoublev(GL_LINE_WIDTH, &old_width);

    int rings = 80;
    int sides = 40;

    auto fx = [](double u, double v) {
        return cos(u) * sin(v) + 3 * cos(u) * (1.5 + 0.5 * sin(1.5 * u));
    };

    auto fy = [](double u, double v) {
        return sin(u) * sin(v) + 3 * sin(u) * (1.5 + 0.5 * sin(1.5 * u));
    };

    auto fz = [](double u, double v) {
        return sin(v) + 2 * cos(1.5 * u);
    };

    glColor3d(0.0, 0.8, 0.8);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < rings; ++i) {
        for (int j = 0; j < sides; ++j) {

            double u1 = -2.0 * M_PI + 4.0 * M_PI * i / rings;
            double u2 = -2.0 * M_PI + 4.0 * M_PI * (i + 1) / rings;

            double v1 = -M_PI + 2.0 * M_PI * j / sides;
            double v2 = -M_PI + 2.0 * M_PI * (j + 1) / sides;

            double x1 = fx(u1, v1), y1 = fy(u1, v1), z1 = fz(u1, v1);
            double x2 = fx(u2, v1), y2 = fy(u2, v1), z2 = fz(u2, v1);
            double x3 = fx(u1, v2), y3 = fy(u1, v2), z3 = fz(u1, v2);
            double x4 = fx(u2, v2), y4 = fy(u2, v2), z4 = fz(u2, v2);

            glVertex3d(x1, y1, z1);
            glVertex3d(x2, y2, z2);
            glVertex3d(x3, y3, z3);

            glVertex3d(x2, y2, z2);
            glVertex3d(x4, y4, z4);
            glVertex3d(x3, y3, z3);
        }
    }
    glEnd();

    glLineWidth(1.5);
    glColor3d(0.1, 0.1, 0.1);

    for (int i = 0; i < rings; ++i) {
        double u = -2.0 * M_PI + 4.0 * M_PI * i / rings;

        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= sides; ++j) {
            double v = -M_PI + 2.0 * M_PI * j / sides;
            glVertex3d(fx(u, v), fy(u, v), fz(u, v));
        }
        glEnd();
    }

    for (int j = 0; j < sides; ++j) {
        double v = -M_PI + 2.0 * M_PI * j / sides;

        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= rings; ++i) {
            double u = -2.0 * M_PI + 4.0 * M_PI * i / rings;
            glVertex3d(fx(u, v), fy(u, v), fz(u, v));
        }
        glEnd();
    }

    glLineWidth(old_width);
}

void Render(double delta_time)
{
    task4();
}

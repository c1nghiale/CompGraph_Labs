#include "OpenGL.h"
#include <cmath>

void task1() {
    double old_width ;
    glGetDoublev ( GL_LINE_WIDTH , & old_width );
    //задаем новую ширину линии
    glLineWidth (4);
    //кол-во точек
    int samples = 180;
    //диапазон графика
    double arg_min = 0;
    double arg_max = 10;
    //шаг между точками
    double step = ( arg_max-arg_min )/( samples-1);
    glColor3d (1, 0, 1);
    glBegin ( GL_LINE_STRIP );
    //for нужно делать по
    //целочисленным типам5
    for (int i = 0; i < samples ; ++i)
    {
        double x = arg_min + i*step;
        double z =  2 * std::sin(2*x);
        glVertex3d (x, 0, z);
    }
    glEnd ();
    //восстанавливаем старую ширину линии
    glLineWidth ( old_width );

    //задаем размер точек
    glPointSize (10);
    //рисуем точки
    glBegin ( GL_POINTS );
    glColor3d (1, 0, 0);
    double point_x = arg_min + 31*step;
    double point_z =  2 * std::sin(2*point_x);
    glVertex3d (point_x, 0, point_z);
    glEnd ();
}

void task3() {
    double t_min = 0;
    double t_max = 12*PI_M;
    int samples = 100;
    double step = (t_max - t_min)/( samples-1);
    glLineWidth (4);
    glBegin ( GL_LINE_STRIP );
    for (int i = 0; i < samples ; ++i)
    {
        double t = t_min + step ∗ i;
        double x = cos(t);
        double y = sin(t);
        double z = -1 + 0.1 * t;
        glVertex3d (x, y, z);
    }
    glEnd ();
    glLineWidth (1);
}

void Render(double delta_time)
{

    task1();

}

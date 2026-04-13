#define _USE_MATH_DEFINES
#include "Render.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <random>
#include <cmath>

void semiCircleArc() {
    // Начальная точка
    double P0x = -8,   P0y = 0, P0z = 4;
    // Центр окружности
    double Cx  = -6.5, Cy = 0, Cz = 6;
    // Конечная точка
    double P2x = -5,   P2y = 0, P2z = 8;

    // Радиус — расстояние от центра до начальной точки
    double radius = sqrt((P0x - Cx)*(P0x - Cx) + (P0z - Cz)*(P0z - Cz));

    // Начальный и конечный угол
    double angleStart = atan2(P0z - Cz, P0x - Cx);
    double angleEnd   = atan2(P2z - Cz, P2x - Cx);

    int steps = 100;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= steps; i++) {
        double t     = (double)i / steps;
        double angle = angleStart - t * M_PI; // полуокружность = π радиан

        double x = Cx + radius * cos(angle);
        double y = Cy;
        double z = Cz + radius * sin(angle);

        glVertex3d(x, y, z);
    }
    glEnd();
}

void Prism();
void quarterCilinder1();
void quarterCilinder2();
void quarterCilinder3();

void Render(double delta_time)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    float m[] = {
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 1
    };
    glMultMatrixf(m);  // умножаем на текущую, а не заменяем

    Prism();
    glPopMatrix();
}


void Prism() {
    std::mt19937 gen(time(nullptr));
    std::uniform_real_distribution<double> r(0.0, 1.0);
    double A[]{ -1, 0, 0 };
    double B[]{ 2, 0, 0 };
    double C[]{ 0, 0, 1 };
    double D[]{ 7, 0, 5 };
    double E[]{ -5, 0, 8 };
    double F[]{ -8, 0, 4 };
    double G[]{ -3, 0, -9 };
    double H[]{ 6, 0, -7 };

    double A1[]{ -1, 5, 0 };
    double B1[]{ 2, 5, 0 };
    double C1[]{ 0, 5, 1 };
    double D1[]{ 7, 5, 5 };
    double E1[]{ -5, 5, 8 };
    double F1[]{ -8, 5, 4 };
    double G1[]{ -3, 5, -9 };
    double H1[]{ 6, 5, -7 };

    //Basement
    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_TRIANGLES);
    glVertex3dv(A); glVertex3dv(B); glVertex3dv(C);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_TRIANGLES);
    glVertex3dv(B); glVertex3dv(C); glVertex3dv(D);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(A); glVertex3dv(B);
    glVertex3dv(H); glVertex3dv(G);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(A); glVertex3dv(C);
    glVertex3dv(E); glVertex3dv(F);
    glEnd();

    //Cover
    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_TRIANGLES);
    glVertex3dv(A1); glVertex3dv(B1); glVertex3dv(C1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_TRIANGLES);
    glVertex3dv(B1); glVertex3dv(C1); glVertex3dv(D1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(A1); glVertex3dv(B1);
    glVertex3dv(H1); glVertex3dv(G1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(A1); glVertex3dv(C1);
    glVertex3dv(E1); glVertex3dv(F1);
    glEnd();

    //Walls
    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(G); glVertex3dv(H);
    glVertex3dv(H1); glVertex3dv(G1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(H); glVertex3dv(B);
    glVertex3dv(B1); glVertex3dv(H1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(B); glVertex3dv(D);
    glVertex3dv(D1); glVertex3dv(B1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(D); glVertex3dv(C);
    glVertex3dv(C1); glVertex3dv(D1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(C); glVertex3dv(E);
    glVertex3dv(E1); glVertex3dv(C1);
    glEnd();
    /*
    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(E); glVertex3dv(F);
    glVertex3dv(F1); glVertex3dv(E1);
    glEnd()*/

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(F); glVertex3dv(A);
    glVertex3dv(A1); glVertex3dv(F1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    glBegin(GL_QUADS);
    glVertex3dv(A); glVertex3dv(G);
    glVertex3dv(G1); glVertex3dv(A1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    quarterCilinder1();

    glColor3d(r(gen), r(gen), r(gen));
    quarterCilinder2();

    glColor3d(r(gen), r(gen), r(gen));
    semiCircleArc();
}

void quarterCilinder1() {
    double Ax = -6.5, Ay = 0, Az = 6;
    double Bx = -5,   By = 0, Bz = 8;
    double height = 5;
    double cx = -6.5, cz = 6;

    int steps = 180;
    double angleStep = M_PI / steps;

    for (int i = 0; i < steps; i++) {
        double a1 = i * angleStep;
        double a2 = (i + 1) * angleStep;

        double cos1 = cos(a1), sin1 = sin(a1);
        double x1 = cx + (Ax - cx) * cos1 - (Az - cz) * sin1;
        double z1 = cz + (Ax - cx) * sin1 + (Az - cz) * cos1;
        double x2 = cx + (Bx - cx) * cos1 - (Bz - cz) * sin1;
        double z2 = cz + (Bx - cx) * sin1 + (Bz - cz) * cos1;

        double cos2 = cos(a2), sin2 = sin(a2);
        double x1n = cx + (Ax - cx) * cos2 - (Az - cz) * sin2;
        double z1n = cz + (Ax - cx) * sin2 + (Az - cz) * cos2;
        double x2n = cx + (Bx - cx) * cos2 - (Bz - cz) * sin2;
        double z2n = cz + (Bx - cx) * sin2 + (Bz - cz) * cos2;

        glBegin(GL_TRIANGLES);

        // Грань вдоль стороны AB (низ)
        glVertex3d(x1,  Ay, z1);
        glVertex3d(x2,  By, z2);
        glVertex3d(x2n, By, z2n);

        glVertex3d(x1,  Ay, z1);
        glVertex3d(x2n, By, z2n);
        glVertex3d(x1n, Ay, z1n);

        // Грань вдоль стороны с высотой
        glVertex3d(x2,  By,          z2);
        glVertex3d(x2,  By + height, z2);
        glVertex3d(x2n, By + height, z2n);

        glVertex3d(x2,  By,          z2);
        glVertex3d(x2n, By + height, z2n);
        glVertex3d(x2n, By,          z2n);

        // Грань от вершины A до верхней точки
        glVertex3d(x1,  Ay,          z1);
        glVertex3d(x2,  By + height, z2);
        glVertex3d(x2n, By + height, z2n);

        glVertex3d(x1,  Ay,          z1);
        glVertex3d(x2n, By + height, z2n);
        glVertex3d(x1n, Ay,          z1n);

        glEnd();
    }
}

void quarterCilinder2() {
    double Ax = -6.5, Ay = 5, Az = 6;
    double Bx = -5,   By = 5, Bz = 8;
    double height = -5;
    double cx = -6.5, cz = 6;

    // Центр для треугольников
    double Ccx = -6.5, Ccy = 2.5, Ccz = 6;

    int steps = 180;
    double angleStep = M_PI / steps;

    for (int i = 0; i < steps; i++) {
        double a1 = i * angleStep;
        double a2 = (i + 1) * angleStep;

        double cos1 = cos(a1), sin1 = sin(a1);
        double x1 = cx + (Ax - cx) * cos1 - (Az - cz) * sin1;
        double z1 = cz + (Ax - cx) * sin1 + (Az - cz) * cos1;
        double x2 = cx + (Bx - cx) * cos1 - (Bz - cz) * sin1;
        double z2 = cz + (Bx - cx) * sin1 + (Bz - cz) * cos1;

        double cos2 = cos(a2), sin2 = sin(a2);
        double x1n = cx + (Ax - cx) * cos2 - (Az - cz) * sin2;
        double z1n = cz + (Ax - cx) * sin2 + (Az - cz) * cos2;
        double x2n = cx + (Bx - cx) * cos2 - (Bz - cz) * sin2;
        double z2n = cz + (Bx - cx) * sin2 + (Bz - cz) * cos2;

        glBegin(GL_TRIANGLES);

        // Грань вдоль стороны AB (низ) — треугольники к центру
        glVertex3d(x1,   Ay,  z1);
        glVertex3d(x2,   By,  z2);
        glVertex3d(Ccx,  Ccy, Ccz);

        glVertex3d(x2,   By,  z2);
        glVertex3d(x2n,  By,  z2n);
        glVertex3d(Ccx,  Ccy, Ccz);

        glVertex3d(x2n,  By,  z2n);
        glVertex3d(x1n,  Ay,  z1n);
        glVertex3d(Ccx,  Ccy, Ccz);

        glVertex3d(x1n,  Ay,  z1n);
        glVertex3d(x1,   Ay,  z1);
        glVertex3d(Ccx,  Ccy, Ccz);

        // Грань вдоль стороны с высотой — треугольники к центру
        glVertex3d(x2,   By,          z2);
        glVertex3d(x2,   By + height, z2);
        glVertex3d(Ccx,  Ccy,         Ccz);

        glVertex3d(x2,   By + height, z2);
        glVertex3d(x2n,  By + height, z2n);
        glVertex3d(Ccx,  Ccy,         Ccz);

        glVertex3d(x2n,  By + height, z2n);
        glVertex3d(x2n,  By,          z2n);
        glVertex3d(Ccx,  Ccy,         Ccz);

        // Грань от вершины A до верхней точки — треугольники к центру
        glVertex3d(x1,   Ay,          z1);
        glVertex3d(x2,   By + height, z2);
        glVertex3d(Ccx,  Ccy,         Ccz);

        glVertex3d(x2,   By + height, z2);
        glVertex3d(x2n,  By + height, z2n);
        glVertex3d(Ccx,  Ccy,         Ccz);

        glVertex3d(x2n,  By + height, z2n);
        glVertex3d(x1n,  Ay,          z1n);
        glVertex3d(Ccx,  Ccy,         Ccz);

        glEnd();
    }
}

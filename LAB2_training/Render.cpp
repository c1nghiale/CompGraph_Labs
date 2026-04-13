#define _USE_MATH_DEFINES
#include "Camera.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <random>
#include <cmath>

#include <string>
#include <iostream>
#include <algorithm>

// Библиотека для разгрузки изображений
// https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//объявлена в main.cpp, папка data где расположенны загружаемые данные программы.
extern std::string data_folder;

//объект, в котором инкапсулирована работа с GLWF и Open GL, создается в main.
extern OpenGL* ogl_object;

Camera camera;

void Prism();
void quarterCilinder1();
void quarterCilinder2();
void SetNormals();
void semiCircleArc();

void InitRender()
{

    //================НАСТРОЙКА КАМЕРЫ======================
    camera.caclulateCameraPos();
    ogl_object->mouseMoveEvent().reaction(&camera, &Camera::MouseMovie);
    ogl_object->wheelEvent().reaction(&camera, &Camera::Zoom);
    ogl_object->mouseButtonEvent().reaction(&camera, &Camera::MouseStartDrag);
    //================++++++++++++++++======================

    //комментаций к листингу 7 из тренировки
    //
    //Текстуру следует загружать не как она написана в pdf(лст.7, стр.6):
    //unsigned char∗ data = stbi_load (” texture .png”, &x, &y, &n, 4);
    //а вот так
    //unsigned char* data = stbi_load( std::format("{}texture.png", data_folder).c_str(), &x, &y, &n, 4);
    //data_folder --- это путь до папки data, где лежит текстура, с которого должен начинатся путь до любого файла в этой папке.
    //std::format --- это функция из стандарта С++20, которая позволяет форматировать строки, как в python. В данном случае она просто склеивает строку data_folder и "texture.png", получая путь до текстуры.

}

void Render(double delta_time)
{

    camera.ApplyCamera(); //в старом шаблоне SetUpCamera
    ogl_object->drawAxisAndCell(); //оси и сетка

    //включаем свет
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    float lightPos[] = { 5.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

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



void SetNormals(const double *v0, const double *v1, const double * v2 ){
    //вычисление координат точек нормального вектора
    double ax =v1[0] - v0[0],  ay =v1[1] - v0[1], az =v1[2] - v0[2];
    double bx =v2[0] - v0[0],  by =v2[1] - v0[1], bz =v2[2] - v0[2];
    double cx =v2[0] - v1[0],  cy =v2[1] - v1[1], cz =v2[2] - v1[2];

    //вычисление координат нормального вектора
    double nx = ay*bz - az*by;
    double ny = az*bx - ax*bz;
    double nz = ax*by - ay*bx;

    //вычисление длины нормального вектора
    double len = sqrt(nx*nx + ny*ny + nz*nz);

    //нормализация XDDD
    glNormal3d(nx/len, ny/len, nz/len);
}

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



void Prism() {
    std::mt19937 gen(time(nullptr));
    std::uniform_real_distribution<double> r(0.0, 1.0);
    // Нижнее основание (Y = 0) с микро-сдвигом для предотвращения мерцания
        double A[]{ -1, 0.0001, 0 };
        double B[]{ 2, 0.0002, 0 };
        double C[]{ 0, 0.0003, 1 };
        double D[]{ 7, 0.0004, 5 };
        double E[]{ -5, 0.0005, 8 };
        double F[]{ -8, 0.0006, 4 };
        double G[]{ -3, 0.0007, -9 };
        double H[]{ 6, 0.0008, -7 };

        // Верхнее основание (Y = 5) с микро-сдвигом
        double A1[]{ -1, 5.0001, 0 };
        double B1[]{ 2, 5.0002, 0 };
        double C1[]{ 0, 5.0003, 1 };
        double D1[]{ 7, 5.0004, 5 };
        double E1[]{ -5, 5.0005, 8 };
        double F1[]{ -8, 5.0006, 4 };
        double G1[]{ -3, 5.0007, -9 };
        double H1[]{ 6, 5.0008, -7 };

    //Basement
    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(A, B, C);
    glBegin(GL_TRIANGLES);
    glVertex3dv(A); glVertex3dv(B); glVertex3dv(C);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(B, C, D);
    glBegin(GL_TRIANGLES);
    glVertex3dv(B); glVertex3dv(C); glVertex3dv(D);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(A, B, H);
    glBegin(GL_QUADS);
    glVertex3dv(A); glVertex3dv(B);
    glVertex3dv(H); glVertex3dv(G);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(A, C, E);
    glBegin(GL_QUADS);
    glVertex3dv(A); glVertex3dv(C);
    glVertex3dv(E); glVertex3dv(F);
    glEnd();

    //Cover
    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(A1, B1, C1);
    glBegin(GL_TRIANGLES);
    glVertex3dv(A1); glVertex3dv(B1); glVertex3dv(C1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(B1, C1, D1);
    glBegin(GL_TRIANGLES);
    glVertex3dv(B1); glVertex3dv(C1); glVertex3dv(D1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(A1, B1, H1);
    glBegin(GL_QUADS);
    glVertex3dv(A1); glVertex3dv(B1);
    glVertex3dv(H1); glVertex3dv(G1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(A1, C1, E1);
    glBegin(GL_QUADS);
    glVertex3dv(A1); glVertex3dv(C1);
    glVertex3dv(E1); glVertex3dv(F1);
    glEnd();

    //Walls
    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(G, H, H1);
    glBegin(GL_QUADS);
    glVertex3dv(G); glVertex3dv(H);
    glVertex3dv(H1); glVertex3dv(G1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(H, B, B1);
    glBegin(GL_QUADS);
    glVertex3dv(H); glVertex3dv(B);
    glVertex3dv(B1); glVertex3dv(H1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(B, D, D1);
    glBegin(GL_QUADS);
    glVertex3dv(B); glVertex3dv(D);
    glVertex3dv(D1); glVertex3dv(B1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(D, C, C1);
    glBegin(GL_QUADS);
    glVertex3dv(D); glVertex3dv(C);
    glVertex3dv(C1); glVertex3dv(D1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(C, E, E1);
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
    SetNormals(F, A, A1);
    glBegin(GL_QUADS);
    glVertex3dv(F); glVertex3dv(A);
    glVertex3dv(A1); glVertex3dv(F1);
    glEnd();

    glColor3d(r(gen), r(gen), r(gen));
    SetNormals(A, G, G1);
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

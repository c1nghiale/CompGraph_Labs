#include "OpenGL.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <format>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Prism();
void quarterCilinder1();
void quarterCilinder2();
void SetNormals(const double *v0, const double *v1, const double *v2, const double *objCenter);
void semiCircleArc();

static double default_center[3] = {0.0, 0.0, 0.0};

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

extern std::string data_folder;
extern OpenGL* ogl_object;

GLuint texId;
Camera camera;
Light light;

bool texture_mode = false;      // По умолчанию текстуры выключены
bool light_mode = true;
bool alpha_mode = false;

void SwitchMode(void* sender, const KeyEventArg &a)
{
    if (a.key == GLFW_KEY_T && a.action == GLFW_PRESS)
    {
        texture_mode = !texture_mode;
        std::cout << "Textures: " << (texture_mode ? "[ON] off " : " on [OFF]") << std::endl;
    }

    if (a.key == GLFW_KEY_L && a.action == GLFW_PRESS)
    {
        light_mode = !light_mode;
        std::cout << "Lightning: " << (light_mode ? "[ON] off " : " on [OFF]") << std::endl;
    }
    if (a.key == GLFW_KEY_A && a.action == GLFW_PRESS)
    {
        alpha_mode = !alpha_mode;
        std::cout << "Alpha blending: " << (alpha_mode ? "[ON] off " : " on [OFF]") << std::endl;
    }
}

void InitRender()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    int x, y, n;
    unsigned char* data = stbi_load(std::format("{}texture.png",data_folder).c_str(), &x, &y, &n, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << data_folder + "texture.png" << std::endl;
    } else {
        const size_t row_size = x * 4;
        for (int i = 0; i < y / 2; ++i) {
            unsigned char* row_i = data + i * row_size;
            unsigned char* row_j = data + (y - 1 - i) * row_size;
            std::swap_ranges(row_i, row_i + row_size, row_j);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    camera.caclulateCameraPos();
    ogl_object->mouseMoveEvent().reaction(&camera, &Camera::MouseMovie);
    ogl_object->wheelEvent().reaction(&camera, &Camera::Zoom);
    ogl_object->mouseButtonEvent().reaction(&camera, &Camera::MouseStartDrag);
    ogl_object->keyEvent().reaction(&SwitchMode);

    light.SetPosition(1, 10, 3);
    ogl_object->mouseMoveEvent().reaction(&light, &Light::MoveLight);
    ogl_object->keyEvent().reaction(&light, &Light::KeyPressed);

    std::cout << "Controls:" << std::endl;
    std::cout << "  L - Enable/disable lighting" << std::endl;
    std::cout << "  T - Enable/disable textures" << std::endl;
    std::cout << "  A - Enable/disable alpha blending" << std::endl;
    std::cout << "  G / G+LMB - Move light source with cursor horizontal/vertical" << std::endl;
    std::cout << "  F - Move light source to camera's position" << std::endl;
    std::cout << "  F(hold) - Move light source with camera" << std::endl;
}

void Render(double delta_time)
{
    if (ogl_object->isKeyPressed(GLFW_KEY_F)) {
        light.SetPosition(camera.x(), camera.y(), camera.z());
    }

    camera.ApplyCamera();
    ogl_object->drawAxisAndCell();
    light.ApplyLight();

    if (texture_mode) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    if (light_mode) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    } else {
        glDisable(GL_LIGHTING);
    }

    if (alpha_mode) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    glColor3d(0.9, 0.9, 0.9);
    glPushMatrix();
           glRotated(90.0, 1.0, 0.0, 0.0); // положить призму на бок
           Prism();
    glPopMatrix();

    light.DrawLightGizmo();
}

// Вспомогательная функция для отрисовки треугольника с автоматическим вычислением нормали
void DrawTriangle(const double* v0, const double* v1, const double* v2, const double* center) {
    SetNormals(v0, v1, v2, center);
    glVertex3dv(v0);
    glVertex3dv(v1);
    glVertex3dv(v2);
}

// Вспомогательная функция для отрисовки квада (разбивает на два треугольника)
void DrawQuad(const double* v0, const double* v1, const double* v2, const double* v3, const double* center) {
    DrawTriangle(v0, v1, v2, center);
    DrawTriangle(v0, v2, v3, center);
}

void Prism() {
    double A[] = {-1, 0.0001, 0}, B[] = {2, 0.0002, 0}, C[] = {0, 0.0003, 1}, D[] = {7, 0.0004, 5},
           E[] = {-5, 0.0005, 8}, F[] = {-8, 0.0006, 4}, G[] = {-3, 0.0007, -9}, H[] = {6, 0.0008, -7};

    double A1[] = {-1, 5.0001, 0}, B1[] = {2, 5.0002, 0}, C1[] = {0, 5.0003, 1}, D1[] = {7, 5.0004, 5},
           E1[] = {-5, 5.0005, 8}, F1[] = {-8, 5.0006, 4}, G1[] = {-3, 5.0007, -9}, H1[] = {6, 5.0008, -7};

    double prismCenter[] = {0.0, 2.5, 0.0};

    // Нижнее основание (два треугольника)
    glBegin(GL_TRIANGLES);
        DrawTriangle(A, B, C, prismCenter);
        DrawTriangle(B, C, D, prismCenter);
        DrawQuad(A, B, H, G, prismCenter);   // A-B-H и A-H-G
        DrawQuad(A, C, E, F, prismCenter);   // A-C-E и A-E-F
    glEnd();

    // Верхнее основание
    glBegin(GL_TRIANGLES);
        DrawTriangle(A1, B1, C1, prismCenter);
        DrawTriangle(B1, C1, D1, prismCenter);
        DrawQuad(A1, B1, H1, G1, prismCenter);
        DrawQuad(A1, C1, E1, F1, prismCenter);
    glEnd();

    // Боковые стены (квады, разбитые на треугольники)
    glBegin(GL_TRIANGLES);
        DrawQuad(G, H, H1, G1, prismCenter);
        DrawQuad(H, B, B1, H1, prismCenter);
        DrawQuad(B, D, D1, B1, prismCenter);
        DrawQuad(D, C, C1, D1, prismCenter);
        DrawQuad(C, E, E1, C1, prismCenter);
        DrawQuad(F, A, A1, F1, prismCenter);
        DrawQuad(A, G, G1, A1, prismCenter);
    glEnd();

    quarterCilinder1();
    quarterCilinder2();
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

        // Боковая поверхность (радиальная нормаль)
        double nx, nz, len;

        // Верхний треугольник боковины
        nx = x2 - cx; nz = z2 - cz;
        len = sqrt(nx*nx + nz*nz);
        if (len > 1e-9) { nx /= len; nz /= len; }
        glNormal3d(nx, 0.0, nz);
        glVertex3d(x2, By + height, z2);

        nx = x2n - cx; nz = z2n - cz;
        len = sqrt(nx*nx + nz*nz);
        if (len > 1e-9) { nx /= len; nz /= len; }
        glNormal3d(nx, 0.0, nz);
        glVertex3d(x2n, By + height, z2n);

        nx = x2 - cx; nz = z2 - cz;
        len = sqrt(nx*nx + nz*nz);
        if (len > 1e-9) { nx /= len; nz /= len; }
        glNormal3d(nx, 0.0, nz);
        glVertex3d(x2, By, z2);

        // Нижний треугольник боковины
        nx = x2n - cx; nz = z2n - cz;
        len = sqrt(nx*nx + nz*nz);
        if (len > 1e-9) { nx /= len; nz /= len; }
        glNormal3d(nx, 0.0, nz);
        glVertex3d(x2n, By + height, z2n);

        nx = x2n - cx; nz = z2n - cz;
        len = sqrt(nx*nx + nz*nz);
        if (len > 1e-9) { nx /= len; nz /= len; }
        glNormal3d(nx, 0.0, nz);
        glVertex3d(x2n, By, z2n);

        nx = x2 - cx; nz = z2 - cz;
        len = sqrt(nx*nx + nz*nz);
        if (len > 1e-9) { nx /= len; nz /= len; }
        glNormal3d(nx, 0.0, nz);
        glVertex3d(x2, By, z2);

        // Нижнее основание
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(x1, Ay, z1);
        glVertex3d(x2, By, z2);
        glVertex3d(x2n, By, z2n);

        // Верхнее основание
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(x1, Ay + height, z1);
        glVertex3d(x2n, By + height, z2n);
        glVertex3d(x2, By + height, z2);

        glEnd();
    }
}

void quarterCilinder2() {
    double Ax = -6.5, Ay = 5, Az = 6;
    double Bx = -5,   By = 5, Bz = 8;
    double height = -5;
    double cx = -6.5, cz = 6;

    double Ccx = -6.5, Ccy = 2.5, Ccz = 6;
    double prismCenter[] = {0.0, 2.5, 0.0};  // общий центр объекта

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

        // Грань вдоль стороны AB (нижнее основание)
        {
            double v0[] = {x1, Ay, z1};
            double v1[] = {x2, By, z2};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }
        {
            double v0[] = {x2, By, z2};
            double v1[] = {x2n, By, z2n};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }
        {
            double v0[] = {x2n, By, z2n};
            double v1[] = {x1n, Ay, z1n};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }
        {
            double v0[] = {x1n, Ay, z1n};
            double v1[] = {x1, Ay, z1};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }

        // Грань вдоль стороны с высотой (внутренняя боковая)
        {
            double v0[] = {x2, By, z2};
            double v1[] = {x2, By + height, z2};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }
        {
            double v0[] = {x2, By + height, z2};
            double v1[] = {x2n, By + height, z2n};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }
        {
            double v0[] = {x2n, By + height, z2n};
            double v1[] = {x2n, By, z2n};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }

        // Грань от вершины A до верхней точки
        {
            double v0[] = {x1, Ay, z1};
            double v1[] = {x2, By + height, z2};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }
        {
            double v0[] = {x2, By + height, z2};
            double v1[] = {x2n, By + height, z2n};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }
        {
            double v0[] = {x2n, By + height, z2n};
            double v1[] = {x1n, Ay, z1n};
            double v2[] = {Ccx, Ccy, Ccz};
            SetNormals(v0, v1, v2, prismCenter);
            glVertex3dv(v0); glVertex3dv(v1); glVertex3dv(v2);
        }

        glEnd();
    }
}

void SetNormals(const double *v0, const double *v1, const double *v2, const double * /*objCenter*/) {
    // objCenter больше не используется – разворот нормали отключён.
    double ax = v1[0] - v0[0], ay = v1[1] - v0[1], az = v1[2] - v0[2];
    double bx = v2[0] - v0[0], by = v2[1] - v0[1], bz = v2[2] - v0[2];

    // Векторное произведение (порядок: v0→v1 × v0→v2)
    double nx = ay * bz - az * by;
    double ny = az * bx - ax * bz;
    double nz = ax * by - ay * bx;

    double len = sqrt(nx * nx + ny * ny + nz * nz);
    if (len > 1e-9) {
        nx /= len;
        ny /= len;
        nz /= len;
        glNormal3d(nx, ny, nz);
    } else {
        // Для вырожденного треугольника задаём заглушку
        glNormal3d(0.0, 1.0, 0.0);
    }
}

void semiCircleArc() {
    double P0x = -8, P0y = 0, P0z = 4;
    double Cx = -6.5, Cy = 0, Cz = 6;
    double P2x = -5, P2y = 0, P2z = 8;

    double radius = sqrt((P0x - Cx)*(P0x - Cx) + (P0z - Cz)*(P0z - Cz));
    double angleStart = atan2(P0z - Cz, P0x - Cx);
    int steps = 100;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= steps; i++) {
        double t = (double)i / steps;
        double angle = angleStart - t * M_PI;
        double x = Cx + radius * cos(angle);
        double y = Cy;
        double z = Cz + radius * sin(angle);
        glVertex3d(x, y, z);
    }
    glEnd();
}

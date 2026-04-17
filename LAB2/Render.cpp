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
void Cilinder();
void SetNormals(const double *v0, const double *v1, const double *v2, const double *objCenter);

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

    light.SetPosition(1, 15, 3);
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
           Cilinder();
    glPopMatrix();



    light.DrawLightGizmo();
}

void DrawTriangle(const double* v0, const double* v1, const double* v2, const double* center) {
    SetNormals(v0, v1, v2, center);
    glVertex3dv(v0);
    glVertex3dv(v1);
    glVertex3dv(v2);
}

void DrawQuad(const double* v0, const double* v1, const double* v2, const double* v3, const double* center) {
    DrawTriangle(v0, v1, v2, center);
    DrawTriangle(v0, v2, v3, center);
}

void SetNormals(const double *v0, const double *v1, const double *v2, const double * /*objCenter*/) {
    double ax = v1[0] - v0[0], ay = v1[1] - v0[1], az = v1[2] - v0[2];
    double bx = v2[0] - v0[0], by = v2[1] - v0[1], bz = v2[2] - v0[2];

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
        glNormal3d(0.0, 1.0, 0.0);
    }
}

void Prism() {
    double A[] = {-1, 0.0001, 0}, B[] = {2, 0.0002, 0}, C[] = {0, 0.0003, 1}, D[] = {7, 0.0004, 5};
    double E[] = {-5, 0.0005, 8}, F[] = {-8, 0.0006, 4}, G[] = {-3, 0.0007, -9}, H[] = {6, 0.0008, -7};

    double A1[] = {-1, 5.0001, 0}, B1[] = {2, 5.0002, 0}, C1[] = {0, 5.0003, 1}, D1[] = {7, 5.0004, 5};
    double E1[] = {-5, 5.0005, 8}, F1[] = {-8, 5.0006, 4}, G1[] = {-3, 5.0007, -9}, H1[] = {6, 5.0008, -7};

    double prismCenter[] = {0.0, 2.5, 0.0};

    // Нижнее основание
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

    // Боковые стены
    glBegin(GL_TRIANGLES);
        DrawQuad(G, H, H1, G1, prismCenter);
        DrawQuad(H, B, B1, H1, prismCenter);
        DrawQuad(B, D, D1, B1, prismCenter);
        DrawQuad(D, C, C1, D1, prismCenter);
        DrawQuad(C, E, E1, C1, prismCenter);
        DrawQuad(F, A, A1, F1, prismCenter);
        DrawQuad(A, G, G1, A1, prismCenter);
    glEnd();
}

struct CilinderParams {
    double Ax = -6.5, Ay = 0, Az = 6;
    double height = 5.0005;
    double realRadius = 2.5;
    double startAngle, endAngle, delta;
    int steps = 180;
};

void DrawCilinderSides(const CilinderParams& p) {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < p.steps; ++i) {
        double a1 = p.startAngle + p.delta * i;
        double a2 = p.startAngle + p.delta * (i + 1);
        if (i == p.steps - 1) a2 = p.endAngle;

        double x1 = p.realRadius * cos(a1) + p.Ax;
        double z1 = p.realRadius * sin(a1) + p.Az;
        double x2 = p.realRadius * cos(a2) + p.Ax;
        double z2 = p.realRadius * sin(a2) + p.Az;

        double p1_low[] = {x1, p.Ay, z1};
        double p2_low[] = {x2, p.Ay, z2};
        double p1_up[]  = {x1, p.Ay + p.height, z1};
        double p2_up[]  = {x2, p.Ay + p.height, z2};

        SetNormals(p1_low, p2_low, p1_up, nullptr);
        glVertex3dv(p1_low); glVertex3dv(p2_low); glVertex3dv(p1_up);

        SetNormals(p2_low, p2_up, p1_up, nullptr);
        glVertex3dv(p2_low); glVertex3dv(p2_up); glVertex3dv(p1_up);
    }
    glEnd();
}

void DrawCilinderCap(const CilinderParams& p, bool isTop) {
    double currentY = isTop ? (p.Ay + p.height) : p.Ay;
    double normalY = isTop ? 1.0 : -1.0;

    glNormal3d(0.0, normalY, 0.0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3d(p.Ax, currentY, p.Az); // Центр
        for (int i = 0; i <= p.steps; ++i) {
            double a = p.startAngle + p.delta * i;
            if (i == p.steps) a = p.endAngle;

            double x = p.realRadius * cos(a) + p.Ax;
            double z = p.realRadius * sin(a) + p.Az;
            glVertex3d(x, currentY, z);
        }
    glEnd();
}

void Cilinder() {
    CilinderParams p;

    double x_start = -5.0, z_start = 8.0;
    double x_end = -7.25, z_end = 5.0;

    p.startAngle = atan2(z_start - p.Az, x_start - p.Ax);
    p.endAngle   = atan2(z_end - p.Az, x_end - p.Ax);

    if (p.endAngle < p.startAngle) p.endAngle += 2.0 * M_PI;
    p.delta = (p.endAngle - p.startAngle) / double(p.steps);

    DrawCilinderSides(p);     // Стенки
    DrawCilinderCap(p, false); // Низ
    DrawCilinderCap(p, true);  // Верх
}

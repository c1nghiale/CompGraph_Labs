#include "Camera.h"
#include <algorithm>

#include "OpenGL.h"
#include <cmath>

#include <stdio.h>


void Camera::setPosition(double x, double y, double z)
{
    m_camX = x;
    m_camY = y;
    m_camZ = z;

    m_camDist = sqrt(x * x + y * y + z * z);

    _fi1 = atan2(y, x);
    _fi2 = atan2(z, sqrt(x * x + y * y));
}

void Camera::caclulateCameraPos()
{
    m_camX = m_camDist * cos(_fi2) * cos(_fi1);
    m_camY = m_camDist * cos(_fi2) * sin(_fi1);
    m_camZ = m_camDist * sin(_fi2);
    if (cos(_fi2) <= 0)
        m_camNz = -1;
    else
        m_camNz = 1;
}

void Camera::Zoom(OpenGL* sender, const MouseWheelEventArg &arg)
{

    m_camDist = std::clamp(m_camDist + 0.03 * arg.value * m_camDist, 0.3, 200.0);

    caclulateCameraPos();

}

void Camera::MouseMovie(OpenGL* sender, const MouseEventArg &arg)
{
    if (sender->isKeyPressed(GLFW_KEY_G))
        return;

    if (m_mouseX == 0 && m_drag)
    {
        m_mouseX = arg.x;
        m_mouseY = arg.y;
        return;
    }


    int dx = m_mouseX - arg.x;
    int dy = m_mouseY - arg.y;
    m_mouseX = arg.x;
    m_mouseY = arg.y;

    if (m_drag)
    {
        _fi1 = _fi1 + 0.01 * dx;
        _fi2 = _fi2 - 0.01 * dy;

        caclulateCameraPos();
    }
       
}

void Camera::ApplyCamera()
{
    // Сообщаем OpenGL настройки нашей камеры,
    // где она находится и куда смотрит
    // https://learn.microsoft.com/ru-ru/windows/win32/opengl/glulookat
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_camX, m_camY, m_camZ, 0, 0, 0, 0, 0, m_camNz);
}

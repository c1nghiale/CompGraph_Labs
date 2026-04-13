#pragma once

#include "OpenGL.h"

class Camera
{
    double m_camDist = 10;

    int m_camNz = 1;

    double m_camX;
    double m_camY;
    double m_camZ;
    int m_mouseX = -1, m_mouseY = -1;

    bool m_drag = false;


  public:
    // Начальные углы камеры
    double _fi1 = 1;
    double _fi2 = 0.5;

    Camera()
    {
        caclulateCameraPos();
    }

    void setPosition(double x, double y, double z);

    double distance()
    {
        return m_camDist;
    }

    int nZ() const
    {
        return m_camNz;
    }
    double x() const
    {
        return m_camX;
    }
    double y() const
    {
        return m_camY;
    }
    double z() const
    {
        return m_camZ;
    }
    double fi1() const
    {
        return _fi1;
    }
    double fi2() const
    {
        return _fi2;
    }

    void caclulateCameraPos();
    void Zoom(OpenGL* sender, const MouseWheelEventArg &arg);
    void MouseMovie(OpenGL* sender, const MouseEventArg &arg);

    void MouseLeave(OpenGL* sender, MouseEventArg arg)
    {
        m_mouseX = 0;
    }

    void MouseStartDrag(OpenGL* sender, const MouseButtonEventArg &arg)
    {
        if (arg.button == GLFW_MOUSE_BUTTON_LEFT && arg.action == GLFW_PRESS)
            m_drag = true;
        if (arg.button == GLFW_MOUSE_BUTTON_LEFT && arg.action == GLFW_RELEASE)
        {
            m_drag = false;
            m_mouseX = 0;
        }
    }



    void ApplyCamera();
};

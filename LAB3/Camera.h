#pragma once

#include "OpenGL.h"

class Camera
{
    double camDist = 10;

    int camNz = 1;

    double camX;
    double camY;
    double camZ;
    int mouseX = -1, mouseY = -1;

    bool drag = false;

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
        return camDist;
    }

    int nZ() const
    {
        return camNz;
    }
    double x() const
    {
        return camX;
    }
    double y() const
    {
        return camY;
    }
    double z() const
    {
        return camZ;
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
        mouseX = 0;
    }

    void MouseStartDrag(OpenGL* sender, const MouseButtonEventArg &arg)
    {
        if (arg.button == GLFW_MOUSE_BUTTON_LEFT && arg.action == GLFW_PRESS)
            drag = true;
        if (arg.button == GLFW_MOUSE_BUTTON_LEFT && arg.action == GLFW_RELEASE)
        {
            drag = false;
            mouseX = 0;
        }
    }



    void SetUpCamera();
};

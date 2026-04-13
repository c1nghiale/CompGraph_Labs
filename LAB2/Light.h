#pragma once

#include "OpenGL.h"

class Light
{
    GLdouble m_posX = 1;
    GLdouble m_posY = 1;
    GLdouble m_posZ = 1;

    bool m_drag = false;

  public:
    
    double x() const
    {
        return m_posX;
    }
    double y() const
    {
        return m_posY;
    }
    double z() const
    {
        return m_posZ;
    }

    void SetPosition(GLdouble x, GLdouble y, GLdouble z);

    void KeyPressed(OpenGL* sender, const KeyEventArg &arg);

    void MoveLight(OpenGL* sender, const MouseEventArg &arg);

    void ApplyLight();

    void DrawLightGizmo();
};

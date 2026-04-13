#include "Light.h"

#include <algorithm>
#include <tuple>
#include <Vector3.h>

extern OpenGL* ogl_object;

std::tuple<GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble> getLookRay(int wndX, int wndY)
{
    GLint viewport[4];       // Параметры viewport-a.
    GLdouble projection[16]; // Матрица проекции.
    GLdouble modelview[16];  // Видовая матрица.
    GLdouble wx, wy, wz;     // Возвращаемые мировые координаты.

    glGetIntegerv(GL_VIEWPORT, viewport);           // Узнаём параметры viewport-a.
    glGetDoublev(GL_PROJECTION_MATRIX, projection); // Узнаём матрицу проекции.
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // Узнаём видовую матрицу.

    // Переводим оконные координаты курсора в систему координат viewport-a.

    double originX, originY, originZ;          // Точка в 3D мире под мышью
    double directionX, directionY, directionZ; // Направление клика

    // Обратное проекцирование 2d->3d (wndX, wndY, 0) -> (wx,wy,wz)   0 - глубина внутрь экрана
    gluUnProject(wndX, wndY, 0, modelview, projection, viewport, &wx, &wy, &wz);
    originX = wx;
    originY = wy;
    originZ = wz;

    // Обратное проекцирование 2d->3d (wndX, wndY, 1) -> (wx,wy,wz)   1 - глубина внутрь экрана
    gluUnProject(wndX, wndY, 1, modelview, projection, viewport, &wx, &wy, &wz);
    directionX = wx;
    directionY = wy;
    directionZ = wz;

    directionX -= originX;
    directionY -= originY;
    directionZ -= originZ;

    double length = sqrt(directionX * directionX + directionY * directionY + directionZ * directionZ);

    directionX /= length;
    directionY /= length;
    directionZ /= length;

    return {originX, originY, originZ, directionX, directionY, directionZ};
}

void Light::SetPosition(GLdouble x, GLdouble y, GLdouble z)
{
    m_posX = x;
    m_posY = y;
    m_posZ = z;
}


void Light::KeyPressed(OpenGL* sender, const KeyEventArg& arg)
{
    if (arg.key == GLFW_KEY_G && arg.action == GLFW_PRESS)
        m_drag = true;
    if (arg.key == GLFW_KEY_G && arg.action == GLFW_RELEASE)
        m_drag = false;
}

void Light::MoveLight(OpenGL* sender, const MouseEventArg &arg)
{
    // Двигаем свет по плоскости

    

    if (ogl_object->isKeyPressed(GLFW_KEY_G))
    {
        int _x = arg.x;
        int _y = ogl_object->fb_height() - arg.y;

        auto [oX, oY, oZ, dX, dY, dZ] = getLookRay(_x, _y);

        if (!ogl_object->isKeyPressed(GLFW_MOUSE_BUTTON_LEFT)) // Если не нажата левая кнопка мыши
        {
            GLdouble z = m_posZ;

            GLdouble k = 0, x = 0, y = 0;
            if (dZ == 0)
                k = 0;
            else
                k = (z - oZ) / dZ;

            x = k * dX + oX;
            y = k * dY + oY;

            if (x * x + y * y > 2500) // Ограничение максимальной дистанции
                return;

            m_posX = x;
            m_posY = y;
            m_posZ = z;
        }
        else // Если нажата ЛКМ
        {
            Vector3 o{oX, oY, oZ};
            Vector3 d{dX, dY, dZ};
            Vector3 z{0, 0, 1};

            Vector3 _top = d ^ Vector3(0, 0, 1) ^ d;

            // Уравнение плоскости Ax+By+Cz+D=0  _top = (A, B, C)

            // Ищем D
            GLdouble D = -_top.x() * oX - _top.y() * oY - _top.z() * oZ;

            // Ищем новую координату Z света
            if (_top.z() == 0)
                m_posZ = 0;
            else
                m_posZ = std::clamp(-(_top.x() * m_posX + _top.y() * m_posY + D) / _top.z(), -20.0, 20.0);
        }
    }
}

void Light::ApplyLight()
{
    // Характеристики излучаемого света

    // Фоновое освещение (рассеянный свет)
    GLfloat lamb[] = {0.2f, 0.2f, 0.2f, 0.0f};
    // Диффузная составляющая света
    GLfloat ldif[] = {0.7f, 0.7f, 0.7f, 0.0f};
    // Зеркально отражаемая составляющая света
    GLfloat lspec[] = {1.0f, 1.0f, 1.0f, 0.0f};
    // Координаты
    GLfloat lposition[] = {(GLfloat)m_posX, (GLfloat)m_posY, (GLfloat)m_posZ, 1.f};

    // Сообщаем эти значения OpenGL
    glLightfv(GL_LIGHT0, GL_POSITION, lposition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ldif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);
    glEnable(GL_LIGHT0);
}

void Light::DrawLightGizmo()
{
    // Рисуем точку, откуда идет свет

    // Устанавливаем размер точки
    GLfloat pointSize;
    glGetFloatv(GL_POINT_SIZE, &pointSize);
    glPointSize(10);

    GLboolean texture_mode, depth_mode, lightning_mode;

    glGetBooleanv(GL_DEPTH_TEST, &depth_mode);
    glGetBooleanv(GL_TEXTURE_2D, &texture_mode);
    glGetBooleanv(GL_LIGHTING, &lightning_mode);

    

    // Отключаем тест глубины, чтобы точка рисовалась сквозь все
    glDisable(GL_DEPTH_TEST);

    // Отключаем свет и текстуры
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Рисуем точку
    glBegin(GL_POINTS);
    glColor3d(1, 0.7, 0.1);
    glVertex3d(m_posX, m_posY, m_posZ);
    glEnd();

    // Восстанавливаем предыдущий размер точки
    glPointSize(pointSize);

    //  Если свет в режиме перетаскивания - рисуем вспомгательные линии  
    if (m_drag)
    {
        GLfloat lineWidth;
        glGetFloatv(GL_LINE_WIDTH, &lineWidth);

        glLineWidth(3.0);

        glBegin(GL_LINES);
        glColor3d(0, 0, 0.8);
        glVertex3d(m_posX, m_posY, m_posZ);
        glVertex3d(m_posX, m_posY, 0);

        glColor3d(0.8, 0, 0);
        glVertex3d(m_posX - 1, m_posY, 0);
        glVertex3d(m_posX + 1, m_posY, 0);

        glColor3d(0, 0.8, 0);
        glVertex3d(m_posX, m_posY - 1, 0);
        glVertex3d(m_posX, m_posY + 1, 0);

        glEnd();

        glLineWidth(lineWidth);
    }

    if (depth_mode) glEnable(GL_DEPTH_TEST);
    if (texture_mode) glEnable(GL_TEXTURE_2D);
    if (lightning_mode) glEnable(GL_LIGHTING);

}

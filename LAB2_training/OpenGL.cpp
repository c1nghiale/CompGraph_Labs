
#include <stdexcept>
#include "OpenGL.h"
#include "Camera.h"

//пользовательская функция рендера

void Render(double);
void InitRender();

void OpenGL::window_size_callback(GLFWwindow* window, int width, int height)
{
    OpenGL* ogl = static_cast<OpenGL*>(glfwGetWindowUserPointer(window));

    if (!ogl)
        throw std::runtime_error("UserPointer ig glwfWindow updefined!");

    ogl->m_window_width = width;
    ogl->m_window_height = height;
    ogl->resize();

}

void OpenGL::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    OpenGL* ogl = static_cast<OpenGL*>(glfwGetWindowUserPointer(window));

    if (!ogl)
        throw std::runtime_error("UserPointer ig glwfWindow updefined!");

    MouseEventArg a{ a.x = (int)(xpos / (ogl->m_window_width-1) * ogl->m_framebuffer_width), 
        a.y = (int) (ypos / (ogl->m_window_height-1) * ogl->m_framebuffer_height )};

    ogl->m_mouseMoveEvent.exec(ogl, a);
}

void OpenGL::scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
    OpenGL* ogl = static_cast<OpenGL*>(glfwGetWindowUserPointer(window));

    if (!ogl)
        throw std::runtime_error("UserPointer ig glwfWindow updefined!");
    MouseWheelEventArg a{ a.value = ypos };
    ogl->wheelEvent().exec(ogl, a);
}

void OpenGL::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    OpenGL* ogl = static_cast<OpenGL*>(glfwGetWindowUserPointer(window));

    if (!ogl)
        throw std::runtime_error("UserPointer ig glwfWindow updefined!");

    MouseButtonEventArg a{ a.button = button, a.action = action };
    ogl->mouseButtonEvent().exec(ogl, a);
}

void OpenGL::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    OpenGL* ogl = static_cast<OpenGL*>(glfwGetWindowUserPointer(window));

    if (!ogl)
        throw std::runtime_error("UserPointer ig glwfWindow updefined!");
    
    KeyEventArg a{ a.key = key, a.action = action };
    ogl->keyEvent().exec(ogl, a);
}

void OpenGL::initWindow()
{
    
    if (!glfwInit())
        throw (std::runtime_error("Error in GLWF initialuzation"));

    m_window_height = 720;
    m_window_width = 1280;

    //фикс размеров под ретины на маках
    glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, true);
    
    m_window = glfwCreateWindow(m_window_width, m_window_height, "Компьютерная графика 2", NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        throw (std::runtime_error("Error in GLWF window creation"));
    }

    

    glfwSetWindowUserPointer(m_window, this); //сохраняем связь текущено класса с glwf окном.

    glfwSetWindowSizeCallback(m_window, window_size_callback);

    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetKeyCallback(m_window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);
    resize();
}



void OpenGL::PreRender()
{
    glClearColor(0.7, 0.7, 0.7, 0);
    InitRender();
    glEnable(GL_DEPTH_TEST);
}


void OpenGL::Render()
{
    PreRender();
    m_old_time = 0;
    

    while (!glfwWindowShouldClose(m_window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                
        glColor3d(0,0,0);
        

        auto time = glfwGetTime();

        ::Render(time - m_old_time );

        m_old_time = time;

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void OpenGL::drawAxisAndCell()
{
    glEnable(GL_SMOOTH);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);



    GLfloat line_width;
    glGetFloatv(GL_LINE_WIDTH, &line_width);

    glColor3d(0.2, 0.2, 0.2);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (int i = -10; i <= 10; ++i)
    {
        glVertex2d(-10, i);
        glVertex2d(10, i);
        glVertex2d(i, -10);
        glVertex2d(i, 10);
    }
    glEnd();

    glLineWidth(3);

    glColor3d(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(10, 0, 0);
    glEnd();

    glColor3d(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 10, 0);
    glEnd();

    glColor3d(0, 0, 1);
    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 10);
    glEnd();

    glLineWidth(line_width);

}

void OpenGL::resize()
{
    glfwGetFramebufferSize(m_window, &m_framebuffer_width, &m_framebuffer_height);

    glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (GLdouble)m_window_width / (GLdouble)m_window_height, 0.2, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}




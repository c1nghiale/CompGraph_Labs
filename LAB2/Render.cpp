#include "OpenGL.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <format>
#include <iostream>
#include <algorithm>

// Библиотека для разгрузки изображений
// https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//объявлена в main.cpp, папка data где расположенны загружаемые данные программы.
extern std::string data_folder; 

//объект, в котором инкапсулирована работа с GLWF и Open GL, создается в main.
extern OpenGL* ogl_object; 

// ID для текстуры
GLuint texId;

Camera camera;
Light light;


bool texture_mode = true;
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

    //==============НАСТРОЙКА ТЕКСТУР================
    // 4 байта на хранение пикселя
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // Генерируем ID текстуры
    glGenTextures(1, &texId);

    // Делаем текущую текстуру активной
    glBindTexture(GL_TEXTURE_2D, texId);



    int x, y, n;

    // Загружаем картинку
    // см. #include "stb_image.h"

        // x - ширина изображения
        // y - высота изображения
        // n - количество каналов
        // 4 - нужное нам количество каналов
        // Пиксели будут хранится в памяти [R-G-B-A]-[R-G-B-A]-[.....
        //  по 4 байта на пиксель - по байту на канал
        // Пустые каналы будут равны 255
    unsigned char* data = stbi_load( std::format("{}texture.png",data_folder).c_str(), &x, &y, &n, 4);
    if (!data) {
        // Обработка ошибки загрузки текстуры
        std::cerr << "Failed to load texture: " << data_folder + "texture.png" << std::endl;
    }
    else
    {

        // Картинка хранится в памяти перевернутой
        // так как ее начало в левом верхнем углу;
        // по этому мы ее переворачиваем -
        // меняем первую строку с последней,
        // вторую с предпоследней, и.т.д.

        const size_t row_size = x * 4;
        for (int i = 0; i < y / 2; ++i)
        {
            unsigned char* row_i = data + i * row_size;
            unsigned char* row_j = data + (y - 1 - i) * row_size;
            std::swap_ranges(row_i, row_i + row_size, row_j);
        }

        // Загрузка изображения в видеопамять
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Выгрузка изображения из оперативной памяти
        stbi_image_free(data);

        // Настройка режима наложения текстур
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        // GL_REPLACE -- полная замена политога текстурой
        // Настройка тайлинга
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Настройка фильтрации
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    //======================================================

    //================НАСТРОЙКА КАМЕРЫ======================
    camera.caclulateCameraPos();
    ogl_object->mouseMoveEvent().reaction(&camera, &Camera::MouseMovie);
    ogl_object->wheelEvent().reaction(&camera, &Camera::Zoom);
    ogl_object->mouseButtonEvent().reaction(&camera, &Camera::MouseStartDrag);

    //==========================

    ogl_object->keyEvent().reaction(&SwitchMode);


    //======== Настройка источника света
    light.SetPosition(1, 1, 3);
    ogl_object->mouseMoveEvent().reaction(&light, &Light::MoveLight);
    ogl_object->keyEvent().reaction(&light, &Light::KeyPressed);


    std::cout << "Controls:" << std::endl;
    std::cout << "  L - Enable/disable ligtning" << std::endl;
    std::cout << "  T - Enable/disable textures" << std::endl;
    std::cout << "  A - Enable/disable alpha blending" << std::endl;
    std::cout << "  G / G+LMB - Move light source with cursor horizontal/vertical" << std::endl;
    std::cout << "  F - Move light source to camera's position" << std::endl;
    std::cout << "  F(hold) - Move light source with camera" << std::endl;

}

void Render(double delta_time)
{
    if (ogl_object->isKeyPressed(GLFW_KEY_F))
    {
        light.SetPosition(camera.x(), camera.y(), camera.z());
    }
    
    camera.ApplyCamera(); //в старом шаблоне SetUpCamera


    ogl_object->drawAxisAndCell(); //оси и сетка
    
    light.ApplyLight(); //в старом шаблоне SetUpLight
    

    if (texture_mode)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0); // Сбрасываем текущую текстуру
    }        
    else
        glDisable(GL_TEXTURE_2D);

    if (light_mode)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    if (alpha_mode)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }


    //Програть тут


    //Рисуем квадратик станкина

    //Биндим текстуру
    glBindTexture(GL_TEXTURE_2D,texId);

    glColor3d(1, 1, 1);
    
    glBegin(GL_QUADS);

    //задаем нормаль 
    glNormal3d(0, 0, 1);

    glTexCoord2d(0, 0);
    glVertex2d(-1, -1);

    glTexCoord2d(1, 0);
    glVertex2d(1, -1);

    glTexCoord2d(1, 1);
    glVertex2d(1, 1);

    glTexCoord2d(0, 1);
    glVertex2d(-1, 1);

    glEnd();



    light.DrawLightGizmo(); //рисуем свет. так как он рисуется поверх всего - идет последним.

}
#include "OpenGL.h"
#include "Camera.h"

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
    
    //программировать тут    

}
cmake_minimum_required(VERSION 3.11)
project(kg_1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(glfw)

# Поиск OpenGL и GLU
find_package(OpenGL REQUIRED)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_executable(kg_lab
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(kg_lab PRIVATE
    glfw
    OpenGL::GL
    OpenGL::GLU
)

target_include_directories(kg_lab PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Копирование dll на Windows
if(WIN32)
    add_custom_command(TARGET kg_lab POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:kg_lab>
        $<TARGET_FILE_DIR:kg_lab>
        COMMAND_EXPAND_LISTS
    )
endif()

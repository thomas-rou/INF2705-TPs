
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"
#include "camera.h"


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

std::string readFile(const char* path);

int main(int argc, char* argv[])
{
    Window w;
    if (!w.init())
        return -1;

    GLenum rev = glewInit();
    if ( rev != GLEW_OK )
    {
        std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
        return -2;
    }

    printGLInfo();

    ShaderProgram transform;
    GLint mvpLocation;
    {
        std::string vertexCode   = readFile("shaders/transform.vs.glsl");
        std::string fragmentCode = readFile("shaders/transform.fs.glsl");

        Shader vertex(GL_VERTEX_SHADER, vertexCode.c_str());
        Shader fragment(GL_FRAGMENT_SHADER, fragmentCode.c_str());
        transform.attachShader(vertex);
        transform.attachShader(fragment);
        transform.link();
        mvpLocation = transform.getUniformLoc("mvp");
    }

    BasicShapeElements cubeElements(cubeVertices, sizeof(cubeVertices), cubeIndexes, sizeof(cubeIndexes));
    cubeElements.enableAttribute(0, 3, 6, 0);
    cubeElements.enableAttribute(1, 3, 6, 3);

    int selectShape = 0;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST); // a ajouter avec les transformations

    // Instanciation camera
    glm::vec3 pos(0, 0, 0);
    glm::vec2 ori(0, 0);
    Camera camera(pos, ori);
    // définition de var pour orientation de la caméra
    int mouseX, mouseY;

    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT a ajouter avec les transformations

        if (w.getKeyPress(Window::Key::T))
        {
            selectShape = ++selectShape < 7 ? selectShape : 0;
            std::cout << "Selected shape: " << selectShape << std::endl;
        }

        // Changement de la position de la caméra
        if (w.getKeyHold(Window::Key::W)){
            pos.z -= 0.05f;
        }
        if (w.getKeyHold(Window::Key::S)){
            pos.z += 0.05f;
        }
        if (w.getKeyHold(Window::Key::A)){
            pos.x -= 0.05f;
        }
        if (w.getKeyHold(Window::Key::D)){
            pos.x += 0.05f;
        }

        // Changement de l'orientation de la caméra
        w.getMouseMotion(mouseX, mouseY);
        ori.x += mouseX * 0.01f;
        ori.y += mouseY * 0.01f;

        // Utilisation shader
        transform.use();

        static float angleDeg = 0.0f;
        angleDeg += 0.5f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.1, 1, 0.1));
        // assigner la position de la caméra et l'orientation, selon le type de vue
        glm::mat4 view = camera.getFirstPersonViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth()/(float)w.getHeight(), 0.1f, 10.0f);
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

        // Toujours dessiner le cube
        cubeElements.draw(GL_TRIANGLES, 36);

        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
    }

    return 0;
}



void checkGLError(const char* file, int line)
{
    GLenum error;
    while ( ( error = glGetError() ) != GL_NO_ERROR )
    {
        std::cerr << "GL_ERROR, File " << file << " (Line " << line << "): ";
        switch ( error )
        {
        case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            std::cerr << "GL_STACK_OVERFLOW";
            break;
        default:
            std::cerr << "Unknown gl error occured!";
            break;
        }
        std::cerr << std::endl;
    }
}

void printGLInfo()
{
    std::cout << "OpenGL info:" << std::endl;
    std::cout << "    Vendor: "   << glGetString(GL_VENDOR)   << std::endl;
    std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: "  << glGetString(GL_VERSION)  << std::endl;
    std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
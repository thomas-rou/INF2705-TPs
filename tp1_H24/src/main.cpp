
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


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

void changeRGB(GLfloat* color);
void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy);

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

    // SDL_GL_SetSwapInterval(1);
    // GLint maxSamples; // Declare maxSamples variable
    // glGetIntegerv(GL_MAX_SAMPLES, &maxSamples); // Call glGetIntegerv function
    // std::cout << "Max samples: " << maxSamples << std::endl;


    // TODO Partie 1: Instancier les shader programs ici.
    ShaderProgram basicShaderProgram;
    { // Les accolades vont permettre de détruire le code des shaders plus rapidement
        // Vous devez lire le code des shaders dans "shaders/"
        // avec la fonction "readFile".
        std::string basicVertexShaderSource = readFile("shaders/basic.vs.glsl");
        std::string basicFragmentShaderSource = readFile("shaders/basic.fs.glsl");
        // Vous pouvez par la suite instancier vos shaders, les attacher et les lier
        // au programme.
        auto src = basicVertexShaderSource.c_str();
        Shader vertexShader(GL_VERTEX_SHADER, src);
        basicShaderProgram.attachShader(vertexShader);
        src = basicFragmentShaderSource.c_str();
        Shader fragmentShader(GL_FRAGMENT_SHADER, src);
        basicShaderProgram.attachShader(fragmentShader);
        basicShaderProgram.link();
    }

    ShaderProgram colorShaderProgram;
    {
        std::string colorVertexShaderSource = readFile("shaders/color.vs.glsl");
        std::string colorFragmentShaderSource = readFile("shaders/color.fs.glsl");
        auto src = colorVertexShaderSource.c_str();
        Shader vertexShader(GL_VERTEX_SHADER, src);
        colorShaderProgram.attachShader(vertexShader);
        src = colorFragmentShaderSource.c_str();
        Shader fragmentShader(GL_FRAGMENT_SHADER, src);
        colorShaderProgram.attachShader(fragmentShader);
        colorShaderProgram.link();
    }

    // TODO Partie 2: Shader program de transformation.
    // ... transform;
    // ... location;
    {
        // ...
    }

    // Variables pour la mise à jour, ne pas modifier.
    float cx = 0, cy = 0;
    float dx = 0.019;
    float dy = 0.0128;

    float angleDeg = 0.0f;

    GLfloat brightRed[] = {1.0f, 0.2f, 0.2f, 1.0f};
    GLfloat brightGreen[] = {0.2f, 1.0f, 0.2f, 1.0f};
    GLfloat brightBlue[] = {0.2f, 0.2f, 1.0f, 1.0f};
    GLfloat brightYellow[] = {1.0f, 1.0f, 0.2f, 1.0f};


    // Tableau non constant de la couleur
    GLfloat onlyColorTriVertices[] = {
        // TODO Partie 1: Rempliser adéquatement le tableau.
        // Vous pouvez expérimenter avec une couleur uniforme
        // de votre choix ou plusieurs différentes en chaque points.
        brightRed[0], brightRed[1], brightRed[2], brightRed[3],
        brightGreen[0], brightGreen[1], brightGreen[2], brightGreen[3],
        brightBlue[0], brightBlue[1], brightBlue[2], brightBlue[3],
    };

    // TODO Partie 1: Instancier vos formes ici.
    BasicShapeArrays onlyColorTri(triVertices, sizeof(triVertices));
    onlyColorTri.enableAttribute(0, 3, 3*sizeof(GLfloat), 0);

    BasicShapeArrays onlyColorSquare(squareVertices, sizeof(squareVertices));
    onlyColorSquare.enableAttribute(0, 3, 3*sizeof(GLfloat), 0);

    BasicShapeArrays coloredtri(colorTriVertices, sizeof(colorTriVertices));
    coloredtri.enableAttribute(0, 3, 6*sizeof(GLfloat), 0);
    coloredtri.enableAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    BasicShapeArrays coloredSquare(colorSquareVertices, sizeof(colorSquareVertices));
    coloredSquare.enableAttribute(0, 3, 6*sizeof(GLfloat), 0);
    coloredSquare.enableAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    BasicShapeMultipleArrays multColoredTri(colorTriVertices, sizeof(colorTriVertices), colorTriVertices, sizeof(colorTriVertices));
    multColoredTri.enablePosAttribute(0, 3, 6*sizeof(GLfloat), 0);
    multColoredTri.enableColorAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    BasicShapeElements elementsSquare(colorSquareVerticesReduced, sizeof(colorSquareVerticesReduced), indexes, sizeof(indexes));
    elementsSquare.enableAttribute(0, 3, 6*sizeof(GLfloat), 0);
    elementsSquare.enableAttribute(1, 3, 6*sizeof(GLfloat), 3*sizeof(GLfloat));

    // TODO Partie 2: Instancier le cube ici.
    // ...

    // TODO Partie 1: Donner une couleur de remplissage aux fonds.
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearDepth(1.0f);

    // TODO Partie 2: Activer le depth test.


    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        // TODO Partie 1: Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (w.getKey(Window::Key::T))
        {
            selectShape = ++selectShape < 7 ? selectShape : 0;
            std::cout << "Selected shape: " << selectShape << std::endl;
        }

        // TODO Partie 1: Mise à jour des données du triangle
        changeRGB(&onlyColorTriVertices[0]);
        changeRGB(&onlyColorTriVertices[3]);
        changeRGB(&onlyColorTriVertices[6]);
        multColoredTri.updateColorData(onlyColorTriVertices, sizeof(onlyColorTriVertices));

        GLfloat* posPtr = multColoredTri.mapPosData();
        multColoredTri.unmapPosData();
        if (posPtr == nullptr)
        {
            std::cerr << "Could not map position data!\n";
            return -1;
        }
        changePos(posPtr, cx, cy, dx, dy);

        // TODO Partie 1: Utiliser le bon shader programme selon la forme.
        // N'hésiter pas à utiliser le fallthrough du switch case.
        switch (selectShape)
        {
            case 0:
            case 1:
            {
                basicShaderProgram.use();
                GLint globalColorLocation = basicShaderProgram.getUniformLoc("globalColor");
                if (globalColorLocation == -1) {
                    std::cerr << "Could not find uniform variable 'globalColor'\n";
                } else {
                glUniform4f(globalColorLocation, brightBlue[0], brightBlue[1], brightBlue[2], brightBlue[3]);
                }
                break;
            }
            case 2:
            case 3:
            case 4:
            case 5:
            {
                colorShaderProgram.use();
                break;
            }
        }

        // TODO Partie 2: Calcul des matrices et envoyer une matrice résultante mvp au shader.
        if (selectShape == 6)
        {
            angleDeg += 0.5f;
            // Utiliser glm pour les calculs de matrices.
            // glm::mat4 matrix;
        }

        // TODO Partie 1: Dessiner la forme sélectionnée.
        switch (selectShape)
        {
            case 0:
                onlyColorTri.draw(GL_TRIANGLES, 3);
                break;
            case 1:
                onlyColorSquare.draw(GL_TRIANGLES, 6);
                break;
            case 2:
                coloredtri.draw(GL_TRIANGLES, 3);
                break;
            case 3:
                coloredSquare.draw(GL_TRIANGLES, 6);
                break;
            case 4:
                multColoredTri.draw(GL_TRIANGLES, 3);
                break;
            case 5:
                elementsSquare.draw(GL_TRIANGLES, 6);
                break;
        }

        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKey(Window::Key::ESC);
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


void changeRGB(GLfloat* color)
{
    unsigned char r = color[0]*255;
    unsigned char g = color[1]*255;
    unsigned char b = color[2]*255;

    if(r > 0 && b == 0)
    {
        r--;
        g++;
    }
    if(g > 0 && r == 0)
    {
        g--;
        b++;
    }
    if(b > 0 && g == 0)
    {
        r++;
        b--;
    }
    color[0] = r/255.0f;
    color[1] = g/255.0f;
    color[2] = b/255.0f;
}

void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy)
{
    if ((cx < -1 && dx < 0) || (cx > 1 && dx > 0))
        dx = -dx;
    pos[0] += dx;
    pos[3] += dx;
    pos[6] += dx;
    cx += dx;
    if ((cy < -1 && dy < 0) || (cy > 1 && dy > 0))
        dy = -dy;
    pos[1] += dy;
    pos[4] += dy;
    pos[7] += dy;
    cy += dy;
}


std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


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
#include "model.h"

using namespace std;
using namespace glm;

void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

string readFile(const char* path);

int main(int argc, char* argv[])
{
    Window w;
    if (!w.init())
        return -1;

    GLenum rev = glewInit();
    if ( rev != GLEW_OK )
    {
        cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << endl;
        return -2;
    }

    printGLInfo();

    ShaderProgram transform;
    GLint mvpLocation;
    {
        string vertexCode   = readFile("shaders/transform.vs.glsl");
        string fragmentCode = readFile("shaders/transform.fs.glsl");

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
    vec3 pos(0, 0, 0);
    vec2 ori(0, 0);
    Camera camera(pos, ori);
    // définition de var pour orientation de la caméra
    int mouseX, mouseY;
    bool isFirstPersonView = true;
    bool isRunning = true;
    // Instanciation objets
    Model tree("");
    Model rock("");
    Model mushroom("");
    
    // Créer un plan carré pour le sol et un plan rectangulaire pour le ruisseau
    
    // Répartir 49 groupes d’objets dans la scène
    // for (int i = 0; i < 49; i++) {
        // Les groupes possèdent une transformation de translation qui est donnée par getGroupRandomPos,
        // de rotation en Y aléatoire entre [0; 2π] et mise à l’échelle aléatoire entre [0.7; 1.3].
    
        // Calculer la matrice de transformation du groupe

        // Stocker la matrice de transformation du groupe

    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT a ajouter avec les transformations

        // Changement entre la vue à la première personne et à la troisième personne en utilisant le scroll de la souris
        int scrollDirection = w.getMouseScrollDirection();
        if (scrollDirection != 0) {
            isFirstPersonView = !isFirstPersonView;
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
        mat4 model = rotate(mat4(1.0f), radians(angleDeg), vec3(0.1, 1, 0.1));
        
        // assigner la position de la caméra et l'orientation, selon le type de vue
        mat4 view;
        if (isFirstPersonView)
        {
            view = camera.getFirstPersonViewMatrix();
        }
        else
        {
            view = camera.getThirdPersonViewMatrix();
        }

        mat4 proj = perspective(radians(70.0f), (float)w.getWidth()/(float)w.getHeight(), 0.1f, 10.0f);
        mat4 mvp = proj * view * model;
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

        // Affichage
        // for (int i = 0; i < 49; i++) {
            // Récupérer la matrice de transformation du groupe
            // Dessiner l'arbre, le rocher et le champignon avec leurs transformations respectives

        // cubeElements.draw(GL_TRIANGLES, 36);

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
        cerr << "GL_ERROR, File " << file << " (Line " << line << "): ";
        switch ( error )
        {
        case GL_INVALID_ENUM:
            cerr << "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            cerr << "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            cerr << "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            cerr << "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_UNDERFLOW:
            cerr << "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            cerr << "GL_STACK_OVERFLOW";
            break;
        default:
            cerr << "Unknown gl error occured!";
            break;
        }
        cerr << endl;
    }
}

void printGLInfo()
{
    cout << "OpenGL info:" << endl;
    cout << "    Vendor: "   << glGetString(GL_VENDOR)   << endl;
    cout << "    Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "    Version: "  << glGetString(GL_VERSION)  << endl;
    cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

string readFile(const char* path)
{
    ifstream file(path);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
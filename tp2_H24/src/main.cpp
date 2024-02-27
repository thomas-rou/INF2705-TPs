
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
#include "utils.h"


const int GROUPS_ROWS_SIZE = 7;
const int GROUPS_TABLE_SIZE = GROUPS_ROWS_SIZE * GROUPS_ROWS_SIZE;

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

    // Instanciation des shaders
    ShaderProgram modelShader;
    GLint modelMvpLoc;
    {
        std::string modelVertexCode   = readFile("./shaders/model.vs.glsl");
        std::string modelFragmentCode = readFile("./shaders/model.fs.glsl");

        Shader vertexShader(GL_VERTEX_SHADER, modelVertexCode.c_str());
        Shader fragmentShader(GL_FRAGMENT_SHADER, modelFragmentCode.c_str());
        modelShader.attachShader(vertexShader);
        modelShader.attachShader(fragmentShader);
        modelShader.link();
        modelMvpLoc = modelShader.getUniformLoc("mvp");
    }

    GLint modelColorLoc = modelShader.getUniformLoc("color");

    // Instanciation des éléments
    BasicShapeElements ground;
    ground.setData(groundVertices, sizeof(groundVertices), groundIndexes, sizeof(groundIndexes));
    ground.enableAttribute(0, 3, 5, 0);

    BasicShapeElements river;
    river.setData(riverVertices, sizeof(riverVertices), riverIndexes, sizeof(riverIndexes));
    river.enableAttribute(0, 3, 5, 0);

    BasicShapeElements hud;
    hud.setData(hudVertices, sizeof(hudVertices), hudIndexes, sizeof(hudIndexes));
    hud.enableAttribute(0, 3, 5, 0);

    // Models
    Model suzanne("../models/suzanne.obj");
    Model tree("../models/tree.obj");
    Model rock("../models/rock.obj");
    Model mushroom("../models/mushroom.obj");

    // Groups
    glm::mat4 treeModel[GROUPS_TABLE_SIZE];
    glm::mat4 rockModel[GROUPS_TABLE_SIZE];
    glm::mat4 mushroomModel[GROUPS_TABLE_SIZE];
    glm::vec3 groupsRepartitionMatrix[GROUPS_TABLE_SIZE];

    // group instances
    {
        glm::mat4 groupsModel[GROUPS_TABLE_SIZE];
        for (int i = 0; i < GROUPS_TABLE_SIZE; ++i) {
            // random translation transformation for each group
            float randPosX, randPosZ;
            getGroupRandomPos(i, GROUPS_ROWS_SIZE, randPosX, randPosZ);
            groupsRepartitionMatrix[i] = glm::vec3(randPosX, -1.0f, randPosZ);

            // random scale [0.7; 1.3]
            float groupScale = float(rand01() * 0.6f + 0.7f);

            // random rotation [0; 2π]
            float groupRotation = float(rand01() * float(2*M_PI));

            // define group model (applied to all objects in the group)
            glm::mat4 groupModel = glm::mat4(1.0f);
            groupModel = glm::translate(groupModel, glm::vec3(randPosX, -1.0f, randPosZ)); // -1.0 in z axis for the ground
            groupModel = glm::scale(groupModel, glm::vec3(groupScale));
            groupModel = glm::rotate(groupModel, groupRotation, glm::vec3(0., 1., 0.));
            groupsModel[i] = groupModel;

            // Tree model
            float treeScale = float(rand01() * 0.6f + 0.7f);
            groupModel = glm::scale(groupsModel[i], glm::vec3(treeScale));
            treeModel[i] = groupModel;

            // Rock Model
            float rockRotation = float(rand01() * float(2 * M_PI));
            float rockRadius = float(rand01() + 1.0f);
            groupModel = glm::rotate(groupsModel[i], rockRotation, glm::vec3(0.0f, 1.0f, 0.0f));
            groupModel = glm::translate(groupModel, glm::vec3(0.0f, 0.2f, rockRadius));
            groupModel = glm::scale(groupModel, glm::vec3(0.3));
            rockModel[i] = groupModel;

            // Mushroom Model
            groupModel = glm::translate(treeModel[i], glm::vec3(0.3, 0.0, 0.3));
            groupModel = glm::scale(groupModel, glm::vec3(0.05));
            mushroomModel[i] = groupModel;
        }
    }

    // Instanciation camera
    glm::vec3 pos(0, 0, 0);
    glm::vec2 ori(0, 0);
    Camera camera(pos, ori);

    // définition de var pour orientation de la caméra
    int mouseX, mouseY;
    bool isthirdPerson = false;

    // Enable face culling
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    bool isRunning = true;
    while (isRunning)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Changement de la position de la caméra
        float theta = glm::radians(ori.x);
        float posSpeed = 0.05f;
        float oriSpeed = 0.01f;
        glm::vec3 forward(glm::sin(theta), 0., -glm::cos(theta));
        glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
        if (w.getKeyHold(Window::Key::W)){
            pos += posSpeed * forward;
        }
        if (w.getKeyHold(Window::Key::S)){
            pos -= posSpeed * forward;
        }
        if (w.getKeyHold(Window::Key::A)){
            pos -= posSpeed * right;
        }
        if (w.getKeyHold(Window::Key::D)){
            pos += posSpeed * right;
        }

        // Changement de l'orientation de la caméra
        w.getMouseMotion(mouseX, mouseY);
        ori.x += mouseX * oriSpeed;
        ori.y += mouseY * oriSpeed;
        
        // view Matrix and suzanne model if thirdPerson
        glm::mat4 view = glm::mat4(1.0f);
        switch (w.getMouseScrollDirection()) {
            case 1:
                view = camera.getFirstPersonViewMatrix();
                isthirdPerson = false;
                break;
            case -1:
                view = camera.getThirdPersonViewMatrix();
                isthirdPerson = true;
                break;
            default:
                if (isthirdPerson) {
                    view = camera.getThirdPersonViewMatrix();
                }
                else {
                    view = camera.getFirstPersonViewMatrix();
                }
                break;
        }
        // Projection Matrix
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth()/(float)w.getHeight(), 0.1f, 200.0f);
        glm::mat4 displayMatrix = proj * view;
        glm::mat4 mvp = displayMatrix;

        // suzanne model matrix if third person
        modelShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        if (isthirdPerson) {
            model = glm::translate(model, pos + glm::vec3(0.0f, -1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f));
            model = glm::rotate(model, -glm::radians(ori.x) - float(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
            mvp = displayMatrix * model;
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
            glUniform3f(modelColorLoc, 1.0f, 0.0f, 0.0f);
            suzanne.draw();
        }

        // display ground
        mvp = displayMatrix;
        glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
        glUniform3f(modelColorLoc, 0.0f, 1.0f, 0.0f);
        ground.draw(GL_TRIANGLES, 6);

        // display river
        mvp = displayMatrix;
        glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
        glUniform3f(modelColorLoc, 0.2f, 0.2f, 1.0f);
        river.draw(GL_TRIANGLES, 6);

        // display objects groups
        // trees
        for (int i = 0; i < GROUPS_TABLE_SIZE; i++) {
            mvp = displayMatrix * treeModel[i];
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
            glUniform3f(modelColorLoc, 0.5f, 0.35f, 0.05f);
            tree.draw();

            // rocks
            mvp = displayMatrix * rockModel[i];
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
            glUniform3f(modelColorLoc, 0.5f, 0.5f, 0.5f);
            rock.draw();

            // mushroom
            mvp = displayMatrix * mushroomModel[i];
            glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
            glUniform3f(modelColorLoc, 1.0f, 1.0f, 1.0f);
            mushroom.draw();
        }

        // hud
        glDepthFunc(GL_ALWAYS);
        model = glm::mat4(1.0f);
        proj = glm::mat4(1.0f);
        float scaleX = 50.0f;
        float scaleY = 50.0f;
        model = glm::translate(model, glm::vec3(750.0f/4.0f, 750.0f/4.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scaleX, scaleY, 0.0f));
        proj = glm::ortho(0.0f, float(w.getWidth()), 0.0f, float(w.getHeight()));
        mvp = proj * model;

        glUniformMatrix4fv(modelMvpLoc, 1, GL_FALSE, &mvp[0][0]);
        glUniform3f(modelColorLoc, 0.0f, 0.0f, 0.0f);
        hud.draw(GL_TRIANGLES, 6);

        glDepthFunc(GL_LESS);

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
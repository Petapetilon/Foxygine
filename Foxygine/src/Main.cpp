#include "Graphics/Graphics.h"
#include "Foxygine.h"
#include <GLFW/glfw3.h>

#ifndef GLEW_ALREADY_INCLUDED
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

const int WWIDTH  = 1600;
const int WHEIGHT = 900;


int main(void)
{
    Graphics::GL_CurrentlyBoundShaderProgram = -2;
    Window::GetInstance()->SetupWindow(Vector2I(WWIDTH, WHEIGHT), "Foxygine");
    Keyboard::SetupKeyboard();
    Mouse::SetupMouse();
    Foxygine foxygine;

    if (glewInit() != GLEW_OK)   std::cout << "GlewInit Error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_FRONT);
    glCullFace(GL_BACK);

    foxygine.StartFoxygine();
    GameObjectHandler::InitHandler();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(Window::GetInstance()->GLFW_GetWindow()))
    {
        // 1.Invokes PreRender
        // 2.Invokes Render
        // 3.Joins Input Threads
        // 4.Invokes Update
        // 5.Invokes FixeUpdate if needed 
        GameObjectHandler::TickHandler(foxygine);

        // 1.Swap front and back buffers 
        // 2.Displays FPS
        // 3.Polls Events
        glfwSwapBuffers(Window::GetInstance()->GLFW_GetWindow());
        Window::GetInstance()->DisplayFPS();
        glfwPollEvents();
    }

    foxygine.EndFoxygine();
    glfwTerminate();
    return 0;
}
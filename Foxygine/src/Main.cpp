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
    Window::GetInstance()->SetupWindow(Vector2I(WWIDTH, WHEIGHT), "Foxygine");
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
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        GameObjectHandler::TickHandler(foxygine);
        GameObjectHandler::InvokeOnPreRender();
        Graphics::DrawMeshRenderer();		
        GameObjectHandler::InvokeOnPostRender();

        /* Swap front and back buffers */
        glfwSwapBuffers(Window::GetInstance()->GLFW_GetWindow());
        Window::GetInstance()->DisplayFPS();

        /* Poll for and process events */
        glfwPollEvents();
    }

    foxygine.EndFoxygine();
    glfwTerminate();
    return 0;
}
#include "Graphics/Graphics.h"
#include "Foxygine.h"
#include <GLFW/glfw3.h>

#ifndef GLEW_ALREADY_INCLUDED
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

const int WWIDTH  = 2100;
const int WHEIGHT = 900;



#ifdef FOXYGINE_DEBUG
bool cursorShown = false;
int pauseFuncHandle = 0;
void Pause(KeyCode key, KeyState state) {
    if (key == KeyCode::Esc && state == KeyState::Down) {
        if (cursorShown)
            glfwSetInputMode(Window::GetInstance()->GLFW_GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(Window::GetInstance()->GLFW_GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursorShown = !cursorShown;
    }
}
#endif




int main(void)
{
    std::chrono::steady_clock::time_point engineBegin = std::chrono::steady_clock::now();

    Window::GetInstance()->SetupWindow(Vector2I(WWIDTH, WHEIGHT), "Foxygine");
    Keyboard::SetupKeyboard();
    Mouse::SetupMouse();
    Foxygine foxygine;

    if (glewInit() != GLEW_OK)   std::cout << "GlewInit Error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_FRONT);
    glCullFace(GL_BACK);


    Graphics::Init();
    foxygine.StartFoxygine();
    GameObjectHandler::InitHandler();

#ifdef FOXYGINE_DEBUG
    pauseFuncHandle = Keyboard::RegisterOnAnyKeyCallback(Pause);
#endif // FOXYGINE_DEBUG

    Graphics::FinishLoadingResources();
    std::chrono::steady_clock::time_point engineEnd = std::chrono::steady_clock::now();
    std::cout << "Time for Engine Start = " << std::chrono::duration_cast<std::chrono::milliseconds>(engineEnd - engineBegin).count() << "[ms]" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(Window::GetInstance()->GLFW_GetWindow()))
    {
        // 1.Invokes PreRender
        // 2.Invokes Render
        // 3.Invokes PostRender
        // 4.Joins Input Threads
        // 5.Invokes Update
        // 6.Invokes FixeUpdate if needed 
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

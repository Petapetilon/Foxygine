#include "Graphics/Graphics.h"
#include "Foxygine Include.h"
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


void CheckGlewError(int initCode) {
    switch (initCode)
    {
    case 0:
        std::cout << "Glew OK!" << std::endl;
        return;

    case 1:
        std::cout << "No GL Version found!" << std::endl;
        break;

    case 2:
        std::cout << "Minimal GL Version is 1.1!" << std::endl;
        break;

    case 3:
        std::cout << "Minimal GLX Version is 1.2!" << std::endl;
        break;

    case 4:
        std::cout << "No GLX Display found!" << std::endl;
        break;
        
    default:
        break;
    }

    __debugbreak();
}
#endif




int main(void)
{
    std::chrono::steady_clock::time_point engineBegin = std::chrono::steady_clock::now();

    Window::GetInstance()->SetupWindow(Vector2I(WWIDTH, WHEIGHT), "Foxygine");
    Keyboard::SetupKeyboard();
    Mouse::SetupMouse();
    Foxygine foxygine;

#ifdef FOXYGINE_DEBUG
    CheckGlewError(glewInit());
#elif
    glewInit();
#endif // FOXYGINE_DEBUG

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
        // 6.Invokes FixedUpdate if needed 
        GameObjectHandler::TickHandler(foxygine);

        // 1.Swap front and back buffers 
        // 2.Displays FPS
        // 3.Polls Events
        // 4.Dispatches Input Threads
        glfwSwapBuffers(Window::GetInstance()->GLFW_GetWindow());
        Window::GetInstance()->DisplayFPS();
        glfwPollEvents();
    }

    foxygine.EndFoxygine();
    glfwTerminate();
    return 0;
}

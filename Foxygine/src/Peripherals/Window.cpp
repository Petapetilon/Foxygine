#include "Window.h"
#include <iostream>
#include <iomanip>
#include <sstream>


Window* Window::instance;

Window::Window()
{
    lastTime = 0;
    timeTillFPSDisplay = 0;
    windowWidth = 0;
    windowHeight = 0;
    windowName = "";
    GLFW_Window = nullptr;
    std::cout << "creating Window" << std::endl;
}


Window* Window::GetInstance()
{
    if (instance == nullptr) {
        instance = new Window();
    }

    return instance;
}


bool Window::SetupWindow(Vector2I windowSize, std::string _windowName)
{
    windowName = _windowName;

    /* Initialize the library */
    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    //glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
    GLFW_Window = glfwCreateWindow(windowSize.x, windowSize.y, windowName.c_str(), NULL, NULL);
    if (!GLFW_Window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(GLFW_Window);
    glfwSwapInterval(0);
    std::setprecision(5);
    return true;
}


GLFWwindow* Window::GLFW_GetWindow()
{
    return GLFW_Window;
}


Vector2I Window::GetWindowRersolution()
{
    return Vector2I(windowWidth, windowHeight);
}


void Window::DisplayFPS() {
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    timeTillFPSDisplay += delta;

    if (timeTillFPSDisplay >= .25f) {
        std::stringstream time;
        time << windowName << " FPS: [" << (double)1 / delta << "]";
        glfwSetWindowTitle(GLFW_Window, time.str().c_str());    
        timeTillFPSDisplay = 0;
    }

    lastTime = currentTime;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Application.h>

int main(int argc, char* argv[])
{
    if (!glfwInit()) return -1;

	Application::Init("OpenGL App", 1920, 1080);

	Application* app = Application::GetApp();

	app->InitScene();

	app->Run();

	Application::Shutdown();

    glfwTerminate();
    return 0;
}
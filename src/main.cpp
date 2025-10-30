#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Application.h>

int main(int argc, char* argv[])
{
    if (!glfwInit()) return -1;

	Application::Init("OpenGL App", 1920, 1080);

	Application* app = Application::GetApp();

	// Query OpenGL version
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);

	std::cout << version << std::endl;
	std::cout << renderer << std::endl;
	std::cout << vendor << std::endl;

	app->InitScene();

	app->Run();

	Application::Shutdown();

    glfwTerminate();
    return 0;
}
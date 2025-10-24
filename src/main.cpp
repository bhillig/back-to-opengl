#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Window/Window.h>

int main(int argc, char* argv[])
{
    if (!glfwInit()) return -1;

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	Window window("Back to OpenGL", WINDOW_WIDTH, WINDOW_HEIGHT);

	// Query OpenGL version
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);

	std::cout << version << std::endl;
	std::cout << renderer << std::endl;
	std::cout << vendor << std::endl;

	window.InitScene();

	window.Run();

    glfwTerminate();
    return 0;
}
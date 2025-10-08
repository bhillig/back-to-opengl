#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Window.h>

int main(int argc, char* argv[])
{
    if (!glfwInit()) return -1;

	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

	Window window("Back to OpenGL", WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Query OpenGL version
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);

	std::cout << version << std::endl;
	std::cout << renderer << std::endl;
	std::cout << vendor << std::endl;

	window.Run();

    glfwTerminate();
    return 0;
}
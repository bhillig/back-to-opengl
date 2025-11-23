#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Application.h>

int main(int argc, char* argv[])
{
	if (!glfwInit()) return -1;

	Core::ApplicationSpecification appSpec;
	appSpec.Name = "OpenGL";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;

	Core::Application app(appSpec);
	app.InitScene();
	app.Run();
    return 0;
}
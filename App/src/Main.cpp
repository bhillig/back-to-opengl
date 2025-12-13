#include <Application.h>

#include <GameSceneLayer.h>
#include <GUILayer.h>

#include <iostream>

int main(int argc, char* argv[])
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "OpenGL App";
	appSpec.WindowSpec.Width = 1920;
	appSpec.WindowSpec.Height = 1080;

	// Create application with specification and initialize it
	Core::Application app(appSpec);
	if (!app.Init())
	{
		return -1;
	}

	app.AddLayer<GameSceneLayer>();
	app.AddLayer<GUILayer>();

	app.Run();

    return 0;
}
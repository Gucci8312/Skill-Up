#include "main.h"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	int WindowWidth = 1200;
	int WindowHeight = 800;
	const char* AppTitle = "Practice";

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE,0);
	auto Window = glfwCreateWindow(WindowWidth, WindowHeight, AppTitle, nullptr, nullptr);

	// Vulkanèâä˙âª
	VulkanAppManager theApp;
	theApp.Initialize(Window,AppTitle);
	while (glfwWindowShouldClose(Window) == GLFW_FALSE)
	{
		glfwPollEvents();
		theApp.Render();
	}

	// VulkanèIóπ
	theApp.Terminate();
	glfwTerminate();


	return 0;
}
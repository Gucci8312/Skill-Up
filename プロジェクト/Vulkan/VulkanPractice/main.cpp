#include "main.h"

int APIENTRY WinMain(HINSTANCE 	hInstance, 		// アプリケーションのハンドル
	HINSTANCE 	hPrevInstance,					// 今のWindowsバージョンでは使われない
	LPSTR 		lpszArgs, 						// 起動時の引数（文字列）
	int 		nWinMode)						// ウインドウ表示モード
{
	//VULKAN Vulkan;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// ウィンドウ生成
	auto window = glfwCreateWindow(1200, 800, WINDOW_NAME, nullptr, nullptr);
	// Vulkan初期化
	VULKAN theApp;
	theApp.Initialize(window, WINDOW_NAME);

	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		glfwPollEvents();
		//Vulkan.Render();
	}
	// Vulkan終了
	theApp.Terminate();
	glfwTerminate();

	return 0;
}
#include "main.h"

int APIENTRY WinMain(HINSTANCE 	hInstance, 		// �A�v���P�[�V�����̃n���h��
	HINSTANCE 	hPrevInstance,					// ����Windows�o�[�W�����ł͎g���Ȃ�
	LPSTR 		lpszArgs, 						// �N�����̈����i������j
	int 		nWinMode)						// �E�C���h�E�\�����[�h
{
	//VULKAN Vulkan;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// �E�B���h�E����
	auto window = glfwCreateWindow(1200, 800, WINDOW_NAME, nullptr, nullptr);
	// Vulkan������
	VULKAN theApp;
	theApp.Initialize(window, WINDOW_NAME);

	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		glfwPollEvents();
		//Vulkan.Render();
	}
	// Vulkan�I��
	theApp.Terminate();
	glfwTerminate();

	return 0;
}
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

//Vulkan���s�N���X
class HelloTriangleApplication
{
public:
	void run()
	{
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	GLFWwindow* window;
	VkInstance instance;

	// �E�B���h�E������
	void InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	// �C���X�^���X����
	void createInstance()
	{
		// �A�v���P�[�V�����ݒ�
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// VulkanInstance�쐬�ݒ�
		VkInstanceCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		CreateInfo.pApplicationInfo = &appInfo;

		// VulkanInstance�쐬
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		CreateInfo.enabledExtensionCount = glfwExtensionCount;
		CreateInfo.ppEnabledExtensionNames = glfwExtensions;
		CreateInfo.enabledLayerCount = 0;
		if (vkCreateInstance(&CreateInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to CreateInstance");
		}

		// �g���@�\���擾
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		// �g���@�\�擾
		std::vector<VkExtensionProperties>extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		// �g���@�\�ꗗ�\��
		std::cout << "available extensions:\n";
		for (const auto& extension : extensions)
		{
			std::cout << '\t' << extension.extensionName << '\n';
		}
	}

	// Vulkan������
	void initVulkan()
	{
		InitWindow();
		createInstance();
	}

	// �Q�[�����[�v
	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	// �I������
	void cleanup()
	{
		vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}
};

int main()
{
	HelloTriangleApplication app;
	try {
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
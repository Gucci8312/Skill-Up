#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan_win32.h>
#include <vector>

#pragma comment(lib, "vulkan-1.lib")

class VulkanAppManager
{
public:
	VulkanAppManager();
	virtual ~VulkanAppManager() {}
	void Initialize(GLFWwindow* Window, const char* AppName);
	void Terminate();
	// �ȉ��h����œ��e���I�[�o�[���C�h����
	virtual void Prapare();
	virtual void Render();
protected:
	// �e����
	VkInstance  m_Instance;
	VkPhysicalDevice  m_physDev;
	VkPhysicalDeviceMemoryProperties m_physMemProps;
};
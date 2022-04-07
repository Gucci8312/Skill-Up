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
	// 以下派生先で内容をオーバーライドする
	virtual void Prapare();
	virtual void Render();
protected:
	// 各処理
	VkInstance  m_Instance;
	VkPhysicalDevice  m_physDev;
	VkPhysicalDeviceMemoryProperties m_physMemProps;
};
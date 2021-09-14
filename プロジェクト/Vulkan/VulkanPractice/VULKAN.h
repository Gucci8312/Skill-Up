#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

// インクルード
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vulkan/vk_layer.h>
#include <vulkan/vulkan_win32.h>
#include <vector>
#include <cassert>

// ライブラリのリンク
#pragma comment(lib, "vulkan-1.lib")

class VULKAN
{
protected:
	/*VkInstance BulkanInstance;*/
	VkInstance  Instance;
	VkDevice    Device;
	VkPhysicalDevice  PhysDevice;
	VkPhysicalDeviceMemoryProperties m_physMemProps;

public:
	VULKAN() {}
	virtual ~VULKAN() {}
	bool Initialize(GLFWwindow* _Window, const char* _AppName);
	void Terminate();
	
private:
	bool CreateInstance(const char* _AppName);
	bool CreatePhysDevice();
	void GetPhysDeviceProperty(VkPhysicalDeviceProperties& _PhyDevProp, VkPhysicalDeviceFeatures& PhyDevFeatures);
	//virtual void prepare();
	//virtual void Render();
	//bool Initlalize(GLFWwindow* _Window,const char* _WindowName);
	//void Render();
};


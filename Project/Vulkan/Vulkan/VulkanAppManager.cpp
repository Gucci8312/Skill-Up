#include "VulkanAppManager.h"

VulkanAppManager::VulkanAppManager()
{
}

void VulkanAppManager::Initialize(GLFWwindow* window, const char* AppName)
{
	// インスタンス生成
	//const char* AppName = "Hello,Vulkan";
	std::vector<const char*> Extensions;
	VkApplicationInfo AppInfo{};
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = AppName;
	AppInfo.pEngineName = AppName;
	AppInfo.apiVersion = VK_API_VERSION_1_1;
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	{
		// レイヤー、検証レイヤーを有効化
		const char* layers[] = { "VK_LAYER_LUNARG_standard_validation" };
		VkInstanceCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		ci.enabledLayerCount = 1;
		ci.ppEnabledLayerNames = layers;
		ci.enabledExtensionCount = Extensions.size();
		ci.ppEnabledExtensionNames = Extensions.data();
		ci.pApplicationInfo = &AppInfo;
		vkCreateInstance(&ci, nullptr, &m_Instance);
		//-------------------
	}
	{
		// 物理デバイス取得コード
		//void SelectPhysicalDevice()
		//{
		uint32_t DevCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &DevCount, nullptr);
		std::vector<VkPhysicalDevice> PhyDevs(DevCount);
		vkEnumeratePhysicalDevices(m_Instance, &DevCount, PhyDevs.data());
		// 最初のデバイスを使用する
		m_physDev = PhyDevs[0];
		// メモリプロパティを取得しておく
		vkGetPhysicalDeviceMemoryProperties(m_physDev, &m_physMemProps);
		//}
	}

	{
		// デバイスキューインデックス取得
		uint32_t propCount;
		std::vector<VkQueueFamilyProperties> props(propCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physDev, &propCount, props.data());
		uint32_t graphicsQueue = -0u;
		for (uint32_t i = 0; i < propCount; ++i)
		{
			if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsQueue = i;
				break;
			}
		}
		//--------------------------------------------------
	}
	VkCommandPoolCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	ci.queueFamilyIndex=
}

void VulkanAppManager::Terminate()
{
}

void VulkanAppManager::Prapare()
{
}

void VulkanAppManager::Render()
{
}

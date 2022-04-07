#include "VulkanAppManager.h"

VulkanAppManager::VulkanAppManager()
{
}

void VulkanAppManager::Initialize(GLFWwindow* window, const char* AppName)
{
	// �C���X�^���X����
	//const char* AppName = "Hello,Vulkan";
	std::vector<const char*> Extensions;
	VkApplicationInfo AppInfo{};
	AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	AppInfo.pApplicationName = AppName;
	AppInfo.pEngineName = AppName;
	AppInfo.apiVersion = VK_API_VERSION_1_1;
	AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	{
		// ���C���[�A���؃��C���[��L����
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
		// �����f�o�C�X�擾�R�[�h
		//void SelectPhysicalDevice()
		//{
		uint32_t DevCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &DevCount, nullptr);
		std::vector<VkPhysicalDevice> PhyDevs(DevCount);
		vkEnumeratePhysicalDevices(m_Instance, &DevCount, PhyDevs.data());
		// �ŏ��̃f�o�C�X���g�p����
		m_physDev = PhyDevs[0];
		// �������v���p�e�B���擾���Ă���
		vkGetPhysicalDeviceMemoryProperties(m_physDev, &m_physMemProps);
		//}
	}

	{
		// �f�o�C�X�L���[�C���f�b�N�X�擾
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

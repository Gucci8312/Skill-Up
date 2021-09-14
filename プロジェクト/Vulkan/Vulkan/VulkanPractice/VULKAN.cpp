#include "VULKAN.h"

bool VULKAN::Initialize(GLFWwindow* _Window, const char* _AppName)
{
	CreateInstance(_AppName);

	CreatePhysDevice();

	// GPU���擾
	VkPhysicalDeviceProperties GpuProperty = {};
	VkPhysicalDeviceFeatures FeaturesProperties = {};
	GetPhysDeviceProperty(GpuProperty, FeaturesProperties);

	return true;
}

void VULKAN::Terminate()
{
}

// �C���X�^���X����
bool VULKAN::CreateInstance(const char* _AppName)
{
	// �A�v���P�[�V�������
	VkApplicationInfo Application_Info{};
	Application_Info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	Application_Info.apiVersion = VK_API_VERSION_1_0;
	Application_Info.pApplicationName = _AppName;

	const char* layers = { "VK_LAYER_LUNARG_standard_validation" };

	// �C���X�^���X�̐������
	VkInstanceCreateInfo Instance_Create_Info{};
	Instance_Create_Info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	Instance_Create_Info.pApplicationInfo = &Application_Info;
	//Instance_Create_Info.enabledLayerCount = 1;
	//Instance_Create_Info.ppEnabledLayerNames = layers;
	
	// �C���X�^���X����
	if (VK_SUCCESS != vkCreateInstance(&Instance_Create_Info, nullptr, &Instance))
	{
		assert(0 && "Vulkan Error");
		return false;
	}

	

	return true;
}

// �����f�o�C�X�쐬
bool VULKAN::CreatePhysDevice()
{
	uint32_t GpuCnt = 0;
	vkEnumeratePhysicalDevices(Instance, &GpuCnt, nullptr);
	std::vector<VkPhysicalDevice> PhyDeviceList(GpuCnt);
	vkEnumeratePhysicalDevices(Instance, &GpuCnt, PhyDeviceList.data());
	PhysDevice = PhyDeviceList[0];

	return true;
}

// �f�o�C�X���擾
void VULKAN::GetPhysDeviceProperty(VkPhysicalDeviceProperties& _PhyDevProp, VkPhysicalDeviceFeatures& PhyDevFeatures)
{
	// GPU���擾
	//VkPhysicalDeviceProperties GpuProperty = {};
	vkGetPhysicalDeviceProperties(PhysDevice, &_PhyDevProp);

	// GPU�g���@�\�擾
	//VkPhysicalDeviceFeatures FeaturesProperties;
	vkGetPhysicalDeviceFeatures(PhysDevice, &PhyDevFeatures);

	// Vulkan���T�|�[�g���Ă��邩
	uint32_t FamilyCnt;
	vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &FamilyCnt, nullptr);
	std::vector<VkQueueFamilyProperties> FamilyProperty(FamilyCnt);
	vkGetPhysicalDeviceQueueFamilyProperties(PhysDevice, &FamilyCnt, FamilyProperty.data());
	bool found = false;
	for (uint32_t i = 0; i < FamilyCnt; ++i)
	{
		if (FamilyProperty[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			found = true;

		}
	}
	if (!found)
	{
		assert(0 && "Vulkan���T�|�[�g���Ă��܂���");
	}
}

#include "graphics/renderDevice.h"

#include <vector>
#include <array>

RenderDevice::RenderDevice(const RenderParametrs &parametrs) {
    initializeVulkanInstance();
    initializePhysicalDevice();
    initializeLogicalDevice();
    initializeQueues();
    initializeCommandBuffers();
}

RenderDevice::~RenderDevice() {
    destroyCommandBuffers();
    destroyLogicalDevice();
    destroyVulkanInstance();
}


void RenderDevice::initializeVulkanInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    appInfo.pApplicationName = "Box render";
    appInfo.pEngineName = "Fox engine";

    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);

    VkInstanceCreateInfo instInfo{};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;

    const std::array<const char*, 1> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    instInfo.enabledLayerCount = validationLayers.size();
    instInfo.ppEnabledLayerNames = validationLayers.data();
    instInfo.enabledExtensionCount = 0;

    vkCreateInstance(&instInfo, nullptr, &vulkanInstance);
}

void RenderDevice::destroyVulkanInstance() {
    vkDestroyInstance(vulkanInstance, nullptr);
    vulkanInstance = nullptr;
}


void RenderDevice::initializePhysicalDevice() {
    std::uint32_t deviceCount;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevices.data());

    for (VkPhysicalDevice& device : physicalDevices) {
        bool passed = false;

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) {
            passed = true;
        }

        if (!passed) {
            continue;
        }
        passed = false;
        
        std::uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (std::uint32_t i = 0; i < queueFamilyCount; i++) {
            if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                passed = true;
                break;
            }
        }

        if (!passed) {
            continue;
        }
        passed = false;

        physicalDevice = device;
        break;
    }
}


void RenderDevice::initializeLogicalDevice() {
    std::uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (std::uint32_t i = 0; i < queueFamilyCount; i++) {
        if ((queueFamilies[i].queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT))) {
            universalQueueIndex = i;
            break;
        }
    }

    std::array<float, 1> priorities = { 1.0f };
    VkDeviceQueueCreateInfo queueCreateInfo{};

    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = universalQueueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = priorities.data();

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.enabledExtensionCount = 0;

    deviceCreateInfo.enabledLayerCount = 0;

    vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
}

void RenderDevice::destroyLogicalDevice() {
    vkDestroyDevice(logicalDevice, nullptr);
}


void RenderDevice::initializeQueues() {
    vkGetDeviceQueue(logicalDevice, universalQueueIndex, 0, &universalQueue);
}


void RenderDevice::initializeCommandBuffers() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = universalQueueIndex;

    vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &universalPool);
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = universalPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice, &allocInfo, &universalBuffer);
}
 
void RenderDevice::destroyCommandBuffers() {
    vkFreeCommandBuffers(logicalDevice, universalPool, 1, &universalBuffer);
    vkDestroyCommandPool(logicalDevice, universalPool, nullptr);
}
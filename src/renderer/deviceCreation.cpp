#include "renderer/deviceCreation.hpp"
#include "renderer/renderer.hpp"
#include "renderer/swapChainCreation.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <vulkan/vulkan_core.h>

namespace Renderer {
	bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface,const std::vector<const char*> deviceExtensions){
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);


        QueueFamilyIndices indices = findQueueFamilies(device,surface);

        bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device,surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        bool result = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && indices.isComplete() && swapChainAdequate && supportedFeatures.samplerAnisotropy;

        if(result) std::cout << deviceProperties.deviceName << std::endl;

        return result ;
    }

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface){
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);

        std::vector<VkQueueFamilyProperties> allQueueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,allQueueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : allQueueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport); 
                if (presentSupport) {
                    indices.presentFamily = i;
                }

            }

            i++;
        }

        return indices;

    }

	bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }


	void VulkanRender::pickPhysicalDevice(){
		uint32_t deviceCount = 0;

		vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

		if(deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);

		vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

		for (const auto& device: devices) {
			if (isDeviceSuitable(device,m_surface,m_deviceExtensions)) {
				m_physicalDevice = device;
				break;
			}
		}



		if(m_physicalDevice == VK_NULL_HANDLE) throw std::runtime_error("failed to find suitable GPU!");
	}


	void VulkanRender::createLogicalDevice(){
        QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice,m_surface);
        
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
            
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        
        VkDeviceCreateInfo deviceCreateInfo{};

        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensions.data();


       

        if(m_enableValidationLayers){
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = m_validationLayers.data();
        }
        else{
            deviceCreateInfo.enabledLayerCount = 0;
        }
 
        if(vkCreateDevice(m_physicalDevice,&deviceCreateInfo,nullptr,&m_device) != VK_SUCCESS){
            throw std::runtime_error("failed to create logical device");
        }
        

        vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);

    }
}

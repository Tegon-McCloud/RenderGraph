#include "RenderContext.h"

#include <optional>
#include <iostream>

#ifndef NDEBUG
#define USE_VALIDATION_LAYERS
#endif

#ifdef USE_VALIDATION_LAYERS
#define VK_USE_
#endif

RenderContext::RenderContext()
{
	initInstance();
	pickAdapterAndQueues();
	initDevice();

	
}

RenderContext::~RenderContext()
{
	device.destroy();
	instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dynamicLoader);
	instance.destroy();
}

std::vector<const char*> getLayers() {
	const std::vector<const char*> requiredLayers = {
#ifdef USE_VALIDATION_LAYERS
		"VK_LAYER_KHRONOS_validation",
#endif
	};

	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	for (auto& requiredLayer : requiredLayers) {
		auto it = std::find_if(
			availableLayers.cbegin(),
			availableLayers.cend(),
			[=](auto& layer) -> bool { return strcmp(layer.layerName, requiredLayer) == 0; }
		);

		if (it == availableLayers.cend()) {
			throw std::runtime_error(std::string("Missing vulkan layer: ") + requiredLayer);
		}
	}

	return requiredLayers;
}

std::vector<const char*> getExtensions() {

	std::vector<const char*> requiredExtensions = {
#ifdef USE_VALIDATION_LAYERS
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
	};

	std::vector<vk::ExtensionProperties> availableExtensions = vk::enumerateInstanceExtensionProperties();

	for (auto& requiredExtension : requiredExtensions) {
		auto it = std::find_if(
			availableExtensions.cbegin(),
			availableExtensions.cend(),
			[=](auto& extension) -> bool { return strcmp(requiredExtension, extension.extensionName) == 0; }
		);

		if (it == availableExtensions.cend()) {
			throw std::runtime_error(std::string("Missing vulkan extension: ") + requiredExtension);
		}
	}

	return requiredExtensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	std::cerr << pCallbackData->pMessage << "\n\n";

	return VK_FALSE;
}

void RenderContext::initInstance()
{
	vk::ApplicationInfo appInfo;
	appInfo.setApiVersion(VK_API_VERSION_1_2);

	std::vector<const char*> layers = getLayers();
	std::vector<const char*> extensions = getExtensions();

#ifdef USE_VALIDATION_LAYERS
	vk::DebugUtilsMessengerCreateInfoEXT debugInfo;
	debugInfo.setMessageSeverity(
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
		.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral)
		.setPfnUserCallback(debugCallback);
#endif

	vk::InstanceCreateInfo info;
	info.setPApplicationInfo(&appInfo)
		.setPEnabledLayerNames(layers)
		.setPEnabledExtensionNames(extensions)
		.setPNext(&debugInfo);

	instance = vk::createInstance(info);
	dynamicLoader.init(instance, vkGetInstanceProcAddr);
	
	debugMessenger = instance.createDebugUtilsMessengerEXT(debugInfo, nullptr, dynamicLoader);
}

std::optional<QueueSpecs> tryGetQueueSpecs(vk::PhysicalDevice adapter, uint32_t graphicsQueueCount, uint32_t computeQueueCount) {

	QueueSpecs specs;
	bool foundGraphics = false;
	bool foundCompute = false;

	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = adapter.getQueueFamilyProperties();

	for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
		auto& props = queueFamilyProperties[i];

		if (props.queueFlags & vk::QueueFlagBits::eGraphics && !foundGraphics) {
			specs.graphics.family = i;
			foundGraphics = true;
		}

		if (props.queueFlags & vk::QueueFlagBits::eCompute && !foundCompute) {
			specs.compute.family = i;
			foundCompute = true;
		}
	}

	if (!foundGraphics || !foundCompute) {
		return std::optional<QueueSpecs>();
	}

	specs.graphics.begin = 0;
	specs.graphics.end = graphicsQueueCount;

	specs.compute.begin = 0;
	specs.compute.end = computeQueueCount;

	if (specs.graphics.family == specs.compute.family) {
		specs.compute.begin += graphicsQueueCount;
		specs.compute.end += graphicsQueueCount;
	}

	return specs;
}

void RenderContext::pickAdapterAndQueues()
{
	std::vector<vk::PhysicalDevice> adapters = instance.enumeratePhysicalDevices();
	
	for (auto& adapter : adapters) {
		vk::PhysicalDeviceProperties props = adapter.getProperties();
		std::optional<QueueSpecs> optQueueSpecs = tryGetQueueSpecs(adapter, 1, 1);
		
		if (!optQueueSpecs) {
			continue;
		}

		this->adapter = adapter;
		this->queueSpecs = *optQueueSpecs;
		break;
	}
}

void RenderContext::initDevice()
{
	std::vector<float> queuePriorities(queueSpecs.graphics.end - queueSpecs.graphics.begin, 1.0f);

	vk::DeviceQueueCreateInfo queueInfo;
	queueInfo
		.setQueueFamilyIndex(queueSpecs.graphics.family)
		.setQueuePriorities(queuePriorities);

	std::array<vk::DeviceQueueCreateInfo, 1> queueInfos = { queueInfo };

	vk::DeviceCreateInfo info;
	info.setQueueCreateInfos(queueInfos);
	
	device = adapter.createDevice(info);
}

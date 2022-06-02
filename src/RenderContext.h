

#include <vulkan/vulkan.hpp>

struct QueueSpecs {
	struct {
		uint32_t family;
		uint32_t begin;
		uint32_t end;
	} graphics;
	
	struct {
		uint32_t family;
		uint32_t begin;
		uint32_t end;
	} compute;
};

class RenderContext {
public:
	RenderContext();
	~RenderContext();
	
private:
	void initInstance();
	void pickAdapterAndQueues();
	void initDevice();

private:
	vk::Instance instance;
	vk::DispatchLoaderDynamic dynamicLoader;
	vk::DebugUtilsMessengerEXT debugMessenger;
	vk::PhysicalDevice adapter;
	vk::Device device;
	
	QueueSpecs queueSpecs;
};

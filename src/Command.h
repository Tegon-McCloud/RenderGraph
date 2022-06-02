#pragma once

#include <vulkan/vulkan.hpp>

struct CommandBuffer {
	CommandBuffer(vk::CommandBuffer raw);

	vk::CommandBuffer raw;
};

class CommandPool {
public:
	CommandPool(vk::Device device, uint32_t queueFamily); 

	void resetAll();
	vk::CommandBuffer getCmdBuffer();

private:	
	vk::CommandPool raw;
	vk::Device device;

	std::vector<vk::CommandBuffer> buffers;
	uint32_t nextBuffer;
};

class CommandQueue {
public:
	CommandQueue(vk::Device device, uint32_t queueFamily, uint32_t queueIndex);
	
	void submit(
		const std::vector<vk::CommandBuffer>& buffers,
		const std::vector<vk::Semaphore>& waits,
		const std::vector<vk::Semaphore>& signals);



private:
	vk::Queue raw;
};








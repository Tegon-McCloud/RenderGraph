#include "Command.h"

CommandQueue::CommandQueue(vk::Device device, uint32_t queueFamily, uint32_t queueIndex)
{
	raw = device.getQueue(queueFamily, queueIndex);
}

void CommandQueue::submit(
	const std::vector<vk::CommandBuffer>& buffers,
	const std::vector<vk::Semaphore>& waits,
	const std::vector<vk::Semaphore>& signals)
{
	vk::SubmitInfo info;
	info.setCommandBuffers(buffers);
	info.setWaitSemaphores(waits);
	info.setSignalSemaphores(signals);
	
	raw.submit(std::array<vk::SubmitInfo, 1> { info });
}

CommandPool::CommandPool(vk::Device device, uint32_t queueFamily)
{
	vk::CommandPoolCreateInfo info;

	info.queueFamilyIndex = queueFamily;

	device.createCommandPool(info);
}

void CommandPool::resetAll()
{
	device.resetCommandPool(raw);
	nextBuffer = 0;
}

vk::CommandBuffer CommandPool::getCmdBuffer()
{	
	if (nextBuffer == buffers.size()) {
		vk::CommandBufferAllocateInfo info;

		info.commandBufferCount = 1;
		info.commandPool = raw;
		info.level = vk::CommandBufferLevel::ePrimary;

		buffers.push_back(device.allocateCommandBuffers(info)[0]);
	}

	return buffers[nextBuffer++];
}

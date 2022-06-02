#pragma once

#include <stdint.h>


#include <vulkan/vulkan.hpp>


enum class ResourceType : uint32_t {
	eTexture   = 0,
	eBuffer    = 1,
	eCpuBuffer = 2,
};

struct Resource {
	Resource(vk::Image texture);
	Resource(vk::Buffer buffer);

	ResourceType type;
	union {
		vk::Image texture;
		vk::Buffer buffer;
	};
};

struct ResourceDesc {
	ResourceType type;
	union {
		vk::ImageCreateInfo texture;
		vk::BufferCreateInfo buffer;
	};
};

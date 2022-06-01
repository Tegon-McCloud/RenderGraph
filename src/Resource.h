#pragma once

#include <stdint.h>


#include <vulkan/vulkan.hpp>


enum class ResourceType : uint32_t {
	eTexture   = 0,
	eBuffer    = 1,
	eCpuBuffer = 2,
};

struct Resource {
	ResourceType type;
	union {
		vk::Image image;
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

#pragma once

#include <stdint.h>

#include <vulkan/vulkan.hpp>


enum class ResourceType : uint32_t {
	eTexture = 0,
	eBuffer = 1,
};

struct Resource {
	ResourceType type;
	union {
		vk::Image texture;
		vk::Buffer buffer;
	};
};



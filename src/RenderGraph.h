#pragma once

#include <stdint.h>
#include <vector>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "Resource.h"

using GraphResourceRef = uint32_t;

//enum class DependencyFlags : uint32_t {
//	eReadStorageBuffer  = 1 << 0,
//	eReadUniformBuffer  = 1 << 1,
//	eWriteRenderTarget  = 1 << 2,
//	eWriteStorageBuffer = 1 << 3,
//};

struct Dependency {
	GraphResourceRef resource;
	union {
		vk::ImageUsageFlags texture;
		vk::BufferUsageFlags buffer;
	} usage;
};

struct RenderPass {
	virtual void execute() const = 0;
	std::vector<Dependency> dependencies;
};


struct ResourceRegistry {
public:
	ResourceRegistry(const std::vector<std::shared_ptr<RenderPass>>& passes, const std::vector<ResourceDesc>& descs);

	Resource getResource(GraphResourceRef ref) const;

private:


private:
	std::vector<Resource> resources;
	std::vector<vk::DeviceMemory> memory;
};

class RenderGraph {
public:
	RenderGraph(std::vector<std::shared_ptr<RenderPass>> passes, const std::vector<ResourceDesc>& resources);

	void execute() const;

private:
	std::vector<RenderPass> passes;
	ResourceRegistry registry;
};

class RenderGraphBuilder {
public:
	RenderGraphBuilder() = default;

	void addPass(RenderPass&& pass);

	RenderGraph compile() const;

private:
	std::vector<std::shared_ptr<RenderPass>> passes;
	std::vector<ResourceDesc> resources;
};


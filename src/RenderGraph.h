#pragma once

#include <stdint.h>
#include <vector>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "Resource.h"

using GraphResourceRef = uint32_t;

struct Dependency {
	Dependency(GraphResourceRef resource, vk::ImageUsageFlags usage);
	Dependency(GraphResourceRef resource, vk::BufferUsageFlags usage);
	
	GraphResourceRef resource;
	union UsageFlags {
		UsageFlags(vk::ImageUsageFlags usage) : texture(usage) {};
		UsageFlags(vk::BufferUsageFlags usage) : buffer(usage) {};

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

	const Resource& getResource(GraphResourceRef ref) const;
	
private:
	Resource createResource(ResourceDesc desc) const;
	Resource createTexture(vk::ImageCreateInfo desc) const;
	Resource createBuffer(vk::BufferCreateInfo desc) const;

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

	void addPass(std::shared_ptr<RenderPass> pass);

	RenderGraph compile() const;

private:
	std::vector<std::shared_ptr<RenderPass>> passes;
	std::vector<ResourceDesc> resources;
};


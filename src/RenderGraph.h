#pragma once

#include <stdint.h>
#include <vector>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "Resource.h"

struct GraphResourceRef {
	friend class RenderGraphBuilder;
private:
	GraphResourceRef(uint32_t index);
	
public:

private:
	uint32_t index;
};

struct GraphResourceMut {
	friend class GraphResourceRef;
	friend class RenderGraphBuilder;
private:
	GraphResourceMut(uint32_t index);

public:
	/*GraphResourceMut(const GraphResourceMut&) = delete;
	GraphResourceMut(GraphResourceMut&&);
	GraphResourceMut& operator=(const GraphResourceMut&) = delete;
	GraphResourceMut& operator=(GraphResourceMut&&);*/

private:
	uint32_t index;
};

struct GraphResource {
	ResourceType type;
	union {
		vk::ImageCreateInfo image;
		vk::BufferCreateInfo buffer;
	};
	std::function<Resource()> external;
};

struct ReadDependency {
	GraphResourceRef resource;
	vk::ShaderStageFlags readStages;
};

struct WriteDependency {
	GraphResourceMut resource;
};

class GraphRenderPass {
	friend class RenderGraphBuilder;
public:
	virtual void execute() = 0;

protected:
	std::vector<ReadDependency> reads;
	std::vector<WriteDependency> writes;
};

class RenderGraph {
public:


private:
	std::vector<GraphRenderPass> passes;

};

class RenderGraphBuilder {
public:
	RenderGraphBuilder() = default;

	GraphResourceMut declareResource(GraphResource&& resource);
	void addPass(GraphRenderPass&& pass);

	RenderGraph compile() const;

private:
	std::vector<GraphRenderPass> passes;
	std::vector<GraphResource> resources;

};


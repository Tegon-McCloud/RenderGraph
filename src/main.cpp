
#include <iostream>

#include <vulkan/vulkan.hpp>

#include "RenderGraph.h"
#include "Resource.h"
#include "RenderContext.h"

struct LambertianPass : public RenderPass {

	LambertianPass(GraphResourceRef renderTarget, GraphResourceRef depthTexture) :
		renderTarget(renderTarget)
	{
		dependencies.emplace_back(renderTarget, vk::ImageUsageFlagBits::eColorAttachment);
		dependencies.emplace_back(depthTexture, vk::ImageUsageFlagBits::eDepthStencilAttachment);
	}

	void execute() const override
	{
		

	}

	GraphResourceRef renderTarget;
	GraphResourceRef depthTexture;
};

int main() {
	
	RenderContext context;

	RenderGraphBuilder graphBuilder;

	
	return 0;
}

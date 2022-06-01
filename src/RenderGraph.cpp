#include "RenderGraph.h"

#include <limits>


RenderGraph::RenderGraph(
    std::vector<std::shared_ptr<RenderPass>> passes,
    const std::vector<ResourceDesc>& resources) :
    registry(passes, resources)
{

}

void RenderGraph::execute() const
{
    for (auto& pass : passes) {
        pass.execute();
    }
}

void RenderGraphBuilder::addPass(RenderPass&& pass)
{
    for (uint32_t i = 0; i < pass.dependencies.size(); i++) {
        Dependency dep = pass.dependencies[i];
        ResourceDesc& res = resources[dep.resource];
        
        switch (res.type) {
        case ResourceType::eTexture:
            res.texture.usage |= dep.usage.texture;
        case ResourceType::eBuffer:
            res.buffer.usage |= dep.usage.buffer;
        }
    }
    
    passes.emplace_back(std::move(pass));
}

RenderGraph RenderGraphBuilder::compile() const
{
    return RenderGraph(passes, resources);
}

ResourceRegistry::ResourceRegistry(const std::vector<RenderPass>& passes, const std::vector<ResourceDesc>& descs)
{

}

Resource ResourceRegistry::getResource(GraphResourceRef ref) const
{
    
    return actives[ref];
}


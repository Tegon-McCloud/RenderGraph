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

void RenderGraphBuilder::addPass(std::shared_ptr<RenderPass> pass)
{
    for (uint32_t i = 0; i < pass->dependencies.size(); i++) {
        Dependency dep = pass->dependencies[i];
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

ResourceRegistry::ResourceRegistry(const std::vector<std::shared_ptr<RenderPass>>& passes, const std::vector<ResourceDesc>& descs)
{
    struct Lifetime {
        uint32_t begin, end;
    };

    std::vector<Lifetime> lifetimes(descs.size(), Lifetime{ std::numeric_limits<uint32_t>::max(), 0 });

    for (uint32_t i = 0; i < passes.size(); i++) {
        const RenderPass& pass = *passes[i];

        for (auto& dep : pass.dependencies) {
            lifetimes[dep.resource].begin = std::min(i, lifetimes[dep.resource].begin);
            lifetimes[dep.resource].end = std::max(i, lifetimes[dep.resource].end);
        }
    }

    resources.reserve(descs.size());
    
    for (auto& desc : descs) {
        resources.push_back(createResource(desc));
    }
    
}

const Resource& ResourceRegistry::getResource(GraphResourceRef ref) const
{
    return resources[ref];
}

Resource ResourceRegistry::createResource(ResourceDesc desc) const
{
    switch (desc.type) {
    case ResourceType::eTexture:
        return createTexture(desc.texture);
    case ResourceType::eBuffer:
        return createBuffer(desc.buffer);
    case ResourceType::eCpuBuffer:
        break;
    }

    assert(false);
}

Resource ResourceRegistry::createTexture(vk::ImageCreateInfo desc) const
{
    Resource res((vk::Image)nullptr);

    return res;
}

Resource ResourceRegistry::createBuffer(vk::BufferCreateInfo desc) const
{
    Resource res((vk::Buffer)nullptr);

    return res;
}

Dependency::Dependency(GraphResourceRef resource, vk::ImageUsageFlags usage) :
    resource(resource), usage(usage)
{}

Dependency::Dependency(GraphResourceRef resource, vk::BufferUsageFlags usage) :
    resource(resource), usage(usage)
{}

#include "RenderGraph.h"

#include <limits>

GraphResourceMut RenderGraphBuilder::declareResource(GraphResource&& resource)
{
    resources.emplace_back(std::move(resource));
    return GraphResourceMut(resources.size() - 1);
}

void RenderGraphBuilder::addPass(GraphRenderPass&& pass)
{
    passes.emplace_back(std::move(pass));
}

RenderGraph RenderGraphBuilder::compile() const
{
    return RenderGraph();
}

GraphResourceMut::GraphResourceMut(uint32_t index) :
    index(index)
{}

//GraphResourceMut::GraphResourceMut(GraphResourceMut&& other) :
//    index(other.index)
//{
//    other.index = std::numeric_limits<uint32_t>::max();
//}
//
//GraphResourceMut& GraphResourceMut::operator=(GraphResourceMut&&)
//{
//    other.
//
//    return *this;
//}
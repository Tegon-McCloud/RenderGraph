#include "Resource.h"

Resource::Resource(vk::Image texture)
	: type(ResourceType::eTexture), texture(texture)
{}

Resource::Resource(vk::Buffer buffer) :
	type(ResourceType::eBuffer), buffer(buffer)
{}

#include "render/DepthResources.hpp"

magmatic::render::DepthResources& magmatic::render::DepthResources::operator=(DepthResources&& rhs) noexcept {
	this->imageView = std::move(rhs.imageView);
	this->memory = std::move(rhs.memory);
	this->format = rhs.format;
	this->image = std::move(rhs.image);

	return *this;
}

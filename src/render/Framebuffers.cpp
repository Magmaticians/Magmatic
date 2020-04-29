#include "render/Framebuffers.hpp"

magmatic::render::Framebuffers& magmatic::render::Framebuffers::operator=(Framebuffers&& rhs) noexcept {
	this->framebuffers = std::move(rhs.framebuffers);
	return *this;
}
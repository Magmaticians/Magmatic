#include "core/Ray.hpp"

glm::vec3 magmatic::core::Ray::point(float distance) const noexcept
{
	return origin_ + direction_ * distance;
}

const glm::vec3& magmatic::core::Ray::origin() const noexcept
{
	return origin_;
}

const glm::vec3& magmatic::core::Ray::direction() const noexcept
{
	return direction_;
}

void magmatic::core::Ray::setOrigin(const glm::vec3& origin) noexcept
{
	origin_ = origin;
}

void magmatic::core::Ray::setDirection(const glm::vec3& direction) noexcept
{
	direction_ = direction;
}

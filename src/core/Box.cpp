#include "core/Box.hpp"

Box::Box(const glm::vec3& min, const glm::vec3& max) noexcept
:min_(min), max_(max)
{

}

const glm::vec3& Box::min() const noexcept
{
	return min_;
}

const glm::vec3& Box::max() const noexcept
{
	return max_;
}

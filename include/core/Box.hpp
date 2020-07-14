#ifndef MAGMATIC_BOX_HPP
#define MAGMATIC_BOX_HPP

#include <glm/vec3.hpp>


class Box
{
	Box() = default;
	Box(const glm::vec3& min, const glm::vec3& max) noexcept;

	[[nodiscard]] const glm::vec3& min() const noexcept;
	[[nodiscard]] const glm::vec3& max() const noexcept;

private:
	glm::vec3 min_;
	glm::vec3 max_;

};


#endif //MAGMATIC_BOX_HPP

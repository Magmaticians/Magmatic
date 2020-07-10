#ifndef MAGMATIC_RAY_HPP
#define MAGMATIC_RAY_HPP

#include <glm/glm.hpp>

namespace magmatic::core
{
	class Ray
	{
	public:
		Ray(const glm::vec3& origin, const glm::vec3& direction) noexcept
		:origin_(origin), direction_(direction)
		{}

		glm::vec3 point(float distance) const noexcept;

		void setOrigin(const glm::vec3& origin) noexcept;
		void setDirection(const glm::vec3& direction) noexcept;

		const glm::vec3& origin() const noexcept;
		const glm::vec3& direction() const noexcept;

	private:
		glm::vec3 origin_;
		glm::vec3 direction_;
	};
}

#endif //MAGMATIC_RAY_HPP

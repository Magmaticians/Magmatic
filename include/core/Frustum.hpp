#ifndef MAGMATIC_FRUSTUM_HPP
#define MAGMATIC_FRUSTUM_HPP

#include <glm/vec3.hpp>


namespace magmatic::core
{
	class Frustum
	{
	public:
		Frustum(const glm::vec3& eye_pos, const glm::vec3& target_dir,
				const glm::vec3& up_dir,
				float fov_y, float aspect,
				float z_near, float z_far
				) noexcept;

		bool pointIn(const glm::vec3& point) const noexcept;
	private:
		glm::vec3 eye_pos_;

		glm::vec3 x_;
		glm::vec3 y_;
		glm::vec3 z_;

		float near_;
		float far_;

		float aspect_;
		float y_factor_;

	};
}


#endif //MAGMATIC_FRUSTUM_HPP

#include "core/Frustum.hpp"
#include <cmath>
#include <stdexcept>
#include <glm/glm.hpp>

magmatic::core::Frustum::Frustum(
		const glm::vec3& eye_pos,
		const glm::vec3& target_dir,
		const glm::vec3& up_dir,
		float fov_y, float aspect,
		float near, float far
) noexcept
: eye_pos_(eye_pos), aspect_(aspect), near_(near), far_(far)
{
	z_ = glm::normalize(target_dir - eye_pos_);
	x_ = glm::normalize(z_ * up_dir);
	y_ = x_ * z_;

	y_factor_ = std::tan(fov_y / 2);
}

bool magmatic::core::Frustum::pointIn(const glm::vec3& point) const noexcept
{
	const glm::vec3 to_point = point - eye_pos_;

	const float distance_z = glm::dot(to_point, z_);
	if(distance_z < near_ || distance_z > far_)
	{
		return false;
	}

	const float distance_y = glm::dot(to_point, y_);
	const float max_y = distance_z * y_factor_;
	if(distance_y > max_y || distance_y < -max_y)
	{
		return false;
	}

	const float distance_x = glm::dot(to_point, x_);
	const float max_x = max_y * aspect_;

	return !(distance_x > max_x || distance_x < -max_x);

}

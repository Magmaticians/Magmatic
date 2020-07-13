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
	sphere_y_factor_ = 1.0f / std::cos(fov_y);

	const auto fov_x = std::atan(y_factor_ * aspect);
	sphere_x_factor_ = 1.0f / std::cos(fov_x);
}

bool magmatic::core::Frustum::pointIn(const glm::vec3& point) const noexcept
{
	const auto to_point = point - eye_pos_;

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

bool magmatic::core::Frustum::sphereIn(const glm::vec3& center, float radius) const noexcept
{
	assert(radius >= 0);
	const auto to_center = center - eye_pos_;

	const auto z_distance = glm::dot(to_center, z_);
	if(z_distance > far_ + radius || z_distance < near_ - radius)
	{
		return false;
	}

	const auto distance_threshold_y = radius * sphere_y_factor_;
	const auto distance_y = glm::dot(to_center, y_);
	const auto max_y = z_distance * y_factor_;

	if(distance_y > max_y + distance_threshold_y || distance_y < -max_y - distance_threshold_y)
	{
		return false;
	}

	const auto distance_threshold_x = sphere_x_factor_ * radius;
	const auto distance_x = glm::dot(to_center, x_);
	const auto max_x = max_y * aspect_;

	return !(distance_x > max_x + distance_threshold_x || distance_x < -max_x - distance_threshold_x);
}

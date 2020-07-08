#include "core/Camera.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

magmatic::core::Camera::Camera(
		glm::vec3 eye_pos, glm::vec3 target_dir, glm::vec3 up_dir, float fov_y, float aspect, float z_near, float z_far
) noexcept
		: eye_pos_(eye_pos), target_dir_(target_dir), up_dir_(up_dir), fov_y_(fov_y), aspect_(aspect), z_near_(z_near), z_far_(z_far)
{

}

const magmatic::core::CameraMatrix &magmatic::core::Camera::getCameraMatrix() const
{
	if(!valid_)
	{
		const auto view = glm::lookAt(eye_pos_, target_dir_, up_dir_);
		const auto projection = glm::perspective(fov_y_, aspect_, z_near_, z_far_);
		matrix_ = CameraMatrix(view, projection);

		valid_ = true;
	}
	return matrix_;
}

void magmatic::core::Camera::setEyePos(glm::vec3 eye_pos) noexcept
{
	eye_pos_ = std::move(eye_pos);
	valid_ = false;
}

void magmatic::core::Camera::setTargetDir(glm::vec3 target_dir) noexcept
{
	target_dir_ = std::move(target_dir);
	valid_ = false;
}

void magmatic::core::Camera::setUpDir(glm::vec3 up_dir) noexcept
{
	up_dir_ = std::move(up_dir);
	valid_ = false;
}

void magmatic::core::Camera::setFovY(float fov_y) noexcept
{
	fov_y_ = fov_y;
	valid_ = false;
}

void magmatic::core::Camera::setAspect(float aspect) noexcept
{
	aspect_ = aspect;
	valid_ = false;
}

void magmatic::core::Camera::setZNear(float z_near) noexcept
{
	z_near_ = z_near;
	valid_ = false;
}

void magmatic::core::Camera::setZFar(float z_far) noexcept
{
	z_far_ = z_far;
	valid_ = false;
}

glm::vec3 magmatic::core::Camera::realUpDir() const
{
	return static_cast<glm::vec3>(matrix_.invView()[1]);
}

glm::vec3 magmatic::core::Camera::rightDir() const
{
	return static_cast<glm::vec3>(matrix_.invView()[0]);
}

bool magmatic::core::operator==(const magmatic::core::Camera &lhs, const magmatic::core::Camera &rhs) noexcept
{
	return lhs.eye_pos_ == rhs.eye_pos_
		&& lhs.target_dir_ == rhs.target_dir_
		&& lhs.up_dir_ == rhs.up_dir_
		&& lhs.fov_y_ == rhs.fov_y_
		&& lhs.aspect_ == rhs.aspect_
		&& lhs.z_near_ == rhs.z_near_
		&& lhs.z_far_ == rhs.z_far_;
}

bool magmatic::core::operator!=(const magmatic::core::Camera &lhs, const magmatic::core::Camera &rhs) noexcept
{
	return !(lhs == rhs);
}



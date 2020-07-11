#include "core/camera/QuaternionCamera.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

magmatic::core::QuaternionCamera::QuaternionCamera(
		glm::quat orientation, glm::vec3 eye_pos, float fov_y, float aspect, float z_near, float z_far
) noexcept
: orientation_(orientation)
{
	camera_configuration_.setEyePos(eye_pos);
	camera_configuration_.setFovY(fov_y);
	camera_configuration_.setAspect(aspect);
	camera_configuration_.setZNear(z_near);
	camera_configuration_.setZFar(z_far);
}

void magmatic::core::QuaternionCamera::setOrientation(const glm::quat& orientation) noexcept
{
	orientation_ = std::move(orientation);
	orientation_changed_ = true;
}

void magmatic::core::QuaternionCamera::setEyePos(const glm::vec3& eye_pos) noexcept
{
	camera_configuration_.setEyePos(eye_pos);
}

void magmatic::core::QuaternionCamera::setFovY(float fov_y) noexcept
{
	camera_configuration_.setFovY(fov_y);
}

void magmatic::core::QuaternionCamera::setAspect(float aspect) noexcept
{
	camera_configuration_.setAspect(aspect);
}

void magmatic::core::QuaternionCamera::setZNear(float z_near) noexcept
{
	camera_configuration_.setZNear(z_near);
}

void magmatic::core::QuaternionCamera::setZFar(float z_far) noexcept
{
	camera_configuration_.setZFar(z_far);
}

const glm::quat magmatic::core::QuaternionCamera::orientation() const noexcept
{
	return orientation_;
}

const glm::vec3 magmatic::core::QuaternionCamera::eyePos() const noexcept
{
	return camera_configuration_.eyePos();
}

float magmatic::core::QuaternionCamera::fovY() const noexcept
{
	return camera_configuration_.fovY();
}

float magmatic::core::QuaternionCamera::aspect() const noexcept
{
	return camera_configuration_.aspect();
}

float magmatic::core::QuaternionCamera::zNear() const noexcept
{
	return camera_configuration_.zNear();
}

float magmatic::core::QuaternionCamera::zFar() const noexcept
{
	return camera_configuration_.zFar();
}

const magmatic::core::CameraConfiguration &magmatic::core::QuaternionCamera::cameraConfiguration() const noexcept
{
	if(orientation_changed_)
	{
		updateFromOrientation();
	}
	return camera_configuration_;
}

void magmatic::core::QuaternionCamera::updateFromOrientation() const noexcept
{
	const auto rotation = glm::toMat3(orientation_);

	const auto forward = glm::normalize(rotation * glm::vec3{0.0f, 0.0f, 1.0f});
	const auto up = glm::normalize(rotation * glm::vec3{0.0f, 1.0f, 0.0f});

	camera_configuration_.setTargetDir(forward);
	camera_configuration_.setUpDir(up);
	orientation_changed_ = false;
}

const magmatic::core::Ray magmatic::core::QuaternionCamera::viewRay() const noexcept
{
	return Ray(camera_configuration_.eyePos(), camera_configuration_.targetDir());
}

const magmatic::core::Ray magmatic::core::QuaternionCamera::screenPointRay(float x, float y) const noexcept
{
	const float shifted_x = x * 2.0f - 1.0f;
	const float shifted_y = y * 2.0f - 1.0f;

	const glm::vec4 screen_near{shifted_x, shifted_y, 0.0f, 1.0f};
	const glm::vec4 screen_far{shifted_x, shifted_y, 1.0f, 1.0f};

	const auto& camera_config = cameraConfiguration();
	const auto& camera_matrix = camera_config.getCameraMatrix();

	const auto world_near = glm::vec3(screen_near * camera_matrix.invViewProjection());
	const auto world_far = glm::normalize(glm::vec3(screen_far * camera_matrix.invViewProjection()));

	return Ray(world_near, world_far);
}

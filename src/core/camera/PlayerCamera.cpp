#include "core/camera/PlayerCamera.hpp"
#include <cmath>

const magmatic::core::CameraConfiguration& magmatic::core::PlayerCamera::cameraConfiguration() const noexcept
{
	if(!valid_)
	{
		const auto forward_dir = glm::vec3(
				std::cos(angle_x_) * std::sin(angle_y_),
				std::sin(angle_x_),
				std::cos(angle_x_) * std::cos(angle_y_)
				);

		glm::vec3 eye_pos = pos_;
		if(camera_distance_ != 0.0f)
		{
			eye_pos -= forward_dir * camera_distance_;
		}
		camera_configuration_.setTargetDir(forward_dir);
		camera_configuration_.setEyePos(eye_pos);

		valid_ = true;
	}
	return camera_configuration_;
}

void magmatic::core::PlayerCamera::setAngleY(float angle_y) noexcept
{
	angle_y_ = angle_y;
	valid_ = false;
}

void magmatic::core::PlayerCamera::setAngleX(float angle_x) noexcept
{
	angle_x_ = angle_x;
	valid_ = false;
}

void magmatic::core::PlayerCamera::setCameraDistance(float distance) noexcept
{
	camera_distance_ = distance;
	valid_ = false;
}

void magmatic::core::PlayerCamera::setPos(const glm::vec3& pos) noexcept
{
	pos_ = pos;
	valid_ = false;
}

void magmatic::core::PlayerCamera::setFovY(float fov_y) noexcept
{
	camera_configuration_.setFovY(fov_y);
}

void magmatic::core::PlayerCamera::setAspect(float aspect) noexcept
{
	camera_configuration_.setAspect(aspect);
}

void magmatic::core::PlayerCamera::setZNear(float z_near) noexcept
{
	camera_configuration_.setZNear(z_near);
}

void magmatic::core::PlayerCamera::setZFar(float z_far) noexcept
{
	camera_configuration_.setZFar(z_far);
}

float magmatic::core::PlayerCamera::angleY() const noexcept
{
	return angle_y_;
}

float magmatic::core::PlayerCamera::angleX() const noexcept
{
	return angle_x_;
}

float magmatic::core::PlayerCamera::cameraDistance() const noexcept
{
	return camera_distance_;
}

const glm::vec3& magmatic::core::PlayerCamera::pos() const noexcept
{
	return pos_;
}

float magmatic::core::PlayerCamera::fovY() const noexcept
{
	return camera_configuration_.fovY();
}

float magmatic::core::PlayerCamera::aspect() const noexcept
{
	return camera_configuration_.aspect();
}

float magmatic::core::PlayerCamera::zNear() const noexcept
{
	return camera_configuration_.zNear();
}

float magmatic::core::PlayerCamera::zFar() const noexcept
{
	return camera_configuration_.zFar();
}

magmatic::core::PlayerCamera::PlayerCamera() noexcept
:angle_y_(0.0f), angle_x_(0.0f), camera_distance_(0.0f), pos_{}
{
	camera_configuration_.setUpDir(up_dir_);
}

magmatic::core::PlayerCamera::PlayerCamera(
		float angle_y, float angle_x, float camera_distance, const glm::vec3& pos, float fov_y, float aspect
		, float z_near, float z_far
) noexcept
: angle_y_(angle_y), angle_x_(angle_x), camera_distance_(camera_distance), pos_(pos)
{
	camera_configuration_.setUpDir(up_dir_);
	camera_configuration_.setFovY(fov_y);
	camera_configuration_.setAspect(aspect);
	camera_configuration_.setZNear(z_near);
	camera_configuration_.setZFar(z_far);
}

const magmatic::core::Ray magmatic::core::PlayerCamera::viewRay() const noexcept
{
	return Ray(camera_configuration_.eyePos(), camera_configuration_.targetDir());
}

const magmatic::core::Ray magmatic::core::PlayerCamera::screenPointRay(float x, float y) const noexcept
{
	const float shifted_x = x * 2.0f - 1.0f;
	const float shifted_y = y * 2.0f - 1.0f;

	const glm::vec4 screen_near{shifted_x, shifted_y, 0.0f, 1.0f};
	const glm::vec4 screen_far{shifted_x, shifted_y, 1.0f, 1.0f};

	const auto& camera_config = cameraConfiguration();
	const auto& camera_matrix = camera_config.getCameraMatrix();

	const auto world_near = glm::vec3(screen_near * camera_matrix.invViewProjection());
	const auto world_far = glm::normalize(glm::vec3(screen_far * camera_matrix.invViewProjection()));

	return Ray(world_near, world_far - world_near);
}

#ifndef MAGMATIC_BASECAMERA_HPP
#define MAGMATIC_BASECAMERA_HPP

#include "CameraConfiguration.hpp"
#include "core/Ray.hpp"


namespace magmatic::core
{
	template<typename Derived>
	class BaseCamera
	{
	public:
		const CameraConfiguration& cameraConfiguration() const noexcept;

		const Ray viewRay() const noexcept;
		const Ray screenPointRay(float x, float y) const noexcept;

	protected:
		mutable CameraConfiguration camera_configuration_;
		mutable bool valid_ = false;
	};

	template<typename Derived>
	const CameraConfiguration& BaseCamera<Derived>::cameraConfiguration() const noexcept
	{
		if(!valid_)
		{
			static_cast<Derived*>(this)->updateConfiguration();
			valid_ = true;
		}
		return camera_configuration_;
	}

	template<typename Derived>
	const Ray BaseCamera<Derived>::viewRay() const noexcept
	{
		const auto& config = cameraConfiguration();
		return Ray(config.eyePos(), config.targetDir());

	}

	template<typename Derived>
	const Ray BaseCamera<Derived>::screenPointRay(float x, float y) const noexcept
	{
		const float shifted_x = x * 2.0f - 1.0f;
		const float shifted_y = y * 2.0f - 1.0f;

		const glm::vec4 screen_near{shifted_x, shifted_y, 0.0f, 1.0f};
		const glm::vec4 screen_far{shifted_x, shifted_y, 1.0f, 1.0f};

		const auto& camera_config = cameraConfiguration();
		const auto& camera_matrix = camera_config.getCameraMatrix();

		const auto world_near = glm::vec3(screen_near * camera_matrix.invViewProjection());
		const auto world_far = glm::vec3(screen_far * camera_matrix.invViewProjection());

		const auto dir = glm::normalize(world_far - world_near);
		return Ray(world_near, dir);
	}
}

#endif //MAGMATIC_BASECAMERA_HPP

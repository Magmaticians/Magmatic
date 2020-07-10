#ifndef MAGMATIC_QUATERNIONCAMERA_HPP
#define MAGMATIC_QUATERNIONCAMERA_HPP

#include "BaseCamera.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>


namespace magmatic::core
{
class QuaternionCamera : public BaseCamera
	{
	public:
		virtual const CameraConfiguration& cameraConfiguration() const noexcept override;

		void setOrientation(glm::quat orientation) noexcept;
		void setEyePos(glm::vec3 eye_pos) noexcept;
		void setFovY(float fov_y) noexcept;
		void setAspect(float aspect) noexcept;
		void setZNear(float z_near) noexcept;
		void setZFar(float z_far) noexcept;

		[[nodiscard]] const glm::quat orientation() const noexcept;
		[[nodiscard]] const glm::vec3 eyePos() const noexcept;
		[[nodiscard]] float fovY() const noexcept;
		[[nodiscard]] float aspect() const noexcept;
		[[nodiscard]] float zNear() const noexcept;
		[[nodiscard]] float zFar() const noexcept;

	private:
		glm::quat orientation_;

	};
}

#endif //MAGMATIC_QUATERNIONCAMERA_HPP

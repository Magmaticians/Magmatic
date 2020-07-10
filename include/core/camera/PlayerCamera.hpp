#ifndef MAGMATIC_PLAYERCAMERA_HPP
#define MAGMATIC_PLAYERCAMERA_HPP

#include "BaseCamera.hpp"
#include "CameraConfiguration.hpp"
#include <glm/vec3.hpp>

namespace magmatic::core
{
	class PlayerCamera : public BaseCamera
	{
	public:
		PlayerCamera() noexcept;

		PlayerCamera(
				float angle_y, float angle_x,
				float camera_distance, const glm::vec3& pos,
				float fov_y, float aspect,
				float z_near, float z_far
				) noexcept;

		void setAngleY(float angle_y) noexcept;
		void setAngleX(float angle_x) noexcept;
		void setCameraDistance(float distance) noexcept;
		void setPos(const glm::vec3 &pos) noexcept;
		void setFovY(float fov_y) noexcept;
		void setAspect(float aspect) noexcept;
		void setZNear(float z_near) noexcept;
		void setZFar(float z_far) noexcept;

		[[nodiscard]] float angleY() const noexcept;
		[[nodiscard]] float angleX() const noexcept;
		[[nodiscard]] float cameraDistance() const noexcept;
		[[nodiscard]] const glm::vec3 &pos() const noexcept;
		[[nodiscard]] float fovY() const noexcept;
		[[nodiscard]] float aspect() const noexcept;
		[[nodiscard]] float zNear() const noexcept;
		[[nodiscard]] float zFar() const noexcept;

		virtual const CameraConfiguration &cameraConfiguration() const noexcept override;

	private:
		constexpr static glm::vec3 up_dir_{0.0f, 1.0f, 0.0f};

		float angle_y_;
		float angle_x_;
		float camera_distance_;
		glm::vec3 pos_;

		mutable bool valid_ = false;
		mutable CameraConfiguration camera_configuration_;

	};
}

#endif //MAGMATIC_PLAYERCAMERA_HPP

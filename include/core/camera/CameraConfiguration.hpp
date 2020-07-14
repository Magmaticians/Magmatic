#ifndef MAGMATIC_CAMERACONFIGURATION_HPP
#define MAGMATIC_CAMERACONFIGURATION_HPP

#include "CameraMatrix.hpp"
#include <glm/vec3.hpp>
#include <memory>

namespace magmatic::core
{
	class CameraConfiguration
	{
	public:
		CameraConfiguration() = default;

		CameraConfiguration(
				const glm::vec3& eye_pos, const glm::vec3& target_dir, const glm::vec3& up_dir, float fov_y, float aspect,
				float z_near, float z_far
		) noexcept;

		[[nodiscard]] const glm::vec3& eyePos() const noexcept { return eye_pos_; };
		[[nodiscard]] const glm::vec3& targetDir() const noexcept { return target_dir_; };
		[[nodiscard]] const glm::vec3& upDir() const noexcept { return up_dir_; };

		[[nodiscard]] const float& fovY() const noexcept { return fov_y_; };
		[[nodiscard]] const float& aspect() const noexcept { return aspect_; };
		[[nodiscard]] const float& zNear() const noexcept { return z_near_; };
		[[nodiscard]] const float& zFar() const noexcept { return z_far_; };

		void setEyePos(const glm::vec3& eye_pos) noexcept;
		void setTargetDir(const glm::vec3& target_dir) noexcept;
		void setUpDir(const glm::vec3& up_dir) noexcept;

		void setFovY(float fov_y) noexcept;
		void setAspect(float aspect) noexcept;
		void setZNear(float z_near) noexcept;
		void setZFar(float z_far) noexcept;

		const CameraMatrix& getCameraMatrix() const;
		glm::vec3 realUpDir() const;
		glm::vec3 rightDir() const;

		friend bool operator==(const CameraConfiguration& lhs, const CameraConfiguration& rhs) noexcept;
		friend bool operator!=(const CameraConfiguration& lhs, const CameraConfiguration& rhs) noexcept;

	private:
		glm::vec3 eye_pos_;
		glm::vec3 target_dir_;
		glm::vec3 up_dir_;

		float fov_y_;
		float aspect_;
		float z_near_;
		float z_far_;

		mutable CameraMatrix matrix_;
		mutable bool valid_ = false;
	};

	bool operator==(const CameraConfiguration& lhs, const CameraConfiguration& rhs) noexcept;
	bool operator!=(const CameraConfiguration& lhs, const CameraConfiguration& rhs) noexcept;
}

#endif //MAGMATIC_CAMERACONFIGURATION_HPP

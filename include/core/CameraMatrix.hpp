#ifndef MAGMATIC_CAMERAMATRIX_HPP
#define MAGMATIC_CAMERAMATRIX_HPP

#include <glm/mat4x4.hpp>
#include <memory>

namespace magmatic::core
{
	class CameraMatrix
	{
	public:
		CameraMatrix() = default;
		CameraMatrix(glm::mat4 view, glm::mat4 projection) noexcept;

		[[nodiscard]] const glm::mat4 &view() const noexcept;

		[[nodiscard]] const glm::mat4 &projection() const noexcept;

		void setView(glm::mat4 view) noexcept;

		void setProjection(glm::mat4 projection) noexcept;

		const glm::mat4 &invView() const;

		const glm::mat4 &invProjection() const;

		const glm::mat4 &viewProjection() const;

		const glm::mat4 &invViewProjection() const;

	private:
		glm::mat4 view_;
		glm::mat4 projection_;

		mutable glm::mat4 inv_view_;
		mutable bool inv_view_valid_ = false;

		mutable glm::mat4 inv_projection_;
		mutable bool inv_projection_valid_ = false;

		mutable glm::mat4 view_projection_;
		mutable bool view_projection_valid_ = false;

		mutable glm::mat4 inv_view_projection_;
		mutable bool inv_view_projection_valid_ = false;
	};
}

#endif //MAGMATIC_CAMERAMATRIX_HPP

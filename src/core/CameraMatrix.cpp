#include "core/CameraMatrix.hpp"

magmatic::core::CameraMatrix::CameraMatrix(glm::mat4 view, glm::mat4 projection) noexcept
: view_(view), projection_(projection)
{

}

const glm::mat4& magmatic::core::CameraMatrix::view() const noexcept
{
	return view_;
}

const glm::mat4& magmatic::core::CameraMatrix::projection() const noexcept
{
	return projection_;
}

const glm::mat4& magmatic::core::CameraMatrix::invView() const
{
	if(!inv_view_valid_)
	{
		inv_view_ = std::make_unique<glm::mat4>(glm::inverse(view_));
		inv_view_valid_ = true;
	}
	return *inv_view_;
}

const glm::mat4& magmatic::core::CameraMatrix::invProjection() const
{
	if(!inv_projection_valid_)
	{
		inv_projection_ = std::make_unique<glm::mat4>(glm::inverse(projection_));
		inv_projection_valid_ = true;
	}
	return *inv_projection_;
}

const glm::mat4& magmatic::core::CameraMatrix::viewProjection() const
{
	if(!view_projection_valid_)
	{
		view_projection_ = std::make_unique<glm::mat4>(view_*projection_);
		view_projection_valid_ = true;
	}
	return *view_projection_;
}

const glm::mat4& magmatic::core::CameraMatrix::invViewProjection() const
{
	if(!inv_view_projection_valid_)
	{
		inv_view_projection_ = std::make_unique<glm::mat4>(glm::inverse(viewProjection()));
		inv_view_projection_valid_ = true;
	}
	return *inv_view_projection_;
}

void magmatic::core::CameraMatrix::setView(glm::mat4 view) noexcept
{
	view_ = std::move(view);
	inv_view_valid_ = false;
	view_projection_valid_ = false;
	inv_view_projection_valid_ = false;
}

void magmatic::core::CameraMatrix::setProjection(glm::mat4 projection) noexcept
{
	projection_ = std::move(projection);
	inv_projection_valid_ = false;
	view_projection_valid_ = false;
	inv_view_projection_valid_ = false;
}

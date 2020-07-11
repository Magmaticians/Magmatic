#ifndef MAGMATIC_BASECAMERA_HPP
#define MAGMATIC_BASECAMERA_HPP

#include "CameraConfiguration.hpp"
#include "core/Ray.hpp"


namespace magmatic::core
{
	class BaseCamera
	{
	public:
		virtual const CameraConfiguration& cameraConfiguration() const noexcept = 0;
		virtual const Ray viewRay() const noexcept = 0;
		virtual const Ray screenPointRay(float x, float y) const noexcept = 0;
	};
}

#endif //MAGMATIC_BASECAMERA_HPP

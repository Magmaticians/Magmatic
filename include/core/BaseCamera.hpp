#ifndef MAGMATIC_BASECAMERA_HPP
#define MAGMATIC_BASECAMERA_HPP

#include "CameraConfiguration.hpp"


namespace magmatic::core
{
	class BaseCamera
	{
	public:
		virtual const CameraConfiguration& cameraConfiguration() const noexcept = 0;

	protected:
		CameraConfiguration camera_configuration_;
	};
}

#endif //MAGMATIC_BASECAMERA_HPP

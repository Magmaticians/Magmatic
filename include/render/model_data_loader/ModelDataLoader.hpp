#ifndef MAGMATIC_MODELDATALOADER_HPP
#define MAGMATIC_MODELDATALOADER_HPP

#include "render/ModelData.hpp"
#include "utils/factory/FileLoaderFactory.hpp"


namespace magmatic::render
{
	using ModelDataLoader = magmatic::utils::FileLoaderFactory<ModelData>;
}

#endif //MAGMATIC_MODELDATALOADER_HPP

#ifndef MAGMATIC_MODELDATALOADER_HPP
#define MAGMATIC_MODELDATALOADER_HPP

#include "render/model/ModelData.hpp"
#include "utils/factory/FileLoaderFactory.hpp"


namespace magmatic::render
{
	using ModelDataLoader = magmatic::utils::FileLoaderFactory<ModelData>;
}

#endif //MAGMATIC_MODELDATALOADER_HPP

#ifndef MAGMATIC_MODELDATALOADERCONCRETE_HPP
#define MAGMATIC_MODELDATALOADERCONCRETE_HPP

#include "utils/factory/FileLoaderFactory.hpp"
#include "render/model/model_data_loader/ModelDataLoader.hpp"


namespace magmatic::render
{
	using ModelDataLoaderConcrete = utils::ConcreteLoader<ModelDataLoader>;
}

#endif //MAGMATIC_MODELDATALOADERCONCRETE_HPP

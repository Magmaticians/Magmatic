#ifndef MAGMATIC_MODELLOADERCONCRETE_HPP
#define MAGMATIC_MODELLOADERCONCRETE_HPP

#include "utils/factory/FileLoaderFactory.hpp"
#include "render/model_data_loader/ModelDataLoader.hpp"


namespace magmatic::render
{
	using ModelLoaderConcrete = utils::ConcreteLoader<ModelDataLoader>;
}

#endif //MAGMATIC_MODELLOADERCONCRETE_HPP

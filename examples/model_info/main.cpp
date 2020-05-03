#include "render/model/model_data_loader/ModelDataLoader.hpp"
#include <fmt/printf.h>

int main(int argc, char** argv)
{
	fmt::print("Available loaders({}):\n", magmatic::render::ModelDataLoader::registeredCount());
	const auto loaders = magmatic::render::ModelDataLoader::registeredTypes();
	auto loader = loaders.begin();
	for(size_t i = 0; i < loaders.size(); ++i)
	{
		fmt::print("{}:{}\n", i, *loader);
		++loader;
	}

	auto model = magmatic::render::ModelDataLoader::load("GLTF", "examples/resources/basic_tank.gltf");
	return 0;
}
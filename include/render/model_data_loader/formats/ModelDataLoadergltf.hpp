#ifndef MAGMATIC_MODELDATALOADERGLTF_HPP
#define MAGMATIC_MODELDATALOADERGLTF_HPP

#include "render/model_data_loader/formats/ModelDataLoaderConcrete.hpp"
#include <memory>

namespace magmatic::render
{
	class ModelDataLoadergltf : public ModelDataLoaderConcrete
	{
		static constexpr const char* ASCII_ext = ".gltf";
		static constexpr const char* binary_ext = ".glb";
	public:
		std::shared_ptr<ModelData> load(const std::filesystem::path &file_path) override;

		static std::string factoryName()
		{
			return "GLTF";
		}

	private:
		static bool registered_;

	};
}


#endif //MAGMATIC_MODELDATALOADERGLTF_HPP

#include "render/model_data_loader/formats/ModelDataLoadergltf.hpp"
#include <tiny_gltf.h>
#include <spdlog/spdlog.h>

bool magmatic::render::ModelDataLoadergltf::registered_ = magmatic::render::ModelDataLoader ::registerLoader(
		ModelDataLoadergltf::factoryName(),
		std::make_unique<ModelDataLoadergltf>()
);

std::shared_ptr<magmatic::render::ModelData> magmatic::render::ModelDataLoadergltf::load(const std::filesystem::path& file_path)
{
	if (!std::filesystem::exists(file_path))
	{
		spdlog::error("Magmatic: SoundLoader file doesn't exist: {}", file_path.string());
		throw std::runtime_error("SoundLoader file doesn't exist");
	}
	tinygltf::Model model;
	tinygltf::TinyGLTF context;

	std::string err;
	std::string warn;

	bool loaded = false;

	#if !defined (NDEBUG)
	spdlog::info("Magmatic: Started loading GLTF file: {}", file_path.string());
	#endif

	if (file_path.extension() == ASCII_ext)
	{
		#if !defined(NDEBUG)
		spdlog::info("\tLoading from ASCII file");
		#endif
		loaded = context.LoadASCIIFromFile(&model, &err, &warn, file_path.string());
	}
	else if (file_path.extension() == binary_ext)
	{
		#if !defined(NDEBUG)
		spdlog::info("\tLoading from binary file");
		#endif
		loaded = context.LoadBinaryFromFile(&model, &err, &warn, file_path.string());
	}
	else
	{
		spdlog::error("Magmatic: Unrecognized file extension for GLTF model");
		throw std::invalid_argument("Unrecognized file extension");
	}
	if(!err.empty())
	{
		spdlog::error("Failed to load GLTF file with error: {}", err);
		throw std::runtime_error(err);
	}

	#if !defined (NDEBUG)
	if(!warn.empty())
	{
		spdlog::warn("During loading GLTF: {}", warn);
	}
	#endif

	if(!loaded)
	{
		spdlog::error("Failed to load GLTF file");
		throw std::runtime_error("Failed to load GLTF file");
	}

	#if !defined(NDEBUG)
	spdlog::info("\tFile info:");
	spdlog::info("\t\tCopyright: {}", model.asset.copyright);
	spdlog::info("\t\tGenerator: {}", model.asset.generator);
	spdlog::info("\t\tVersion: {}", model.asset.version);
	spdlog::info("\t\tMinVersion: {}", model.asset.minVersion);

	spdlog::info("\tDeafult scene index: {}", model.defaultScene);
	#endif

	return nullptr;
}

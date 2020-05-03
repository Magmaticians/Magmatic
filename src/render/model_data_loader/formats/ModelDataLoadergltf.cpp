#include "render/model_data_loader/formats/ModelDataLoadergltf.hpp"
#include <tiny_gltf.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


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
	spdlog::info("\tScenes({}):", model.scenes.size());
	for(size_t i = 0; i < model.scenes.size(); ++i)
	{
		spdlog::info("\t\t{}.{}", i, model.scenes[i].name);
	}


	spdlog::info("\tNodes({})", model.nodes.size());
	for(size_t i = 0; i < model.nodes.size(); ++i)
	{
		spdlog::info("\t\t{}.{}", i, model.nodes[i].name);
	}

	auto model_data = std::make_shared<ModelData>();

	for(int index : model.scenes[model.defaultScene].nodes)
	{
		model_data->nodes.emplace_back(loadNode(nullptr, model.nodes[index], model, model_data->vertices, model_data->indices));
	}
	#endif

	//todo: loadSamplers


	return nullptr;
}
magmatic::render::ModelDataLoadergltf::PNodeData magmatic::render::ModelDataLoadergltf::loadNode(
	const magmatic::render::ModelDataLoadergltf::PNodeData &parent,
	const tinygltf::Node &node, const tinygltf::Model &model,
	std::vector<Vertex> &vertices, std::vector<uint32_t> &indices
)
{
	auto new_node = std::make_shared<NodeData>();
	new_node->parent = parent;

	if(node.matrix.size() == 16)
	{
		new_node->matrix = glm::make_mat4x4(node.matrix.data());
	}
	else
	{
		new_node->matrix = glm::mat4(1.0f);

		if(node.translation.size() == 3)
		{
			new_node->matrix = glm::translate(new_node->matrix, glm::vec3(glm::make_vec3(node.translation.data())));
		}
		if(node.rotation.size() == 4)
		{
			glm::quat temp_quat = glm::make_quat(node.rotation.data());
			new_node->matrix *= glm::mat4(temp_quat);
		}
		if(node.scale.size() == 3)
		{
			new_node->matrix = glm::scale(new_node->matrix, glm::vec3(glm::make_vec3(node.scale.data())));
		}
	}


	if(!node.children.empty())
	{
		for(int index : node.children)
		{
			new_node->children.emplace_back(loadNode(new_node, model.nodes[index], model, vertices, indices));
		}
	}
	//todo: loading meshes
	return new_node;
}

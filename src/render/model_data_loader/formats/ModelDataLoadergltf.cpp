#include "render/model_data_loader/formats/ModelDataLoadergltf.hpp"
#include <tiny_gltf.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vulkan/vulkan.hpp>


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

	spdlog::info("\tSamplers({})", model.samplers.size());
	for(size_t i = 0; i < model.samplers.size(); ++i)
	{
		spdlog::info("\t\t{}.{}", i, model.samplers[i].name);
	}

	spdlog::info("Loading default scene");

	spdlog::info("\tNodes({})", model.nodes.size());
	for(size_t i = 0; i < model.nodes.size(); ++i)
	{
		spdlog::info("\t\t{}.{}", i, model.nodes[i].name);
	}
	#endif


	auto model_data = std::make_shared<ModelData>();
	//todo: handle no default scene
	#if !defined(NDEBUG)
	spdlog::info("\tStarted loading nodes");
	#endif
	for(int index : model.scenes[model.defaultScene].nodes)
	{
		model_data->nodes.emplace_back(loadNode(nullptr, model.nodes[index], model, model_data->vertices, model_data->indices));
	}
	#if !defined(NDEBUG)
	spdlog::info("\tEnded loading nodes");
	#endif
	loadSamplers(model_data->sampler_settings, model);


	return model_data;
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

	if(node.mesh > -1)
	{
		const auto& mesh = model.meshes[node.mesh];

		for(const auto & primitive : mesh.primitives)
		{
			const auto first_index = static_cast<uint32_t>(indices.size());
			const auto first_vertex = static_cast<uint32_t>(vertices.size());

			const float* vertex_buffer = nullptr;
			const float* normal_buffer = nullptr;
			const float* tex_coord_buffer = nullptr;

			size_t vertex_count = 0;

			if(primitive.attributes.find("POSITION") == primitive.attributes.end())
			{
				spdlog::error("Magmatic: Failed to load mesh! No position data");
				throw std::runtime_error("No position data");
			}
			else
			{
				const auto& accessor = model.accessors[primitive.attributes.find("POSITION")->second];
				const auto& view = model.bufferViews[accessor.bufferView];
				vertex_buffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset+view.byteOffset]));
				vertex_count = accessor.count;
				#if !defined(NDEBUG)
				spdlog::info("\t\tFound {} vertices", vertex_count);
				#endif

			}

			if(primitive.attributes.find("NORMAL") == primitive.attributes.end())
			{
				spdlog::warn("Magmatic:No normal data");
			}
			else
			{
				const auto& accessor = model.accessors[primitive.attributes.find("NORMAL")->second];
				const auto& view = model.bufferViews[accessor.bufferView];
				normal_buffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset+view.byteOffset]));
			}

			if(primitive.attributes.find("TEXCOORD_0") == primitive.attributes.end())
			{
				spdlog::error("Magmatic: Failed to load mesh! No texcord data");
				throw std::runtime_error("No texcord data");
			}
			else
			{
				const auto& accessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
				const auto& view = model.bufferViews[accessor.bufferView];
				tex_coord_buffer = reinterpret_cast<const float*>(&(model.buffers[view.buffer].data[accessor.byteOffset+view.byteOffset]));
			}

			for(size_t i = 0; i < vertex_count; ++i)
			{
				Vertex temp = {};
				temp.position = glm::vec4(glm::make_vec3(&vertex_buffer[i * 3]), 1.0f);
				const auto normal = normal_buffer ? glm::make_vec3(&normal_buffer[i * 3]) : glm::vec3(0.0f);
				temp.normal = glm::normalize(glm::vec3(normal));
				temp.uv = glm::make_vec2(&tex_coord_buffer[i * 2]);
				vertices.push_back(temp);
			}



			const auto& accessor = model.accessors[primitive.indices];
			const auto& view = model.bufferViews[accessor.bufferView];
			const auto& buffer = model.buffers[view.buffer];

			uint32_t index_count = 0;
			#if !defined(NDEBUG)
			spdlog::info("\t\tFound {} indices", index_count);
			#endif

			indices.reserve(indices.size() + index_count);

			switch(accessor.componentType)
			{

				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					{
						const auto begin32 = reinterpret_cast<const uint32_t *>(&buffer.data[accessor.byteOffset + view.byteOffset]);
						std::copy(begin32, begin32 + index_count, std::back_inserter(indices));
					}
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					{
						const auto begin16 = reinterpret_cast<const uint16_t *>(&buffer.data[accessor.byteOffset + view.byteOffset]);
						std::copy(begin16, begin16 + index_count, std::back_inserter(indices));
					}
					break;
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					{
						const auto begin8 = reinterpret_cast<const uint8_t *>(&buffer.data[accessor.byteOffset + view.byteOffset]);
						std::copy(begin8, begin8 + index_count, std::back_inserter(indices));
					}
					break;
				default:
					spdlog::error("Magmatic: Failed to load model! Unkown index format");
					throw std::runtime_error("Failed to load model! Unkown index format");
			}
			ModelData::PrimitiveData primitive_data{};
			primitive_data.first_index = first_index;
			primitive_data.index_count = index_count;
			primitive_data.vertex_count = vertex_count;
			//todo: materials
			new_node->mesh_data.emplace_back(primitive_data);
		}
	}
	return new_node;
}

void magmatic::render::ModelDataLoadergltf::loadSamplers(
		std::vector<ModelData::SamplerSettings>& samplers,
		const tinygltf::Model& model
)
{
	#if !defined(NDEBUG)
	spdlog::info("\tStarted loading Samplers settings");
	#endif

	for(const auto& samp : model.samplers)
	{
		ModelData::SamplerSettings sampler{};
		sampler.min_filter = decodeFilterMode(samp.minFilter);
		sampler.mag_filter = decodeFilterMode(samp.magFilter);
		sampler.modeU = decodeAddressMode(samp.wrapS);
		sampler.modeV = decodeAddressMode(samp.wrapT);
		sampler.modeW = sampler.modeU;
		samplers.emplace_back(sampler);
	}
	#if !defined(NDEBUG)
	spdlog::info("\tEnded loading Samplers settings");
	#endif
}

//value decoded according to: https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md
vk::Filter magmatic::render::ModelDataLoadergltf::decodeFilterMode(int filter)
{
	switch(filter)
	{
		case 9728://NEAREST
			return vk::Filter::eNearest;
		case 9729://LINEAR
			return vk::Filter::eLinear;
		case 9984://NEAREST_MIPMAP_NEAREST
			[[fallthrough]];
		case 9985://LINEAR_MIPMAP_NEAREST
			return vk::Filter::eNearest;
		case 9986://NEAREST_MIPMAP_LINEAR
			[[fallthrough]];
		case 9987://LINEAR_MIPMAP_LINEAR
			return vk::Filter::eLinear;
		default:
			return vk::Filter::eLinear;
	}
}

vk::SamplerAddressMode magmatic::render::ModelDataLoadergltf::decodeAddressMode(int wrap_mode)
{
	switch(wrap_mode)
	{
		case 33071:
			return vk::SamplerAddressMode::eClampToEdge;
		case 33648:
			return vk::SamplerAddressMode::eMirroredRepeat;
		case 10497:
			return vk::SamplerAddressMode::eRepeat;
		default:
			return vk::SamplerAddressMode::eClampToEdge;
	}
}

#ifndef MAGMATIC_ABSTRACTFACTORY_HPP
#define MAGMATIC_ABSTRACTFACTORY_HPP

#include <memory>
#include <unordered_map>
#include <filesystem>
#include <set>
#include <cassert>


namespace magmatic::utils
{
	template<typename Factory>
	class ConcreteLoader
	{
	public:
		using Product = typename Factory::Product;

		ConcreteLoader() = default;

		virtual ~ConcreteLoader() = default;

		virtual Product load(const std::filesystem::path& file_path) {
			throw std::runtime_error("Not implemented");
		};

	};

	template<typename Prod>
	class FileLoaderFactory
	{
	public:
		using AFactory = FileLoaderFactory<Prod>;
		using Creator = std::unique_ptr<ConcreteLoader<AFactory>>;
		using Product = std::shared_ptr<Prod>;
		using Key = std::string;

	private:
		static std::unordered_map<Key, Creator>& getInternalMap() noexcept
		{
			static std::unordered_map<Key, Creator> map_;
			return map_;
		}

	public:
		FileLoaderFactory() = delete;

		static bool registerLoader(const std::string& type, Creator creator)
		{
			auto& map = getInternalMap();
			assert((map.find(type) == std::end(map)));
			map[type] = std::move(creator);
			return true;
		}

		static Product load(const std::string& type, const std::filesystem::path& file_path)
		{
			const auto& map = getInternalMap();
			const auto loader = map.find(type);
			assert((loader != std::end(map)));
			return loader->second->load(file_path);
		}

		static bool isRegistered(const std::string& type) noexcept
		{
			const auto& map = getInternalMap();
			return map.contains(type);
		}

		static std::set<Key> registeredTypes() noexcept
		{
			std::set<Key> types;
			const auto& map = getInternalMap();
			for (const auto&[key, val] : map)
			{
				types.insert(key);
			}

			return types;
		}

		static size_t registeredCount() noexcept
		{
			const auto& map = getInternalMap();
			return map.size();
		}
	};
}

#endif //MAGMATIC_ABSTRACTFACTORY_HPP

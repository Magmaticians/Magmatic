#ifndef MAGMATIC_ABSTRACTFACTORY_HPP
#define MAGMATIC_ABSTRACTFACTORY_HPP

#include <memory>
#include <unordered_map>
#include <filesystem>
#include <set>


template<typename Factory>
class ConcreteLoader
{
public:
	using TProduct = typename Factory::TProduct;

	ConcreteLoader() = default;
	virtual ~ConcreteLoader() = default;

	virtual typename Factory::TProduct create(const std::string&) = delete;

};

template<typename Product>
class FileLoaderFactory
{
public:
	using TAFactory = FileLoaderFactory<Product>;
	using TCreator = std::unique_ptr<ConcreteLoader<TAFactory>>;
	using TProduct = Product;
	using TKey = std::string;

private:
	static std::unordered_map<TKey, TCreator>& getInternalMap() noexcept
	{
		static std::unordered_map<TKey, TCreator> map_;
		return map_;
	}
public:
	FileLoaderFactory() = delete;

	static bool registerLoader(const std::string type, TCreator creator)
	{
		auto& map = getInternalMap();
		assert(map.find(type) == std::end(map));
		map[type] = std::move(creator);
		return true;
	}

	static TProduct load(const std::string& type, const std::filesystem::path& file_path)
	{
		const auto& map = getInternalMap();
		const auto loader = map.find(type);
		assert(loader != std::end(map));
		return loader->second->create(file_path);
	}

	static bool isRegistered(const std::string& type) noexcept
	{
		const auto& map = getInternalMap();
		return map.contains(type);
	}

	static std::set<TKey> registeredTypes() noexcept
	{
		std::set<TKey> types;
		const auto& map = getInternalMap();
		for(const auto& [key, val] : map)
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

#endif //MAGMATIC_ABSTRACTFACTORY_HPP

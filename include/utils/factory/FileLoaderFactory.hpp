#ifndef MAGMATIC_ABSTRACTFACTORY_HPP
#define MAGMATIC_ABSTRACTFACTORY_HPP

#include <memory>
#include <unordered_map>
#include <filesystem>


template<typename Factory>
class ConcreteLoader
{
public:
	using Product = typename Factory::TProduct;

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

private:
	static std::unordered_map<std::string, TCreator>& getMap() {
		static std::unordered_map<std::string, TCreator> map_;
		return map_;
	}
public:
	FileLoaderFactory() = delete;

	static bool registerLoader(const std::string name, TCreator creator)
	{
		auto& map = getMap();
		assert(map.find(name) == std::end(map));
		map[name] = std::move(creator);
		return true;
	}

	static TProduct load(const std::string& name, const std::filesystem::path& file_path)
	{
		auto& map = getMap();
		const auto loader = map.find(name);
		assert(loader != std::end(map));
		return loader->second->create(file_path);
	}
};

#endif //MAGMATIC_ABSTRACTFACTORY_HPP

target_sources(
		device_enumeration
		PRIVATE
		main.cpp
)
target_include_directories(device_enumeration PRIVATE Vulkan::Vulkan)
target_link_libraries(device_enumeration PRIVATE tabulate::tabulate magmatic-render magmatic-sound Vulkan::Vulkan)
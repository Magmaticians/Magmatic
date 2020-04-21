target_sources(
		magmatic-render
		PRIVATE
		render/Window.cpp
		render/Instance.cpp
		render/PhysicalDevice.cpp
		render/LogicalDevice.cpp
		render/Surface.cpp
		render/SwapChain.cpp
		render/Shader.cpp
		render/Pipeline.cpp
		render/RenderPass.cpp
		render/CommandBuffer.cpp
		render/utils.cpp
)

add_subdirectory(sound)

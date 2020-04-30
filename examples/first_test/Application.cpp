#define GLM_FORCE_RADIANS
#include <algorithm>
#include <spdlog/spdlog.h>
#include <vulkan/vulkan.hpp>
#include "render/UniformBufferObject.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <render/Bitmap.hpp>
#include "Application.hpp"

Application::Application(const std::string& mode):
vertices(std::move(getVertexConfig(mode))),
indices(std::move(getIndexConfig(mode))),
window(magmatic::render::Window(DEFAULT_NAME)),
instance(magmatic::render::Instance(DEFAULT_NAME, window.getRequiredExtensions())),
surface(instance.createSurface(window)),
physicalDevice(magmatic::render::PhysicalDevice(instance.getBestDevice())),
logicalDevice(magmatic::render::LogicalDevice(physicalDevice, surface)),
vertShader(logicalDevice, "./examples/first_test/vert.spv", vk::ShaderStageFlagBits::eVertex),
fragShader(logicalDevice, "./examples/first_test/frag.spv", vk::ShaderStageFlagBits::eFragment),
swapChain(logicalDevice, surface, window.getSize().first, window.getSize().second),
commandPool(logicalDevice, magmatic::render::QueueType::GraphicalQueue),
depthResources(logicalDevice,swapChain.extent, commandPool),
renderPass(logicalDevice, surface, depthResources),
descriptorSets(logicalDevice, bindings, MAX_FRAMES_IN_FLIGHT, descriptor_types),
pipeline(logicalDevice, swapChain.extent.width, swapChain.extent.height, {vertShader, fragShader}, renderPass, descriptorSets.getDescriptorSetLayout()),
framebuffers(logicalDevice, renderPass, swapChain, depthResources.imageView),
texture(logicalDevice, magmatic::render::Bitmap("examples/resources/statue.jpg"), commandPool),
sampler(logicalDevice),
vertexBuffer(logicalDevice, vertices, commandPool),
indexBuffer(logicalDevice, indices, commandPool),
commandBuffers(magmatic::render::CommandBuffer::createCommandBuffers(MAX_FRAMES_IN_FLIGHT, commandPool)),
uniformBuffers(magmatic::render::UniformBuffer<magmatic::render::UniformBufferObject>::createUniformBuffers(MAX_FRAMES_IN_FLIGHT, logicalDevice, commandPool)),
fences(logicalDevice, MAX_FRAMES_IN_FLIGHT),
imageAcquiredSemaphores(logicalDevice, magmatic::render::SemaphoreType::ImageAvailableSemaphore, MAX_FRAMES_IN_FLIGHT),
renderFinishedSemaphores(logicalDevice, magmatic::render::SemaphoreType::RenderFinishedSemaphore, MAX_FRAMES_IN_FLIGHT)
{
	spdlog::info("Application constructor called and finished work");
}

void Application::run() {
	imagesInFlight.resize(MAX_FRAMES_IN_FLIGHT, -1);
	currentBuffer = 0;
	currentFrame = 0;

	while(!window.shouldClose()) {
		glfwPollEvents();
		drawFrame();
	}

	logicalDevice.waitIdle();
}

void Application::updateUniformBuffer(uint32_t currentBuffer) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	magmatic::render::UniformBufferObject ubo = {};
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChain.extent.width/(float) swapChain.extent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	uniformBuffers[currentBuffer].copyMemory(sizeof(ubo), ubo);
}

void Application::updateDescriptorSet() {
	std::vector<magmatic::render::DescriptorWriteUpdate> updates;
	magmatic::render::DescriptorWriteUpdate write_update;
	write_update.type = magmatic::render::DescriptorWriteUpdate::eUniform;
	write_update.dst_binding = 0;
	write_update.dst_array_elem = 0;
	vk::DescriptorBufferInfo info(
			uniformBuffers[currentFrame].getBuffer().get(),
			0,
			sizeof(magmatic::render::UniformBufferObject)
	);
	write_update.data_info = info;
	updates.emplace_back(write_update);

	updates.emplace_back(texture.getWriteInfo(1, 0));
	updates.emplace_back(sampler.getWriteInfo(2, 0));
	descriptorSets.updateDescriptorSet(currentFrame, updates);
}

void Application::recordCommandBuffer() {
	vk::Buffer vertexBuffers[] = { vertexBuffer.getBuffer().get() };
	vk::DeviceSize offsets[] = { 0 };

	const auto& commandBufferHandle = commandBuffers[currentFrame].beginRecording();
	std::array<vk::ClearValue, 2> clearValues;
	clearValues[0].color =  vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 1.0f}));
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
	vk::RenderPassBeginInfo beginInfo(renderPass.renderPass.get(),
	                                  framebuffers[currentBuffer].get(),
	                                  vk::Rect2D(vk::Offset2D(0, 0), swapChain.extent),
	                                  static_cast<uint32_t>(clearValues.size()),
	                                  clearValues.data());
	commandBufferHandle->beginRenderPass(beginInfo, vk::SubpassContents::eInline);
	commandBufferHandle->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.pipeline.get());
	commandBufferHandle->bindVertexBuffers(0, 1, vertexBuffers, offsets);
	commandBufferHandle->bindIndexBuffer(indexBuffer.getBuffer().get(), 0, vk::IndexType::eUint32);
	commandBufferHandle->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.getPipelineLayout().get(), 0, 1, &descriptorSets[currentFrame].get(), 0, nullptr);
	commandBufferHandle->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	commandBufferHandle->endRenderPass();
	commandBuffers[currentFrame].endRecording();

}

void Application::drawFrame() {
	auto state = swapChain.acquireNextImageKHR(imageAcquiredSemaphores, currentFrame, currentBuffer, 0);
	if(state == vk::Result::eNotReady)
	{
		std::this_thread::yield();
		return;
	}

	if(imagesInFlight[currentBuffer] != -1) {
		fences.waitForFence(imagesInFlight[currentBuffer], fenceTimeout);
		fences.resetFence(currentFrame);
	}
	imagesInFlight[currentBuffer] = currentFrame;
	updateDescriptorSet();
	recordCommandBuffer();

	updateUniformBuffer(currentBuffer);
	commandBuffers[currentBuffer].submit(imageAcquiredSemaphores, renderFinishedSemaphores, fences[currentFrame], currentFrame);
	swapChain.presentKHR(logicalDevice, renderFinishedSemaphores, currentFrame, currentBuffer);
	currentFrame = (currentFrame+1)%MAX_FRAMES_IN_FLIGHT;
}

std::vector<magmatic::render::Vertex> Application::getVertexConfig(const std::string& mode) const {
	if(mode == "square") {
		return squareVertices;
	} else if(mode == "triangle") {
		return triangleVertices;
	} else if(mode == "hourglass") {
		return hourglassVertices;
	} else if(mode == "hourglass on square") {
		std::vector<magmatic::render::Vertex> res;
		res.reserve(hourglassVertices.size() + squareVertices.size());
		res.insert(res.end(), squareVertices.begin(), squareVertices.end());
		res.insert(res.end(), hourglassVertices.begin(), hourglassVertices.end());
		return res;
	} else {
		spdlog::error("Mode {} not implemented", mode);
		throw std::runtime_error("Mode '" + mode + "' is not yet implemented");
	}
}

std::vector<uint32_t> Application::getIndexConfig(const std::string& mode) const {
	if(mode == "square") {
		return squareIndices;
	} else if(mode == "triangle") {
		return triangleIndices;
	} else if(mode == "hourglass") {
		return hourglassIndices;
	} else if(mode == "hourglass on square") {
		std::vector<uint32_t> res;
		res.reserve(hourglassIndices.size() + squareIndices.size());
		res.insert(res.end(), squareIndices.begin(),squareIndices.end());
		for(uint32_t index : hourglassIndices) {
			res.emplace_back(index + static_cast<uint32_t>(squareVertices.size()));
		}
		return res;
	} else {
		spdlog::error("Mode {} not implemented", mode);
		throw std::runtime_error("Mode '" + mode + "' is not yet implemented");
	}
}
#include <tabulate/table.hpp>
#include <fmt/format.h>
#include "render/Instance.hpp"
#include "sound/SoundDevice.hpp"
#include "render/Monitor.hpp"
#include <vulkan/vulkan.hpp>

std::string decodeAPIVersion(uint32_t api_version)
{
	return std::to_string(VK_VERSION_MAJOR(api_version))
	+ "." + std::to_string(VK_VERSION_MINOR(api_version))
	+ "." + std::to_string(VK_VERSION_PATCH(api_version));
}

std::string decodeVendorID(uint32_t vendor_ID)
{
	switch (vendor_ID)
	{
		case 0x8086:
			return "Intel Corporation";
		case 0x1022:
			return "Advanced Micro Devices";
		case 0x10DE:
			return "NVidia Corporation";
		default:
			return "Unknown";
	}
}

template<typename T>
std::string toBoolalphaString(T val)
{
	return fmt::format("{}", static_cast<bool>(val));
}

tabulate::Table getDeviceSummary(const magmatic::render::PhysicalDevice& phys_device)
{
	tabulate::Table device_summary;
	tabulate::Table device_details;
	device_summary.add_row({fmt::format("Name: {}", phys_device.device_properties.deviceName)});

	device_details.add_row({"Type: ", vk::to_string(phys_device.device_properties.deviceType)});
	device_details.add_row({"Device ID: ", std::to_string(phys_device.device_properties.deviceID)});
	device_details.add_row({"Vendor name: ", decodeVendorID(phys_device.device_properties.vendorID)});
	device_details.add_row({"API version", decodeAPIVersion(phys_device.device_properties.apiVersion)});
	device_details.add_row({"Points: ", std::to_string(phys_device.calculateScore())});

	device_summary.add_row({device_details});
	device_summary[1].format().border_top("~");
	device_summary[1].format().font_align(tabulate::FontAlign::center);

	device_summary.add_row({"Features"});

	tabulate::Table device_features;
	device_features.add_row({
			                        "wide lines",
			                        toBoolalphaString(phys_device.device_features.wideLines)
	});
	device_features.add_row({
			                        "geometry shader",
			                        toBoolalphaString(phys_device.device_features.geometryShader)
	});
	device_features.add_row({
			                        "depth clamp",
			                        toBoolalphaString(phys_device.device_features.depthClamp)
	});
	device_features.add_row({
			                        "depth bias clamp",
			                        toBoolalphaString(phys_device.device_features.depthBiasClamp)
	});
	device_features.add_row({
			                        "logic op",
			                        toBoolalphaString(phys_device.device_features.logicOp)
	});
	device_features.add_row({
			                        "alpha to one",
			                        toBoolalphaString(phys_device.device_features.alphaToOne)
	});
	device_features.add_row({
			                        "depth bounds",
			                        toBoolalphaString(phys_device.device_features.depthBounds)
	});
	device_features.add_row({
			                        "large points",
			                        toBoolalphaString(phys_device.device_features.largePoints)
	});
	device_features.add_row({
			                        "fill mode non solid",
			                        toBoolalphaString(phys_device.device_features.fillModeNonSolid)
	});
	device_features.add_row({
			                        "texture compression ASTC_LDR",
			                        toBoolalphaString(phys_device.device_features.textureCompressionASTC_LDR)
	});
	device_features.add_row({
			                        "texture compression BC",
			                        toBoolalphaString(phys_device.device_features.textureCompressionBC)
	});
	device_features.add_row({
			                        "texture compression ETC2",
			                        toBoolalphaString(phys_device.device_features.textureCompressionETC2)
	});

	device_summary.add_row({device_features});
	device_summary[3].format().hide_border_top();
	device_summary[2].format().border_top("~");


	return device_summary;
}

void printDevicesSummary(const std::vector<magmatic::render::PhysicalDevice>& devices)
{
	tabulate::Table devices_summary;
	devices_summary.add_row({"Physical devices: ", std::to_string(devices.size())});
	devices_summary[0].format().font_align(tabulate::FontAlign::center);
	devices_summary[0].format().font_color(tabulate::Color::green);
	if(devices.size() >=2 )
	{
		for(size_t i = 0; i < devices.size(); i+=2)
		{
			devices_summary.add_row({getDeviceSummary(devices[i]), getDeviceSummary(devices[i+1])});
		}
	}
	if(devices.size()%2 != 0)
	{
		devices_summary.add_row({getDeviceSummary(devices[devices.size()-1])});
	}
	devices_summary[0].format().border_color(tabulate::Color::blue);
	devices_summary.format().border_color(tabulate::Color::blue);

	devices_summary[0].format().corner_color(tabulate::Color::blue);
	devices_summary.format().corner_color(tabulate::Color::blue);
	std::cout << devices_summary << std::endl;
}

void printSoundDevicesSummary()
{
	const auto enumerated_devices = magmatic::sound::SoundDevice::enumerateDevices();
	tabulate::Table devices_summary;
	devices_summary.add_row({"SoundLoader devices", std::to_string(enumerated_devices.size())});
	devices_summary[0].format().font_align(tabulate::FontAlign::center);
	devices_summary[0].format().font_color(tabulate::Color::green);
	devices_summary.format().border_color(tabulate::Color::cyan);
	devices_summary[0].format().border_color(tabulate::Color::cyan);
	devices_summary.format().corner_color(tabulate::Color::red);
	devices_summary[0].format().corner_color(tabulate::Color::red);
	for(size_t i = 0; i < enumerated_devices.size(); ++i)
	{
		devices_summary.add_row({std::to_string(i), enumerated_devices[i]});
	}
	std::cout<<devices_summary << std::endl;
}

void printMonitorsSummary()
{
	const auto monitors = magmatic::render::Monitor::getAll();
	tabulate::Table monitors_summary;
	monitors_summary.add_row({"Monitors:", std::to_string(monitors.size())});
	monitors_summary[0].format().font_align(tabulate::FontAlign::center);
	monitors_summary[0].format().font_color(tabulate::Color::green);
	monitors_summary.format().border_color(tabulate::Color::cyan);
	monitors_summary[0].format().border_color(tabulate::Color::cyan);
	monitors_summary.format().corner_color(tabulate::Color::red);
	monitors_summary[0].format().corner_color(tabulate::Color::red);
	for(size_t i = 0; i < monitors.size(); ++i)
	{
		const auto name = monitors[i].name();
		const auto size = monitors[i].physicalSize();
		const auto pos = monitors[i].position();
		const auto monitor_description = fmt::format(
				"{} Size: {}-{} mm Pos: {}-{} px",
				name,
				size.width,
				size.height,
				pos.x,
				pos.y
				);
		monitors_summary.add_row({std::to_string(i), monitor_description});
	}
	std::cout<< monitors_summary << std::endl;
}

int main() {
	glfwInit();
	magmatic::render::Instance instance("Device enumeration");

	auto physical_devices = instance.enumeratePhysicalDevices();

	printDevicesSummary(physical_devices);
	std::cout << "\n";
	printSoundDevicesSummary();
	printMonitorsSummary();
	glfwTerminate();
}
#include <tabulate/table.hpp>
#include "Instance.hpp"

tabulate::Table get_device_summary(const magmatic::PhysicalDevice& phys_device)
{
	tabulate::Table device_summary;
	device_summary.add_row({"Name: ", phys_device.device_properties.deviceName});
	device_summary.add_row({"Type: ", vk::to_string(phys_device.device_properties.deviceType)});
	device_summary.add_row({"Device ID: ", std::to_string(phys_device.device_properties.deviceID)});
	device_summary.add_row({"Points: ", std::to_string(phys_device.calculateScore())});


	return device_summary;
}

void print_devices_summary(const std::vector<magmatic::PhysicalDevice>& devices)
{
	tabulate::Table devices_summary;
	devices_summary.add_row({"Physical devices: ", std::to_string(devices.size())});
	devices_summary[0].format().hide_border_bottom().font_color(tabulate::Color::green);
	for(const auto& device : devices)
	{
		devices_summary.add_row({get_device_summary(device)});
	}
	std::cout << devices_summary << std::endl;
}

int main() {
	magmatic::Instance instance("Device enumeration");

	auto physical_devices = instance.enumeratePhysicalDevices();

	print_devices_summary(physical_devices);

}
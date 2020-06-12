#include "network/AddressIPv4.h"
#include <arpa/inet.h>
#include <stdexcept>

magmatic::network::AddressIPv4::AddressIPv4(const std::string& address_str, unsigned short port)
:address(parseAddress(address_str, port))
{

}

sockaddr_in magmatic::network::AddressIPv4::parseAddress(const std::string& address_str, unsigned short port)
{
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	int err = inet_pton(AF_INET, address_str.c_str(), &addr.sin_addr);
	if(err != 1)
	{
		throw std::invalid_argument("Invalid address format");
	}
	return addr;
}

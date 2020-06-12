#ifndef MAGMATIC_ADDRESSIPV4_H
#define MAGMATIC_ADDRESSIPV4_H

#include <netinet/in.h>
#include <string>


namespace magmatic::network
{
	class AddressIPv4
	{
	public:
		AddressIPv4(const std::string& address_str, unsigned short port);

		const sockaddr_in raw_address;

	private:
		static sockaddr_in parseAddress(const std::string& address_str, unsigned short port);
	};
}

#endif //MAGMATIC_ADDRESSIPV4_H

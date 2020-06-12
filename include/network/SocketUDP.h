#ifndef MAGMATIC_SOCKETUDP_H
#define MAGMATIC_SOCKETUDP_H

#include "AddressIPv4.h"

namespace magmatic::network
{
	class SocketUDP
	{
	public:
		explicit SocketUDP(const AddressIPv4 &address);

		SocketUDP(const SocketUDP&) = delete;
		SocketUDP &operator=(const SocketUDP&) = delete;

		SocketUDP(SocketUDP &&) = default;
		SocketUDP &operator=(SocketUDP &&) = default;

		~SocketUDP();

	private:
		int socket_fd;
	};
}

#endif //MAGMATIC_SOCKETUDP_H

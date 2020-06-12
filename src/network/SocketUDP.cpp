#include "network/SocketUDP.h"
#include <sys/socket.h>
#include <stdexcept>
#include <cstring>
#include <unistd.h>

magmatic::network::SocketUDP::SocketUDP(const AddressIPv4 &address)
{
	socket_fd = socket(address.raw_address.sin_family, SOCK_DGRAM, IPPROTO_UDP);
	if(socket_fd != 0)
	{
		throw std::runtime_error(strerror(errno));
	}

	const int err = bind(socket_fd, reinterpret_cast<const sockaddr*>(&address.raw_address), sizeof(address.raw_address));
	if(err != 0)
	{
		close(socket_fd);
		throw std::runtime_error(strerror(errno));
	}
}

magmatic::network::SocketUDP::~SocketUDP()
{
	shutdown(socket_fd, SHUT_RDWR);
	close(socket_fd);
}

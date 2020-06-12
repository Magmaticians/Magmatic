#include "network/SocketUDP.h"
#include <sys/socket.h>
#include <sys/fcntl.h>
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

void magmatic::network::SocketUDP::setBlockMode(bool mode)
{
	int flags = fcntl(socket_fd, F_GETFL, 0);
	if(flags == -1)
	{
		throw std::runtime_error(strerror(errno));
	}
	if(mode)
	{
		flags |= O_NONBLOCK;
	}
	else
	{
		flags &= ~O_NONBLOCK;
	}
	const int err = fcntl(socket_fd, F_SETFL, flags);
	if(err == -1)
	{
		throw std::runtime_error(strerror(errno));
	}

	block_mode = mode;
}

bool magmatic::network::SocketUDP::getBlockMode() noexcept
{
	return block_mode;
}

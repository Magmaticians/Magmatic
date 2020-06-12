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

		void setBlockMode(bool mode);
		[[nodiscard]] bool getBlockMode() noexcept;

	private:
		int socket_fd;
		bool block_mode = true;
	};
}

#endif //MAGMATIC_SOCKETUDP_H

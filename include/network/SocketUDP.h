#ifndef MAGMATIC_SOCKETUDP_H
#define MAGMATIC_SOCKETUDP_H


class SocketUDP
{
public:
	SocketUDP();

	SocketUDP(const SocketUDP&) = delete;
	SocketUDP& operator=(const SocketUDP&) = delete;

	SocketUDP(SocketUDP&&) = default;
	SocketUDP& operator=(SocketUDP&&) = default;


};


#endif //MAGMATIC_SOCKETUDP_H

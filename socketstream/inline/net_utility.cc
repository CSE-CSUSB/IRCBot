/*
 * net_utility.icc
 * Author: Mark Swoope
 * Date: July 2017
 */

#if defined(__linux__) || defined(__APPLE__)

#include <sys/socket.h>
#include <netdb.h>

inline int sockets_init()
{
	return 0;
}

inline int sockets_quit()
{
	return 0;
}

#elif defined(_WIN32)

#include <Winsock2.h>
#include <Ws2tcpip.h>

inline int sockets_init()
{
	WSADATA wd;
	return WSAStartup(MAKEWORD(2, 2), &wd) == 0 ? 0 : -1;
}

inline int sockets_quit()
{
	return WSACleanup == 0 ? 0 : -1;
}

#endif

inline socket_traits::socket_type make_connected_socket(int family,
						int socktype,
						int protocol,
						const std::string& node,
						const std::string& service)
{
	addrinfo hints, *ai;
	socket_traits::socket_type result{socket_traits::invalid()}, client;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_protocol = protocol;
	if (::getaddrinfo(node.c_str(), service.c_str(), &hints, &ai))
		return result;
	client = ::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (client != result && ::connect(client, ai->ai_addr,
					ai->ai_addrlen) == 0)
		result = client;
	::freeaddrinfo(ai);
	return result;
}

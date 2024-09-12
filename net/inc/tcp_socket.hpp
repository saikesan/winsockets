#pragma once
#ifndef TCP_SOCKET_HPP
#define TCP_SOCKET_HPP

#include "winsock_initializer.hpp"

namespace net {
	class tcp_socket{
	public:
		tcp_socket() {};

		tcp_socket(const char* address, const char* port) {
			init_addrinfo(address, port);
		}

		int init_addrinfo(const char* address, const char* port) {
			addrinfo* result;

			ZeroMemory(&hints_, sizeof(hints_));
			hints_.ai_family = AF_UNSPEC;
			hints_.ai_socktype = SOCK_STREAM;
			hints_.ai_protocol = IPPROTO_TCP;
			
			return getaddrinfo(address, port, &hints_, &result);
		}

		int send(const char* buf) {
			
		}

		~tcp_socket() {}

	private:
		tcp_socket(const tcp_socket&) = delete;
		tcp_socket& operator=(const tcp_socket&) = delete;

	private:
		SOCKET socket_ = INVALID_SOCKET;
		addrinfo hints_ = { 0 };
	};
}

#endif // !TCP_SOCKET_HPP
#pragma once
#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "socket_base.hpp"

namespace net {
	class server_socket : public socket_base {
		virtual void listen() = 0;
		virtual void accept() = 0;
		virtual void bind() = 0;
	};
}

#endif // !SERVER_SOCKET_HPP
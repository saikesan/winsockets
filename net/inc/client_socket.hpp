#pragma once
#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include "socket_base.hpp"

namespace net {

	class client_socket: public socket_base{
		virtual void connect() = 0;
	};
}

#endif // !CLIENT_SOCKET_HPP
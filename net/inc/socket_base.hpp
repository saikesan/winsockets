#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>

#include "winsock_initializer.hpp"

namespace net {
	class socket_base {
	public:
		virtual int send(const std::string&) const = 0;
		virtual int recv(std::string&) const = 0;
		virtual int shutdown() const = 0;

		virtual ~socket_base() = default;
	};
}

#endif // !SOCKET_HPP
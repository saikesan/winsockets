#pragma once
#ifndef WINSOCK_INITIALIZER_HPP
#define WINSOCK_INITIALIZER_HPP

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace net {

	class winsock_initializer{ // It's Singlton.
		bool is_initialized_ = false;
	
		winsock_initializer() {
			WSADATA wsa_data = { 0 };
			if (!WSAStartup(MAKEWORD(2, 2), &wsa_data))
				is_initialized_ = true;
		}

		~winsock_initializer() {
			if(is_initialized_)
				WSACleanup();
		}

		winsock_initializer(const winsock_initializer&) = delete;
		winsock_initializer& operator=(const winsock_initializer&) = delete;

	public:
		static winsock_initializer& init() {
			static winsock_initializer wsi;

			return wsi;
		}
	};

	winsock_initializer& wsi = winsock_initializer::init();
}

#endif // !WINSOCK_INITIALIZER_HPP
#pragma once
#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <mutex>
#include <thread>
#include <functional>

#include "server_socket.hpp"
#include "net_exception.hpp"

namespace net {
	class tcp_server : public server_socket { // Todo: need to add Executor.
	public:
		tcp_server(const std::string& port)
			: port_(port) {
		}

		void listen() override {
			bind();

			if ((::listen(listen_socket_, SOMAXCONN)) == SOCKET_ERROR)
				throw net_exception{ "listen() failed", WSAGetLastError() };

			server_thread_ = std::thread{&tcp_server::accept, this};
		}

		void accept() override {
			while (listen_socket_) {
				SOCKET client_socket = ::accept(listen_socket_, nullptr, nullptr);
				if (listen_socket_ == INVALID_SOCKET)
					__debugbreak();
				else if (listen_socket_ == NULL)
					__debugbreak();

				__debugbreak();
			}
		}

		/**
		* Send data to remote endpoint.
		*
		* @param data buffer.
		*
		* @return if successed return S_OK, else throw exception.
		*/
		int send(const std::string& data) const override {
			int sended = 0;

			if ((sended = ::send(listen_socket_, data.c_str(), data.length(), 0)) == SOCKET_ERROR)
				throw net_exception{ "send() failed", WSAGetLastError() };

			if (sended != data.length())
				throw net_exception{ "Not all data has been sended" };

			return S_OK;
		}

		/**
		* Receive data from remote endpoint.
		*
		* @param data buffer for data.
		*
		* @return if successed return S_OK, else throw exception.
		*/
		int recv(std::string& received_data) const override {
			int received = 0;
			const int recv_buffer_len = 1024 * 8;
			char recv_buffer[recv_buffer_len] = { 0 };

			do {
				if ((received = ::recv(listen_socket_, recv_buffer, recv_buffer_len, 0)) == SOCKET_ERROR)
					throw net_exception{ "recv() failed", WSAGetLastError() };

				if (received == 0)
					break;

				received_data.append(recv_buffer);

			} while (received > 0);

			return S_OK;
		}

		/**
		* Shutdown connection in both directions.
		*
		* @return if successed return S_OK, else throw exception.
		*/
		int shutdown() const override {
			if (::shutdown(listen_socket_, SD_BOTH) == SOCKET_ERROR)
				throw net_exception{ "shutdown() failed", WSAGetLastError() };

			return S_OK;
		}

		~tcp_server() {
			if (listen_socket_) {
				closesocket(listen_socket_);
				listen_socket_ = NULL;
			}

			server_thread_.join();
		}

	private:
		void bind() override {
			addrinfo* address = nullptr;

			ZeroMemory(&hints_, sizeof(hints_));
			hints_.ai_family = AF_UNSPEC;
			hints_.ai_socktype = SOCK_STREAM;
			hints_.ai_protocol = IPPROTO_TCP;
			hints_.ai_flags = AI_PASSIVE;

			if (getaddrinfo(nullptr, port_.c_str(), &hints_, &address) != 0)
				throw net_exception{ "getaddrinfo() failed", WSAGetLastError() };

			listen_socket_ = ::socket(address->ai_family, address->ai_socktype, address->ai_protocol);
			if (!listen_socket_) {
				freeaddrinfo(address);

				throw net_exception{ "socket() failed", WSAGetLastError() };
			}

			if ((::bind(listen_socket_, address->ai_addr, static_cast<int>(address->ai_addrlen))) == SOCKET_ERROR) {
				freeaddrinfo(address);
				closesocket(listen_socket_);

				throw net_exception{ "bind() failed", WSAGetLastError() };
			}

			freeaddrinfo(address);
		}

		tcp_server(const tcp_server&) = delete;
		tcp_server& operator=(const tcp_server&) = delete;

	private:
		std::thread	server_thread_;
		bool		is_listened_ = false;
		SOCKET		listen_socket_ = INVALID_SOCKET;
		addrinfo	hints_ = { 0 };
		std::string	port_;
	};
}

#endif // !TCP_SERVER_HPP
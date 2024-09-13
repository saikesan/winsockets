#pragma once
#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "client_socket.hpp"
#include "net_exception.hpp"

namespace net {
	class tcp_client : public client_socket{
	public:
		tcp_client(const std::string& address, const std::string& port)
			: rem_addr_(address)
			, rem_port_(port) {
		}
		
		/**
		* Connect to remote endpoint.
		*/
		void connect() override {
			addrinfo* addresses = nullptr;

			ZeroMemory(&hints_, sizeof(hints_));
			hints_.ai_family = AF_UNSPEC;
			hints_.ai_socktype = SOCK_STREAM;
			hints_.ai_protocol = IPPROTO_TCP;

			if (getaddrinfo(rem_addr_.c_str(), rem_port_.c_str(), &hints_, &addresses) != 0)
				throw net_exception{"getaddrinfo() failed", WSAGetLastError()};

			for (auto addr = addresses; addr != nullptr; addr = addr->ai_next) {
				socket_ = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
				if (!socket_) {
					freeaddrinfo(addresses);

					throw net_exception{ "socket() failed", WSAGetLastError() };
				}

				if (::connect(socket_, addr->ai_addr, static_cast<int>(addr->ai_addrlen)) == SOCKET_ERROR) {
					closesocket(socket_);
					socket_ = INVALID_SOCKET;
					continue;
				}

				break;
			}

			freeaddrinfo(addresses);

			if (socket_ == INVALID_SOCKET)
				throw net_exception{"can't connect."};

			is_connected_ = true;
		}

		/**
		* Send data to remote endpoint.
		* 
		* @param data buffer of data.
		* 
		* @return if successed return S_OK, else throw excpetion.
		*/
		int send(const std::string& data) const override {
			if(!is_connected_)
				throw net_exception{ "try send, but not connected to remote endpoint" };

			int sended = 0;

			if ((sended = ::send(socket_, data.c_str(), data.length(), 0)) == SOCKET_ERROR)
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
		* @return if successed return S_OK, else throw excpetion.
		*/
		int recv(std::string& received_data) const override{
			if (!is_connected_)
				throw net_exception{ "try recv, but not connected to remote endpoint" };

			int received = 0;
			const int recv_buffer_len = 1024 * 8;
			char recv_buffer[recv_buffer_len] = { 0 };

			do {
				if ((received = ::recv(socket_, recv_buffer, recv_buffer_len, 0)) == SOCKET_ERROR)
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
		* @return if successed return S_OK, else throw excpetion.
		*/
		int shutdown() const override {
			if (::shutdown(socket_, SD_BOTH) == SOCKET_ERROR)
				throw net_exception{ "shutdown() failed", WSAGetLastError() };

			return S_OK;
		}

		~tcp_client() {
			if (socket_) closesocket(socket_);
		}

	private:
		tcp_client(const tcp_client&) = delete;
		tcp_client& operator=(const tcp_client&) = delete;

	private:
		bool		is_connected_ = false;
		SOCKET		socket_ = INVALID_SOCKET;
		addrinfo	hints_ = { 0 };
		std::string	rem_addr_;
		std::string rem_port_;
	};
}

#endif // !TCP_CLIENT_HPP
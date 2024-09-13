#pragma once
#ifndef NET_EXCEPTION_HPP
#define NET_EXCEPTION_HPP

#include <exception>
#include <sstream>

namespace net {
	class net_exception : public std::exception {
	public:
		explicit net_exception(const char* what) noexcept {
			std::ostringstream oss;
			oss << "\nError: " << what << std::endl;
			msg_ = oss.str();
		}

		net_exception(const char* what, int code) noexcept {
			std::ostringstream oss;
			oss << "\nError: " << what << "\nCode: " << code << std::endl;
			msg_ = oss.str();
		}

		char const* what() const noexcept override {
			return msg_.empty() ? "Unknown exception\n" : msg_.c_str();
		}

	private:
		std::string msg_;
	};
}

#endif // !NET_EXCEPTION_HPP
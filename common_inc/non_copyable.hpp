#pragma once
#ifndef NON_COPYABLE_HPP
#define NON_COPYABLE_HPP

namespace utils {
	class non_copyable{
	public:
		non_copyable() = default;
		non_copyable(const non_copyable&) = delete;
		non_copyable& operator=(const non_copyable&) = delete;
	};
}

#endif // !NON_COPYABLE_HPP
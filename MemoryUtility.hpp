#ifndef MEMORY_UTILITY_HPP
#define MEMORY_UTILITY_HPP

#include <cstddef> // size_t

namespace SDA
{
	const std::size_t CalculateMemoryPadding(const std::size_t baseAddress, const std::size_t alignment);
}

#endif /* MEMORY_UTILITY_HPP */
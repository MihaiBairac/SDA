#include "MemoryUtility.hpp"

namespace SDA
{
	const std::size_t CalculateMemoryPadding(const std::size_t baseAddress, const std::size_t alignment)
	{
		const std::size_t multiplier = (baseAddress / alignment) + 1;
		const std::size_t alignedAddress = multiplier * alignment;
		const std::size_t padding = alignedAddress - baseAddress;

		return padding;
	}
}
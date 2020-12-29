#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <cstddef> // size_t
#include <cassert>
#include "Vector.hpp"

namespace SDA
{
	/* Suit of search algorithms */

	/* NOTE! Container must be sorted first in an increasing order! */

	template <class T>
	size_t BinarySearch(const SDA::Vector<T>& container, const T& val, size_t startIdx, size_t endIdx)
	{
		assert(startIdx < container.Size());
		assert(endIdx < container.Size());
		assert(container.Size() >= 2);
		assert(container[1] > container[0]);

		size_t halfIdx = (endIdx - startIdx + 1) / 2;

		if (container[halfIdx] == val)
		{
			return halfIdx;
		}
		else if (val < container[halfIdx])
		{
			return BinarySearch(container, val, startIdx, halfIdx);
		}
		else if (val > container[halfIdx])
		{
			return BinarySearch(container, val, halfIdx, endIdx);
		}
	}
}

#endif /* SEARCH_HPP */
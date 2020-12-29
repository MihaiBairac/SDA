#include "LiniarAllocator.hpp"
#include "MemoryUtility.hpp"
#include <cstdlib> // malloc(), free()
#include <algorithm> // max
#include <cassert>

namespace SDA
{
	LiniarAllocator::LiniarAllocator()
		: Allocator(), mMemoryBuffer(nullptr), mOffset(0)
	{}

	LiniarAllocator::LiniarAllocator(const std::size_t totalSize)
		: Allocator(totalSize), mMemoryBuffer(nullptr), mOffset(0)
	{}

	LiniarAllocator::~LiniarAllocator()
	{
		if (mMemoryBuffer)
		{
			free(mMemoryBuffer);
		}
		mMemoryBuffer = nullptr;
		mOffset = 0;
	}

	void LiniarAllocator::Init()
	{
		mMemoryBuffer = malloc(mTotalSize);
	}

	void LiniarAllocator::Reset()
	{
		mOffset = 0;
		mUsed = 0;
		mPeak = 0;
	}

	void* LiniarAllocator::Allocate(const std::size_t size, const std::size_t alignment)
	{
		assert(size > 0);

		const std::size_t currentAddress = (std::size_t)mMemoryBuffer + mOffset;

		// in case alignment is needed we find the correct padding
		std::size_t padding = 0;
		if ((alignment > 0) && (mOffset % alignment != 0))
		{
			padding = SDA::CalculateMemoryPadding(currentAddress, alignment);
		}

		// check if there is space left to allocate
		if (mOffset + padding + size > mTotalSize)
		{
			return nullptr;
		}

		const std::size_t nextAvailableAddress = currentAddress + padding;

		mOffset += (size + padding);
		mUsed = mOffset;
		mPeak = std::max(mPeak, mUsed);

		return (void*)nextAvailableAddress;
	}

	void LiniarAllocator::Free(void* ptr)
	{
		// This allocator does not support this operation!
		assert(false && "Use Reset() method");
	}
}

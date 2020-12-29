#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

/* 
   info: 
   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html
   https://github.com/mtrebi/memory-allocators
*/

#include "ClassHelper.h"
#include <cstddef> // size_t

namespace SDA
{
	/* Base class for all Allocators */
	class Allocator
	{
	public:
		Allocator()
			: mTotalSize(0), mUsed(0), mPeak(0)
		{}

		Allocator(const std::size_t totalSize)
			: mTotalSize(totalSize), mUsed(0), mPeak(0)
		{}

		virtual ~Allocator()
		{
			mTotalSize = 0;
			mUsed = 0;
			mPeak = 0;
		}

		std::size_t TotalSize() { return mTotalSize; }
		std::size_t Used() { return mUsed; }
		std::size_t Peak() { return mPeak; }

		virtual void Init() = 0;
		virtual void Reset() = 0;

		virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) = 0;
		virtual void Free(void* ptr) = 0;

	protected:
		std::size_t mTotalSize;
		std::size_t mUsed;
		std::size_t mPeak;

	private:
		NON_COPY_AND_MOVE(Allocator)
	};
}
#endif /* LINIAR_ALLOCATOR_HPP */
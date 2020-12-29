#ifndef LINIAR_ALLOCATOR_HPP
#define LINIAR_ALLOCATOR_HPP

#include "Allocator.hpp"
#include <cstddef> // size_t

namespace SDA
{
	class LiniarAllocator : public Allocator
	{
	public:
		LiniarAllocator();
		LiniarAllocator(const std::size_t totalSize);
		virtual ~LiniarAllocator();

		void Init();
		void Reset();

		void* Allocate(const std::size_t size, const std::size_t alignment);
		void Free(void* ptr);

	private:
		NON_COPY_AND_MOVE(LiniarAllocator)

		void* mMemoryBuffer;
		std::size_t mOffset;
	};
}

#endif /* LINIAR_ALLOCATOR_HPP */
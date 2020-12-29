#ifndef MEMORY_BENCHMARK_HPP
#define MEMORY_BENCHMARK_HPP

#include "ClassHelper.h"
#include <cstddef> // size_t
#include "Timer.hpp"

namespace SDA
{
	class Allocator;

	class MemoryBenchmark
	{
	public:
		MemoryBenchmark();
		MemoryBenchmark(const std::size_t operationCount);
		virtual ~MemoryBenchmark();

		void SingleAllocation(Allocator* allocatorPtr, const std::size_t size, const std::size_t alignment);
		void SingleFree(Allocator* allocatorPtr, const std::size_t size, const std::size_t alignment);

		void CollectResults(Timer::long_t elapsedTime, const std::size_t memoryPeak);

	private:
		NON_COPY_AND_MOVE(MemoryBenchmark)

		std::size_t mOperationCount;
		SDA::Timer mTimer;
	};
}
#endif /* MEMORY_BENCHMARK_HPP */
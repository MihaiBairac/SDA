#include "MemoryBenchmark.hpp"
#include "Allocator.hpp"
#include <cassert>
#include <cstddef> // size_t
#include <iostream>

namespace SDA
{
	MemoryBenchmark::MemoryBenchmark()
		: mOperationCount(0), mTimer()
	{}

	MemoryBenchmark::MemoryBenchmark(const std::size_t operationCount)
		: mOperationCount(operationCount)
	{}

	MemoryBenchmark::~MemoryBenchmark()
	{}

	void MemoryBenchmark::SingleAllocation(Allocator* allocatorPtr, const std::size_t size, const std::size_t alignment)
	{
		assert(nullptr != allocatorPtr);

		mTimer.Start();

		allocatorPtr->Init();

		for (std::size_t operation = 0; operation < mOperationCount; ++ operation)
		{
			allocatorPtr->Allocate(size, alignment);
		}


		mTimer.Stop();

		CollectResults(mTimer.ElapsedTimeInMiliseconds(), allocatorPtr->Peak());
	}

	void MemoryBenchmark::SingleFree(Allocator* allocatorPtr, const std::size_t size, const std::size_t alignment)
	{
		assert(nullptr != allocatorPtr);

		void** allocatedMemory = new void*[mOperationCount];

		allocatorPtr->Init();

		for (std::size_t operation = 0; operation < mOperationCount; ++operation)
		{
			allocatedMemory[operation] = allocatorPtr->Allocate(size, alignment);
		}

		for (std::size_t operation = 0; operation < mOperationCount; ++operation)
		{
			allocatorPtr->Free(allocatedMemory[operation]);
		}

		delete[] allocatedMemory;
	}

	void MemoryBenchmark::CollectResults(Timer::long_t elapsedTime, const std::size_t memoryPeak)
	{
		float operationsPerSecond = static_cast<float>(mOperationCount) / elapsedTime;
		float timePerOperation = static_cast<float>(elapsedTime) / mOperationCount;

		// Print results
		std::cout << "---------- BENCHMARK --------- " << std::endl;
		std::cout << "Operations per sec: " << operationsPerSecond << std::endl;
		std::cout << "Time per Operation: " << timePerOperation << std::endl;
		std::cout << "Memory peak: " << memoryPeak << std::endl;
		std::cout << "---------- BENCHMARK --------- " << std::endl;
	}
}
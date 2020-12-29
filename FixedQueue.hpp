#ifndef FIXED_QUEUE_HPP
#define FIXED_QUEUE_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/* A Fixed size Stack - standard FIFO container

TIME COMPLEXITY:
- Traversal/Search = O(n)
- Add/Delete an element = O(1) at best or at worst
- Access an element = O(1)
- Get size = O(1)

SPACE COMPLEXITY:
- O(N) as we store n elements in a contiguos buffer

ADVANTAGES:
- very fast random access compared to DynamicQueue
- cache friendly

DISADVANTAGES:
- fixed size

USAGES:
Several algorithms like:
- When a resource is shared among multiple consumers. Examples include CPU scheduling, Disk Scheduling.
- When data is transferred asynchronously (data not necessarily received at same rate as sent) between two processes.
Examples include IO Buffers, pipes, file IO, etc.


*/

namespace SDA
{
	template <class T>
	class FixedQueue
	{
	public:
		FixedQueue();
		FixedQueue(size_t capacity);
		FixedQueue(const FixedQueue<T>& queue);
		FixedQueue(FixedQueue<T>&& queue);
		virtual ~FixedQueue();

		FixedQueue<T>& operator =(const FixedQueue<T>& queue);
		FixedQueue<T>& operator =(FixedQueue<T>&& queue);

		T& Front();
		const T& Front() const;
		T& Back();
		const T& Back() const;

		bool IsEmpty() const;
		bool IsFull() const;

		size_t Capacity() const;
		size_t Size() const;

		void PushBack(const T& val);
		void PopFront();

		void Swap(FixedQueue<T>& queue);

	private:
		void Copy(const FixedQueue<T>& queue);
		void Move(FixedQueue<T>&& queue);
		void Destroy();

		T* mBuffer;
		size_t mCapacity;
		size_t mSize;

		static const size_t DEFAULT_CAPACITY = 100;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	FixedQueue<T>::FixedQueue()
		: mBuffer(nullptr), mCapacity(DEFAULT_CAPACITY), mSize(0)
	{
		mBuffer = new T[mCapacity];
	}

	template <class T>
	FixedQueue<T>::FixedQueue(size_t capacity)
		: mCapacity(capacity), mSize(0)
	{
		mBuffer = new T[mCapacity];
	}

	template <class T>
	FixedQueue<T>::FixedQueue(const FixedQueue<T>& queue)
	{
		Copy(queue);
	}

	template <class T>
	FixedQueue<T>::FixedQueue(FixedQueue<T>&& queue)
	{
		Move(queue);
	}

	template <class T>
	FixedQueue<T>::~FixedQueue()
	{
		Destroy();
	}

	template <class T>
	void FixedQueue<T>::Copy(const FixedQueue<T>& queue)
	{
		if (this != &queue)
		{
			// delete the old buffer
			Destroy();

			mCapacity = queue.mCapacity;
			mSize = queue.mSize;

			mBuffer = new T[mCapacity];

			for (size_t i = 0; i < mSize; ++i)
			{
				// here we copy the elements as we do not want
				// to invalidate vec elements by moving them to this vec 
				mBuffer[i] = queue.mBuffer[i];
			}
		}
	}

	template <class T>
	void FixedQueue<T>::Move(FixedQueue<T>&& queue)
	{
		if (this != &queue)
		{
			// delete the old buffer
			Destroy();

			mCapacity = queue.mCapacity;
			mSize = queue.mSize;

			// just copy the pointer
			mBuffer = queue.mBuffer;

			// vec is invalidated after move
			queue.mBuffer = nullptr;
			queue.mCapacity = 0;
			queue.mSize = 0;
		}
	}

	template <class T>
	void FixedQueue<T>::Destroy()
	{
		if (mBuffer)
		{
			delete[] mBuffer;
		}

		mCapacity = 0;
		mSize = 0;
	}

	template <class T>
	FixedQueue<T>& FixedQueue<T>::operator =(const FixedQueue<T>& queue)
	{
		Copy(queue);

		return *this;
	}

	template <class T>
	FixedQueue<T>& FixedQueue<T>::operator =(FixedQueue<T>&& queue)
	{
		Move(queue);

		return *this;
	}

	template <class T>
	T& FixedQueue<T>::Front()
	{
		assert(mSize > 0);

		return mBuffer[0];
	}

	template <class T>
	const T& FixedQueue<T>::Front() const
	{
		assert(mSize > 0);

		return mBuffer[0];
	}

	template <class T>
	T& FixedQueue<T>::Back()
	{
		assert(mSize > 0);

		return mBuffer[mSize - 1];
	}

	template <class T>
	const T& FixedQueue<T>::Back() const
	{
		assert(mSize > 0);

		return mBuffer[mSize - 1];
	}

	template <class T>
	bool FixedQueue<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <class T>
	bool FixedQueue<T>::IsFull() const
	{
		return mSize == mCapacity;
	}

	template <class T>
	size_t FixedQueue<T>::Capacity() const
	{
		return mCapacity;
	}

	template <class T>
	size_t FixedQueue<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void FixedQueue<T>::PushBack(const T& val)
	{
		if (false == IsFull())
		{
			mBuffer[mSize++] = val;
		}
	}

	template <class T>
	void FixedQueue<T>::PopFront()
	{
		if (false == IsEmpty())
		{
			// destroy first element
			mBuffer[0].~T();

			// shift elements to left with 1 position
			for (size_t i = 0; i < mSize; ++i)
			{
				// better move them then copy
				mBuffer[i] = std::move(mBuffer[i + 1]);
			}

			--mSize;
		}

	}

	template <class T>
	void FixedQueue<T>::Swap(FixedQueue<T>& queue)
	{
		SDA::Swap(mBuffer, queue.mBuffer);
		SDA::Swap(mCapacity, queue.mCapacity);
		SDA::Swap(mSize, queue.mSize);
	}

	/////////// OUTPUT /////////////
	template <class T>
	std::ostream& operator << (std::ostream& out, const FixedQueue<T>& queue)
	{

		// cannot acces elements freely so we copy the queue and popfront from it each element
		FixedQueue<T> queue2(queue);
		// we cache the size as it changes per pop()
		size_t initSize = queue2.Size();

		std::cout << "queue: ";
		for (size_t i = 0; i < initSize; ++i)
		{
			out << queue2.Front() << " ";
			queue2.PopFront();
		}
		std::cout << std::endl;

		return out;
	}
}

#endif /* FIXED_QUEUE_HPP */
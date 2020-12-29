#ifndef DYNAMIC_QUEUE_HPP
#define DYNAMIC_QUEUE_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/*
DynamicQueue - FIFO container, but dynamic in size
Based on SinglyLinkedList

TIME COMPLEXITY:
- Traversal/Search = O(n)
- Add/Delete an element = O(1)
- Random access an element = O(n)
- Get size = O(1) or O(n) implementation dependent

SPACE COMPLEXITY:
- O(N) as we store n elements + pointers

ADVANTAGES:
- dynamic compared to FixedQueue
- really fast insertion or deletion compared to FixedQueue

DISADVANTAGES:
- random access is expensive
- non cache friendly compared to FixedQueue
- extra memory needed for node connections (pointers)

USAGES:
Several algorithms like:
- When a resource is shared among multiple consumers. Examples include CPU scheduling, Disk Scheduling.
- When data is transferred asynchronously (data not necessarily received at same rate as sent) between two processes.
Examples include IO Buffers, pipes, file IO, etc.

Model:
Head -> Node -> Node -> Tail 

*/
namespace SDA
{
	template <class T>
	struct QueueNode
	{
		QueueNode()
			: data(), nextPtr(nullptr)
		{}

		QueueNode(const T& data_)
			: data(data_), nextPtr(nullptr)
		{}

		QueueNode(const QueueNode<T>& node)
			: data(node.data), nextPtr(node.nextPtr)
		{}

		~QueueNode()
		{
			nextPtr = nullptr;
		}

		T data;
		QueueNode<T>* nextPtr;
	};

	template <class T>
	class DynamicQueue
	{
	public:
	

		DynamicQueue();
		DynamicQueue(const DynamicQueue<T>& queue);
		DynamicQueue(DynamicQueue<T>&& queue);
		virtual ~DynamicQueue();

		DynamicQueue<T>& operator =(const DynamicQueue<T>& queue);
		DynamicQueue<T>& operator =(DynamicQueue<T>&& queue);

		QueueNode<T>* First();
		QueueNode<T>* Last();

		size_t Size() const;

		void PushBack(const T& val);
		void PopFront();

	private:
		void Copy(const DynamicQueue<T>& queue);
		void Move(DynamicQueue<T>&& queue);
		void Destroy();

		QueueNode<T> *mHeadPtr, *mTailPtr;
		size_t mSize;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	DynamicQueue<T>::DynamicQueue()
		: mHeadPtr(nullptr), mTailPtr(nullptr), mSize(0)
	{}

	template <class T>
	DynamicQueue<T>::DynamicQueue(const DynamicQueue<T>& queue)
	{
		Copy(queue);
	}

	template <class T>
	DynamicQueue<T>::DynamicQueue(DynamicQueue<T>&& queue)
	{
		Move(queue);
	}

	template <class T>
	DynamicQueue<T>::~DynamicQueue()
	{
		Destroy();
	}

	template <class T>
	void DynamicQueue<T>::Copy(const DynamicQueue<T>& queue)
	{
		if (this != &queue)
		{
			Destroy();

			QueueNode<T>* crrNodePtr = nullptr;
			for (crrNodePtr = queue.mHeadPtr; crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
			{
				Push(crrNodePtr->data);
			}
		}
	}

	template <class T>
	void DynamicQueue<T>::Move(DynamicQueue<T>&& queue)
	{
		if (this != &queue)
		{
			Destroy();

			mSize = queue.mSize;

			mHeadPtr = queue.mHeadPtr;
			mTailPtr = queue.mTailPtr;

			queue.mHeadPtr = nullptr;
			queue.mTailPtr = nullptr;
		}
	}

	template <class T>
	void DynamicQueue<T>::Destroy()
	{
		QueueNode<T>* crrNode = mHeadPtr;
		while (crrNode != nullptr)
		{
			mHeadPtr = crrNode->nextPtr;
			delete crrNode;
			crrNode = mHeadPtr;
		}

		mHeadPtr = nullptr;
		mTailPtr = nullptr;
		mSize = 0;
	}

	template <class T>
	DynamicQueue<T>& DynamicQueue<T>::operator =(const DynamicQueue<T>& queue)
	{
		Copy(queue);

		return *this;
	}

	template <class T>
	DynamicQueue<T>& DynamicQueue<T>::operator =(DynamicQueue<T>&& queue)
	{
		Move(queue);

		return *this;
	}

	template <class T>
	QueueNode<T>* DynamicQueue<T>::First()
	{
		return mHeadPtr;
	}

	template <class T>
	QueueNode<T>* DynamicQueue<T>::Last()
	{
		return mTailPtr;
	}

	template <class T>
	size_t DynamicQueue<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void DynamicQueue<T>::PushBack(const T& val)
	{
		QueueNode<T>* newNodePtr = new QueueNode<T>(val);

		if (mTailPtr)
		{
			mTailPtr->nextPtr = newNodePtr;
			mTailPtr = newNodePtr;
		}
		else
			mTailPtr = newNodePtr;

		++mSize;
	}

	template <class T>
	void DynamicQueue<T>::PopFront()
	{
		assert(mSize == 0);

		QueueNode<T>* nodeToDeletePtr = mHeadPtr;

		if (mHeadPtr)
		{
			mHeadPtr = mHeadPtr->nextPtr;
		}

		delete nodeToDeletePtr;
		--mSize;
	}

	/////////// OUTPUT /////////////
	template <class T>
	std::ostream& operator << (std::ostream& out, const DynamicQueue<T>& queue)
	{
		std::cout << "queue: ";
		//DynamicQueue<T>::template QueueNode<T>* crrNodePtr = nullptr;
		QueueNode<T>* crrNodePtr = nullptr;
		for (crrNodePtr = queue.First(); crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
		{
			out << crrNodePtr->data << " ";
		}
		std::cout << std::endl;

		return out;
	}
}

#endif /* DYNAMIC_QUEUE_HPP */
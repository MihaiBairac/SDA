#ifndef CIRCULAR_SINGLY_LINKED_LIST_HPP
#define CIRCULAR_SINGLY_LINKED_LIST_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/*
CircularSinglyLinkedList - a type of collection/container to store elements of the same type
 dynamically, in a non-contiguos way. It's circular.

TIME COMPLEXITY:
- Traversal/Search = O(n)
- Add/Delete an element = O(1) at best or at worst
- Access an element = O(n)
- Get size = O(1) or O(n) implementation dependent

SPACE COMPLEXITY:
- O(N) as we store n elements + pointers

ADVANTAGES:
- dynamic over Arrays
- very fast insertion/deletion compared to Vectors
- any element can be the starting one

DISADVANTAGES:
- random access is expensive
- non cache friendly compared to Vectors, Arrays
- extra memory needed for node connections (pointers)

USAGES:
- when you need to store elements of the same type, but don't know the size of the input data
- when you need very fast, insertion and deletion
- Implementation of other data structures:
a) Queues
b) Fibonacci Heap
c) OS process scheduling (it's convenitent to use a circular list of applications to choose from)

*/

namespace SDA
{
	template <class T>
	struct CircularSinglyLinkedListNode
	{
		CircularSinglyLinkedListNode()
			: data(), nextPtr(nullptr)
		{}

		CircularSinglyLinkedListNode(const T& data_)
			: data(data_), nextPtr(nullptr)
		{}

		CircularSinglyLinkedListNode(const CircularSinglyLinkedListNode<T>& node)
			: data(node.data), nextPtr(node.nextPtr)
		{}

		~CircularSinglyLinkedListNode()
		{
			nextPtr = nullptr;
		}

		T data;
		CircularSinglyLinkedListNode<T>* nextPtr;
	};

	template <class T>
	class CircularSinglyLinkedList
	{
	public:
		CircularSinglyLinkedList();
		CircularSinglyLinkedList(const CircularSinglyLinkedList<T>& list);
		CircularSinglyLinkedList(CircularSinglyLinkedList<T>&& list);
		virtual ~CircularSinglyLinkedList();

		CircularSinglyLinkedList<T>& operator =(const CircularSinglyLinkedList<T>& list);
		CircularSinglyLinkedList<T>& operator =(CircularSinglyLinkedList<T>&& list);

		CircularSinglyLinkedListNode<T>* First() const;

		size_t Size() const;

		void InsertFirst(const T& val);
		void InsertLast(const T& val);
		void Insert(size_t index, const T& val);

		void EraseFirst();
		void EraseLast();
		void Erase(size_t index);

		void Reverse();
		void InsertionSort();

	private:
		void Copy(const CircularSinglyLinkedList<T>& list);
		void Move(CircularSinglyLinkedList<T>&& list);
		void Destroy();

		CircularSinglyLinkedList<T>* mHeadPtr;
		size_t mSize;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	CircularSinglyLinkedList<T>::CircularSinglyLinkedList()
		: mHeadPtr(nullptr), mSize(0)
	{}

	template <class T>
	CircularSinglyLinkedList<T>::CircularSinglyLinkedList(const CircularSinglyLinkedList<T>& list)
	{
		Copy(list);
	}

	template <class T>
	CircularSinglyLinkedList<T>::CircularSinglyLinkedList(CircularSinglyLinkedList<T>&& list)
	{
		Move(list);
	}

	template <class T>
	CircularSinglyLinkedList<T>::~CircularSinglyLinkedList()
	{
		Destroy();
	}

	template <class T>
	void CircularSinglyLinkedList<T>::Copy(const CircularSinglyLinkedList<T>& list)
	{
		if (this != &list)
		{
			Destroy();

			CircularSinglyLinkedListNode<T>* crrNodePtr = list.mHeadPtr;
			while (crrNodePtr && (crrNodePtr->nextPtr != list.mHeadPtr))
			{
				InsertLast(crrNodePtr->data);

				crrNodePtr = crrNodePtr->nextPtr;
			}

			if (crrNodePtr)
				InsertLast(crrNodePtr->data);
		}
	}

	template <class T>
	void CircularSinglyLinkedList<T>::Move(CircularSinglyLinkedList<T>&& list)
	{
		if (this != &list)
		{
			Destroy();

			mSize = list.mSize;

			mHeadPtr = list.mHeadPtr;
			list.mHeadPtr = nullptr;
		}
	}

	template <class T>
	void CircularSinglyLinkedList<T>::Destroy()
	{
		CircularSinglyLinkedListNode<T>* crrNodePtr = mHeadPtr;
		while (crrNode && (crrNodePtr->nextPtr != mHeadPtr))
		{
			mHeadPtr = crrNodePtr->nextPtr;
			delete crrNodePtr;
			crrNodePtr = mHeadPtr;
		}

		if (crrNodePtr)
			delete crrNodePtr;

		mHeadPtr = nullptr;
		mSize = 0;
	}

	template <class T>
	CircularSinglyLinkedList<T>& CircularSinglyLinkedList<T>::operator =(const CircularSinglyLinkedList<T>& list)
	{
		Copy(list);

		return *this;
	}

	template <class T>
	CircularSinglyLinkedList<T>& CircularSinglyLinkedList<T>::operator =(CircularSinglyLinkedList<T>&& list)
	{
		Move(list);

		return *this;
	}

	template <class T>
	CircularSinglyLinkedListNode<T>* CircularSinglyLinkedList<T>::First() const
	{
		return mHeadPtr;
	}

	template <class T>
	size_t CircularSinglyLinkedList<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void CircularSinglyLinkedList<T>::InsertFirst(const T& val)
	{
		CircularSinglyLinkedListNode<T>* newNodePtr = new CircularSinglyLinkedListNode<T>(val);

		// we can insert even if the lsit is empty

		if (mHeadPtr)
		{
			newNodePtr->nextPtr = mHeadPtr;
			mHeadPtr = newNodePtr;
		}
		else
		{
			mHeadPtr = newNodePtr;
			newNodePtr->nextPtr = mHeadPtr;
		}

		++mSize;
	}

	template <class T>
	void CircularSinglyLinkedList<T>::InsertLast(const T& val)
	{
		CircularSinglyLinkedListNode<T>* newNodePtr = new CircularSinglyLinkedListNode<T>(val);

		if (mHeadPtr)
		{
			CircularSinglyLinkedListNode<T>* crrNodePtr = mHeadPtr;

			while (crrNodePtr && (crrNodePtr->nextPtr != mHeadPtr))
			{
				crrNodePtr = crrNodePtr->nextPtr;
			}

			crrNodePtr->nextPtr = newNodePtr;
			newNodePtr->nextPtr = mHeadPtr;
		}
		else
		{
			mHeadPtr = newNodePtr;
			newNodePtr->nextPtr = mHeadPtr;
		}

		++mSize;
	}

	template <class T>
	void CircularSinglyLinkedList<T>::Insert(size_t index, const T& val)
	{
		assert(index < mSize);

		if (index == 0) // first
		{
			InsertFirst(val);
		}
		else if (index == mSize - 1) // last
		{
			InsertLast(val);
		}
		else // in between
		{
			CircularSinglyLinkedListNode<T>* newNodePtr = new CircularSinglyLinkedListNode<T>(val);

			size_t idx = -1; // we start with an an index before to arrive at the node before the one we must insert the new node 
			CircularSinglyLinkedListNode<T>* beforeNode = nullptr;
			for (beforeNode = mHeadPtr; beforeNode->nextPtr != mHeadPtr; beforeNode = beforeNode->nextPtr)
			{
				if (idx == index) // the index of the node before the one to add
				{
					if (beforeNode->nextPtr)
					{
						newNodePtr->nextPtr = beforeNode->nextPtr;
					}
					beforeNode->nextPtr = newNodePtr;

					break;
				}

				++idx;
			}

			++mSize;
		}
	}

	template <class T>
	void CircularSinglyLinkedList<T>::EraseFirst()
	{
		assert(mSize > 0);

		CircularSinglyLinkedListNode<T>* nodeToDeletePtr = mHeadPtr;

		mHeadPtr = mHeadPtr->nextPtr;

		nodeToDeletePtr->nextPtr = nullptr;
		delete nodeToDeletePtr;
		--mSize;
	}

	template <class T>
	void CircularSinglyLinkedList<T>::EraseLast()
	{
		assert(mSize > 0);

		CircularSinglyLinkedListNode<T>* nodeToDeletePtr = nullptr;

		if (mSize == 1)
		{
			nodeToDeletePtr = mHeadPtr;
			mHeadPtr = nullptr;
		}
		else if (mSize == 2)
		{
			nodeToDeletePtr = mHeadPtr->nextPtr;

			mHeadPtr->nextPtr = mHeadPtr;
		}
		else
		{
			CircularSinglyLinkedListNode<T>* crrNodePtr = mHeadPtr;
			while (crrNodePtr->nextPtr->nextPtr != mHeadPtr))
			{
				crrNodePtr = crrNodePtr->nextPtr;
			}

			nodeToDeletePtr = crrNodePtr->nextPtr;

			crrNodePtr->nextPtr = mHeadPtr;
		}

		nodeToDeletePtr->nextPtr = nullptr;
		delete nodeToDeletePtr;
		--mSize;
	}

	template <class T>
	void CircularSinglyLinkedList<T>::Erase(size_t index)
	{
		assert(index < mSize);

		CircularSinglyLinkedListNode<T>* nodeToDeletePtr = nullptr;

		if (index == 0) // first
		{
			EraseFirst();
		}
		else if (index == mSize - 1) // last
		{
			EraseLast();
		}
		else // in between
		{
			size_t idx = -1; // we start with an an index before to arrive at the node before the one we must insert the new node 
			CircularSinglyLinkedListNode<T>* beforeNodePtr = nullptr;
			for (beforeNodePtr = mHeadPtr; beforeNodePtr != nullptr; beforeNodePtr = beforeNodePtr->nextPtr)
			{
				if (idx == index) // the index of the node before the one to add
				{
					nodeToDeletePtr = beforeNodePtr->nextPtr;
					beforeNodePtr->nextPtr = beforeNodePtr->nextPtr->nextPtr;

					break;
				}

				++idx;
			}

			nodeToDeletePtr->nextPtr = nullptr;
			delete nodeToDeletePtr;
			--mSize;
		}
	}

	template <class T>
	void CircularSinglyLinkedList<T>::Reverse()
	{
		/* Idea:
			We use one extra pointer called reversed to which we add the elements from the initial list (in order)

			Time complexity: O(n)
			Space complexity O(1)
		*/

		CircularSinglyLinkedListNode<T>* reversedHeadPtr = mHeadPtr;
		reversedHeadPtr->nextPtr = nullptr; // last elements of the reversed list

		// in case the original list has no elements or only one
		if ((mHeadPtr == nullptr) || (mHeadPtr->nextPtr == nullptr))
		{
			return;
		}

		CircularSinglyLinkedListNode<T>* remainingListPtr = mHeadPtr->next; // next element in the line

		while (remainingListPtr != nullptr)
		{
			SinglyLinkedListNode<T>* crrNodePtr = remainingListPtr;
			remainingListPtr = remainingListPtr->nextPtr; // remaining ptr moves forward

			// link the crrNode to the reversed one
			crrNodePtr->nextPtr = reversedHeadPtr;
			reversedHeadPtr = crrNodePtr;
		}

		mHeadPtr = reversedHeadPtr;
	}

	template <class T>
	void CircularSinglyLinkedList<T>::InsertionSort()
	{
		/* Idea:
			We use one extra pointer called reversed to which we add the elements from the initial list (in order)

			Time complexity: O(n)
			Space complexity O(1)
		*/

		CircularSinglyLinkedListNode<T>* sortedHeadPtr = mHeadPtr;

		CircularSinglyLinkedListNode<T>* remainingListPtr = mHeadPtr->next; // next element in the line

		while (remainingListPtr != nullptr)
		{
			CircularSinglyLinkedListNode<T>* crrNodePtr = remainingListPtr;
			remainingListPtr = remainingListPtr->nextPtr; // remaining ptr moves forward

			// decresing order sort
			if (crrNodePtr->data < sortedHeadPtr->data)
			{
				// insert before
				crrNodePtr->nextPtr = sortedHeadPtr;

				sortedHeadPtr = crrNodePtr;
			}
			else
			{
				// insert after
				sortedHeadPtr->nextPtr = crrNodePtr;
			}
		}

		mHeadPtr = sortedHeadPtr;
	}

	/////////// OUTPUT /////////////
	template <class T>
	std::ostream& operator << (std::ostream& out, const CircularSinglyLinkedList<T>& list)
	{
		std::cout << "list: ";
		//CircularSinglyLinkedList<T>::template CircularSinglyLinkedListNode<T>* crrNodePtr = nullptr;
		CircularSinglyLinkedListNode<T>* crrNodePtr = nullptr;
		for (crrNodePtr = list.First(); crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
		{
			out << crrNodePtr->data << " ";
		}
		std::cout << std::endl;

		return out;
	}
}

#endif /* CIRCULAR_SINGLY_LINKED_LIST_HPP */
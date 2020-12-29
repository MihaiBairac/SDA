#ifndef SINGLY_LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/*
SinglyLinkedList - a type of collection/container to store elements of the same type
 dynamically, in a non-contiguos way.

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

DISADVANTAGES:
- random access is expensive
- non cache friendly compared to Vectors, Arrays
- extra memory needed for node connections (pointers)

USAGES:
- when you need to store elements of the same type, but don't know the size of the input data
- when you need very fast, insertion and deletion
- Implementation of other data structures:
a) Circular buffers, lists
b) Stack
c) Queue
d) Graph
e) HashTable - collision list

- In other practicies/alogirthms/problems:
a) Dynamic memory allocation - to streo blocks of free memory
b) Maintaining directory of names 
c) Large number arithmetic (store each vey big digits as elements of the list)

*/

namespace SDA
{
	template <class T>
	struct SinglyLinkedListNode
	{
		SinglyLinkedListNode()
			: data(), nextPtr(nullptr)
		{}

		SinglyLinkedListNode(const T& data_)
			: data(data_), nextPtr(nullptr)
		{}

		SinglyLinkedListNode(const SinglyLinkedListNode<T>& node)
			: data(node.data), nextPtr(node.nextPtr)
		{}

		~SinglyLinkedListNode()
		{
			nextPtr = nullptr;
		}

		T data;
		SinglyLinkedListNode<T>* nextPtr;
	};

	template <class T>
	class SinglyLinkedList
	{
	public:
		SinglyLinkedList();
		SinglyLinkedList(const SinglyLinkedList<T>& list);
		SinglyLinkedList(SinglyLinkedList<T>&& list);
		virtual ~SinglyLinkedList();

		SinglyLinkedList<T>& operator =(const SinglyLinkedList<T>& list);
		SinglyLinkedList<T>& operator =(SinglyLinkedList<T>&& list);

		SinglyLinkedListNode<T>* First() const;

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
		void Copy(const SinglyLinkedList<T>& list);
		void Move(SinglyLinkedList<T>&& list);
		void Destroy();

		SinglyLinkedListNode<T>* mHeadPtr;
		size_t mSize;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	SinglyLinkedList<T>::SinglyLinkedList()
		: mHeadPtr(nullptr), mSize(0)
	{}

	template <class T>
	SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& list)
	{
		Copy(list);
	}

	template <class T>
	SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& list)
	{
		Move(list);
	}

	template <class T>
	SinglyLinkedList<T>::~SinglyLinkedList()
	{
		Destroy();
	}

	template <class T>
	void SinglyLinkedList<T>::Copy(const SinglyLinkedList<T>& list)
	{
		if (this != &list)
		{
			Destroy();

			SinglyLinkedListNode<T>* crrNodePtr = nullptr;
			for (crrNodePtr = list.mHeadPtr; crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
			{
				InsertLast(crrNodePtr->data);
			}
		}
	}

	template <class T>
	void SinglyLinkedList<T>::Move(SinglyLinkedList<T>&& list)
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
	void SinglyLinkedList<T>::Destroy()
	{
		SinglyLinkedListNode<T>* crrNodePtr = mHeadPtr;
		while (crrNodePtr != nullptr)
		{
			mHeadPtr = crrNodePtr->nextPtr;
			delete crrNodePtr;
			crrNodePtr = mHeadPtr;
		}

		mHeadPtr = nullptr;
		mSize = 0;
	}

	template <class T>
	SinglyLinkedList<T>& SinglyLinkedList<T>::operator =(const SinglyLinkedList<T>& list)
	{
		Copy(list);

		return *this;
	}

	template <class T>
	SinglyLinkedList<T>& SinglyLinkedList<T>::operator =(SinglyLinkedList<T>&& list)
	{
		Move(list);

		return *this;
	}

	template <class T>
	SinglyLinkedListNode<T>* SinglyLinkedList<T>::First() const
	{
		return mHeadPtr;
	}

	template <class T>
	size_t SinglyLinkedList<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void SinglyLinkedList<T>::InsertFirst(const T& val)
	{
		SinglyLinkedListNode<T>* newNodePtr = new SinglyLinkedListNode<T>(val);

		// we can insert even if the lsit is empty

		if (mHeadPtr)
		{
			newNodePtr->nextPtr = mHeadPtr;
		}
		mHeadPtr = newNodePtr;

		++mSize;
	}

	template <class T>
	void SinglyLinkedList<T>::InsertLast(const T& val)
	{
		SinglyLinkedListNode<T>* newNodePtr = new SinglyLinkedListNode<T>(val);

		if (mHeadPtr)
		{
			SinglyLinkedListNode<T>* crrNodePtr = mHeadPtr;

			while (crrNodePtr != nullptr)
			{
				crrNodePtr = crrNodePtr->nextPtr;
			}

			crrNodePtr->nextPtr = newNodePtr;
		}
		else
		{
			mHeadPtr = newNodePtr;
		}

		++mSize;
	}

	template <class T>
	void SinglyLinkedList<T>::Insert(size_t index, const T& val)
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
			SinglyLinkedListNode<T>* newNodePtr = new SinglyLinkedListNode<T>(val);

			size_t idx = -1; // we start with an an index before to arrive at the node before the one we must insert the new node 
			SinglyLinkedListNode<T>* beforeNode = nullptr;
			for (beforeNode = mHeadPtr; beforeNode != nullptr; beforeNode = beforeNode->nextPtr)
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
	void SinglyLinkedList<T>::EraseFirst()
	{
		assert(mSize > 0);

		SinglyLinkedListNode<T>* nodeToDeletePtr = mHeadPtr;

		mHeadPtr = mHeadPtr->nextPtr;

		nodeToDeletePtr->nextPtr = nullptr;
		delete nodeToDeletePtr;
		--mSize;
	}

	template <class T>
	void SinglyLinkedList<T>::EraseLast()
	{
		assert(mSize > 0);

		SinglyLinkedListNode<T>* nodeToDeletePtr = nullptr;

		if (mSize == 1)
		{
			nodeToDeletePtr = mHeadPtr;

			mHeadPtr = nullptr;
		}
		else if (mSize == 2)
		{
			nodeToDeletePtr = mHeadPtr->nextPtr;

			mHeadPtr->nextPtr = nullptr;
		}
		else
		{
			SinglyLinkedListNode<T>* crrNodePtr = mHeadPtr;
			while (crrNodePtr->nextPtr->nextPtr != nullptr))
			{
				crrNodePtr = crrNodePtr->nextPtr;
			}

			nodeToDeletePtr = crrNodePtr->nextPtr;

			crrNodePtr->nextPtr = nullptr;
		}

		nodeToDeletePtr->nextPtr = nullptr;
		delete nodeToDeletePtr;
		--mSize;
	}

	template <class T>
	void SinglyLinkedList<T>::Erase(size_t index)
	{
		assert(index < mSize);

		SinglyLinkedListNode<T>* nodeToDeletePtr = nullptr;

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
			SinglyLinkedListNode<T>* beforeNodePtr = nullptr;
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
	void SinglyLinkedList<T>::Reverse()
	{
		/* Idea:
			We use one extra pointer called reversed to which we add the elements from the initial list (in order)

			Time complexity: O(n)
			Space complexity O(1)
		*/

		SinglyLinkedListNode<T>* reversedHeadPtr = mHeadPtr;
		reversedHeadPtr->nextPtr = nullptr; // last elements of the reversed list

		// in case the original list has no elements or only one
		if ((mHeadPtr == nullptr) || (mHeadPtr->nextPtr == nullptr))
		{
			return;
		}

		SinglyLinkedListNode<T>* remainingListPtr = mHeadPtr->next; // next element in the line
		
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
	void SinglyLinkedList<T>::InsertionSort()
	{
		/* Idea:
			We use one extra pointer called reversed to which we add the elements from the initial list (in order)

			Time complexity: O(n)
			Space complexity O(1)
		*/

		SinglyLinkedListNode<T>* sortedHeadPtr = mHeadPtr;

		SinglyLinkedListNode<T>* remainingListPtr = mHeadPtr->next; // next element in the line

		while (remainingListPtr != nullptr)
		{
			SinglyLinkedListNode<T>* crrNodePtr = remainingListPtr;
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
	std::ostream& operator << (std::ostream& out, const SinglyLinkedList<T>& list)
	{
		std::cout << "list: ";
		//SinglyLinkedList<T>::template SinglyLinkedListNode<T>* crrNodePtr = nullptr;
		SinglyLinkedListNode<T>* crrNodePtr = nullptr;
		for (crrNodePtr = list.First(); crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
		{
			out << crrNodePtr->data << " ";
		}
		std::cout << std::endl;

		return out;
	}
}

#endif /* LINKED_LIST_HPP */
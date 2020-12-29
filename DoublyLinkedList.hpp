#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/*
DoublyLinkedList - a type of collection/container to store elements of the same type
 dynamically, in a non-contiguos way. Is more flexible over SinglyLinkedList.

TIME COMPLEXITY:
- Traveral/Search = O(n)
- Add/Delete an element = O(1) at best or at worst
- Index an element = O(n)
- Get size = O(1) or O(n) dependent on implementation

SPACE COMPLEXITY:
- O(N) as we store n elements + pointers

ADVANTAGES:
- dynamic over Arrays
- very fast insertion/deletion compared to Vectors
- forward and backward traversals compared to SinglyLinkedList

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
	struct DoublyLinkedListNode
	{
		DoublyLinkedListNode()
			: data(), nextPtr(nullptr), prevPtr(nullptr)
		{}

		DoublyLinkedListNode(const T& data_)
			: data(data_), nextPtr(nullptr), prevPtr(nullptr)
		{}

		DoublyLinkedListNode(const DoublyLinkedListNode<T>& node)
			: data(node.data), nextPtr(node.nextPtr), prevPtr(node.prevPtr)
		{}

		~DoublyLinkedListNode()
		{
			nextPtr = nullptr;
			prevPtr = nullptr;
		}

		T data;
		DoublyLinkedListNode<T>* nextPtr;
		DoublyLinkedListNode<T>* prevPtr;
	};

	template <class T>
	class DoublyLinkedList
	{
	public:
		DoublyLinkedList();
		DoublyLinkedList(const DoublyLinkedList<T>& list);
		DoublyLinkedList(DoublyLinkedList<T>&& list);
		virtual ~DoublyLinkedList();

		DoublyLinkedList<T>& operator =(const DoublyLinkedList<T>& list);
		DoublyLinkedList<T>& operator =(DoublyLinkedList<T>&& list);

		DoublyLinkedListNode<T>* First() const;
		DoublyLinkedListNode<T>* Last() const;

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
		void Copy(const DoublyLinkedList<T>& list);
		void Move(DoublyLinkedList<T>&& list);
		void Destroy();

		DoublyLinkedListNode<T>* mHeadPtr, mTailPtr;
		size_t mSize;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	DoublyLinkedList<T>::DoublyLinkedList()
		: mHeadPtr(nullptr), mTailPtr(nullptr), mSize(0)
	{}

	template <class T>
	DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& list)
	{
		Copy(list);
	}

	template <class T>
	DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList<T>&& list)
	{
		Move(list);
	}

	template <class T>
	DoublyLinkedList<T>::~DoublyLinkedList()
	{
		Destroy();
	}

	template <class T>
	void DoublyLinkedList<T>::Copy(const DoublyLinkedList<T>& list)
	{
		if (this != &list)
		{
			Destroy();

			DoublyLinkedListNode<T>* crrNodePtr = nullptr;
			for (crrNodePtr = list.mHeadPtr; crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
			{
				InsertLast(crrNodePtr->data);
			}
		}
	}

	template <class T>
	void DoublyLinkedList<T>::Move(DoublyLinkedList<T>&& list)
	{
		if (this != &list)
		{
			Destroy();

			mSize = list.mSize;

			mHeadPtr = list.mHeadPtr;
			mTailPtr = list.mTailPtr;

			list.mHeadPtr = nullptr;
			list.mTailPtr = nullptr;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::Destroy()
	{
		DoublyLinkedListNode<T>* crrNodePtr = mHeadPtr;
		while (crrNodePtr != nullptr)
		{
			mHeadPtr = crrNodePtr->nextPtr;
			delete crrNodePtr;
			crrNodePtr = mHeadPtr;
		}

		mHeadPtr = nullptr;
		mTailPtr = nullptr;
		mSize = 0;
	}

	template <class T>
	DoublyLinkedList<T>& DoublyLinkedList<T>::operator =(const DoublyLinkedList<T>& list)
	{
		Copy(list);

		return *this;
	}

	template <class T>
	DoublyLinkedList<T>& DoublyLinkedList<T>::operator =(DoublyLinkedList<T>&& list)
	{
		Move(list);

		return *this;
	}

	template <class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::First() const
	{
		return mHeadPtr;
	}

	template <class T>
	DoublyLinkedListNode<T>* DoublyLinkedList<T>::Last() const
	{
		return mTailPtr;
	}

	template <class T>
	size_t DoublyLinkedList<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void DoublyLinkedList<T>::InsertFirst(const T& val)
	{
		DoublyLinkedListNode<T>* newNodePtr = new DoublyLinkedListNode<T>(val);

		if (mHeadPtr)
		{
			newNodePtr->nextPtr = mHeadPtr;
			mHeadPtr->prevPtr = newNodePtr;
		}
		mHeadPtr = newNodePtr;

		++mSize;
	}

	template <class T>
	void DoublyLinkedList<T>::InsertLast(const T& val)
	{
		DoublyLinkedListNode<T>* newNodePtr = new DoublyLinkedListNode<T>(val);

		if (mSize == 0)
		{
			mHeadPtr = mTailPtr = newNodePtr;
		}
		else
		{
			mTailPtr->nextPtr = newNodePtr;
			newNodePtr->prevPtr = mTailPtr;
		}

		++mSize;
	}

	template <class T>
	void DoublyLinkedList<T>::Insert(size_t index, const T& val)
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
			DoublyLinkedListNode<T>* newNodePtr = new DoublyLinkedListNode<T>(val);

			size_t idx = 0;
			DoublyLinkedListNode<T>* crrNode = nullptr;
			for (crrNode = mHeadPtr; crrNode != nullptr; crrNode = crrNode->nextPtr)
			{
				if (idx == index)
				{
					// inserting the new node before the crrNode
					if (crrNode->prevPtr)
						crrNode->prevPtr->nextPtr = newNodePtr;
					newNodePtr->prevPtr = crrNode->prevPtr;

					newNodePtr->nextPtr = crrNode;
					crrNode->prevPtr = newNodePtr;

					break;
				}

				++idx;
			}

			++mSize;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::EraseFirst()
	{
		assert(mSize > 0);

		DoublyLinkedListNode<T>* nodeToDeletePtr = mHeadPtr;

		mHeadPtr = mHeadPtr->nextPtr;
		mHeadPtr->prevPtr = nullptr;

		nodeToDeletePtr->nextPtr = nullptr;
		nodeToDeletePtr->prevPtr = nullptr;
		delete nodeToDeletePtr;
		--mSize;
	}

	template <class T>
	void DoublyLinkedList<T>::EraseLast()
	{
		assert(mSize > 0);

		DoublyLinkedListNode<T>* nodeToDeletePtr = mTailPtr;

		mTailPtr = mTailPtr->prevPtr;
		mTailPtr->nextPtr = nullptr;

		nodeToDeletePtr->nextPtr = nullptr;
		nodeToDeletePtr->prevPtr = nullptr;
		delete nodeToDeletePtr;
		--mSize;
	}

	template <class T>
	void DoublyLinkedList<T>::Erase(size_t index)
	{
		assert(index < mSize);

		DoublyLinkedListNode<T>* nodeToDeletePtr = nullptr;

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
			size_t idx = 0;
			DoublyLinkedListNode<T>* crrNodePtr = nullptr;
			for (crrNodePtr = mHeadPtr; crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
			{
				if (idx == index)
				{
					// erasing the crrNode
					nodeToDeletePtr = crrNodePtr;

					if (crrNodePtr->prevPtr)
						crrNodePtr->prevPtr->nextPtr = crrNodePtr->nextPtr;
					if (crrNodePtr->nextPtr)
						crrNodePtr->nextPtr->prevPtr = crrNodePtr->prevPtr;

					break;
				}

				++idx;
			}

			nodeToDeletePtr->nextPtr = nullptr;
			nodeToDeletePtr->prevPtr = nullptr;
			delete nodeToDeletePtr;
			--mSize;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::Reverse()
	{
		/* Idea: we reverse the link pointers keeping the elements in place
			Time complexity : O(n)
			Space complexity O(1)
		*/

		// we go backwards
		DoublyLinkedListNode<T>* crrNodePtr = nullptr;
		for (crrNodePtr = mTailPtr; crrNodePtr != nullptr; crrNodePtr = crrNodePtr->prevPtr)
		{
			DoublyLinkedListNode<T>* prevLinkPtr = crrNodePtr->prevPtr; // cache prev link

			if (crrNodePtr->nextPtr)
				crrNodePtr->prevPtr = crrNodePtr->nextPtr;
			else
				crrNodePtr->prevPtr = nullptr;

			if (prevLinkPtr)
				crrNodePtr->nextPtr = prevLinkPtr;
			else
				crrNodePtr->nextPtr = nullptr;
		}
	}

	template <class T>
	void DoublyLinkedList<T>::InsertionSort()
	{
		/* Idea:
			We use one extra pointer called reversed to which we add the elements from the initial list (in order)

			Time complexity: O(n)
			Space complexity O(1)
		*/

		DoublyLinkedListNode<T>* sortedHeadPtr = mHeadPtr;

		DoublyLinkedListNode<T>* remainingListPtr = mHeadPtr->next; // next element in the line

		while (remainingListPtr != nullptr)
		{
			DoublyLinkedListNode<T>* crrNodePtr = remainingListPtr;
			remainingListPtr = remainingListPtr->nextPtr; // remaining ptr moves forward

			// insert crrNodePtr before sorted
			if (crrNodePtr->data < sortedHeadPtr->data)
			{
				// insert before
				if (sortedHeadPtr->prevPtr)
					sortedHeadPtr->prevPtr->nextPtr = crrNodePtr;
				crrNodePtr->prevPtr = sortedHeadPtr->prevPtr;

				crrNodePtr->nextPtr = sortedHeadPtr;
				sortedHeadPtr->prevPtr = crrNodePtr;

				sortedHeadPtr = crrNodePtr;
			}
			else
			{
				// insert after
				if (sortedHeadPtr->nextPtr)
					sortedHeadPtr->nextPtr->prevPtr = crrNodePtr;
				crrNodePtr->nextPtr = sortedHeadPtr->nextPtr;

				crrNodePtr->prevPtr = sortedHeadPtr;
				sortedHeadPtr->nextPtr = crrNodePtr;
			}
		}

		mHeadPtr = sortedHeadPtr;
	}

	/////////// OUTPUT /////////////
	template <class T>
	std::ostream& operator << (std::ostream& out, const DoublyLinkedList<T>& list)
	{
		std::cout << "list: ";
		//DoublyLinkedListNode<T>::template DoublyLinkedListNode<T>* crrNodePtr = nullptr;
		DoublyLinkedListNode<T>* crrNodePtr = nullptr;
		for (crrNodePtr = list.First(); crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
		{
			out << crrNodePtr->data << " ";
		}
		std::cout << std::endl;

		return out;
	}
}

#endif /* DOUBLY_LINKED_LIST_HPP */
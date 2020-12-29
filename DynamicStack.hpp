#ifndef DYNAMIC_STACK_HPP
#define DYNAMIC_STACK_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/*
DynamicQueue - LIFO container, but dynamic in size
Based on SinglyLinkedList

TIME COMPLEXITY:
- Traversal/Search = O(n)
- Add/Delete an element = O(1) at best or at worst
- Random access an element = O(n)
- Get size = O(1)

SPACE COMPLEXITY:
- O(N) as we store n elements in a contiguos buffer

ADVANTAGES:
- dynamic compared to FixedStack
- really fast insertion or deletion compared to FixedStack

DISADVANTAGES:
- random access is expensive
- non cache friendly compared to FixedStack
- extra memory needed for node connections (pointers)


USAGES:
Several algorithms like:
- Function-call abstraction. Used in debuggers to show call stack.
- Check for balanced parentheses in an expression
- Graph traversals
- Backtracking
- Forward and backward feature in web browsers
- Redo-undo features at many places like editors, photoshop.
- Lua stack

Model:
Top -> Node -> Node

*/
namespace SDA
{
	template <class T>
	struct StackNode
	{
		StackNode()
			: data(), nextPtr(nullptr)
		{}

		StackNode(const T& data_)
			: data(data_), nextPtr(nullptr)
		{}

		StackNode(const StackNode<T>& node)
			: data(node.data), nextPtr(node.nextPtr)
		{}

		~StackNode()
		{
			nextPtr = nullptr;
		}

		T data;
		StackNode<T>* nextPtr;
	};

	template <class T>
	class DynamicStack
	{
	public:


		DynamicStack();
		DynamicStack(const DynamicStack<T>& stack);
		DynamicStack(DynamicStack<T>&& stack);
		virtual ~DynamicStack();

		DynamicStack<T>& operator =(const DynamicStack<T>& stack);
		DynamicStack<T>& operator =(DynamicStack<T>&& stack);

		StackNode<T>* Top();

		size_t Size() const;

		void Push(const T& val);
		void Pop();

	private:
		void Copy(const DynamicStack<T>& stack);
		void Move(DynamicStack<T>&& stack);
		void Destroy();

		StackNode<T>* mTopPtr;
		size_t mSize;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	DynamicStack<T>::DynamicStack()
		: mTopPtr(nullptr), mSize(0)
	{}

	template <class T>
	DynamicStack<T>::DynamicStack(const DynamicStack<T>& stack)
	{
		Copy(stack);
	}

	template <class T>
	DynamicStack<T>::DynamicStack(DynamicStack<T>&& stack)
	{
		Move(stack);
	}

	template <class T>
	DynamicStack<T>::~DynamicStack()
	{
		Destroy();
	}

	template <class T>
	void DynamicStack<T>::Copy(const DynamicStack<T>& stack)
	{
		if (this != &stack)
		{
			Destroy();

			StackNode<T>* newStackPtr = nullptr;
			StackNode<T>* crrNodePtr = stack.Top();
			while (crrNodePtr != nullptr)
			{
				if (newStackPtr)
				{
					newStackPtr->nextPtr = crrNodePtr;
					newStackPtr = crrNodePtr;
				}
				else
				{
					newStackPtr = crrNodePtr;
					mTopPtr = newStackPtr;
				}
			}
		}
	}

	template <class T>
	void DynamicStack<T>::Move(DynamicStack<T>&& stack)
	{
		if (this != &stack)
		{
			Destroy();

			mSize = stack.mSize;

			mTopPtr = stack.mTopPtr;
			stack.mTopPtr = nullptr;
		}
	}

	template <class T>
	void DynamicStack<T>::Destroy()
	{
		StackNode<T>* crrNode = mTopPtr;
		while (crrNode != nullptr)
		{
			mTopPtr = crrNode->nextPtr;
			delete crrNode;
			crrNode = mTopPtr;
		}

		mTopPtr = nullptr;
		mSize = 0;
	}

	template <class T>
	DynamicStack<T>& DynamicStack<T>::operator =(const DynamicStack<T>& stack)
	{
		Copy(stack);

		return *this;
	}

	template <class T>
	DynamicStack<T>& DynamicStack<T>::operator =(DynamicStack<T>&& stack)
	{
		Move(stack);

		return *this;
	}

	template <class T>
	StackNode<T>* DynamicStack<T>::Top()
	{
		return mTopPtr;
	}

	template <class T>
	size_t DynamicStack<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void DynamicStack<T>::Push(const T& val)
	{
		StackNode<T>* newNodePtr = new StackNode<T>(val);

		if (mTopPtr)
		{
			// add new node before the top (ontop)
			newNodePtr->nextPtr = mTopPtr;
			mTopPtr = newNodePtr;
		}
		else
			mTopPtr = newNodePtr;

		++mSize;
	}

	template <class T>
	void DynamicStack<T>::Pop()
	{
		assert(mSize == 0);

		StackNode<T>* nodeToDeletePtr = mTopPtr;

		if (mTopPtr)
		{
			mTopPtr = mTopPtr->nextPtr;
		}

		delete nodeToDeletePtr;
		--mSize;
	}

	/////////// OUTPUT /////////////
	template <class T>
	std::ostream& operator << (std::ostream& out, const DynamicStack<T>& stack)
	{
		std::cout << "stack: ";
		//DynamicStack<T>::template StackNode<T>* crrNodePtr = nullptr;
		StackNode<T>* crrNodePtr = nullptr;
		for (crrNodePtr = stack.Top(); crrNodePtr != nullptr; crrNodePtr = crrNodePtr->nextPtr)
		{
			out << crrNodePtr->data << " ";
		}
		std::cout << std::endl;

		return out;
	}
}

#endif /* DYNAMIC_STACK_HPP */
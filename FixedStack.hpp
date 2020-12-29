#ifndef FIXED_STACK_HPP
#define FIXED_STACK_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/* A Fixed size Stack - standard LIFO container

TIME COMPLEXITY:
- Traversal/Search = O(n)
- Add/Delete an element = O(1) at best or at worst
- Access an element = O(1)
- Get size = O(1)

SPACE COMPLEXITY:
- O(N) as we store n elements in a contiguos buffer

ADVANTAGES:
- very fast random access compared to DynamicStack
- cache friendly

DISADVANTAGES:
- fixed size

USAGES:
Several algorithms like:
- Function-call abstraction. Used in debuggers to show call stack.
- Check for balanced parentheses in an expression
- Graph traversals
- Backtracking
- Forward and backward feature in web browsers
- Redo-undo features at many places like editors, photoshop.
- Lua stack

*/

namespace SDA
{
	template <class T>
	class FixedStack
	{
	public:
		FixedStack();
		FixedStack(size_t capacity);
		FixedStack(const FixedStack<T>& stack);
		FixedStack(FixedStack<T>&& stack);
		virtual ~FixedStack();

		FixedStack<T>& operator =(const FixedStack<T>& stack);
		FixedStack<T>& operator =(FixedStack<T>&& stack);

		T& Top();
		const T& Top() const;

		bool IsEmpty() const;
		bool IsFull() const;

		size_t Capacity() const;
		size_t Size() const;

		void Push(const T& val);
		void Pop();

		void Swap(FixedStack<T>& stack);

	private:
		void Copy(const FixedStack<T>& stack);
		void Move(FixedStack<T>&& stack);
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
	FixedStack<T>::FixedStack()
		:  mBuffer(nullptr), mCapacity(DEFAULT_CAPACITY), mSize(0)
	{
		mBuffer = new T[mCapacity];
	}

	template <class T>
	FixedStack<T>::FixedStack(size_t capacity)
		: mCapacity(capacity), mSize(0)
	{
		mBuffer = new T[mCapacity];
	}

	template <class T>
	FixedStack<T>::FixedStack(const FixedStack<T>& stack)
	{
		Copy(stack);
	}

	template <class T>
	FixedStack<T>::FixedStack(FixedStack<T>&& stack)
	{
		Move(stack);
	}

	template <class T>
	FixedStack<T>::~FixedStack()
	{
		Destroy();

		mCapacity = 0;
		mSize = 0;
	}

	template <class T>
	void FixedStack<T>::Copy(const FixedStack<T>& stack)
	{
		if (this != &stack)
		{
			// delete the old buffer
			Destroy();

			mCapacity = stack.mCapacity;
			mSize = stack.mSize;

			mBuffer = new T[mCapacity];

			for (size_t i = 0; i < mSize; ++i)
			{
				// here we copy the elements as we do not want
				// to invalidate vec elements by moving them to this vec 
				mBuffer[i] = stack.mBuffer[i];
			}
		}
	}

	template <class T>
	void FixedStack<T>::Move(FixedStack<T>&& stack)
	{
		if (this != &stack)
		{
			// delete the old buffer
			Destroy();

			mCapacity = stack.mCapacity;
			mSize = stack.mSize;

			// just copy the pointer
			mBuffer = stack.mBuffer;

			// vec is invalidated after move
			stack.mBuffer = nullptr;
			stack.mCapacity = 0;
			stack.mSize = 0;
		}
	}

	template <class T>
	void FixedStack<T>::Destroy()
	{
		if (mBuffer)
		{
			delete[] mBuffer;
		}
	}

	template <class T>
	FixedStack<T>& FixedStack<T>::operator =(const FixedStack<T>& stack)
	{
		Copy(stack);

		return *this;
	}

	template <class T>
	FixedStack<T>& FixedStack<T>::operator =(FixedStack<T>&& stack)
	{
		Move(stack);

		return *this;
	}

	template <class T>
	T& FixedStack<T>::Top()
	{
		assert(mSize > 0);

		return mBuffer[mSize - 1];
	}

	template <class T>
	const T& FixedStack<T>::Top() const
	{
		assert(mSize > 0);

		return mBuffer[mSize - 1];
	}

	template <class T>
	bool FixedStack<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <class T>
	bool FixedStack<T>::IsFull() const
	{
		return mSize == mCapacity;
	}

	template <class T>
	size_t FixedStack<T>::Capacity() const
	{
		return mCapacity;
	}

	template <class T>
	size_t FixedStack<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void FixedStack<T>::Push(const T& val)
	{
		if (false == IsFull())
		{
			mBuffer[mSize++] = val;
		}
	}

	template <class T>
	void FixedStack<T>::Pop()
	{
		if (false == IsEmpty())
		{
			mBuffer[mSize - 1].~T();
			--mSize;
		}
	}

	template <class T>
	void FixedStack<T>::Swap(FixedStack<T>& stack)
	{
		SDA::Swap(mBuffer, stack.mBuffer);
		SDA::Swap(mCapacity, stack.mCapacity);
		SDA::Swap(mSize, stack.mSize);
	}

	/////////// OUTPUT /////////////
	template <class T>
	std::ostream& operator << (std::ostream& out, const FixedStack<T>& stack)
	{

		// cannot acces elements freely so we copy the stack and pop from it each element
		FixedStack<T> stack2(stack);
		// we cache the size as it changes per pop()
		size_t initSize = stack2.Size();

		std::cout << "stack: ";
		for (size_t i = 0; i < initSize; ++i)
		{
			out << stack2.Top() << " ";
			stack2.Pop();
		}
		std::cout << std::endl;

		return out;
	}
}

#endif /* FIXED_STACK_HPP */
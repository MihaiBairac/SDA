#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Utility.hpp"
#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <iostream>

/*
Vector - a type of collection/container to store elements of the same type
, but dynamically. Can change size. Stored in a contiguos way.

TIME COMPLEXITY:
- Traveral = O(n)
- Add/Delete an element = O(1) at best, O(n) at worst (when new allocation + movement of the old elements is required)
- Index an element = O(1)
- Get size = O(1)

SPACE COMPLEXITY:
- O(N) as we use a contiguos memory space to allocate the vector elements

ADVANTAGES:
 - fast indexing of an element
 - fast traversal
 - cache friendly compared to LinkedLists
 - is dynamic compared to Arrays

DISADVANTAGES:
 - insertion/deletion aren't so fast when many are required

USAGES:
 - anytime you need to store data of same type, quickly access it
e.g. constant string (an array of chars)
 - you know the size of your data only at runtime
 - if you store numerical data and want sort it very fast
 - you know you have to keep modifying the data container as more data is available
 e.g. reading froma file or database, receiving data from a stream

*/

namespace SDA
{
	/* Vector container - dynamic size */
	template <class T>
	class Vector
	{
	public:
		Vector();
		Vector(size_t size);
		Vector(size_t size, const T& val);
		Vector(const Vector<T>& vec);
		Vector(Vector<T>&& vec);
		virtual ~Vector();

		Vector<T>& operator =(const Vector<T>& vec);
		Vector<T>& operator =(Vector<T>&& vec);

		T& operator [](size_t index);
		const T& operator [](size_t index) const;
		T& At(size_t index);
		const T& At(size_t index) const;

		T& Front();
		const T& Front() const;
		T& Back();
		const T& Back() const;

		bool IsEmpty() const;
		bool IsFull() const;

		size_t Capacity() const;
		size_t Size() const;

		void Insert(size_t index, const T& val);
		void Erase(size_t index);

		void PushBack(const T& val);
		void PopBack();

		void Reserve(size_t capacity);
		void Resize(size_t size);

		void Swap(Vector<T>& vec);

		T* GetData();
		const T* GetData() const;

	private:
		void Copy(const Vector<T>& vec);
		void Move(Vector<T>&& vec);
		void Destroy();

		T* mBuffer;
		size_t mCapacity; //at least double of the size
		size_t mSize;

		static const size_t ORDER_OF_GROWTH = 2;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	/*
		 Incidentally, one common way of regrowing an array is to double the size as needed.
		 This is so that if you are inserting n items at most only O(log n) regrowths are performed
		 and at most O(n) space is wasted.
	 */

	template <class T>
	Vector<T>::Vector()
		: mCapacity(0), mSize(0), mBuffer(nullptr) //avoid nullptr buffer
	{}

	template <class T>
	Vector<T>::Vector(size_t size)
		: mCapacity(size * ORDER_OF_GROWTH), mSize(size)
	{
		mBuffer = new T[mCapacity];
	}

	template <class T>
	Vector<T>::Vector(size_t size, const T& val)
		: Vector(size)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			// create element via placement new()
			new (mBuffer + i) T(val);
		}
	}

	template <class T>
	Vector<T>::Vector(const Vector<T>& vec)
	{
		Copy(vec);
	}

	template <class T>
	Vector<T>::Vector(Vector<T>&& vec)
	{
		Move(vec);
	}

	template <class T>
	Vector<T>::~Vector()
	{
		Destroy();
	}

	template <class T>
	void Vector<T>::Copy(const Vector<T>& vec)
	{
		if (this != &vec)
		{
			// delete the old buffer
			Destroy();

			mCapacity = vec.mCapacity;
			mSize = vec.mSize;

			mBuffer = new T[mCapacity];

			for (size_t i = 0; i < mSize; ++i)
			{
				// here we copy the elements as we do not want
				// to invalidate vec elements by moving them to this vec 
				mBuffer[i] = vec.mBuffer[i];
			}
		}
	}

	template <class T>
	void Vector<T>::Move(Vector<T>&& vec)
	{
		if (this != &vec)
		{
			// delete the old buffer
			Destroy();

			mCapacity = vec.mCapacity;
			mSize = vec.mSize;

			// just copy the pointer
			mBuffer = vec.mBuffer;

			// vec is invalidated after move
			vec.mBuffer = nullptr;
			vec.mCapacity = 0;
			vec.mSize = 0;
		}
	}

	template <class T>
	void Vector<T>::Destroy()
	{
		if (mBuffer)
		{
			delete[] mBuffer;
		}

		mCapacity = 0;
		mSize = 0;
	}

	template <class T>
	Vector<T>& Vector<T>::operator =(const Vector<T>& vec)
	{
		Copy(vec);

		return *this;
	}

	template <class T>
	Vector<T>& Vector<T>::operator =(Vector<T>&& vec)
	{
		Move(vec);

		return *this;
	}

	template <class T>
	T& Vector<T>::operator [](size_t index)
	{
		assert(index < mSize);

		return mBuffer[index];
	}

	template <class T>
	const T& Vector<T>::operator [](size_t index) const
	{
		assert(index < mSize);

		return mBuffer[index];
	}

	template <class T>
	T& Vector<T>::At(size_t index)
	{
		return operator [](index);
	}

	template <class T>
	const T& Vector<T>::At(size_t index) const
	{
		return operator [](index);
	}

	template <class T>
	T& Vector<T>::Front()
	{
		assert(mSize > 0);

		return mBuffer[0];
	}

	template <class T>
	const T& Vector<T>::Front() const
	{
		assert(mSize > 0);

		return mBuffer[0];
	}

	template <class T>
	T& Vector<T>::Back()
	{
		assert(mSize > 0);

		return mBuffer[mSize - 1];
	}

	template <class T>
	const T& Vector<T>::Back() const
	{
		assert(mSize > 0);

		return mBuffer[mSize - 1];
	}

	template <class T>
	bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <class T>
	bool Vector<T>::IsFull() const
	{
		return mSize == mCapacity;
	}


	template <class T>
	size_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template <class T>
	size_t Vector<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	void Vector<T>::Insert(size_t index, const T& val)
	{
		assert(index < mSize);

		// check if we have to reallocate
		if (mSize + 1 > mCapacity)
		{
			Reserve(mCapacity * ORDER_OF_GROWTH);
		}

		// shift the other elements to make free one slot for the new one
		if (index == 0) // the first
		{
			// shift elements to right with 1 position
			for (size_t i = mSize - 1; i > 0; --i)
			{
				// better move them then copy
				mBuffer[i + 1] = std::move(mBuffer[i]);
			}
		}
		else if (index == mSize - 1) // the last
		{
			// nothing to do 
		}
		else // in between
		{
			// shift elements after the required position to right with 1 position
			for (size_t i = mSize - index; i > index; --i)
			{
				// better move them then copy
				mBuffer[i + 1] = std::move(mBuffer[i]);
			}
		}
		// add the new element
		mBuffer[index] = val;

		++mSize;
	}

	template <class T>
	void Vector<T>::Erase(size_t index)
	{
		assert(index < mSize);

		// shift the other elements thus deleting the required one
		if (index == 0) // the first
		{
			// destroy the element
			mBuffer[0].~T();

			// shift elements to left with 1 position
			for (size_t i = 0; i < mSize; ++i)
			{
				// better move them then copy
				mBuffer[i] = std::move(mBuffer[i + 1]);
			}
		}
		else if (index == mSize - 1) // the last
		{
			// destroy the element
			mBuffer[mSize - 1].~T();
		}
		else // in between
		{
			// destroy the element
			mBuffer[index].~T();

			// shift elements after the required position to left with 1 position
			for (size_t i = index; i < mSize - index; ++i)
			{
				// better move them then copy
				mBuffer[i] = std::move(mBuffer[i + 1]);
			}
		}

		--mSize;
	}

	template <class T>
	void Vector<T>::PushBack(const T& val)
	{
		if (mSize == 0)
		{
			Reserve(2); // if empty resize for 2 elements
		}
		// we reserve new space if required by doubing the capacity
		else if (mSize == mCapacity)
		{
			Reserve(2 * mCapacity);
		}

		mBuffer[mSize++] = val;
	}

	template <class T>
	void Vector<T>::PopBack()
	{
		assert(mSize > 0);

		// destroy the last element as it is removed
		mBuffer[mSize - 1].~T();

		--mSize;
	}

	template <class T>
	void Vector<T>::Reserve(size_t capacity)
	{
		// we reserve new space only if required
		if (capacity > mCapacity)
		{
			// create the new buffer
			T* newBuffer = new T[capacity];

			// the elements from the old to the new one
			for (size_t i = 0; i < mSize; ++i)
			{
				// better move them then copy
				newBuffer[i] = std::move(mBuffer[i]);
			}
			size_t oldSize = mSize;

			// free the old buffer
			Destroy();

			// update buffer, capacity and size
			mBuffer = newBuffer;
			mCapacity = capacity;
			mSize = oldSize;
		}
	}

	template <class T>
	void Vector<T>::Resize(size_t size)
	{
		// resize only if required
		if (size > mCapacity)
		{
			Reserve(size * ORDER_OF_GROWTH); // still double the capcity
		}
		mSize = size;
	}

	template <class T>
	void Vector<T>::Swap(Vector<T>& vec)
	{
	//	SDA::Swap(mBuffer, vec.mBuffer);
//		SDA::Swap(mCapacity, vec.mCapacity);
//		SDA::Swap(mSize, vec.mSize);
	}

	template <class T>
	T* Vector<T>::GetData()
	{
		return mBuffer;
	}

	template <class T>
	const T* Vector<T>::GetData() const
	{
		return mBuffer;
	}

	/////////// INPUT & OUTPUT /////////////
	template <class T>
	std::ostream& operator << (std::ostream& out, const Vector<T>& vec)
	{
		std::cout << "vec: ";
		for (size_t i = 0; i < vec.Size(); ++i)
		{
			out << vec.At(i) << " ";
		}
		std::cout << std::endl;

		return out;
	}

	template <class T>
	std::istream& operator >> (std::istream& in, Vector<T>& vec)
	{
		//TODO - FIX
		do
		{
			T el;
			in >> el;
			vec.PushBack(el);
		} while (in.get() != '\n');

		return in;
	}
}

#endif /* VECTOR_HPP */
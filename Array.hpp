#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "Utility.hpp"
#include <cstddef> // size_t
#include <utility> // std::move()
#include <initializer_list>
#include <cassert>
#include <iostream>

/*
Array - a type of collection/container to store elements of the same type
Can't change size. Stored in a contiguos way.

TIME COMPLEXITIES:
- Traveral = O(n)
- Index an element = O(1)
- Get size = O(1)

SPACE COMPLEXITY:
- O(N) as we use a contiguos memory space to allocate the array elements

ADVANTAGES:
 - fast indexing of an element
 - fast traversal
 - cache friendly

DISADVANTAGES:
 - can't be modified

USAGES:
 - anytime you need to store data of same type, quicly access it
e.g. constant string (an array of chars)
 - you know the size of the inout data at compile time
 - if you store numerical data and want sort it very fast 

*/

namespace SDA
{
	template <class T, size_t Size>
	class Array
	{
	public:
		Array(std::initializer_list<T> initList);
	//	Array(const Array<T, Size>& ar);
	//	Array(Array<T, Size>&& ar);
		virtual ~Array();

	//	Array<T, Size>& operator =(const Array<T, Size>& ar);
	//	Array<T, Size>& operator =(Array<T, Size>&& ar);

		T& operator [](size_t index);
		const T& operator [](size_t index) const;
		T& At(size_t index);
		const T& At(size_t index) const;

		T& Front();
		const T& Front() const;
		T& Back();
		const T& Back() const;

		bool IsEmpty() const;

		size_t Size() const;

		void Swap(Array<T, Size>& ar);

		T* GetData();
		const T* GetData() const;

	private:
	//	void Copy(const Array<T, Size>& ar);
	//	void Move(Array<T, Size>&& ar);

		T mBuffer[Size];
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T, size_t Size>
	Array<T, Size>::Array(std::initializer_list<T> initList)
	{

	}
/*
	template <class T, size_t Size>
	Array<T, Size>::Array(const Array<T, Size>& ar)
	{

	}

	template <class T, size_t Size>
	Array<T, Size>::Array(Array<T, Size>&& ar)
	{

	}
*/
	template <class T, size_t Size>
	Array<T, Size>::~Array()
	{

	}
/*
	template <class T, size_t Size>
	void Array<T, Size>::Copy(const Array<T, Size>& ar)
	{

	}

	template <class T, size_t Size>
	void Array<T, Size>::Move(Array<T, Size>&& ar)
	{

	}

	template <class T, size_t Size>
	Array<T, Size>& operator =(const Array<T, Size>& ar)
	{

	}

	template <class T, size_t Size>
	Array<T, Size>& operator =(Array<T, Size>&& ar)
	{

	}
*/
	template <class T, size_t Size>
	T& Array<T, Size>::operator [](size_t index)
	{
		assert(index < Size);

		return mBuffer[index];
	}

	template <class T, size_t Size>
	const T& Array<T, Size>::operator [](size_t index) const
	{
		assert(index < Size);

		return mBuffer[index];
	}

	template <class T, size_t Size>
	T& Array<T, Size>::At(size_t index)
	{
		return operator [](index);
	}

	template <class T, size_t Size>
	const T& Array<T, Size>::At(size_t index) const
	{
		return operator [](index);
	}

	template <class T, size_t Size>
	T& Array<T, Size>::Front()
	{
		assert(Size > 0);

		return mBuffer[0];
	}

	template <class T, size_t Size>
	const T& Array<T, Size>::Front() const
	{
		assert(Size > 0);

		return mBuffer[0];
	}

	template <class T, size_t Size>
	T& Array<T, Size>::Back()
	{
		assert(Size > 0);

		return mBuffer[Size - 1];
	}

	template <class T, size_t Size>
	const T& Array<T, Size>::Back() const
	{
		assert(Size > 0);

		return mBuffer[Size - 1];
	}

	template <class T, size_t Size>
	bool  Array<T, Size>::IsEmpty() const
	{
		return Size == 0;
	}

	template <class T, size_t Size>
	size_t  Array<T, Size>::Size() const
	{
		return Size;
	}


#include <array>
	template <class T, size_t Size>
	void  Array<T, Size>::Swap(Array<T, Size>& ar)
	{
		SDA:Swap(mBuffer, ar.mBuffer);
	}

	template <class T, size_t Size>
	T* Array<T, Size>::GetData()
	{
		return mBuffer;
	}

	template <class T, size_t Size>
	const T* Array<T, Size>::GetData() const
	{
		return mBuffer;
	}
}

#endif /* ARRAY_HPP */
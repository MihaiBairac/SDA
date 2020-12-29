#ifndef LINIAR_SET_HPP
#define LINIAR_SET_HPP

#include <cassert>

namespace SDA
{
	template <class T>
	class LiniarSet
	{
	public:
		LiniarSet();
		LiniarSet(size_t size);
		LiniarSet(size_t size, const T& el);
		LiniarSet(const LiniarSet<T>& set);
		LiniarSet(LiniarSet<T>&& set);
		~LiniarSet();

		LiniarSet<T>& operator =(const LiniarSet<T>& set);
		LiniarSet<T>& operator =(LiniarSet<T>&& set);

		size_t getElement(const T& el);

		bool isEmpty();
		size_t capacity();
		size_t size();

		void addElement(const T& el);

		bool exists(const T& el);

		LiniarSet<T> reunion(const LiniarSet<T>& set);
		LiniarSet<T> intersection(const LiniarSet<T>& set);
		LiniarSet<T> difference(const LiniarSet<T>& set);

		void reserve(size_t capacity);
		void resize(size_t size);

	private:
		void Copy(const LiniarSet<T>& set);
		void Move(LiniarSet<T>&& set);
		void Destroy();

		T* mBuffer;
		size_t mSize;
		size_t mCapacity; //at least double of the size
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
	LiniarSet<T>::LiniarSet()
		: mCapacity(0), mSize(0), mBuffer(nullptr) //avoid nullptr buffer
	{}

	template <class T>
	LiniarSet<T>::LiniarSet(size_t size)
		: mSize(size), mCapacity(size)
	{
		mBuffer = new T[mCapacity];
	}

	template <class T>
	LiniarSet<T>::LiniarSet(size_t size, const T& el)
		: LiniarSet(size)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			// create via placement new()
			new (mBuffer + i) T(el);
		}
	}

	template <class T>
	LiniarSet<T>::LiniarSet(const LiniarSet<T>& set)
	{
		operator = (set);
	}

	template <class T>
	LiniarSet<T>::LiniarSet(LiniarSet<T>&& set)
	{
		operator = (set);
	}

	template <class T>
	LiniarSet<T>::~LiniarSet()
	{
		mSize = 0;
		mCapacity = 0;

		if (mBuffer)
		{
			delete[] mBuffer;
		}
	}

	template <class T>
	void LiniarSet<T>::Copy(const LiniarSet<T>& set)
	{
		if (this != &set)
		{
			mSize = set.mSize;
			mCapacity = set.mCapacity;

			Destroy();

			mBuffer = new T[mCapacity];

			for (size_t i = 0; i < mSize; ++i)
			{
				mBuffer[i] = set.mBuffer[i];
			}
		}
	}

	template <class T>
	void LiniarSet<T>::Move(LiniarSet<T>&& set)
	{
		if (this != &set)
		{
			mSize = set.mSize;
			mCapacity = set.mCapacity;

			Destroy();

			mBuffer = new T[mCapacity];

			for (size_t i = 0; i < mSize; ++i)
			{
				mBuffer[i] = set.mBuffer[i];
			}
		}
	}

	template <class T>
	void LiniarSet<T>::Destroy()
	{
		if (mBuffer)
		{
			delete[] mBuffer;
		}
	}

	template <class T>
	LiniarSet<T>& LiniarSet<T>::operator =(const LiniarSet<T>& set)
	{
		if (this != &set)
		{
			mSize = set.mSize;
			mCapacity = set.mCapacity;

			if (mBuffer)
			{
				delete[] mBuffer;
			}

			mBuffer = new T[mCapacity];

			for (size_t i = 0; i < mSize; ++i)
			{
				// copy via placement new()
				new (mBuffer + i) T(set.mBuffer[i]);
			}
		}
		return *this;
	}

	template <class T>
	LiniarSet<T>& LiniarSet<T>::operator =(LiniarSet<T>&& set)
	{
		if (this != &set)
		{
			mSize = set.mSize;
			mCapacity = set.mCapacity;

			if (mBuffer)
			{
				delete[] mBuffer;
			}

			mBuffer = set.mBuffer;

			set.mBuffer = nullptr;
			set.mSize = 0;
			set.capacity = 0;

		}
		return *this;
	}

	template <class T>
	size_t LiniarSet<T>::getElement(const T& el)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (mBuffer[i] == el)
				return  i;
		}

		return 0;
	}

	template <class T>
	bool LiniarSet<T>::isEmpty()
	{
		return mSize == 0;
	}

	template <class T>
	size_t LiniarSet<T>::size()
	{
		return mSize;
	}

	template <class T>
	size_t LiniarSet<T>::capacity()
	{
		return mCapacity;
	}

	template <class T>
	void LiniarSet<T>::clear()
	{
		// destorying all the elements int eh buffer, not the buffer itself though
		for (size_t i = 0; i < mSize; ++i)
		{
			mBuffer[i].~T();
		}
	}

	template <class T>
	void LiniarSet<T>::addElement(const T& el)
	{
		if (mSize >= mCapacity)
		{
			reserve(mCapacity * 2); //when not enough space we double the capacity
		}
		mBuffer[mSize++] = el;
	}

	template <class T>
	bool LiniarSet<T>::exists(const T& el)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (mBuffer[i] == el)
			{
				return true;
			}
		}
		return false;
	}

	template <class T>
	LiniarSet<T> LiniarSet<T>::reunion(const LiniarSet<T>& set)
	{
		// crete a new set from this set
		LiniarSet<T> reu(*this);

		// add the second set to the new one
		// hence creating a reunion set
		for (size_t i = 0; i < set.mSize; ++i)
		{
			T& el = set.mBuffer[i];
			if (reu.exists(el) == false)
			{
				reu.addElement(el);
			}
		}

		return reu;
	}

	template <class T>
	LiniarSet<T> LiniarSet<T>::intersection(const LiniarSet<T>& set)
	{
		LiniarSet<T> inter;

		// check if elements from the second set exist in this one
		// add those that exist to the intersection set
		for (size_t i = 0; i < set.mSize; ++i)
		{
			T& el = set.mBuffer[i];
			if (this.exists(el) == true)
			{
				inter.addElement(el);
			}
		}

		return inter;
	}

	template <class T>
	LiniarSet<T> LiniarSet<T>::difference(const LiniarSet<T>& set)
	{
		// a difference set is the set with
		// elements of this set that are not in the second set

		LiniarSet<T> diff;
		for (size_t i = 0; i < set.mSize; ++i)
		{
			T& el = set.mBuffer[i];
			if (this.exists(el) == false)
			{
				diff.addElement(mBuffer[i]); //add elements from this set
			}
		}

		return diff;
	}

	template <class T>
	void LiniarSet<T>::reserve(size_t capacity)
	{
		if (capacity > mCapacity)
		{
			mCapacity = capacity;
			T* newBuffer = new T[mCapacity];

			for (size_t i = 0; i < mSize; ++i)
			{
				// copy via placement new()
				new (newBuffer + i) T(mBuffer[i]);
			}

			delete[] mBuffer;
			mBuffer = newBuffer;
		}
	}

	template <class T>
	void LiniarSet<T>::resize(size_t size)
	{
		if (size > mCapacity)
		{
			reserve(size);
		}
		mSize = size;
	}
}

#endif /* LINIAR_SET_HPP */
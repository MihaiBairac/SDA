#ifndef REF_COUNTED_PTR_HPP
#define REF_COUNTED_PTR_HPP

#include "ClassHelper.h"
#include "Utility.hpp"
#include <iostream>

// Smart ptr - non thread-safe

namespace SDA
{
	/* RefCounter Helper class */
	class RefCounter
	{
	public:
		RefCounter() : mRefCount(0) {}
		~RefCounter() {}

		inline size_t GetRefCount() { return mRefCount; }

		inline size_t AddRefCount() { return ++mRefCount; }
		inline size_t DecRefCount() { return --mRefCount; }
	private:
		NON_COPY_AND_MOVE(RefCounter)

		size_t mRefCount;
	};

	/* RefCountedPtr - a reference counted auto pointer */
	template <class T>
	class RefCountedPtr
	{
	public:
		RefCountedPtr();
		RefCountedPtr(T* dataPtr);
		RefCountedPtr(const RefCountedPtr<T>& refCntPtr);
		RefCountedPtr(RefCountedPtr<T>&& refCntPtr);
		virtual ~RefCountedPtr();

		RefCountedPtr<T>& operator =(const RefCountedPtr<T>& refCntPtr);
		RefCountedPtr<T>& operator =(RefCountedPtr<T>&& refCntPtr);

		void Swap(RefCountedPtr<T>& refCntPtr);

		void Reset();
		void Reset(T* dataPtr);

		size_t GetRefCount() const;
		T* GetPtr();
		const T* GetPtr() const;

		T* operator ->();
		const T* operator ->() const;
		T& operator *();
		const T& operator *() const;

	private:
		void Copy(const RefCountedPtr<T>& refCntPtr);
		void Move(RefCountedPtr<T>&& refCntPtr);
		void Release();
		size_t AddRefCount();
		size_t DecRefCount();

		RefCounter* mRefCounterPtr;
		T* mDataPtr;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T>
	RefCountedPtr<T>::RefCountedPtr()
		: mRefCounterPtr(new RefCounter()), mDataPtr(nullptr)
	{}

	template <class T>
	RefCountedPtr<T>::RefCountedPtr(T* dataPtr)
		: mRefCounterPtr(new RefCounter()), mDataPtr(dataPtr)
	{
	//	std::cout << "RefCountedPtr::Ctor(T*data) " << std::endl;

		AddRefCount();
	}

	template <class T>
	RefCountedPtr<T>::RefCountedPtr(const RefCountedPtr<T>& refCntPtr)
	{
	//	std::cout << "RefCountedPtr::Copy<T> Ctor() " << std::endl;

		Copy(refCntPtr);
	}

	template <class T>
	RefCountedPtr<T>::RefCountedPtr(RefCountedPtr<T>&& refCntPtr)
	{
	//	std::cout << "RefCountedPtr::Move<T> Ctor() " << std::endl;

		Move(std::move(refCntPtr));
	}

	template <class T>
	RefCountedPtr<T>::~RefCountedPtr()
	{
	//	std::cout << "RefCountedPtr::Dtor() " << std::endl;

		Release();
	}

	template <class T>
	RefCountedPtr<T>& RefCountedPtr<T>::operator =(const RefCountedPtr<T>& refCntPtr)
	{
	//	std::cout << "RefCountedPtr::Copy<T> operator = " << std::endl;

		RefCountedPtr<T>(refCntPtr).Swap(*this);

		return *this;
	}

	template <class T>
	RefCountedPtr<T>& RefCountedPtr<T>::operator =(RefCountedPtr<T>&& refCntPtr)
	{
	//	std::cout << "RefCountedPtr::Move<T> operator = " << std::endl;

		RefCountedPtr<T>(std::move(refCntPtr)).Swap(*this);

		return *this;
	}

	template <class T>
	void RefCountedPtr<T>::Swap(RefCountedPtr<T>& refCntPtr)
	{
		SDA::Swap(mDataPtr, refCntPtr.mDataPtr);
		SDA::Swap(mRefCounterPtr, refCntPtr.mRefCounterPtr);
	}

	template <class T>
	void RefCountedPtr<T>::Reset()
	{
		RefCountedPtr<T>().Swap(*this);
	}

	template <class T>
	void RefCountedPtr<T>::Reset(T* dataPtr)
	{
		RefCountedPtr<T>(dataPtr).Swap(*this);
	}

	template <class T>
	void RefCountedPtr<T>::Release()
	{
		if (DecRefCount() == 0)
		{
			if (mDataPtr)
			{
				delete mDataPtr;
				mDataPtr = nullptr;

			//	std::cout << "RefCountedPtr:: Data destroyed " << std::endl;
			}

			if (mRefCounterPtr)
			{
				delete mRefCounterPtr;
				mRefCounterPtr = nullptr;

			//	std::cout << "RefCountedPtr:: RefCounter destroyed " << std::endl;
			}
		}
	}

	template <class T>
	void RefCountedPtr<T>::Copy(const RefCountedPtr<T>& refCntPtr)
	{
		if (this != &refCntPtr) // Avoid self assignment
		{
			refCntPtr.mRefCounterPtr->AddRefCount();

			mDataPtr = refCntPtr.mDataPtr;
			mRefCounterPtr = refCntPtr.mRefCounterPtr;
		}
	}

	template <class T>
	void RefCountedPtr<T>::Move(RefCountedPtr<T>&& refCntPtr)
	{
		if (this != &refCntPtr) // Avoid self assignment
		{
			mDataPtr = refCntPtr.mDataPtr;
			mRefCounterPtr = refCntPtr.mRefCounterPtr;

			refCntPtr.mDataPtr = nullptr;
			refCntPtr.mRefCounterPtr = nullptr;
		}
	}

	template <class T>
	size_t RefCountedPtr<T>::AddRefCount()
	{
		return mRefCounterPtr->AddRefCount();
	}

	template <class T>
	size_t RefCountedPtr<T>::DecRefCount()
	{
		return mRefCounterPtr->DecRefCount();
	}

	template <class T>
	T* RefCountedPtr<T>::operator ->()
	{
		return mDataPtr;
	}

	template <class T>
	const T* RefCountedPtr<T>::operator ->() const
	{
		return mDataPtr;
	}

	template <class T>
	T& RefCountedPtr<T>::operator *()
	{
		return *mDataPtr;
	}

	template <class T>
	const T& RefCountedPtr<T>::operator *() const
	{
		return *mDataPtr;
	}

	template <class T>
	size_t RefCountedPtr<T>::GetRefCount() const
	{
		return mRefCounterPtr->GetRefCount();
	}

	template <class T>
	T* RefCountedPtr<T>::GetPtr()
	{
		return mDataPtr;
	}

	template <class T>
	const T* RefCountedPtr<T>::GetPtr() const
	{
		return mDataPtr;
	}

	///////// COMPARISON //////////

	template <class T>
	bool operator == (const RefCountedPtr<T>& left, const RefCountedPtr<T>& right)
	{
		return left.GetPtr() == right.GetPtr();
	}

	template <class T>
	bool operator != (const RefCountedPtr<T>& left, const RefCountedPtr<T>& right)
	{
		return left.GetPtr() != right.GetPtr();
	}

	template <class T>
	bool operator > (const RefCountedPtr<T>& left, const RefCountedPtr<T>& right)
	{
		return left.GetPtr() > right.GetPtr();
	}

	template <class T>
	bool operator >= (const RefCountedPtr<T>& left, const RefCountedPtr<T>& right)
	{
		return left.GetPtr() >= right.GetPtr();
	}

	template <class T>
	bool operator < (const RefCountedPtr<T>& left, const RefCountedPtr<T>& right)
	{
		return left.GetPtr() < right.GetPtr();
	}

	template <class T>
	bool operator <= (const RefCountedPtr<T>& left, const RefCountedPtr<T>& right)
	{
		return left.GetPtr() <= right.GetPtr();
	}
}

#endif /* REF_COUNTED_PTR_HPP */
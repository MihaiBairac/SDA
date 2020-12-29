#ifndef SINGLETON_HPP
#define SINGLETON_HPP

// The Scott Meyers - thread-safe singleton 
// using static variables with block scope

#include "ClassHelper.h"
#include <iostream>

namespace SDA
{
	template<class T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			// the static instance is created only once
			static T mInstance;

			return mInstance;
		}

	protected:
		Singleton();
		virtual ~Singleton();

	private:
		NON_COPY_AND_MOVE(Singleton)
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template<class T>
	Singleton<T>::Singleton() = default;

	template<class T>
	Singleton<T>::~Singleton() = default;
}

#endif /* SINGLETON_HPP */
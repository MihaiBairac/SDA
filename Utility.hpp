#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <utility> // std::move()
#include "Vector.hpp"

namespace SDA
{
	template <class T>
	void Swap(T& t1, T& t2)
	{
		T tmp(std::move(t1));
		t1 = std::move(t2);
		t2 = std::move(tmp);
	}

	template <class T>
	T Min(T& t1, T& t2)
	{
		return (t1 < t2 ? t1 : t2);
	}

	template <class T>
	T Max(T& t1, T& t2)
	{
		return (t1 > t2 ? t1 : t2);
	}

	template <class T>
	T Min(const SDA::Vector<T>& vec)
	{
		T min{};
		
		for (size_t i = 0; i < vec.Size(); ++i)
		{
			if (vec[i] < min)
			{
				min = vec[i];
			}
		}

		return min;
	}

	template <class T>
	T Max(const SDA::Vector<T>& vec)
	{
		T max{};

		for (size_t i = 0; i < vec.Size(); ++i)
		{
			if (vec[i] > max)
			{
				max = vec[i];
			}
		}

		return max;
	}
}

#endif // UTILITY_HPP

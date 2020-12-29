#ifndef PAIR_HPP
#define PAIR_HPP

#include <utility> // std::move()
#include <cassert>
#include <iostream>

namespace SDA
{
	/* Pair container */
	template <class T1, class T2>
	class Pair
	{
	public:
		Pair();
		Pair(const T1& v1, const T2& v2);
		Pair(const Pair<T1, T2>& pair);
		Pair(Pair<T1, T2>&& pair);
		virtual ~Pair();

		Pair<T1, T2>& operator =(const Pair<T1, T2>& pair);
		Pair<T1, T2>& operator =(Pair<T1, T2>&& pair);

		void Swap(Pair<T1, T2>& pair);

		T1& First();
		const T1& First() const;
		T2& Second();
		const T2& Second() const;

		void SetFirst(const T1& v1);
		void SetSecond(const T2& v2);
		void Set(const T1& v1, const T2& v2);
		void Set(const Pair<T1, T2>& pair);


	private:
		void Copy(const Pair<T1, T2>& pair);
		void Move(Pair<T1, T2>&& pair);
		void Destroy();

		T1 mFirst;
		T2 mSecond;
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	template <class T1, class T2>
	Pair<T1, T2>::Pair()
		: mFirst(), mSecond()
	{}

	template <class T1, class T2>
	Pair<T1, T2>::Pair(const T1& v1, const T2& v2)
		: mFirst(v1), mSecond(v2)
	{}

	template <class T1, class T2>
	Pair<T1, T2>::Pair(const Pair<T1, T2>& pair)
	{
		Copy(pair);
	}

	template <class T1, class T2>
	Pair<T1, T2>::Pair(Pair<T1, T2>&& pair)
	{
		Move(pair);
	}

	template <class T1, class T2>
	Pair<T1, T2>::~Pair()
	{
		Destroy();
	}

	template <class T1, class T2>
	Pair<T1, T2 >& Pair<T1, T2>::operator =(const Pair<T1, T2>& pair)
	{
		Copy(pair);

		return *this;
	}

	template <class T1, class T2>
	Pair<T1, T2>& Pair<T1, T2>::operator =(Pair<T1, T2>&& pair)
	{
		Move(pair);

		return *this;
	}

	template <class T1, class T2>
	void Pair<T1, T2>::Swap(Pair<T1, T2>& pair)
	{
		SDA::Swap(mFirst, pair.mFirst);
		SDA::Swap(mSecond, pair.mSecond);
	}

	template <class T1, class T2>
	void Pair<T1, T2>::Copy(const Pair<T1, T2>& pair)
	{
		if (this != &pair)
		{
			mFirst = pair.mFirst;
			mSecond = pair.mSecond;
		}
	}

	template <class T1, class T2>
	void Pair<T1, T2>::Move(Pair<T1, T2>&& pair)
	{
		if (this != &pair)
		{
			mFirst = std::move(pair.mFirst);
			mSecond = std::move(pair.mSecond);
		}
	}

	template <class T1, class T2>
	void Pair<T1, T2>::Destroy()
	{
		mFirst.~T1();
		mSecond.~T2();
	}

	template <class T1, class T2>
	T1& Pair<T1, T2>::First()
	{
		return mFirst;
	}

	template <class T1, class T2>
	const T1& Pair<T1, T2>::First() const
	{
		return mFirst;
	}

	template <class T1, class T2>
	T2& Pair<T1, T2>::Second()
	{
		return mSecond;
	}

	template <class T1, class T2>
	const T2& Pair<T1, T2>::Second() const
	{
		return mSecond;
	}

	template <class T1, class T2>
	void Pair<T1, T2>::SetFirst(const T1& v1)
	{
		mFirst = v1;
	}

	template <class T1, class T2>
	void Pair<T1, T2>::SetSecond(const T2& v2)
	{
		mSecond = v2;
	}

	template <class T1, class T2>
	void Pair<T1, T2>::Set(const T1& v1, const T2& v2)
	{
		Pair(v1, v2);
	}

	template <class T1, class T2>
	void Pair<T1, T2>::Set(const Pair<T1, T2>& pair)
	{
		Pair(pair);
	}

	///////////// COMPARISON /////////////
	template <class T1, class T2>
	bool operator == (const Pair<T1, T2>& left, const Pair<T1, T2>& right)
	{
		return (left.First() == right.First()) && (left.Second() == right.Second());
	}

	template <class T1, class T2>
	bool operator != (const Pair<T1, T2>& left, const Pair<T1, T2>& right)
	{
		return ! (left == right);
	}

	template <class T1, class T2>
	bool operator > (const Pair<T1, T2>& left, const Pair<T1, T2>& right)
	{
		return (left.First() > right.First()) && (left.Second() > right.Second());
	}

	template <class T1, class T2>
	bool operator < (const Pair<T1, T2>& left, const Pair<T1, T2>& right)
	{
		return (right > left);
	}

	template <class T1, class T2>
	bool operator >=(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
	{
		return ! (left < right);
	}

	template <class T1, class T2>
	bool operator <=(const Pair<T1, T2>& left, const Pair<T1, T2>& right)
	{
		return ! (left > right);
	}

	/////////// INPUT & OUTPUT /////////////
	template <class T1, class T2>
	std::ostream& operator << (std::ostream& out, const Pair<T1, T2>& pair)
	{
		std::cout << "pair: (" << pair.First() << ", " << pair.Second() << ")" << std::endl;

		return out;
	}

	template <class T1, class T2>
	std::istream& operator >> (std::istream& in, Pair<T1, T2>& pair)
	{
		in >> pair.First() >> pair.Second();

		return in;
	}
}

#endif /* PAIR_HPP */
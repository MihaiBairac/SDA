#ifndef TUPLE_HPP
#define TUPLE_HPP

// http://blogs.microsoft.co.il/sasha/2015/01/12/implementing-tuple-part-1/

namespace SDA
{
	template <class T>
	class Tuple
	{
	public:

		Tuple();
		Tuple(size_t size);
		Tuple(size_t size, const T& val);
		Tuple(const Tuple<T>& vec);
		Tuple(Tuple<T>&& vec);
		~Tuple();


	private:
	};
}
#endif /* TUPLE_HPP */
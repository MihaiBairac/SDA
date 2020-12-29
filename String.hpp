#ifndef STRING_HPP
#define STRING_HPP

#include "Vector.hpp"

namespace SDA
{
	/* String container */
	class String : public Vector<char>
	{
	public:
		String();
		String(size_t count, char chr);
		String(const char* charPtr);
		String(const String& str);
		String(String&& str);
		virtual ~String();

		String& operator =(const String& str);
		String& operator =(String&& str);

	private:
	};
}

/* As we do use templates we have to provie the definition in the header */
//////////////// IMPLEMENTATION ////////////

namespace SDA
{
	String::String()
		: Vector<char>()
	{}

	String::String(size_t count, char chr)
		: Vector<char>(count + 1, chr)
	{
		mBuffer[count] = '/0';
	}

	String::String(const char* charPtr)
	{
		//TODO
	}

	String::String(const String& str)
	{
		Vector<char>::Copy(str);
	}

	String::String(String&& str)
	{
		Vector<char>::Move(str);
	}

	String::~String()
	{
		Destroy();
	}

	String& String::operator =(const String& str)
	{
		Copy(str);

		return *this;
	}

	String& String::operator =(String&& str)
	{
		Move(str);

		return *this;
	}

}

#endif /* STRING_HPP */
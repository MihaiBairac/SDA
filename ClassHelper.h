/* Author: BAIRAC MIHAI */

#ifndef CLASS_HELPER_H
#define CLASS_HELPER_H

// macro that helps make classes non copyable and non movable
#define NON_COPY_AND_MOVE(TypeName) \
	TypeName(const TypeName&) = delete; \
	TypeName(const TypeName&&) = delete; \
	void operator =(const TypeName&) = delete; \
	void operator =(const TypeName&&) = delete;

#endif /* CLASS_HELPER_H */
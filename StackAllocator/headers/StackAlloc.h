#pragma once
#include "BufferChain.h"

//template<typename T>
template<typename T>
class StackAlloc {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

public:

	inline explicit StackAlloc() {}
	inline explicit StackAlloc(StackAlloc&&) {}
	inline ~StackAlloc() {}
	inline explicit StackAlloc(StackAlloc const&) {}
	template<typename U>
	inline explicit StackAlloc(StackAlloc<U> const&) {}

	//template<typename T>
	inline pointer allocate(size_type cnt)//size_type cnt,
		//typename std::allocator<T>::const_ptr = 0) {
	{
		return static_cast<pointer>(_chain->get_ptr_for_type<value_type>(cnt));//reinterpret_cast<ptr<T> >(::operator new(cnt * sizeof(T)));
	}

	inline void deallocate(pointer p, size_type) {}

	inline bool operator==(StackAlloc const&) { return true; }
	inline bool operator!=(StackAlloc const& a) { return !operator==(a); }
	
	template<typename U>
	struct rebind {
		using other = StackAlloc<U>;
	};

private:
	std::shared_ptr<BufferChain> _chain;
};


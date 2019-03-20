#pragma once
#include "BufferChain.h"

//template<typename T>
template<typename T>
class StackAlloc {
	template<class U>
	friend class StackAlloc;

public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

public:
	template<class U>
	StackAlloc(const StackAlloc<U>& other) :
		_chain(other._chain) {}

	explicit StackAlloc() = default;
	explicit StackAlloc(StackAlloc&&) = default;
	~StackAlloc() {}
	StackAlloc(const StackAlloc&) = default;

	//template<typename U>
	//explicit StackAlloc(StackAlloc<U> const&) = default;

	pointer allocate(size_type cnt) {
		return static_cast<pointer>(_chain->get_ptr_for_type<value_type>(cnt));
	}

	inline void deallocate(pointer p, size_type) {}

	inline bool operator==(StackAlloc const&) { 
		return true; 
	}

	inline bool operator!=(StackAlloc const& a) { 
		return !operator==(a); 
	}
	
	template<typename U>
	struct rebind {
		using other = StackAlloc<U>;
	};

private:
	std::shared_ptr<BufferChain> _chain = std::make_shared<BufferChain>(BufferChain());
};

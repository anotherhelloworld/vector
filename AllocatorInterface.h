#pragma once
#include <cstddef>
#include <iostream>
#include "Pool.h"

template <class T>
struct Allocator {
	typedef T value_type;	
	typedef value_type* pointer;	
	
	Allocator() throw() {
		//pool = &Pool(1023);
	};

	Allocator(const Pool& _pool) throw() {
		pool = const_cast<Pool*>(&_pool); 
	};

	template<class U>	
	Allocator(const Allocator<U>& a) {
		pool = a.pool;
	}

	~Allocator() throw() {}	

	T* allocate(std::size_t n) {				
		return static_cast<pointer>((T*)(*pool).Allocate(sizeof(T) * n));		
	};

	void deallocate(T* p, std::size_t n) {		
		(*pool).Deallocate(p);		
	};	

	template<class U, class... Args>
	void construct(U* p, Args&&... args) {
		new((void *)p) U(std::forward<Args>(args)...);
	}

	template<class U>
	void destroy(U* p) {
		p->~U();
	}

	void Increment(void* ptr) {
		(*pool).Increment(ptr);
	}
	Pool* pool;	
protected:	
};

template <class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&) throw() {
	return true;
};
template <class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) throw() {
	return false;
};


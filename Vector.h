#pragma once
#include "Allocator.h"

template <class T, class Alloc = Allocator<T>>
class Vector {
public:
	Vector(size_t n = 0) {		
		sizeRe = n * 2;		
		sizeLogic = n;
		if (n != 0) {			
			begin = alloc.allocate(sizeRe);
		}
	};

	Vector(size_t n, const Alloc& _alloc) {
		sizeRe = n * 2;
		sizeLogic = n;
		alloc = _alloc;
		if (n != 0) {			
			begin = alloc.allocate(sizeRe);
		}
	};
	
	/*Vector(size_t n, T a = T(), const Alloc& _alloc = Alloc()) {
		sizeRe = n * 2;	
		sizeLogic = n;
		alloc = _alloc;
		if (n != 0) {			
			begin = (*alloc).allocate(sizeRe);
		}		
	};*/

	Vector(Vector<T, Alloc>& v) { 
		sizeRe = v.sizeRe;
		sizeLogic = v.sizeLogic;
		begin = v.begin;
		alloc = v.alloc;
		alloc.Increment(begin);		
	}
	Vector<T, Alloc>& operator =(const Vector<T, Alloc>& v) {
		sizeRe = v.sizeRe;
		sizeLogic = v.sizeLogic;
		begin = v.begin;
		alloc = v.alloc;
		alloc.Increment(begin);		
		return *this;
	}
	~Vector() {
		for (size_t i = 0; i < sizeLogic; i++) {
			(begin + i)->~T();
		}
		if (sizeRe != 0) {			
			alloc.deallocate(begin, 0);
		}
	}	
	T& operator [](size_t index) { return *(begin + index); }
	void PushBack(const T elem) {
		if (++sizeLogic > sizeRe) {			
			T* newBegin = alloc.allocate(sizeLogic * 2);

			if (sizeRe != 0) {
				memcpy(newBegin, begin, sizeof(T) * sizeRe);
				alloc.deallocate(begin, 0);
			}						
			begin = newBegin;
			sizeRe = sizeLogic * 2;
		}
		*(begin + sizeLogic - 1) = elem;
	}
	size_t Size() { return sizeLogic; }
private:
	Alloc alloc;
	size_t sizeRe;
	size_t sizeLogic;
	T* begin;
};


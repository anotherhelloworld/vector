#pragma once
#include "Allocator.h"

const int include = Allocator::GetInstance().Include();

template <class T>
class Vector {
public:
	Vector(size_t n = 0) {		
		sizeRe = n * 2;		
		//begin = (T*)Allocator::Allocate(sizeof(T) * sizeRe);
		sizeLogic = n;
		if (n != 0) {
			begin = (T*)Allocator::GetInstance().Allocate(sizeof(T) * sizeRe);
		}
	};
	Vector(Vector<T>& v) { 
		sizeRe = v.sizeRe;
		sizeLogic = v.sizeLogic;
		begin = v.begin;
		Allocator::GetInstance().Increment(begin);
	}
	Vector<T>& operator =(const Vector<T>& v) {
		sizeRe = v.sizeRe;
		sizeLogic = v.sizeLogic;
		begin = v.begin;
		Allocator::GetInstance().Increment(begin);
		return *this;
	}
	~Vector() {
		if (sizeRe != 0) {			
			Allocator::GetInstance().Deallocate(begin);
		}
	}	
	T& operator [](size_t index) { return *(begin + index); }
	void PushBack(const T elem) {
		if (++sizeLogic > sizeRe) {
			T* newBegin = (T*)Allocator::GetInstance().Allocate(sizeof(T) * sizeLogic * 2);
			//T* newBegin = (T*)Allocator::Allocate(sizeof(T) * sizeRe * 2);
			
			if (sizeRe != 0) {
				memcpy(newBegin, begin, sizeof(T) * sizeRe);
				Allocator::GetInstance().Deallocate(begin);
			}			
			//Allocator::Deallocate(begin);
			begin = newBegin;
			sizeRe = sizeLogic * 2;
		}
		*(begin + sizeLogic - 1) = elem;
	}
	size_t Size() { return sizeLogic; }
private:
	size_t sizeRe;
	size_t sizeLogic;
	T* begin;
};


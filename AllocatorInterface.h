#include <cstddef>
#include "MemoryManager.h"
template <class T>
struct Allocator {
	typedef T value_type;	
	typedef value_type* pointer;	
	
	Allocator();
	~Allocator() throw() {}
	template <class U> Allocator(const Allocator<U>& other);
	T* allocate(std::size_t n) {
		//return static_cast<pointer>(operator new(n * sizeof(T)));
		return static_cast<pointer>((T*)MemoryManager::GetInstance().Allocate(sizeof(T) * n));
	};
	void deallocate(T* p, std::size_t n) {
		//operator delete(p);
		MemoryManager::GetInstance().Deallocate(p);
	};
};

template <typename T>
Allocator<T>::Allocator() {		
	//alloc = MemoryManager();
};

template <class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&) throw() {
	return true;
};
template <class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) throw() {
	return false;
};
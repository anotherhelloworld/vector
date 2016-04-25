#pragma once

class Allocator {
public:
	static Allocator& GetInstance() {
		static Allocator allocator;
		return allocator;
	}
	static void* Allocate(size_t n) {
		return new char[n];
	}
	static void Deallocate(void* ptr) {
		delete[] ptr;
	}
private:
	Allocator() {	
	}
	~Allocator() {}	
};
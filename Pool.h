#pragma once
#define MBYTE 1048576
#include <map>
#include <algorithm>
#include <vector>

struct Index {
	size_t size;
	size_t counter;
	Index(size_t size = 0, size_t counter = 0) :
		size(size), counter(counter) {};
};

class Pool {
public:	
	Pool(size_t);	
	Pool(const Pool&);
	Pool& operator =(const Pool&);
	~Pool() {}
	void* Allocate(size_t);
	void Deallocate(void*);	
	void Increment(void*);	
private:	
	std::map <size_t, Index>::iterator& Merge(std::map <size_t, Index>::iterator&,
		std::map <size_t, Index>::iterator&);	
	std::map <size_t, Index> fragments;
	std::map <size_t, size_t> sortedFragments;
	char* mem;
};

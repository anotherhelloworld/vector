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

class Allocator {
public:	
	static Allocator& GetInstance() {
		static Allocator allocator;
		return allocator;
	}
	void* Allocate(size_t n) {
		auto it = sortedFragments.lower_bound(n);
		if (it == sortedFragments.end()) {			
			throw "Fragment not found";
		}
		else {
			size_t index = it->second;
			sortedFragments.erase(it);
			auto it2 = fragments.find(index);
			it2->second.counter = 1;			
			if (it2->second.size > n) {				
				size_t tmpIndex = it2->first + n;
				size_t tmpSize = it2->second.size - n;
				fragments.insert(std::make_pair(tmpIndex, Index(tmpSize, 1)));
				sortedFragments.insert(std::make_pair(tmpSize, tmpIndex));
			}
			it2->second.size = n;
			return &mem[it2->first];
		}
		return nullptr;
	}
	void Deallocate(void* ptr) {
		size_t index = (char*)ptr - &mem[0];		
		auto cur = fragments.find(index);
		if (--cur->second.counter != 0) {
			return;
		}
		auto it = cur;		
		if (it != fragments.begin()) {
			if ((--it)->second.counter == 0) {
				Merge(it, cur);
			}						
		}		
		it = cur;
		if (++it != fragments.end()) {
			if (it->second.counter == 0) {
				Merge(it, cur);
			}
		}		
		sortedFragments.insert(std::make_pair(fragments[index].size, index));		
	}
	void Increment(void* ptr) {
		size_t index = (char*)ptr - &mem[0];
		fragments[index].counter++;		
	}
	int Include() { return 1; }
private:	
	Allocator() {		
		fragments.insert(std::make_pair(0, Index(MBYTE * 500, 0)));
		sortedFragments.insert(std::make_pair(MBYTE * 500, 0));
	}
	~Allocator() {}
	void Merge(std::map <size_t, Index>::iterator& it,
		std::map <size_t, Index>::iterator& cur) {
		cur->second.size += it->second.size;
		auto range = sortedFragments.equal_range(it->second.size);
		for (auto i = range.first; i != range.second; i++) {
			if (i->second == it->first) {
				sortedFragments.erase(i);
				break;
			}
		}
		fragments.erase(it);
	};
	std::map <size_t, Index> fragments; 
	std::map <size_t, size_t> sortedFragments;
	char mem[MBYTE * 500];
};


#pragma once
#define MBYTE 1048576
#include <map>
#include <algorithm>
#include <vector>

struct Index {
	size_t size;	
	size_t counter;
	std::map<size_t, std::map <size_t, Index>::iterator>::iterator sortedFragment;
	Index(size_t size = 0, size_t counter = 0) :
		size(size), counter(counter) {};
};

class MemoryManager {
public:	
	static MemoryManager& GetInstance() {
		static MemoryManager allocator;
		return allocator;
	}
	void* Allocate(size_t n) {
		auto it = sortedFragments.lower_bound(n);
		if (it == sortedFragments.end()) {			
			throw "Fragment not found";
		}
		else {
			auto it2 = it->second;
			sortedFragments.erase(it);			
			it2->second.counter = 1;			
			if (it2->second.size > n) {
				size_t tmpIndex = it2->first + n;
				size_t tmpSize = it2->second.size - n;
				//std::map<size_t, Index>::iterator* tmp = new std::map<size_t, Index>::iterator;
				auto tmp = fragments.insert(std::make_pair(tmpIndex, Index(tmpSize, 1))).first;
				tmp->second.sortedFragment = sortedFragments.insert(std::make_pair(tmpSize, tmp)).first;				
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
				cur  = Merge(cur, it);
			}						
		}		
		it = cur;
		if (++it != fragments.end()) {
			if (it->second.counter == 0) {
				Merge(it, cur);
			}
		}

		//std::map<size_t, Index>::iterator* tmp = new std::map<size_t, Index>::iterator;	
		//tmp = cur;
		cur->second.sortedFragment = sortedFragments.insert(std::make_pair(cur->second.size, cur)).first;

		//sortedFragments.insert(std::make_pair(fragments[index].size, index));		
	}
	void Increment(void* ptr) {
		size_t index = (char*)ptr - &mem[0];
		fragments[index].counter++;		
	}
	int Include() { return 1; }
private:	
	MemoryManager() {		
		//std::map<size_t, Index>::iterator* tmp = new std::map<size_t, Index>::iterator;
		auto tmp = fragments.insert(std::make_pair(0, Index(MBYTE * 500, 0))).first;
		tmp->second.sortedFragment = sortedFragments.insert(std::make_pair(MBYTE * 500, tmp)).first;
	}
	~MemoryManager() {}
	std::map <size_t, Index>::iterator& Merge(std::map <size_t, Index>::iterator& it,
		std::map <size_t, Index>::iterator& cur) {
		cur->second.size += it->second.size;	
		sortedFragments.erase(it->second.sortedFragment);
		fragments.erase(it);
		return cur;
	};
	std::map <size_t, Index> fragments; 
	std::map <size_t, std::map <size_t, Index>::iterator> sortedFragments;
	char mem[MBYTE * 500];
};


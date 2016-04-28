#include "Pool.h"

Pool::Pool(size_t n = 1024) {
	fragments.insert(std::make_pair(0, Index(n, 0)));
	sortedFragments.insert(std::make_pair(n, 0));
	mem = new char[n];
}

Pool::Pool(const Pool& pool) {
	fragments = pool.fragments;
	sortedFragments = pool.sortedFragments;
	mem = pool.mem;
}

Pool& Pool::operator =(const Pool& pool) {
	fragments = pool.fragments;
	sortedFragments = pool.sortedFragments;
	mem = pool.mem;
	return *this;
}

void* Pool::Allocate(size_t n) {
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
			fragments.insert(std::make_pair(tmpIndex, Index(tmpSize, 0)));
			sortedFragments.insert(std::make_pair(tmpSize, tmpIndex));
		}
		it2->second.size = n;
		return &mem[it2->first];
	}
	return nullptr;
}

void Pool::Deallocate(void* ptr) {
	size_t index = (char*)ptr - &mem[0];
	auto cur = fragments.find(index);
	if (cur == fragments.end()) {
		return;
	}
	if (--cur->second.counter != 0) {
		return;
	}
	auto it = cur;
	if (it != fragments.begin()) {
		if ((--it)->second.counter == 0) {
			cur = Merge(cur, it);
		}
	}
	it = cur;
	if (++it != fragments.end()) {
		if (it->second.counter == 0) {
			Merge(it, cur);
		}
	}
	sortedFragments.insert(std::make_pair(cur->second.size, cur->first));
}

void Pool::Increment(void* ptr) {
	size_t index = (char*)ptr - &mem[0];
	fragments[index].counter++;
}

std::map <size_t, Index>::iterator& Pool::Merge(std::map <size_t, Index>::iterator& it,
	std::map <size_t, Index>::iterator& cur) {
	auto range = sortedFragments.equal_range(cur->second.size);
	for (auto i = range.first; i != range.second; i++) {
		if (i->second == cur->first) {
			sortedFragments.erase(i);
			break;
		}
	}
	cur->second.size += it->second.size;
	range = sortedFragments.equal_range(it->second.size);
	for (auto i = range.first; i != range.second; i++) {
		if (i->second == it->first) {
			sortedFragments.erase(i);
			break;
		}
	}
	fragments.erase(it);
	return cur;
};
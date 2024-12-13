#pragma once
#include <vector>

template <typename T> class Registry {
private:
	int count = 0;
	std::vector<T> items;
public:
	Registry(std::vector<T> defaults = {}) : items{ defaults } {

	}
	void add(T item) {
		items.push_back(item);
		count++;
	}
	const T& operator[](int index) {
		return items[index];
	}
};
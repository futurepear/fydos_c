#pragma once
#include <vector>

template <typename T> class Registry {
private:
	int count = 0;
	std::vector<T> items;
public:
	Registry(std::vector<T> defaults = {}) : items{ defaults } {
		count = static_cast<int>(items.size());
	}
	void add(T item) {
		items.push_back(item);
		count++;
	}
	const T& operator[](int index) {
		return items[index];
	}

	using iterator = std::vector<T>::iterator;
	using const_iterator = std::vector<T>::const_iterator;

	iterator begin() {
		return items.begin();
	}
	iterator end() {
		return items.end();
	}
	const_iterator cbegin() {
		return items.cbegin();
	}
	const_iterator cend() {
		return items.cend();
	}
};
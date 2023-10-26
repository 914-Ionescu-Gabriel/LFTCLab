#pragma once

#include <string>
#include <vector>

template <typename T>
class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	std::pair<int, int> GetHashAndIndex(const T& value);

	void SetSymbol(const T& value);

	bool HasValue(const T& value);

	T FindByHashAndIndex(int hash, int index);

	static const int tableSize = 4096;

	struct Node
	{
		T value;
		bool valid;
		Node(const T& v) : value(v), valid(true) {}
	};

	std::vector<std::vector<Node>> table;

	int HashValue(const T& value);
};

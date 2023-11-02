#include "SymbolTable.h"
#include "Scanner.h"

template <typename T>
SymbolTable<T>::SymbolTable() 
{
    table.resize(tableSize);
}

template <typename T>
SymbolTable<T>::~SymbolTable() {}

template <typename T>
std::pair<int, int> SymbolTable<T>::GetHashAndIndex(const T& value) 
{
    int hash = HashValue(value);
    int index = 0;
    for (const Node& node : table[hash]) 
    {
        if (node.valid && node.value == value) 
        {
            return std::make_pair(hash, index);
        }
        index++;
    }
    return std::make_pair(hash, -1);
}

template <typename T>
void SymbolTable<T>::SetSymbol(const T& value, std::ofstream& st)
{
    bool toPrint = HasValue(value);
    int hash = HashValue(value);
    table[hash].push_back(Node(value));
    if (!toPrint)
    {
        auto hi = GetHashAndIndex(value);
        st << "Entry:(Symbol: '" << value << "', Hash: " << hash << ", Index: " << hi.second << ")\n";
    }
}

template <typename T>
bool SymbolTable<T>::HasValue(const T& value) 
{
    int hash = HashValue(value);
    for (const Node& node : table[hash]) 
    {
        if (node.valid && node.value == value) 
        {
            return true;
        }
    }
    return false;
}

template <typename T>
T SymbolTable<T>::FindByHashAndIndex(int hash, int index) 
{
    if (hash >= 0 && hash < tableSize && index >= 0 && index < table[hash].size() && table[hash][index].valid) 
    {
        return table[hash][index].value;
    }
}

template <typename T>
int SymbolTable<T>::HashValue(const T& value) {
    if constexpr (std::is_same<T, int>::value) {
        return std::hash<T>{}(value) % tableSize;
    }
    else if constexpr (std::is_same<T, std::string>::value) {
        int sum = 0;
        for (char c : value) {
            sum += static_cast<int>(c);
        }
        return sum % tableSize;
    }
    return 0;
}

int main() 
{
    SymbolTable<int> intConst;
    SymbolTable<std::string> stringConst;
    SymbolTable<std::string> idTable;
    const std::string stFileIgnnore = "stIgnore.txt";
    std::ofstream sti(stFileIgnnore);
    intConst.SetSymbol(42, sti);
    stringConst.SetSymbol("test", sti);
    idTable.SetSymbol("test", sti);
    Scanner();
}
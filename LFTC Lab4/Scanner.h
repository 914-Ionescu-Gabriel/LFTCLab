#pragma once

#include <string>
#include <vector>
#include "SymbolTable.h"

class Scanner
{
private:
	std::vector<std::string> tokens;
	const std::string tokenFile = "token.txt";
	const std::string pifFile = "pif.txt";
	const std::string stFile = "st.txt";
	const std::string inFile = "p1err.txt";
	
public:
	bool checkLine(const std::string line, std::ofstream &pif, std::ofstream& st, SymbolTable<int> intConst, SymbolTable<std::string> stringConst, SymbolTable<std::string> idTable);
	void readTokensFromFile();
	void tokenize();
	Scanner();
	~Scanner();

};


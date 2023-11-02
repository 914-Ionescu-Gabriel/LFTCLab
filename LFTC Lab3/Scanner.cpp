#include "Scanner.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <set>

Scanner::Scanner()
{
	readTokensFromFile();
	tokenize();
}

Scanner::~Scanner() {}

void Scanner::readTokensFromFile()
{
	std::ifstream file(tokenFile);
	if (!file.is_open())
	{
		throw std::runtime_error("Unable to open token file");
	}

	std::string token;
	while (file >> token)
	{
		for (size_t i = 0; i < token.size() - 1; ++i)
		{
			if (token[i] == '\\' && token[i + 1] == 'n')
			{
				token.replace(i, 2, "\n");
			}
			else if (token[i] == '\\' && token[i + 1] == 't')
			{
				token.replace(i, 2, "\t");
			}
		}
		tokens.push_back(token);
	}
	file.close();
}

bool isPunctuator(char ch)
{
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
		ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
		ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
		ch == '&' || ch == '|' || ch == '%')
	{
		return true;
	}
	return false;
}

bool isOperator(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '>' || ch == '<' ||
		ch == '=' || ch == '|' || ch == '&' || ch == '%')
	{
		return true;
	}
	return false;
}

char* subString(const char* realStr, int l, int r)
{
	int length = r - l + 1;
	if (length < 0 || l < 0 || l >= strlen(realStr) || r < 0) {
		return NULL;
	}

	char* str = (char*)malloc(length + 1);

	if (str) {
		for (int i = 0; l <= r; i++, l++) {
			str[i] = realStr[l];
		}
		str[length] = '\0';
	}

	return str;
}

bool isKeyword(char* str)
{
	if (!strcmp(str, "if") || !strcmp(str, "else")
		|| !strcmp(str, "for") || !strcmp(str, "int64")
		|| !strcmp(str, "return") || !strcmp(str, "string")
		|| !strcmp(str, "function") || !strcmp(str, "input")
		|| !strcmp(str, "intstring") || !strcmp(str, "cout")
		|| !strcmp(str, "true") || !strcmp(str, "false"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isNumber(char* str)
{
	int i, len = strlen(str), numOfDecimal = 0;
	if (len == 0)
	{
		return false;
	}
	for (i = 0; i < len; i++)
	{
		if (numOfDecimal > 1 && str[i] == '.')
		{
			return false;
		}
		else if (numOfDecimal <= 1)
		{
			numOfDecimal++;
		}
		if (str[i] != '0' && str[i] != '1' && str[i] != '2'
			&& str[i] != '3' && str[i] != '4' && str[i] != '5'
			&& str[i] != '6' && str[i] != '7' && str[i] != '8'
			&& str[i] != '9' || (str[i] == '-' && i > 0))
		{
			return false;
		}
	}
	return true;
}

bool validIdentifier(char* str)
{
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
		str[0] == '3' || str[0] == '4' || str[0] == '5' ||
		str[0] == '6' || str[0] == '7' || str[0] == '8' ||
		str[0] == '9' || isPunctuator(str[0]) == true)
	{
		return false;
	}
	int i, len = strlen(str);
	if (len == 1)
	{
		return true;
	}
	else
	{
		for (i = 1; i < len; i++)
		{
			if (isPunctuator(str[i]) == true)
			{
				return false;
			}
		}
	}
	return true;
}

void cleanup()
{
	std::ifstream input("st.txt");
	std::ofstream stClean("stClean.txt");
	std::set<std::string> uniqueLines;
	std::string line;

	while (std::getline(input, line))
	{
		if (uniqueLines.insert(line).second) 
		{
			stClean << line << '\n';
		}
	}
}

bool Scanner::checkLine(const std::string line, std::ofstream &pif, std::ofstream& st, SymbolTable<int> intConst, SymbolTable<std::string> stringConst, SymbolTable<std::string> idTable)
{
	int left = 0, right = 0;
	const char* lineArray = line.c_str();
	int len = strlen(lineArray);
	while (right <= len && left <= right)
	{
		if (isPunctuator(lineArray[right]) == false)
		{

			if (len - right >= 1)
			{
				if (lineArray[right] == '\t')
				{
					pif << "Entry: (Token: 'TAB')\n";
					std::cout << "Entry: (Token: 'TAB')\n";
					right++;
					while (lineArray[right] == '\t')
					{
						pif << "Entry: (Token: 'TAB')\n";
						std::cout << "Entry: (Token: 'TAB')\n";
						right++;
					}
					left = right;
					right--;
				}
				if (lineArray[right] == '\"')
				{
					pif << "Entry: (Token: '\"')\n";
					std::cout << "Entry: (Token: '\"')\n";
					right++;
					char text[1024] = "";
					int textIndex = 0;
					while (right <= len && lineArray[right] != '\"')
					{
						text[textIndex] = lineArray[right];
						textIndex++;
						right++;
					}
					if (lineArray[right] != '\"')
					{
						pif << "Missing ending \" token. \n";
						pif << "Erroneous line: " << lineArray << "\n";
						std::cout << "Missing ending \" token. \n";
						std::cout << "Erroneous line: " << lineArray << "\n";
						return true;
					}
					else
					{
						stringConst.SetSymbol(text, st);
						std::string textConvert = text;
						pif << "Entry: (Token: '" << text << "')\n";
						std::cout << "Entry: (Token: '" << text << "')\n";
						pif << "Entry: (Token: '\"')\n";
						std::cout << "Entry: (Token: '\"')\n";
						right++;
						left = right;
					}
						}
					}
			//check char in tokens or nah
			std::string converted(1,lineArray[right]);
			if (std::find(tokens.begin(), tokens.end(), converted) != tokens.end())
			{
				right++;
			}
			else if (lineArray[right]!='\n' && lineArray[right]!='\t' && lineArray[right]!='\0')
			{
				pif << "Illegal token present: " << lineArray[right] << "\n";
				pif << "Erroneous line: " << lineArray << "\n";
				std::cout << "Illegal token present: " << lineArray[right] << "\n";
				std::cout << "Erroneous line: " << lineArray << "\n";
				return true;
			}
			else
			{
				right++;
			}
		}
		if (right > len)
		{
			return false;
		}
		if (isPunctuator(lineArray[right]) == true && left == right)
		{
			if (isOperator(lineArray[right]) == true)
			{
				if (len - right >= 1)
				{
					if (lineArray[right] == '>' || lineArray[right] == '<')
					{
						if (lineArray[right + 1] == '=')
						{
							pif << "Entry: (Token: '" << lineArray[right] << "=')\n";
							std::cout << "Entry: (Token: '" << lineArray[right] << "=')\n";
							right++;
						}
						else
						{
							if (lineArray[right]=='<' && lineArray[right + 1] == '|')
							{
								right+=2;
								while (right <= len && lineArray[right] != '|')
								{
									right++;
								}
								if (right >= len)
								{
									pif << "Missing comment ending tokens |>\n";
									pif << "Erroneous line: " << lineArray << "\n";
									std::cout << "Missing comment ending tokens |>\n";
									std::cout << "Erroneous line: " << lineArray << "\n";
									return true;
								}
								if (lineArray[right] != '|' && lineArray[right+1] != '>')
								{
									pif << "Missing comment ending tokens |>\n";
									pif << "Erroneous line: " << lineArray << "\n";
									std::cout << "Missing comment ending tokens |>\n";
									std::cout << "Erroneous line: " << lineArray << "\n";
									return true;
								}
								right++;
							}
							else
							{
								pif << "Entry: (Token: '" << lineArray[right] << "')\n";
								std::cout << "Entry: (Token: '" << lineArray[right] << "=)\n";
							}
						}
					}
					else if (lineArray[right] == '=' && lineArray[right + 1] != '=')
					{
						pif << "Entry: (Token: '" << lineArray[right] << "')\n";
						std::cout << "Entry: (Token: '" << lineArray[right] << "')\n";
					}
					if (lineArray[right] == '=' && lineArray[right + 1] == '=')
					{
						pif << "Entry: (Token: '" << lineArray[right] << "=')\n";
						std::cout << "Entry: (Token: '" << lineArray[right] << "=')\n";
						right++;
					}
					else if (lineArray[right] != '=' && lineArray[right] != '<' && lineArray[right] != '>')
					{
						if (lineArray[right] == '+' && lineArray[right + 1] == '+')
						{
							pif << "Entry: (Token: '" << lineArray[right] << "+')\n";
							std::cout << "Entry: (Token: '" << lineArray[right] << "+')\n";
							right++;
						}
						else if (lineArray[right] == '+' && lineArray[right + 1] == '=')
						{
							pif << "Entry: (Token: '" << lineArray[right] << "=')\n";
							std::cout << "Entry: (Token: '" << lineArray[right] << "=')\n";
							right++;
						}
						else if(lineArray[right] != '\0')
						{
							pif << "Entry: (Token: '" << lineArray[right] << "')\n";
							std::cout << "Entry: (Token: '" << lineArray[right] << "')\n";
						}
					}
				}
				else
				{
					pif << "Entry: (Token: '" << lineArray[right] << "')\n";
					std::cout << "Entry: (Token: '" << lineArray[right] << "')\n";
				}
				
			}
			if (lineArray[right]=='('|| lineArray[right] == ')' || lineArray[right] == ',')
			{
				pif << "Entry: (Token: '" << lineArray[right] << "')\n";
				std::cout << "Entry: (Token: '" << lineArray[right] << "')\n";
			}
			right++;
			left = right;
		}
		else if (isPunctuator(lineArray[right]) == true && left != right || (right == len && left != right))
		{
			char* sub = subString(lineArray, left, right - 1); //Watch it risky conversion cpp moment

			if (isKeyword(sub) == true)
			{
				if (len - right >= 2 && !strcmp(sub,"cout"))
				{
					if (lineArray[right] == '<' && lineArray[right + 1] == '<')
					{
						right += 2;
						pif << "Entry: (Token: '" << sub << "<<')\n";
						std::cout << "Entry: (Token: '" << sub << "<<')\n";
					}
					else
					{
						pif << "Illegal keyword compound: " << lineArray[right] << lineArray[right + 1] << "\n";
						pif << "Erroneous line: " << lineArray << "\n";
						std::cout << "Illegal keyword compound: " << lineArray[right] << lineArray[right + 1] << "\n";
						std::cout << "Erroneous line: " << lineArray << "\n";
						return true;
					}
				}
				else
				{
					pif << "Entry: (Token: '" << sub << "')\n";
					std::cout << "Entry: (Token: '" << sub << "')\n";
				}
			}
			else if (isNumber(sub) == true)
			{
				int converted = std::stoi(sub);
				intConst.SetSymbol(converted, st);
				pif << "Entry: (Token: '" << sub << "')\n";
				std::cout << "Entry: (Token: '" << sub << "')\n";
			}
			else if (validIdentifier(sub) == true
				&& isPunctuator(lineArray[right - 1]) == false)
			{
				idTable.SetSymbol(sub, st);
				pif << "Entry: (Token: '" << sub << "')\n";
				std::cout << "Entry: (Token: '" << sub << "')\n";
			}
			else if (validIdentifier(sub) == false
				&& isPunctuator(lineArray[right - 1]) == false)
			{
				pif << "Illegal identifier: " << sub << "\n";
				pif << "Erroneous line: " << lineArray << "\n";
				std::cout << "Illegal identifier: " << sub << "\n";
				std::cout << "Erroneous line: " << lineArray << "\n";
				return true;
				return true;
			}
			left = right;
		}
	}
	return false;
}

void Scanner::tokenize()
{
    std::ifstream file(inFile);
    std::ofstream pif(pifFile);
	std::ofstream st(stFile);
	bool err = false;
	int lineCount = 1;

	SymbolTable<int> intConst;
	SymbolTable<std::string> stringConst;
	SymbolTable<std::string> idTable;

    if (!file.is_open() || !pif.is_open())
    {
        throw std::runtime_error("Unable to open input or output file");
    }

    std::string line;
    while (std::getline(file, line))
    {
		if (line != "")
		{
			err = checkLine(line, pif, st, intConst, stringConst, idTable);
			if (err)
			{
				pif << "Lexical error at line: " << lineCount << "\n";
				std::cout << "Lexical error at line: " << lineCount << "\n";
				exit(0);
			}
		}
		pif << "Entry: (Token: 'EOL')\n";
		std::cout << "Entry: (Token: 'EOL')\n";
		lineCount++;
    }
	pif << "Entry: (Token: 'EOF')\n";
	std::cout << "Entry: (Token: 'EOF')\n";
	
	st.close();
    file.close();
    pif.close();
	cleanup();
}

//lr0

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

class Grammar
{
public:
    std::set<std::string> terminals, nonTerminals;
    std::string S;
    std::map<std::string, std::set<std::string>> productions;
    std::set<std::string> cutProductions;

    void printTerminals()
    {
        std::cout << "Terminals: ";
        for (std::string terminal : terminals)
        {
            std::cout << terminal << " ";
        }
        std::cout << "\n";
    }
    
    void printNonTerminals()
    {
        std::cout << "Non-terminals: ";
        for (std::string nonTerminal : nonTerminals)
        {
            std::cout << nonTerminal << " ";
        }
        std::cout << "\n";
    }

    void printStart()
    {
        std::cout << "S: "<<S<<"\n";
    }
    void printProductions()
    {
        std::cout << "Productions:\n";

        for (const auto& entry : productions)
        {
            std::cout << entry.first << " -> ";
            for (const std::string& production : entry.second)
            {
                std::cout << production << " | ";
            }
            std::cout << "\n";
        }
    }

    void printProductionsOf(std::string nonTerminal)
    {
        if (productions.find(nonTerminal) != productions.end())
        {
            std::cout << "Productions of " << nonTerminal << ": ";
            for (const std::string& production : productions[nonTerminal])
            {
                std::cout << production << " | ";
            }
            std::cout << "\n";
        }
        else
        {
            std::cout << "No productions found for " << nonTerminal << "\n";
        }
    }

    void processAllTerminals(std::string allTerminals, std::string symbol)
    {
        std::string lineSymbol;

        size_t start = allTerminals.find('{');
        size_t end = allTerminals.find('}');
        std::string content = allTerminals.substr(start + 1, end - start - 1);
        std::stringstream ss(content);
        
        while (std::getline(ss, lineSymbol, ','))
        {
            size_t first = lineSymbol.find_first_not_of(" \t");
            size_t last = lineSymbol.find_last_not_of(" \t");
            lineSymbol = lineSymbol.substr(first, last - first + 1);
            if (symbol == "N")
            {
                nonTerminals.insert(lineSymbol);
            }
            else if (symbol == "E")
            {
                terminals.insert(lineSymbol);
            }
            else if (symbol == "S")
            {
                S = lineSymbol;
            }
            else
            {
                std::cerr << "Unsupported symbol.\n";
            }
        }
    }

    void populateProductions(std::string rawProduction)
    {                                                                                       //Maybe do validation of any sort.
        std::string lineSymbol, arrow, rawNonTerminal, rawResult, right, left;
        arrow = '->';

        rawResult = rawProduction.substr(rawProduction.find(arrow) + 1, rawProduction.length());
        while (rawResult.find('|') != std::string::npos)
        {
            left = rawResult.substr(0, rawResult.find('|'));
            right = rawResult.substr(rawResult.find('|') + 1, rawResult.length());
            size_t first = left.find_first_not_of(" \t");
            size_t last = left.find_last_not_of(" \t");
            left = left.substr(first, last - first + 1);
            cutProductions.insert(left);
            rawResult = right;
        }
        if (rawResult.find('|') == std::string::npos)
        {
            size_t first = rawResult.find_first_not_of(" \t");
            size_t last = rawResult.find_last_not_of(" \t");
            rawResult = rawResult.substr(first, last - first + 1);
            cutProductions.insert(rawResult);
        }
    }

    void insertProduction(std::string symbol)
    {
        productions[symbol] = cutProductions;
        cutProductions.clear();
    }

    bool isContextFree()
    {
        for (const auto& entry : productions)
        {
            if (nonTerminals.find(entry.first) == nonTerminals.end())
            {
                std::cout << "Error: Invalid left-hand side in production: " << entry.first << "\n";
                return false;
            }

            for (const std::string& production : entry.second)
            {
                std::istringstream iss(production);
                std::string word;

                while (iss >> word)
                {
                    if (terminals.find(word) == terminals.end() && nonTerminals.find(word) == nonTerminals.end() && word != "E")
                    {
                        std::cout << "Error: Invalid symbol in production: " << production << "\n";
                        return false;
                    }
                }
            }
        }

        return true;
    }
};

int main()
{
    std::ifstream inputfile("g1.txt");
    Grammar grammar;
    bool expectProduction = false;
    
    if (!inputfile.is_open())
    {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    std::string symbol;
    std::string rawNonTerminals, production, rawTerminals, rawStart, rawProduction, furtherValidationMaby;
    while (inputfile >> symbol)                                                     //Further validation for grammer text file
    {
        if (symbol == "N" && expectProduction == false)
        {
            std::getline(inputfile, rawNonTerminals);
            grammar.processAllTerminals(rawNonTerminals, symbol);
        }
        else if (symbol == "E" && expectProduction == false)
        {
            std::getline(inputfile, rawTerminals);
            grammar.processAllTerminals(rawTerminals, symbol);
        }
        else if (symbol == "S" && expectProduction == false)
        {
            std::getline(inputfile, rawStart);
            grammar.processAllTerminals(rawStart, symbol);
        }
        else if (symbol == "P" && expectProduction == false)    //Scan for { for flexible grammar file maybe
        {
            expectProduction = true;
            std::getline(inputfile, furtherValidationMaby);
        }
        else if (symbol == "}")
        {
            break;
        }
        else if (expectProduction == true)
        {
            std::getline(inputfile, rawProduction);
            grammar.populateProductions(rawProduction);
            grammar.insertProduction(symbol);
        }
    }

    inputfile.close();

    grammar.printTerminals();
    grammar.printNonTerminals();
    grammar.printStart();
    grammar.printProductions();

    grammar.printProductionsOf("S");
    if (grammar.isContextFree())
    {
        std::cout << "Grammar is context free";
    }
    else
    {
        std::cout << "Grammar is not context free";
    }

    return 0;
}
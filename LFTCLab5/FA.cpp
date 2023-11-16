#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

struct Transition
{
    std::string currentState;
    std::string nextTerminal;
    std::string targetState;
};

struct FiniteAutomaton
{
    std::vector<std::string> states;
    std::vector<std::string> alphabet;
    std::vector<Transition> transitions;
    std::string initialState;
    std::vector<std::string> finalStates;
};

void displayElements(const FiniteAutomaton& fa)
{
    int choice;
    std::cout << "1. States: ";
    std::cout << "\n2. Alphabet: ";
    std::cout << "\n3. Transitions:";
    std::cout << "\n4. Initial State: \n";
    std::cout << "5. Final States: \n";
    std::cout << "6. Back\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        for (const auto& state : fa.states)
        {
            std::cout << state << " ";
        }
        std::cout << "\n";
        break;
    case 2:
        for (const auto& symbol : fa.alphabet)
        {
            std::cout << symbol << " ";
        }
        std::cout << "\n";
        break;
    case 3:
        for (const auto& transition : fa.transitions)
        {
            std::cout << "(" << transition.currentState << " " << transition.nextTerminal << " " << transition.targetState << ") ";
        }
        std::cout << "\n";
        break;
    case 4:
        std::cout << fa.initialState << "\n";
        break;
    case 5:
        for (const auto& finalState : fa.finalStates)
        {
            std::cout << finalState << " ";
        }
        break;
    case 6:
        break;
    default:
        std::cout << "Invalid choice. Please try again.\n";
    }
    std::cout << "\n";
}

bool isSequenceAccepted(const FiniteAutomaton& fa, const std::string& sequence)
{
    std::string currentState = fa.initialState;

    for (char symbol : sequence)
    {
        std::string symbolStr(1, symbol);

        auto it = std::find_if(fa.transitions.begin(), fa.transitions.end(), [currentState, symbolStr](const Transition& t)
        {
                return (t.currentState == currentState && t.nextTerminal == symbolStr);
        });

        if (it != fa.transitions.end())
        {
            currentState = it->targetState;
        }
        else
        {
            return false;
        }
    }

    return std::find(fa.finalStates.begin(), fa.finalStates.end(), currentState) != fa.finalStates.end();
}

int main()
{
    int choice, confirmationExit;
    FiniteAutomaton fa;
    std::ifstream inputFile("FA.in");
    std::string key, value, line, symbol, transitionStr, sequence, finalState;
    Transition transition;

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::getline(iss, key, '=');
        std::getline(iss, value, '\n');

        if (key == "states")
        {
            std::istringstream statesStream(value);
            std::string state;
            while (std::getline(statesStream, state, ','))
            {
                fa.states.push_back(state);
            }
        }
        else if (key == "alphabet")
        {
            std::istringstream alphabetStream(value);
            while (std::getline(alphabetStream, symbol, ','))
            {
                fa.alphabet.push_back(symbol);
            }
        }
        else if (key == "transitions")
        {
            std::istringstream transitionsStream(value);
            while (std::getline(transitionsStream, transitionStr, ','))
            {
                std::istringstream transitionIss(transitionStr);
                std::getline(transitionIss, transition.currentState, ' ');
                std::getline(transitionIss, transition.nextTerminal, ' ');
                std::getline(transitionIss, transition.targetState, ' ');
                fa.transitions.push_back(transition);
            }
        }
        else if (key == "initialState")
        {
            fa.initialState = value;
        }
        else if (key == "finalStates")
        {
            std::istringstream finalStatesStream(value);
            while (std::getline(finalStatesStream, finalState, ','))
            {
                fa.finalStates.push_back(finalState);
            }
        }
    }

    inputFile.close();

    do
    {
        std::cout << "Menu:\n";
        std::cout << "1. Display FA Elements\n";
        std::cout << "2. Check Sequence\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cout << "\n";
        switch (choice)
        {
        case 1:
            displayElements(fa);
            break;
        case 2:
            std::cout << "1. Enter the sequence to check: \n";
            std::cout << "2. Back\n";
            std::cout << "Choice: ";
            std::cin >> confirmationExit;
            if (confirmationExit == 2)
            {
                std::cout << "\n";
                break;
            }
            std::cout << "Sequence: ";
            std::cin >> sequence;
            if (isSequenceAccepted(fa, sequence))
            {
                std::cout << "Sequence accepted by the DFA.\n\n";
            }
            else
            {
                std::cout << "Sequence not accepted by the DFA.\n\n";
            }
            break;
        case 3:
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}
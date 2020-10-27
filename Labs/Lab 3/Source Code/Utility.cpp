#include "Utility.h"

#include <set>
#include <cctype>

#define EPLISON 'e'

// Checks if a set has an eplison 
bool Utility::hasEplison(std::set<char> &s)
{
    return s.find(EPLISON) != s.end();
}

// Combines two sets which one set will contain
// all of the elements from the two sets
std::set<char> Utility::setUnion(std::set<char> &s1, std::set<char> &s2)
{
    std::set<char> result;
    result.insert(s1.begin(), s1.end());
    result.insert(s2.begin(), s2.end());
    return result;
}

// Checks if a character is an non-terminal
// where a non-terminal is uppercase and alphanumeric
bool Utility::isNonTerminal(char input)
{
    return std::isalpha(input) && std::isupper(input);
}

#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <set>

class Utility
{
  public:
    Utility(){};
    bool hasEplison(std::set<char> &s);
    std::set<char> setUnion(std::set<char> &s1, std::set<char> &s2);
    bool isNonTerminal(char input);

};

#endif // UTILITY_H

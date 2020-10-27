#include "Parser.h"
#include "Utility.h"
#include "Item.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>

#define EPLISON 'e'

Parser::Parser(std::string &grammerFile) : grammer(grammerFile){};

void Parser::parse()
{
    std::ifstream inputGrammerFile;

    inputGrammerFile.open(grammer.c_str());

    if (!inputGrammerFile.is_open())
    {
        std::cerr << "Error: " << grammer << " not found." << '\n';
        exit(-1);
    }

    std::string input = "";

    int productionsPositionInTheGrammer = 0;

    while (inputGrammerFile >> input)
    {
        if (input == "$") // End of non-terminals
        {
            productionsPositionInTheGrammer = inputGrammerFile.tellg();
            break;
        }
        else
        {
            // Add FIRST(X) = {X}
            first[input[0]].insert(input[0]);
            terminals.insert(input[0]);
        }
    }

    inputGrammerFile.seekg(productionsPositionInTheGrammer);
    input.clear();

    // Now add the productions
    while (inputGrammerFile >> input)
    {
        productions.insert(input);
    }

    // Do not need the $ in our productions
    // since we just want the productions
    productions.erase(productions.find("$"));

    for (auto p : productions)
    {
        std::string currProd = p;
        char nonTerminal = currProd[0];
        nonTerminals.insert(nonTerminal);

        std::string rhs = currProd.substr(3);
        size_t index = 0;

        // If X -> ε is production, add ε to FIRST(X)
        while (index < rhs.length())
        {
            if (rhs[index] == EPLISON)
            {
                first[nonTerminal].insert(EPLISON);
            }
            index++;
        }
    }

    //computeFirst();
    //computeFollow();
    findCanonicalSet();
}

// Computes the FIRST set following the algorithm
// given in the CSE 570/670 website
void Parser::computeFirst()
{
    do
    {
        changed = false;

        // Go through the productions
        for (auto p : productions)
        {
            std::string currProduction = p;
            char nonTerminal = currProduction[0];
            std::string rhs = currProduction.substr(3);

            for (size_t i = 0; i < rhs.length();)
            {
                // Retrieve a set from FIRST(X)
                std::set<char> temp = first[rhs[i]];

                // Add everything in FIRST(Y_i) except for ε and
                // increment i
                if (util.hasEplison(temp))
                {
                    temp.erase(temp.find(EPLISON));
                    std::set<char> unionSet = util.setUnion(first[nonTerminal], temp);

                    for (auto s : unionSet)
                    {
                        auto checkInsert = first[nonTerminal].insert(s);

                        // Check if a set in FIRST has been changed after inserting
                        // a new nonTerminal key
                        if (checkInsert.second)
                        {
                            changed = true;
                        }
                    }

                    i++;
                }
                // Else FIRST(X) = {X}
                else
                {
                    std::set<char> unionSet = util.setUnion(first[nonTerminal], temp);

                    for (auto s : unionSet)
                    {
                        auto checkInsert = first[nonTerminal].insert(s);

                        if (checkInsert.second)
                        {
                            changed = true;
                        }
                    }
                    break;
                }
                // If i > k then add ε to FIRST(X)
                if (i >= rhs.length())
                {
                    auto checkInsert = first[nonTerminal].insert(EPLISON);

                    if (checkInsert.second)
                    {
                        changed = true;
                    }
                }
                i++;
            }
        }
    } while (changed);
}

// Computes the FOLLOW set following the algorithm
// given in the CSE 570/670 website
void Parser::computeFollow()
{
    do
    {
        changed = false;

        // Go through the productions of G
        for (auto p : productions)
        {
            std::string currProduction = p;
            char nonTerminal = currProduction[0];
            std::string rhs = currProduction.substr(3);

            // Go through the characters in the right hand side of the production
            // e.g. E -> T + F where T + F is the RHS
            for (size_t i = 0; i < rhs.length();)
            {
                // Check if the current char in the RHS is a non-terminal
                // and making sure we still have input to process on the RHS
                if (util.isNonTerminal(rhs[i]) && i < rhs.length() - 1)
                {
                    // Retrieve a set in FIRST based the non-terminal
                    // we get from the RHS of the production
                    std::set<char> temp = first[rhs[i + 1]];

                    // If A -> αBβ and ε is in FIRST(β) then
                    // add everything in FOLLOW(A) to FOLLOW(B)
                    if (util.hasEplison(temp))
                    {

                        std::set<char> nonTerminalFollowSet = follow[nonTerminal];
                        for (auto n : nonTerminalFollowSet)
                        {
                            auto checkInsert = follow[rhs[i]].insert(n);
                            if (checkInsert.second)
                            {
                                changed = true;
                            }
                        }
                    }
                    // Else, just add everything in FIRST(β) except ε to FOLLOW(B)
                    for (auto t : temp)
                    {
                        auto checkInsert = follow[rhs[i]].insert(t);
                        if (checkInsert.second)
                        {
                            changed = true;
                        }
                    }
                }
                // If we reached the last character in the RHS
                // then just add everything in FOLLOW(A) to FOLLOW(B)
                else if (util.isNonTerminal(rhs[i]) && i == rhs.length() - 1)
                {
                    std::set<char> temp = follow[nonTerminal];
                    for (auto t : temp)
                    {
                        auto checkInsert = follow[rhs[i]].insert(t);
                        if (checkInsert.second)
                        {
                            changed = true;
                        }
                    }
                }
                i++;
            }
        }
    } while (changed);

    // Add the $ symbol to FOLLOW(S)
    follow['S'].insert('$');
}

std::set<Item> Parser::closure(std::set<Item> items)
{
    std::set<Item> closure;

    // Add evrery item to Closure(i)
    for (auto i : items)
    {
        closure.insert(i);
    }

    do
    {
        changed = false;
        for (auto c : closure)
        {
            std::string currProduction = c.getProduction();
            int dotPos = c.getDotPos();

            char lookAhead = currProduction[dotPos];

            // If A -> αBβ is in Closure(j)
            if (util.isNonTerminal(lookAhead))
            {
                for (auto p : productions)
                {
                    // If B is a production by using a look ahead
                    if (p[0] == lookAhead)
                    {
                        Item temp(p, 3);

                        // Add B to Closure(j)
                        auto checkInsert = closure.insert(temp);
                        if (checkInsert.second)
                        {
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);

    return closure;
}

std::set<Item> Parser::getGoto(std::set<Item> items, char symbol)
{
    // The empty set j
    std::set<Item> j;

    for (auto item : items)
    {
        std::string currProduction = item.getProduction();
        int dotPos = item.getDotPos();

        char lookAhead = currProduction[dotPos];

        if (symbol == lookAhead)
        {
            Item temp(currProduction, dotPos + 1);
            j.insert(temp);
        }
    }
    return closure(j);
}

void Parser::findCanonicalSet()
{
    std::set<char> symbols;

    std::string startProduction = "S->E";

    // Add the first production S->E to the canonical set to
    // generate the correct item sets
    auto firstItem = Item(startProduction, 3);
    std::set<Item> tempItem;
    tempItem.insert(firstItem);
    auto firstClosure = closure(tempItem);
    canonicalSet.insert(LRSet(firstClosure, 0, '\0'));

    for (auto p : productions)
    {
        auto currentItem = Item(p, 3);
        std::set<Item> tempItem;
        tempItem.insert(currentItem);
        auto currentClosure = closure(tempItem);

        symbols = util.setUnion(terminals, nonTerminals);

        int id = 1;
        do
        {
            changed = false;
            for (auto item : canonicalSet)
            {
                for (auto symbol : symbols)
                {
                    auto temp = getGoto(item.getClosure(), symbol);

                    // If GOTO(I, X) is not empty && not in the canonicalSet
                    // then add the GOTO(I, X) to the canonicalSet
                    if (!temp.empty() && !isIn(canonicalSet, temp))
                    {
                        LRSet lrset(temp, id, symbol);
                        canonicalSet.insert(lrset);
                        changed = true;
                        ++id;
                    }
                }
            }
        } while (changed);
    }
}

// Checks if a set of items is in the LRSet
bool Parser::isIn(std::set<LRSet> lrset, std::set<Item> items)
{
    for (auto curr : lrset)
    {
        if (curr.getClosure() == items)
            return true;
    }

    return false;
}

void Parser::printFirst() const
{
    std::cout << "FIRST = " << '\n';
    for (auto f : first)
    {
        std::cout << f.first << " -> ";
        std::cout << "{ ";
        for (auto s : f.second)
        {
            std::cout << s << ' ';
        }
        std::cout << "}";
        std::cout << '\n';
    }
}

void Parser::printFollow() const
{
    std::cout << "FOLLOW = " << '\n';
    for (auto f : follow)
    {
        std::cout << f.first << " -> ";
        std::cout << "{ ";
        for (auto s : f.second)
        {
            std::cout << s << ' ';
        }
        std::cout << "}";
        std::cout << '\n';
    }
}

void Parser::printProductions() const
{
    std::cout << "The productions in " << grammer << " are:" << '\n';
    for (auto p : productions)
    {
        std::cout << p << '\n';
    }
    std::cout << '\n';
}

void Parser::printCanonicalSet() const
{
    for (auto c : canonicalSet)
    {
        auto temp = c.getClosure();
        std::cout << "Item: " << c.getId() << '\n';
        std::cout << "Symbol: " << c.getSymbol() << '\n';
        for (auto t : temp)
        {
            std::string currProd = t.getProduction();
            currProd.insert(currProd.begin() + t.getDotPos(), '.');
            std::cout << "Production: [" << t.getDotPos() << "] = " << currProd << '\n';
        }
        std::cout << '\n';
    }
}

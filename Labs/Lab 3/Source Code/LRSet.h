#pragma once
#ifndef LRSET_H
#define LRSET_H

#include "Item.h"
#include <set>

class LRSet
{
  public:
    LRSet(std::set<Item> closure, int id, char symbol)
        : closure(closure), id(id), symbol(symbol){};

    std::set<Item> getClosure() const;
    int getId() const;
    char getSymbol() const;

    bool operator<(const LRSet &) const;

  private:
    std::set<Item> closure;
    int id;
    char symbol;
};

#endif // LRSET_H

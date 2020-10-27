#include "LRSet.h"
#include "Item.h"

#include <set>

std::set<Item> LRSet::getClosure() const
{
    return closure;
}

int LRSet::getId() const
{
    return id;
}

char LRSet::getSymbol() const
{
    return symbol;
}

bool LRSet::operator<(const LRSet &other) const
{
    return id < other.id;
}

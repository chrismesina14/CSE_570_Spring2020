#include "Item.h"
#include <string>

std::string Item::getProduction()
{
    return production;
}

int Item::getDotPos()
{
    return dotPos;
}

bool Item::operator==(const Item &other) const
{
    return other.production == production;
}

bool Item::operator<(const Item &other) const
{
    return other.production > production || dotPos < other.dotPos;
}

Item &Item::operator=(const Item &other)
{
    production = other.production;
    dotPos = other.dotPos;
    return *this;
}

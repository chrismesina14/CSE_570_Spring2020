#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "Utility.h"
#include "Item.h"
#include "LRSet.h"

#include <string>
#include <map>
#include <set>
#include <fstream>

class Parser
{
public:
  Parser(std::string &grammerFile);

  void parse();
  void computeFirst();
  void computeFollow();
  void findCanonicalSet();

  bool isIn(std::set<LRSet> lrset, std::set<Item> items);

  std::set<Item> closure(std::set<Item> items);
  std::set<Item> getGoto(std::set<Item> items, char symbol);

  void printFirst() const;
  void printFollow() const;
  void printProductions() const;
  void printCanonicalSet() const;

private:
  std::string grammer;

  std::set<std::string> productions;
  std::set<char> nonTerminals;
  std::set<char> terminals;
  std::map<char, std::set<char> > first;
  std::map<char, std::set<char> > follow;
  std::set<LRSet> canonicalSet;

  Utility util;

  bool changed;
};

#endif // PARSER_H

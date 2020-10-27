/*
 * Name: Christian Mesina, Hugo Romero, Luis Escobar Urrutia
 * Class: CSE 570 Compilers Lab 3
 * Instructor: Dr. Ernesto Gomez
**/

#include "Parser.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Please run the program in this format: ./lab3 <filename>\n";
        return 0;
    }

    std::string grammer = argv[1];

    Parser *parser = new Parser(grammer);

    parser->parse();

    //parser->printFirst();

    //parser->printFollow();

    parser->printCanonicalSet();

    delete parser;

    return 0;
}

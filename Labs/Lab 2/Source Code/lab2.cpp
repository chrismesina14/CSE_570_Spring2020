/*
* Christian Mesina, Hugo Romero, Luis Escobar Urrutia
* CSE 570 Compilers Lab 2
* Instructor: Dr. Ernesto Gomez
*
**/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<string> first_set;
vector<string> follow_set;
vector<string> productions;
vector<string> terminal_sets;
vector<string> non_terminal_sets;
vector<string> final_non_terminal_sets;
vector<string> right_side_non_terminal_sets;
vector<string> follow_non_terminal_sets;

bool productionRules = true;
bool expressionsFirst = false;
bool isTerminal(string val);

string dollar = "$";
string epsilon = "e";
string arrow = "->";

int followCounter = 0;
int First(string line);
int Follow(string line);

void printSets();

int main(int argc, char *argv[])
{
	string line;
	int lineCount = 0;
	int productionRuleCounter = 0;

	ifstream myfile("G417.txt");
	if(myfile.is_open())
	{
		while(getline(myfile,line))
		{
			lineCount++;

			if(line == dollar && productionRuleCounter == 1) 
            {
				productionRuleCounter = 1; 
				productionRules = false;
			}

			if(line == dollar && productionRuleCounter == 0) 
            {
				productionRuleCounter = 1; 
				productionRules = true;
				expressionsFirst = true;
			}

			First(line);
			Follow(line);
		}

		myfile.close();
		cout << "Line count " << lineCount << endl << endl;
	}
	else cout << "Unable to open file" << endl << endl; 
	
    printSets();
	
    return 0;
}


int First(string line) 
{
	// First(a) is the set of terminals derived from a
	// the First of a terminal is that terminal
	if(line.length() == 1 && line != dollar) 
    {

		char c = line[0];
		if(!isupper(c)) 
        {
			terminal_sets.push_back(line);
		}
	}

	if(line.substr(1,2) == arrow) 
    {
		string terminalSymbol = line.substr(3,10);
		string non_terminals = line.substr(0,1);

		if(expressionsFirst == true) 
        {
			right_side_non_terminal_sets.push_back(terminalSymbol);
		}

		// found arrow operator so we add entire line as production rule
		productions.push_back(line);

		if(find(non_terminal_sets.begin(), non_terminal_sets.end(), non_terminals) != non_terminal_sets.end())
		{
		  //does nothing
		}
		else 
        {
			char c = line[0];
			if(isupper(c)) 
            {
				non_terminal_sets.push_back(non_terminals);
			}
		}

		//for rule of A → ϵ, add the empty string ϵ to FIRST(A).
		if(find(final_non_terminal_sets.begin(), final_non_terminal_sets.end(), terminalSymbol) != final_non_terminal_sets.end())
		{
			//does nothing
		}
		else 
        {
			final_non_terminal_sets.push_back(terminalSymbol);
		}

		//for rule of A → X1⋯Xk, add to FIRST(A)
		if(find(final_non_terminal_sets.begin(), final_non_terminal_sets.end(), non_terminals) != final_non_terminal_sets.end())
		{
			//does nothing
		}
		else 
        {
			final_non_terminal_sets.push_back(non_terminals);
		}

		//ϵ is in FIRST(X1⋯Xk), if ϵ is in FIRST(X1),…, FIRST(Xk)
		if(find(first_set.begin(), first_set.end(), terminalSymbol) != first_set.end()) 
        {
            //does nothing
		}
		else 
        {
			first_set.push_back(terminalSymbol);
		}
	}
	return 0;
}

int Follow(string line) 
{
	//for start symbol S, add the end-of-input marker $ into FOLLOW(S)
	if(line.substr(1,2) == arrow) 
    {
		string terminalSymbol = line.substr(3,10);
		string non_terminals = line.substr(0,1);

		if(follow_set.size() == 0) 
        {
			follow_set.push_back("$");
			followCounter = 1;
		}

		if(!isTerminal(non_terminals)) 
        {
			if(find(follow_non_terminal_sets.begin(), follow_non_terminal_sets.end(), non_terminals) != follow_non_terminal_sets.end()) 
            {
				//does nothing
			}
			else 
            {
				follow_non_terminal_sets.push_back(non_terminals);
			}
		}
	}
	return 0;
}

void printSets() 
{	
	cout << "FIRST: " << endl;
	for(int i = 0; i < first_set.size(); i++) 
    {
		cout << first_set[i] << endl;
	}

	cout  << endl;

	cout << "FOLLOW (nonterm): " << endl;
	for(int i = 0; i < follow_non_terminal_sets.size(); i++) 
    {
		cout << follow_non_terminal_sets[i] << endl;
	} 

	cout  << endl;

	cout << "FOLLOW: " << endl;
	for(int i = 0; i < follow_set.size(); i++) 
    {
		cout << follow_set[i] << endl;
	}
	cout  << endl;

	cout << "prod. sets: " << endl;
	for(int i = 0; i < productions.size(); i++) 
    {
		cout << productions[i] << endl;
	}
	cout  << endl;

	cout << "term symbols: " << endl;
	for(int i = 0; i < terminal_sets.size(); i++) 
    {
		cout << terminal_sets[i] << endl;
	}
	cout  << endl;

	cout << "nonterm symbols on right side: " << endl;
	for(int i = 0; i < right_side_non_terminal_sets.size(); i++) 
    {
		cout << right_side_non_terminal_sets[i] << endl;
	}

	cout  << endl;

	cout << "nonterm symbols: " << endl;
	for(int i = 0; i < final_non_terminal_sets.size(); i++) 
    {
		cout << final_non_terminal_sets[i] << endl;
	}

	cout << endl;

	cout << "FIRST size: " << final_non_terminal_sets.size() << endl;
}

bool isTerminal(string val)
{
	char c = val[0];
	if (!isupper(c) && val.length() == 1) 
    {
		return true;
	}
	else 
    {
		return false;
	}
}

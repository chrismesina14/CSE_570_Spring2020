lab3: lab3.o Parser.o Utility.o LRSet.o Item.o
		g++ -std=c++1y lab3.o Parser.o Utility.o LRSet.o Item.o -o lab3

lab3.o: lab3.cpp
		g++ -c lab3.cpp

Parser.o: Parser.cpp Parser.h
		g++ -Wno-everything -c Parser.cpp

Utility.o: Utility.cpp Utility.h
		g++ -c Utility.cpp

LRSet.o: LRSet.cpp LRSet.h
		g++ -c LRSet.cpp

Item.o: Item.cpp Item.h
		g++ -c Item.cpp

clean:
		rm *.o lab3

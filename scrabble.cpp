// g++ -Wall -Werror -std=c++14 -g -O -o main Tile.cpp Node.cpp LinkedList.cpp scrabble.cpp
#include "LinkedList.h"

#include <iostream>

#define EXIT_SUCCESS    0

void printMenu();

int main(void) {
   LinkedList* list = new LinkedList();
   delete list;

   int menuOption;

   std::cout << "Welcome to Scrabble!" << std::endl;
   std::cout << "--------------------" << std::endl;
   printMenu();
   std::cin >> menuOption;

   return EXIT_SUCCESS;
}

void printMenu() {
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Quit" << std::endl;

}
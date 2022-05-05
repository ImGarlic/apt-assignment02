// g++ -Wall -Werror -std=c++14 -g -O -o main Tile.cpp Node.cpp LinkedList.cpp scrabble.cpp
// ./main
#include "LinkedList.h"

#include <iostream>

#define EXIT_SUCCESS    0

void printMenu();
void newGame();
void loadGame();
void credit();


int main(void) {
   LinkedList* list = new LinkedList();
   delete list;

   int option;

   std::cout << "Welcome to Scrabble!" << std::endl;
   std::cout << "--------------------" << std::endl;
   std::cout << std::endl;
   printMenu();

   std::cin >> option;
   std::cout << std::endl;

   if (option == 1) {
      newGame();
   }
   else if (option == 2) {
      loadGame();
   }
   else if (option == 3) {
      credit();
   }
   else {
      std::cout << "Goodbye" << std::endl;
      return EXIT_SUCCESS;
   }


   return EXIT_SUCCESS;
}

void printMenu() {
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Quit" << std::endl;
   std::cout << std::endl;

}

void newGame() {

}
void loadGame() {

}
void credit() {

}

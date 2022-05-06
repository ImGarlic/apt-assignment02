// g++ -Wall -Werror -std=c++14 -g -O -o main Tile.cpp Node.cpp LinkedList.cpp scrabble.cpp
// ./main
#include "LinkedList.h"
#include <iostream>
#include <memory>
#include <vector>

#define EXIT_SUCCESS    0

void printMenu();
void newGame(std::string name1, std::string name2);
void loadGame();
void credit();
void playGame(std::string name1, std::string name2, std::vector<std::vector<std::string>> board);
void printBoard(std::vector<std::vector<std::string>> board);

int main(void) {
   LinkedList* list = new LinkedList();
   delete list;

   int option = 0;
   // Name for player 1
   std::unique_ptr<std::string> name1 = std::make_unique<std::string>("name1");
   // Name for player 2
   std::unique_ptr<std::string> name2 = std::make_unique<std::string>("name2");
   // A 2d Vector that will serve as the board
   std::vector<std::vector<std::string>> board(15,std::vector<std::string>(15, " "));

   do {
      std::cout << "Welcome to Scrabble!" << std::endl;
      std::cout << "--------------------" << std::endl;
      std::cout << std::endl;
      printMenu();
      std::cout << "> ";
      std::cin >> option;
      std::cout << std::endl;

      if (option == 1) {
         newGame(*name1, *name2);
      }
      else if (option == 2) {
         loadGame();
      }
      else if (option == 3) {
         credit();
      }
      else if (option == 4){
         std::cout << "Goodbye" << std::endl;
         return EXIT_SUCCESS;
      }
   } while ( option != 1 && option != 2);

   playGame(*name1, *name2, board);

   return EXIT_SUCCESS;
}

void playGame(std::string name1, std::string name2, std::vector<std::vector<std::string>> board) {
   
   printBoard(board);

   }

void printBoard(std::vector<std::vector<std::string>> board) {
   std::cout << "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  " << std::endl;
   std::cout << "  -------------------------------------------------------------" << std::endl;
   char a = 'A';
   for (int i = 0; i < 15; i++){
      std::cout << a << " |";
      for (int j = 0; j < 15; j++) {
         std::cout << " " << board[i][j] << " |";
      }

      std::cout << std::endl;
      a++;
   }
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

void newGame(std::string name1, std::string name2) {
   std::cout << "Starting a New Game" << std::endl;
   std::cout << std::endl;
   std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
   std::cout << "> ";
   std::cin >> name1;
   std::cout << std::endl;
   std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
   std::cout << "> ";
   std::cin >> name2;
   std::cout << std::endl;
   std::cout << "Let's Play!" << std::endl;
   std::cout << std::endl;


}
void loadGame() {

}
void credit() {
   std::cout << "----------------------------------" << std::endl;
   std::cout << "Name: Alex Ly" << std::endl;
   std::cout << "Student ID: S3660743" << std::endl;
   std::cout << "Email: S3660743@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;
   std::cout << "Name: Radiyah Islam" << std::endl;
   std::cout << "Student ID: S3866803" << std::endl;
   std::cout << "Email: S3866803@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;
   std::cout << "Name: Sayed Walif Ali" << std::endl;
   std::cout << "Student ID: S3866910" << std::endl;
   std::cout << "Email: S3866910@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;
   std::cout << "Name: Dylan Marsh" << std::endl;
   std::cout << "Student ID: S3784998" << std::endl;
   std::cout << "Email: S3784998@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;
   std::cout << "----------------------------------" << std::endl;
}

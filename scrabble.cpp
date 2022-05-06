// g++ -Wall -Werror -std=c++14 -g -O -o main Tile.cpp Node.cpp LinkedList.cpp scrabble.cpp
// ./main
#include "LinkedList.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>

#define EXIT_SUCCESS    0

void printMenu();
void newGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2);
void loadGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2);
void credit();
void playGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr);
void printBoard(std::vector<std::vector<std::string>>* board);
void printNameAndScore(std::string player1, std::string player2, std::string curPlayer, int scorePlayer1, int scorePlayer2);
void printHand(LinkedList* hand);
void placeTile();
void replaceTile(LinkedList* tileBag, LinkedList* hand, Letter letter);
LinkedList* createTileBag();


int main(void) {
   LinkedList* list = new LinkedList();
   delete list;

   int option = 0;
   // Name for player 1
   std::shared_ptr<std::string> player1 = std::make_shared<std::string>("player1");
   // Name for player 2
   std::shared_ptr<std::string> player2 = std::make_shared<std::string>("player2");
   // Current player
   std::shared_ptr<std::string> curPlayer = std::make_shared<std::string>("");
   // Score for player 1
   std::shared_ptr<int> scorePlayer1 = std::make_shared<int>(0);
   // Score for player 2
   std::shared_ptr<int> scorePlayer2 = std::make_shared<int>(0);
   // A 2D Vector that will serve as the board
   std::vector<std::vector<std::string>> board(15,std::vector<std::string>(15, " "));
   // std::shared_ptr<std::vector<std::vector<std::string>>> board = std::make_shared<std::vector<std::vector<std::string>>>(15,std::vector<std::string>(15, " "));
   std::vector<std::vector<std::string>> *boardPtr = &board;

   do {
      std::cout << "Welcome to Scrabble!" << std::endl;
      std::cout << "--------------------" << std::endl;
      std::cout << std::endl;
      printMenu();
      std::cout << "> ";
      std::cin >> option;
      std::cout << std::endl;

      if (option == 1) {
         newGame(player1, player2);
      }
      else if (option == 2) {
         loadGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2);
      }
      else if (option == 3) {
         credit();
      }
      else if (option == 4){
         std::cout << "Goodbye" << std::endl;
         return EXIT_SUCCESS;
      }
   } while ( option != 1 && option != 2);

   playGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2, boardPtr);

   return EXIT_SUCCESS;
}

void playGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr) {
   std::string curOption = "";

   if (*curPlayer == "") {
      *curPlayer = *player1;
   }
   else if (*curPlayer == *player1) {
      *curPlayer = *player2;
   }
   else {
      *curPlayer = *player1;
   }
   printNameAndScore(*player1, *player2, *curPlayer, *scorePlayer1, *scorePlayer2);
   printBoard(boardPtr);
   // printHand();
   std::cout << std::endl;
   while (curOption != "Quit" && curOption != "place Done" && curOption != "pass") {
      std::cout << "> ";
      std::cin >> curOption;
      if (curOption.find("place") != std::string::npos) {
         placeTile();
      }
      else if (curOption.find("replace") != std::string::npos) {
         // replaceTile();
      }
   }
   // if (curOption == "Quit") {
   //    exit;
   // }

   }

void replaceTile(LinkedList* tileBag, LinkedList* hand, Letter letter) {
    Tile* tile = hand->remove(letter);
    if(tile == NULL) {
        return;
    }
    tileBag->append(tile);
    tile = tileBag->pop();
    hand->append(tile);
}

void placeTile() {

}

void printHand(LinkedList* hand) {
   hand->print();
}

void printNameAndScore(std::string player1, std::string player2, std::string curPlayer, int scorePlayer1, int scorePlayer2) {
   std::cout << curPlayer << ", it's your turn" << std::endl;
   std::cout << "Score for " << player1 << ": " << scorePlayer1 << std::endl;
   std::cout << "Score for " << player2 << ": " << scorePlayer2 << std::endl;
}

void printBoard(std::vector<std::vector<std::string>>* boardPtr) {
   std::cout << "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  " << std::endl;
   std::cout << "  -------------------------------------------------------------" << std::endl;
   char a = 'A';
   for (int i = 0; i < 15; i++){
      std::cout << a << " |";
      for (int j = 0; j < 15; j++) {
         std::cout << " " << boardPtr->at(i).at(j) << " |";
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

void newGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2) {
   std::cout << "Starting a New Game" << std::endl;
   std::cout << std::endl;
   std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
   std::cout << "> ";
   std::cin >> *player1;
   std::cout << std::endl;
   std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
   std::cout << "> ";
   std::cin >> *player2;
   std::cout << std::endl;
   std::cout << "Let's Play!" << std::endl;
   std::cout << std::endl;


}
void loadGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2) {

}

LinkedList* createTileBag() {
   LinkedList* tileBag = new LinkedList();
   std::vector<Tile*> tiles;
   Letter letter;
   Value value;

   // Read file into vector
   std::ifstream file("ScrabbleTiles.txt");
   while(file >> letter >> value) {
      tiles.push_back(new Tile(letter, value));
   }
   // Shuffle bag 
   std::random_device rd; 
   auto rng = std::default_random_engine { rd() };
   std::shuffle(std::begin(tiles), std::end(tiles), rng);

   // Turn vector into linked list
   for(Tile* i : tiles) {
      tileBag->append(i);
   }

   return tileBag;
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

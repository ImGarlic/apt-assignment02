
// g++ -Wall -Werror -std=c++14 -g -O -o main Tile.cpp Node.cpp LinkedList.cpp scrabble.cpp
// ./main


#include "LinkedList.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>

#define EXIT_SUCCESS    0

void printMenu();
void newGame(Player* _player1, Player* _player2, LinkedList* tileBag, std::shared_ptr<std::string> curOpt);
void loadGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt);
void saveGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::string fileName);
void credits();
void playGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt);
void printBoard(std::vector<std::vector<std::string>>* board);
void printNameAndScore(Player* _player1, Player* _player2);
void placeTile(char replaceLetter, char letter, char number1, char number2, std::vector<std::vector<std::string>>* boardPtr);
void replaceTile(LinkedList* tileBag, LinkedList* hand, char letter);
void endGame(Player* _player1, Player* _player2);
bool inputCheck(std::string curOption);
LinkedList* createTileBag();


int main(void) {
   int option = 0;

   // Allocate empty tile bag
   LinkedList* tileBag = new LinkedList;
   // Allocate memory for players
   Player* _player1 = new Player;
   Player* _player2 = new Player;
   // A 2D Vector that will serve as the board
   std::vector<std::vector<std::string>> board(15,std::vector<std::string>(15, " "));
   // std::shared_ptr<std::vector<std::vector<std::string>>> board = std::make_shared<std::vector<std::vector<std::string>>>(15,std::vector<std::string>(15, " "));
   std::vector<std::vector<std::string>> *boardPtr = &board;
   // String the record the users current input
   std::shared_ptr<std::string> curOpt = std::make_shared<std::string>("");

   do {
      std::cout << "Welcome to Scrabble!" << std::endl;
      std::cout << "--------------------\n" << std::endl;
      printMenu();
      std::cout << "> ";
      std::cin >> option;
      if (std::cin.eof()) {
         std::cout << "\nGoodbye" << std::endl;
         return EXIT_SUCCESS;         
      }
      std::cout << std::endl;
      // Make new game
      if (option == 1) {
         newGame(_player1, _player2, tileBag, curOpt);
      }
      // Load a game from a save file
      else if (option == 2) {
         loadGame(_player1, _player2, boardPtr, tileBag, curOpt);
      }
      // Show credits
      else if (option == 3) {
         credits();
      }
      // Quit
      else if (option == 4){
         std::cout << "Goodbye" << std::endl;
         return EXIT_SUCCESS;
      }
   } while ( option != 1 && option != 2);
   // Enter playGame to play scrabble
   playGame(_player1, _player2, boardPtr, tileBag, curOpt);
   std::cout << "Goodbye" << std::endl;
   return EXIT_SUCCESS;
}

void playGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt) {
   Player* curPlayer;
   std::string curOption = *curOpt;
   bool bingo = false;
   int bingoCounter = 0;
   int length = 0;

   // Ignore first input before cin
   std::cin.ignore();
   // Keep changing player turns until quit
   while (curOption != "Quit") {
      // Boolean array to track if a player passes twice
      bool passTracker[4] = {false};
      // For loop running 4 times to alternate between players turns and track if a player passes twice
      for (int i = 0; i < 4 && curOption != "Quit"; i++) { 
         // If statements to set the current players information
         if ((_player1)->turn) {
            curPlayer = _player1;
         }
         else {
            curPlayer = _player2;
         }
         // Print the name, score, board and hand
         printNameAndScore(_player1, _player2);
         printBoard(boardPtr);
         std::cout << std::endl;
         curPlayer->hand->print();
         // Get user input
         std::cout << "> ";
         std::cin.clear();
         std::cin.sync();
         std::getline(std::cin, curOption);
         if (std::cin.eof()) {
            curOption = "Quit";
         }
         else {
            while (!inputCheck(curOption)) {
               std::cout << "Invalid Input" << std::endl;
               std::cout << "> ";
               std::getline(std::cin, curOption);
            }
         }
         // If a player passes, record it in the boolean array
         if (curOption.find("pass") != std::string::npos) {
            passTracker[i] = true;
         }
         // Enter a do while loop after getting user input and running the command according to user input
         do {
            // If user input is a replace command
            if (curOption.find("replace") != std::string::npos) {
               replaceTile(tileBag, curPlayer->hand, curOption[8]);
            }
            else if (curOption.find("place Done") != std::string::npos) {
               curOption = "pass";
            }            
            // If user input is a place command
            else if (curOption.find("place") != std::string::npos) {
               length = curOption.length();
               if (length == 13) {
                  placeTile(curOption[6], curOption[11],curOption[12], ' ', boardPtr);
               }
               else if (length == 14){
                  placeTile(curOption[6], curOption[11],curOption[12], curOption[13], boardPtr);
               }
               Tile* tile = curPlayer->hand->remove(curOption[6]);
               std::cout << tile->letter << std::endl;
               // Add tile to hand from bag
               curPlayer->hand->append(tileBag->pop());
               // Update player score
               curPlayer->score += tile->value;
               bingoCounter += 1;
               // If player enters all 7 tiles from their hand, print bingo and go to next player
               if (bingoCounter == 7) {
                  bingo = true;
                  std::cout << std::endl;
                  std::cout << "BINGO!!!" << std::endl;
               }
               else {
                  std::cout << "> ";
                  std::getline(std::cin, curOption);
                  if (std::cin.eof()) {
                     curOption = "Quit";
                  }
                  else {
                     while (!inputCheck(curOption)) {
                        std::cout << "Invalid Input" << std::endl;
                        std::cout << "> ";
                        std::getline(std::cin, curOption);
                     }                     
                  }
               }
            }
            // If user input is a save command
            else if (curOption.find("save") != std::string::npos) {
               std::string fileName = "saveFiles/" + curOption.substr(5) + ".txt";
               saveGame(_player1, _player2, boardPtr, tileBag, fileName);
               std::cout << "> ";
               std::getline(std::cin, curOption);
               if (std::cin.eof()) {
                  curOption = "Quit";
               }
               else {
                  while (!inputCheck(curOption)) {
                     std::cout << "Invalid Input" << std::endl;
                     std::cout << "> ";
                     std::getline(std::cin, curOption);
                  }                     
               }
            }
           // Conditional checks   
         } while (curOption != "place Done" && curOption != "pass" && curOption != "Quit" && curOption.find("replace") == std::string::npos && bingo == false);
         // End the game if tile bag is empty, a player has no more tiles in their hand or a player passes twice
         if ((tileBag->peek() == NULL) || (_player1->hand->peek() == NULL) || (_player2->hand->peek() == NULL) ||
             (passTracker[0] == true && passTracker[2] == true) || (passTracker[1] == true and passTracker[3] == true)) {
            endGame(_player1, _player2);
            curOption = "Quit";
         }
         std::cout << std::endl;
         bingo = false;
         bingoCounter = 0;
      }
   }
   }

bool inputCheck(std::string curOption) {
   int length = curOption.length();

   if (length == 9) {
      if (curOption.find("replace") != std::string::npos) {
         if (curOption[7] == ' ') {
            if (curOption[8] >= 'A' && curOption[8] <= 'Z') {
               return true;
            }
         }
      }
   }
   else if (length == 13 || length == 14){
      if (curOption.find("place") != std::string::npos) {
         if (curOption[5] == ' ' && curOption[8] == 'a' && curOption[9] == 't' && curOption[5] == ' ') {
               if (curOption[11] >= 'A' && curOption[11] <= 'O') {
                  if (length == 13) {
                     if (curOption[12] >= '0' && curOption[12] <= '9') {
                           return true;
                     }
                  }
                  else if (length == 14) {
                     if (curOption[13] >= '0' && curOption[13] <= '9') {
                           return true;
                     }                        
                  }
               }
         }
      }
   }
   else if (length == 10) {
      if (curOption.find("place Done") != std::string::npos) {
         return true;
      }
   }
   else if (length == 4) {
      if (curOption.find("pass") != std::string::npos) {
         return true;
      }
   }
   else if (curOption.find("save") != std::string::npos) {
      if (curOption[4] == ' ') {  
         return true;
      }
   }
   return false;
}

void endGame(Player* _player1, Player* _player2) {
   std::cout << std::endl;
   std::cout << "Game over\n" <<
                "Score for " << _player1->name << ": " << _player1->score <<
                "\nScore for " << _player2->name << ": " << _player2->score << 
   std::endl;
   if (_player1->score >= _player2->score) {
      std::cout << "Player " << _player1->name << " won!" << std::endl;
   }
   else {
      std::cout << "Player " << _player2->name << " won!" << std::endl; 
   }
}

void replaceTile(LinkedList* tileBag, LinkedList* hand, char letter) {
   // Remove specified tile
   Tile* tile = hand->remove(letter);
   if(tile != NULL) {
      std::cout << "Replaced " << tile->letter << std::endl;
      tileBag->append(tile);
      tile = tileBag->pop();
      hand->append(tile);
   }
}

void placeTile(char replaceLetter, char letter, char number1, char number2, std::vector<std::vector<std::string>>* boardPtr) {
   std::unordered_map<char, int> alphabet = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}};
   if (number2 == ' ') {
      const char* num1 = &number1;
      int row = alphabet[letter];
      int col = std::atoi(num1);
      boardPtr->at(row).at(col) = replaceLetter;
   }
   else {
      std::string num;
      num += std::string(1, number1);
      num += std::string(1, number2);
      int row = alphabet[letter];
      int col = std::stoi(num);
      boardPtr->at(row).at(col) = replaceLetter;
   }
}

void printNameAndScore(Player* _player1, Player* _player2) {
   if(_player1->turn) {
      std::cout << _player1->name << ", it's your turn" << std::endl;
   }
   else {
      std::cout << _player2->name << ", it's your turn" << std::endl;
   }
   std::cout << "Score for " << _player1->name << ": " << _player1->score << std::endl;
   std::cout << "Score for " << _player2->name << ": " << _player2->score << std::endl;
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
std::cout << "Menu\n"
         << "----\n"
         << "1. New Game\n"
         << "2. Load Game\n"
         << "3. Credits (Show student information)\n"
         << "4. Quit\n" 
<< std::endl;
}

void newGame(Player* _player1, Player* _player2, LinkedList* tileBag, std::shared_ptr<std::string> curOpt) {
   std::string input;
   std::cout << "Starting a New Game\n" << std::endl;

   // Set player 1 name
   do {
      if (std::cin.eof()) {
         *curOpt = "Quit";
         return;
      }
      std::cout << "Enter a name for player 1\n"
               << "> ";
      std::cin >> input;
      std::cout << std::endl;
      if (!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); }))) {
         std::cout << "Name must be uppercase characters only\n" << std::endl;
      }
   } while(!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); })));
   _player1->setName(input);

   // Set player 2 name
   do {
      if (std::cin.eof()) {
         *curOpt = "Quit";
         return;
      }
      std::cout << "Enter a name for player 2\n"
               << "> ";
      std::cin >> input;
      std::cout << std::endl;
      if (!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); }))) {
         std::cout << "Name must be uppercase characters only\n" << std::endl;
      }
   } while(!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); })));
   _player2->setName(input);
   
   std::cout << "\n\nLet's Play!\n" << std::endl;

   // Generate new tile bag and player hands
   *tileBag = *createTileBag();
   _player1->createHand(tileBag);
   _player2->createHand(tileBag);
   // Set turn to player 1
   _player1->turn = true;
}

void loadGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt) {
   std::string fileName;
   std::ifstream saveFile;   
   std::string input;
   Tile* tile = NULL;

   do {
      if (std::cin.eof()) {
         *curOpt = "Quit";
         std::cout << std::endl;
         return;
      }
      std::cout << "Enter the fileaname from which to load a game" << std::endl;
         std::cout << "> ";
         std::cin >> fileName;
      saveFile.open("saveFiles/" + fileName + ".txt");
      if (!saveFile) {
         std::cout << "\nFile not found\n" << std::endl;
      }
   } while(!saveFile);
   std::cout << "\nOpened save file: " << fileName << "\n" << std::endl;

   // Player 1 name and score
   saveFile >> (_player1)->name;
   saveFile >> (_player1)->score;
   // Player 1 hand
   do {
      saveFile >> input;
      // Exception for literally just the letters Z and Q
      if(input.length() == 5) {
         tile = new Tile(input[0], std::stoi(input.substr(2,3)));
      }
      else {
         tile = new Tile(input[0], input[2] - '0');
      }
      _player1->hand->append(tile);
   } while(input.find(',') != std::string::npos);
   // Player 2 name and score
   saveFile >> (_player2)->name;
   saveFile >> (_player2)->score;
   // Player 2 hand
   input = "resetting";
   do {
      saveFile >> input;
      // Exception for literally just the letters Z and Q
      if(input.length() == 5) {
         tile = new Tile(input[0], std::stoi(input.substr(2,3)));
      }
      else {
         tile = new Tile(input[0], input[2] - '0');
      }
      _player2->hand->append(tile);
   } while(input.find(',') != std::string::npos);
   // Set getline to first row of board
   std::string line;
   for(int i = 0; i < 3; ++i) {
      std::getline(saveFile, line);
   }
   // Get board 
   for(int i = 0; i < 15; ++i) {
      std::getline(saveFile, line);
      for(int j = 1; j < 16; ++j) {
         input = line[(j * 4)];
         boardPtr->at(i).at(j - 1) = input;
      }
   }
   // Get tile bag
   input = "resetting";
   do {
      saveFile >> input;
      // Exception for literally just the letters Z and Q
      if(input.length() == 5) {
         tile = new Tile(input[0], std::stoi(input.substr(2,3)));
      }
      else {
         tile = new Tile(input[0], input[2] - '0');
      }
      tileBag->append(tile);
   } while(input.find(',') != std::string::npos);
   // Get player turn
   saveFile >> input;
   if(_player1->name == input) {
      _player1->turn = true;
   }
   else {
      _player2->turn = true;
   }
}

void saveGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::string fileName) {
   std::ofstream saveFile(fileName);
   Tile* tile = NULL;
   LinkedList* player1HandCopy = new LinkedList(*_player1->hand);
   LinkedList* player2HandCopy = new LinkedList(*_player2->hand);
   LinkedList* tileBagCopy = new LinkedList(*tileBag);

   // Write player 1 name and score
   saveFile << _player1->name << std::endl;
   saveFile << _player1->score << std::endl;
   // Write player1 hand
   do {
      tile = new Tile(*player1HandCopy->pop());
      // // Check for last tile
      if(player1HandCopy->peek() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
      }
      else {
         saveFile << tile->letter << "-" << tile->value << ", ";
      }
      delete tile;
   }
   while(player1HandCopy->peek() != NULL);
   // Write player2 name and score
   saveFile << _player2->name << std::endl;
   saveFile << _player2->score << std::endl;
   // Write player2 hand
   do {
      tile = new Tile(*player2HandCopy->pop());
      // Check for last tile
      if(player2HandCopy->peek() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
      }
      else {
         saveFile << tile->letter << "-" << tile->value << ", ";
      }
      delete tile;
   }
   while(player2HandCopy->peek() != NULL);
   // Write the board state
   saveFile << "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  " << std::endl;
   saveFile << "  -------------------------------------------------------------" << std::endl;
   char a = 'A';
   for (int i = 0; i < 15; i++){
      saveFile << a << " |";
      for (int j = 0; j < 15; j++) {
         saveFile << " " << boardPtr->at(i).at(j) << " |";
      }
      saveFile << std::endl;
      a++;
   }
   // Write tile bag
   do {
      tile = new Tile(*tileBagCopy->pop());
      // // Check for last tile
      if(tileBagCopy->peek() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
      }
      else {
         saveFile << tile->letter << "-" << tile->value << ", ";
      }
      delete tile;
   }
   while(tileBagCopy->peek() != NULL);
   // Write player turn
   if(_player1->turn) {
      saveFile << _player1->name;
   }
   else {
      saveFile << _player2->name;
   }
   std::cout << "Successfully saved game to " << fileName << "\n" << std::endl;

   saveFile.close();
   delete tileBagCopy;
   delete player1HandCopy;
   delete player2HandCopy;
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

void credits() {
   std::cout << "-------------------------------------\n"
            << "Name: Alex Ly\n" 
            << "Student ID: S3660743\n" 
            << "Email: S3660743@student.rmit.edu.au\n\n" 

            << "Name: Radiyah Islam\n" 
            << "Student ID: S3866803\n" 
            << "Email: S3866803@student.rmit.edu.au\n\n" 
   
            << "Name: Sayed Walif Ali\n" 
            << "Student ID: S3866910\n" 
            << "Email: S3866910@student.rmit.edu.au\n\n" 

            << "Name: Dylan Marsh\n" 
            << "Student ID: S3784998\n" 
            << "Email: S3784998@student.rmit.edu.au\n" 
            << "-------------------------------------\n" 
<< std::endl;
}
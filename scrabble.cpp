
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
void loadGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag);
void saveGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::string fileName);
void credits();
void playGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt);
void printBoard(std::vector<std::vector<std::string>>* board);
void printNameAndScore(Player* _player1, Player* _player2);
void printHand(LinkedList* hand);
void placeTile(char replaceLetter,char letter, char number, std::vector<std::vector<std::string>>* boardPtr);
void replaceTile(LinkedList* tileBag, LinkedList* hand, char letter);
void calculateScore(char letter, std::shared_ptr<int> curScore);
void endGame(Player* _player1, Player* _player2);
bool inputCheck(std::string curOption);
LinkedList* createTileBag();
LinkedList* createHand(LinkedList* tileBag);


int main(void) {
   LinkedList* tileBag = new LinkedList;
   Player* _player1 = new Player;
   Player* _player2 = new Player;

   int option = 0;
   // Name for player 1
   std::shared_ptr<std::string> player1 = std::make_shared<std::string>("player1");
   // Name for player 2
   std::shared_ptr<std::string> player2 = std::make_shared<std::string>("player2");
   // Current player
   // std::shared_ptr<std::string> curPlayer = std::make_shared<std::string>("");
   // Score for player 1
   std::shared_ptr<int> scorePlayer1 = std::make_shared<int>(0);
   // Score for player 2
   std::shared_ptr<int> scorePlayer2 = std::make_shared<int>(0);
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
         loadGame(_player1, _player2, boardPtr, tileBag);
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
   std::string player1PrevInput = "";
   std::string player1CurInput = "";
   std::shared_ptr<int> curScore;
   bool bingo = false;
   int bingoCounter = 0;

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
         printHand(curPlayer->hand);
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
               std::cout << "replaced test" << std::endl;
            }
            else if (curOption.find("place Done") != std::string::npos) {
               curOption = "pass";
            }            
            // If user input is a place command
            else if (curOption.find("place") != std::string::npos) {
               placeTile(curOption[6], curOption[11],curOption[12], boardPtr);
               Tile* tile = curPlayer->hand->remove(curOption[6]);
               std::cout << tile->letter << std::endl;
               curPlayer->hand->append(tileBag->pop());
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
            }

           // Conditional checks   
         } while (curOption != "place Done" && curOption != "pass" && curOption != "Quit" && curOption.find("replace") == std::string::npos && bingo == false);
         // End the game if tile bag is empty, a player has no more tiles in their hand or a player passes twice
         if ((tileBag->peek() == NULL) || (_player1->hand->peek() == NULL) || (_player2->hand->peek() == NULL) || (passTracker[0] == true && passTracker[2] == true) || (passTracker[1] == true and passTracker[3] == true)) {
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
   Tile* tile = hand->remove(letter);
   std::cout << tile->letter << std::endl;
   if(tile == NULL) {
      return;
   }
   tileBag->append(tile);
   tile = tileBag->pop();
   hand->append(tile);
}

void placeTile(char replaceLetter, char letter, char number, std::vector<std::vector<std::string>>* boardPtr) {
   std::unordered_map<char, int> alphabet = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}};
   const char* num = &number;
   int row = alphabet[letter];
   int col = std::atoi(num);
   boardPtr->at(row).at(col) = replaceLetter;
}

void calculateScore(char letter, std::shared_ptr<int> curScore) {
   std::unordered_map<char, int> alphabet = {{'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1}, {'F', 4}, {'G', 2}, {'H', 4}, {'I', 1}, {'J', 8}, {'K', 5}, {'L', 1}, {'M', 3}, {'N', 1}, {'O', 1},
                                             {'P', 3}, {'Q', 10}, {'R', 1}, {'S', 1}, {'T', 1}, {'U', 1}, {'V', 4}, {'W', 4}, {'X', 8}, {'Y', 4}, {'Z', 10}};
   *curScore = *curScore +  alphabet[letter];              
}

void printHand(LinkedList* hand) {
   hand->print();
   std::cout << std::endl;
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
   std::string input = "a";
   std::cout << "Starting a New Game\n\n"
            << "Enter a name for player 1 (uppercase characters only)\n"
            << "> ";
   std::cin >> input;
   if (std::cin.eof()) {
      *curOpt = "Quit";
   }
   else {
      while(!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); }))) {
         std::cout << "\nName must be uppercase characters only\n"
                  << "> ";
         std::cin >> input;
         if (std::cin.eof()) {
            input = "Q";
            *curOpt = "Quit";
         }
      }
      _player1->setName(input);
   }
   if (*curOpt != "Quit"){
      std::cout << "\nEnter a name for player 2 (uppercase characters only)\n"
               << "> ";
      std::cin >> input;
      if (std::cin.eof()) {
         input = "Q";
         *curOpt = "Quit";
      }
      else {
         while(!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); }))) {
            std::cout << "\nName must be uppercase characters only\n"
                     << "> ";
            std::cin >> input;
            if (std::cin.eof()) {
               *curOpt = "Quit";
            }
         }
         _player2->setName(input);
      }
   }
   if (*curOpt != "Quit") {
      std::cout << "\n\nLet's Play!\n" << std::endl;
   }
   else {
      std::cout << std::endl;
   }

   // Generate new tile bag and player hands
   *tileBag = *createTileBag();
   _player1->createHand(tileBag);
   _player2->createHand(tileBag);
   // Set turn to player 1
   _player1->turn = true;
}

void loadGame(Player* _player1, Player* _player2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* tileBag) {
   std::string fileName;
   std::ifstream saveFile;   
   std::string input;
   Tile* tile = NULL;

   std::cout << "Enter the fileaname from which to load a game" << std::endl;
      std::cout << "> ";
      std::cin >> fileName;
   saveFile.open("saveFiles/" + fileName + ".txt");

   // Check file exists
   while(!saveFile) {
      std::cout << "\nFile not found" << std::endl;
      std::cout << "> ";
      std::cin >> fileName;
      saveFile.open("saveFiles/" + fileName + ".txt");
   }
   std::cout << "\nOpened save file: " << fileName << "\n" << std::endl;

   // Player 1 name and score
   saveFile >> (_player1)->name;
   saveFile >> (_player1)->score;
   // Player 1 hand
   while(input.length() != 3) {
      saveFile >> input;
      // Exception for literally just the letters Z and Q
      if(isalpha(input[3])) {
         tile = new Tile(input[0], std::stoi(input.substr(2,3)));
      }
      else {
         tile = new Tile(input[0], input[2] - '0');
      }
      _player1->hand->append(tile);
   }
   // Player 2 name and score
   saveFile >> (_player2)->name;
   saveFile >> (_player2)->score;
   // Player 2 hand
   input = "resetting";
   while(input.length() != 3) {
      saveFile >> input;
      // Exception for literally just the letters Z and Q
      if(isalpha(input[3])) {
         tile = new Tile(input[0], std::stoi(input.substr(2,3)));
      }
      else {
         tile = new Tile(input[0], input[2] - '0');
      }
      _player2->hand->append(tile);
   }
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
   while(input.length() != 3) {
      saveFile >> input;
      // Exception for literally just the letters Z and Q
      if(isalpha(input[3])) {
         tile = new Tile(input[0], std::stoi(input.substr(2,3)));
      }
      else {
         tile = new Tile(input[0], input[2] - '0');
      }
      tileBag->append(tile);
   }
   // Get player turn
   saveFile >> input;
   if(_player1->name == input) {
      _player1->turn = true;
   }
   else {
      _player2->turn = true;
   }
   delete tile;
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
      // // Check for last tile
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

LinkedList* createHand(LinkedList* tileBag) {
    LinkedList* hand = new LinkedList();
    Tile* tile;

    for(int i = 0; i < 7; ++i) {
      tile = tileBag->pop();
      hand->append(tile);
    }
    return hand;
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
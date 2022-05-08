
// g++ -Wall -Werror -std=c++14 -g -O -o main Tile.cpp Node.cpp LinkedList.cpp scrabble.cpp
// ./main


#include "LinkedList.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>

#define EXIT_SUCCESS    0

void printMenu();
void newGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, LinkedList* tileBag, LinkedList* player1Hand, LinkedList* player2Hand);
void loadGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag);
void saveGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag, std::string fileName);
void credits();
void playGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag);
void printBoard(std::vector<std::vector<std::string>>* board);
void printNameAndScore(std::string player1, std::string player2, std::string curPlayer, int scorePlayer1, int scorePlayer2);
void printHand(LinkedList* hand);
void placeTile(char replaceLetter,char letter, char number, std::vector<std::vector<std::string>>* boardPtr);
void replaceTile(LinkedList* tileBag, LinkedList* hand, char letter);
void calculateScore(char letter, std::shared_ptr<int> curScore);
void endGame(std::string player1, std::string player2, int scorePlayer1, int scorePlayer2);
LinkedList* createTileBag();
LinkedList* createHand(LinkedList* tileBag);


int main(void) {
   LinkedList* tileBag = new LinkedList;
   LinkedList* player1Hand = new LinkedList;
   LinkedList* player2Hand = new LinkedList;

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
      // Make new game
      if (option == 1) {
         newGame(player1, player2, tileBag, player1Hand, player2Hand);
      }
      // Load a game from a save file
      else if (option == 2) {
         loadGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2, boardPtr,player1Hand, player2Hand, tileBag);
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
   playGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2, boardPtr, player1Hand, player2Hand, tileBag);
   std::cout << "Goodbye" << std::endl;
   return EXIT_SUCCESS;
}

void playGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag) {
   std::string curOption = "";
   std::string player1PrevInput = "";
   std::string player1CurInput = "";
   LinkedList *curHand;
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
      for (int i = 0; i < 4 || curOption != "Quit"; i++) { 
         // If statements to set the current players information
         if (*curPlayer == "") {
            *curPlayer = *player1;
            curHand = player1Hand;
            curScore = scorePlayer1;
         }
         else if (*curPlayer == *player1) {
            *curPlayer = *player2;
            curHand = player2Hand;
            curScore = scorePlayer2;
         }
         else {
            *curPlayer = *player1;
            curHand = player1Hand;
            curScore = scorePlayer1;
         }
         // Print the name, score, board and hand
         printNameAndScore(*player1, *player2, *curPlayer, *scorePlayer1, *scorePlayer2);
         printBoard(boardPtr);
         std::cout << std::endl;
         if (*curPlayer == *player1) {
            printHand(player1Hand);
         }
         else {
            printHand(player2Hand);
         }
         // Get user input
         std::cout << "> ";
         std::cin.clear();
         std::cin.sync();
         std::getline(std::cin, curOption);
         // If a player passes, record it in the boolean array
         if (curOption.find("pass") != std::string::npos) {
            passTracker[i] = true;
         }
         // Enter a do while loop after getting user input and running the command according to user input
         do {
            // If user input is a replace command
            if (curOption.find("replace") != std::string::npos) {
               replaceTile(tileBag, curHand, curOption[8]);
               std::cout << "replaced test" << std::endl;
            }
            // If user input is a place command
            else if (curOption.find("place") != std::string::npos) {
               placeTile(curOption[6], curOption[11],curOption[12], boardPtr);
               Tile* tile = curHand->remove(curOption[6]);
               std::cout << tile->letter << std::endl;
               curHand->append(tileBag->pop());
               calculateScore(curOption[6], curScore);
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
               }
            }
            // If user input is a save command
            else if (curOption.find("save") != std::string::npos) {
               std::string fileName = "saveFiles/" + curOption.substr(5) + ".txt";
               saveGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2, boardPtr, player1Hand, player2Hand, tileBag, fileName);
            }
           // Conditional checks   
         } while (curOption != "place Done" && curOption != "pass" && curOption != "Quit" && curOption.find("replace") && curOption.find("save") == std::string::npos && bingo == false);
         // End the game if tile bag is empty, a player has no more tiles in their hand or a player passes twice
         if ((tileBag->peek() == NULL) || (player1Hand->peek() == NULL) || (player2Hand->peek() == NULL) || (passTracker[0] == true && passTracker[2] == true) || (passTracker[1] == true and passTracker[3] == true)) {
            endGame(*player1, *player2, *scorePlayer1, *scorePlayer2);
            curOption = "Quit";
         }
         
         std::cout << std::endl;
         bingo = false;
         bingoCounter = 0;
      }
   }
   }

void endGame(std::string player1, std::string player2, int scorePlayer1, int scorePlayer2) {
   std::cout << std::endl;
   std::cout << "Game over" << std::endl;
   std::cout << "Score for " << player1 << ": " << scorePlayer1 << std::endl;
   std::cout << "Score for " << player2 << ": " << scorePlayer2 << std::endl;
   if (scorePlayer1 >= scorePlayer2) {
      std::cout << "Player " << player1 << " won!" << std::endl;
   }
   else {
      std::cout << "Player " << player2 << " won!" << std::endl; 
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
std::cout << "Menu\n"
         << "----\n"
         << "1. New Game\n"
         << "2. Load Game\n"
         << "3. Credits (Show student information)\n"
         << "4. Quit\n" 
<< std::endl;
}

void newGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, LinkedList* tileBag, LinkedList* player1Hand, LinkedList* player2Hand) {
   std::cout << "Starting a New Game\n\n"
            << "Enter a name for player 1 (uppercase characters only)\n"
            << "> ";
   std::cin >> *player1;
   while(!(std::all_of((*player1).begin(), (*player1).end(), [](unsigned char c){ return std::isupper(c); }))) {
      std::cout << "\nName must be uppercase characters only\n"
               << "> ";
      std::cin >> *player1;
   }
   std::cout << "\nEnter a name for player 2 (uppercase characters only)\n"
            << "> ";
   std::cin >> *player2;
   while(!(std::all_of((*player2).begin(), (*player2).end(), [](unsigned char c){ return std::isupper(c); }))) {
      std::cout << "\nName must be uppercase characters only\n"
               << "> ";
      std::cin >> *player2;
   }
   std::cout << "\n\nLet's Play!\n" << std::endl;

   // Generate new tile bag and player hands
   *tileBag = *createTileBag();
   *player1Hand = *createHand(tileBag);
   *player2Hand = *createHand(tileBag);
}

void loadGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag) {
   std::string fileName;
   std::ifstream saveFile;
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
   
   std::string tileString;
   Tile* tile = NULL;

   // Player 1 name and score
   saveFile >> *player1;
   saveFile >> *scorePlayer1;
   // Player 1 hand
   while(tileString.length() != 3) {
      saveFile >> tileString;
      // Exception for literally just the letters Z and Q
      if(isalpha(tileString[3])) {
         tile = new Tile(tileString[0], std::stoi(tileString.substr(2,3)));
      }
      else {
         tile = new Tile(tileString[0], tileString[2] - '0');
      }
      player1Hand->append(tile);
   }
   // Player 2 name and score
   saveFile >> *player2;
   saveFile >> *scorePlayer2;
   // Player 2 hand
   tileString = "resetting";
   while(tileString.length() != 3) {
      saveFile >> tileString;
      // Exception for literally just the letters Z and Q
      if(isalpha(tileString[3])) {
         tile = new Tile(tileString[0], std::stoi(tileString.substr(2,3)));
      }
      else {
         tile = new Tile(tileString[0], tileString[2] - '0');
      }
      player2Hand->append(tile);
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
         tileString = line[(j * 4)];
         boardPtr->at(i).at(j - 1) = tileString;
      }
   }
   // Get tile bag
   tileString = "resetting";
   while(tileString.length() != 3) {
      saveFile >> tileString;
      // Exception for literally just the letters Z and Q
      if(isalpha(tileString[3])) {
         tile = new Tile(tileString[0], std::stoi(tileString.substr(2,3)));
      }
      else {
         tile = new Tile(tileString[0], tileString[2] - '0');
      }
      tileBag->append(tile);
   }
   delete tile;
}

void saveGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag, std::string fileName) {
   std::ofstream saveFile(fileName);
   Tile* tile = NULL;
   LinkedList* player1HandCopy = new LinkedList(*player1Hand);
   LinkedList* player2HandCopy = new LinkedList(*player2Hand);
   LinkedList* tileBagCopy = new LinkedList(*tileBag);

   // Write player 1 name and score
   saveFile << *player1 << std::endl;
   saveFile << *scorePlayer1 << std::endl;
   // Write player1 hand
   do {
      tile = player1HandCopy->pop();
      // // Check for last tile
      if(player1HandCopy->peek() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
      }
      else {
         saveFile << tile->letter << "-" << tile->value << ", ";
      }
   }
   while(player1HandCopy->peek() != NULL);
   // Write player2 name and score
   saveFile << *player2 << std::endl;
   saveFile << *scorePlayer2 << std::endl;
   // Write player2 hand
   do {
      tile = player2HandCopy->pop();
      // // Check for last tile
      if(player2HandCopy->peek() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
      }
      else {
         saveFile << tile->letter << "-" << tile->value << ", ";
      }
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
      tile = tileBagCopy->pop();
      // // Check for last tile
      if(tileBagCopy->peek() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
      }
      else {
         saveFile << tile->letter << "-" << tile->value << ", ";
      }
   }
   while(tileBagCopy->peek() != NULL);
   // Write player turn
   saveFile << *curPlayer;
   std::cout << "Successfully saved game to " << fileName << "\n" << std::endl;
   saveFile.close();
   delete tile;
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
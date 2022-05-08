
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
void newGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2);
void loadGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag);
void saveGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag, std::string fileName);
void credit();
void playGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag);
void printBoard(std::vector<std::vector<std::string>>* board);
void printNameAndScore(std::string player1, std::string player2, std::string curPlayer, int scorePlayer1, int scorePlayer2);
void printHand(LinkedList* hand);
void placeTile(char replaceLetter,char letter, char number, std::vector<std::vector<std::string>>* boardPtr);
void replaceTile(LinkedList* tileBag, LinkedList* hand, Letter letter);
void calculateScore(char letter, std::shared_ptr<int> curScore);
LinkedList* createTileBag();
LinkedList* createHand(LinkedList* tileBag);


int main(void) {
   LinkedList* tileBag = createTileBag();
   LinkedList* player1Hand = createHand(tileBag);
   LinkedList* player2Hand = createHand(tileBag);

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
         loadGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2, boardPtr,player1Hand, player2Hand, tileBag);
      }
      else if (option == 3) {
         credit();
      }
      else if (option == 4){
         std::cout << "Goodbye" << std::endl;
         return EXIT_SUCCESS;
      }
   } while ( option != 1 && option != 2);

   playGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2, boardPtr, player1Hand, player2Hand, tileBag);
   std::cout << "Goodbye" << std::endl;
   return EXIT_SUCCESS;
}

void playGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag) {
   std::string curOption = "";
   LinkedList *curHand;
   std::shared_ptr<int> curScore;

   std::cin.ignore();

   while (curOption != "Quit") {

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

      printNameAndScore(*player1, *player2, *curPlayer, *scorePlayer1, *scorePlayer2);
      printBoard(boardPtr);
      std::cout << std::endl;
      if (*curPlayer == *player1) {
         printHand(player1Hand);
      }
      else {
         printHand(player2Hand);
      }

      std::cout << "> ";
      std::cin.clear();
      std::cin.sync();
      std::getline(std::cin, curOption);

      while (curOption != "place Done" && curOption != "pass" && curOption != "replace" && curOption != "Quit") {
         if (curOption.find("place") != std::string::npos) {
            placeTile(curOption[6], curOption[11],curOption[12], boardPtr);
            curHand->remove(curOption[6]);
            curHand->append(tileBag->pop());
            calculateScore(curOption[6], curScore);
            std::cout << "> ";
            std::getline(std::cin, curOption);
         }

         else if (curOption.find("replace") != std::string::npos) {
            replaceTile(tileBag, curHand, curOption[8]);
            break;
         }

         else if (curOption.find("save") != std::string::npos) {
            std::string fileName = curOption.substr(5) + ".txt";
            saveGame(player1, player2, curPlayer, scorePlayer1, scorePlayer2, boardPtr, player1Hand, player2Hand, tileBag, fileName);
            break;
         }
      }
   }

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
void loadGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag) {
   std::string fileName;
   std::ifstream saveFile;
   saveFile.open(fileName);

   // Check file exists
   while(!saveFile) {
      std::cout << "Enter the fileaname from which to load a game" << std::endl;
      std::cout << "> ";
      std::cin >> fileName;
      saveFile.open(fileName);
   }
   std::cout << "opened " << fileName << std::endl;
   // Player 1 name and score
   std::string line;
   std::getline(saveFile, line);
   *player1 = line;
   std::getline(saveFile, line);
   if(std::stoi(line)) {
      *scorePlayer1 = std::stoi(line);
   }
}
   

void saveGame(std::shared_ptr<std::string> player1, std::shared_ptr<std::string> player2, std::shared_ptr<std::string> curPlayer, std::shared_ptr<int> scorePlayer1, std::shared_ptr<int> scorePlayer2, std::vector<std::vector<std::string>>* boardPtr, LinkedList* player1Hand, LinkedList* player2Hand, LinkedList* tileBag, std::string fileName) {
   std::ofstream saveFile(fileName);
   Tile* tile;
   LinkedList* player1HandCopy = new LinkedList(*player1Hand);
   LinkedList* player2HandCopy = new LinkedList(*player2Hand);

   // Write player 1 name and score
   saveFile << *player1 << std::endl;
   saveFile << *scorePlayer1 << std::endl;
   // Write player1 hand
   while(true) {
      tile = player1HandCopy->pop();
      // Check for last tile
      if(player1HandCopy->peak() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
         break;;
      }
      saveFile << tile->letter << "-" << tile->value << ", ";
   }
   // Write player2 name and score
   saveFile << *player2 << std::endl;
   saveFile << *scorePlayer2 << std::endl;
   // Write player2 hand
   while(true) {
      tile = player2HandCopy->pop();
      // Check for last tile
      if(player2HandCopy->peak() == NULL) {
         saveFile << tile->letter << "-" << tile->value << std::endl;
         break;;
      }
      saveFile << tile->letter << "-" << tile->value << ", ";
   }
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
   std::cout << "Successfully saved game to " << fileName << "\n" << std::endl;
   saveFile.close();
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
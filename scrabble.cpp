
#include "LinkedList.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <unordered_set>

#define EXIT_SUCCESS    0

void printMenu();
void newGame(Player* player1, Player* player2, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, char** argv);
void loadGame(Player* player1, Player* player2, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt);
void saveGame(Player* player1, Player* player2, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::string fileName);
void playGame(Player* player1, Player* player2, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, char** argv);
void printBoard(std::vector<std::vector<Tile*>>* boardPtr);
void printNameAndScore(Player* player1, Player* player2);
void endGame(Player* player1, Player* player2);
bool inputCheck(std::string curOption);
bool betterInputCheck(std::string curOption);
LinkedList* createTileBag();
void help();
void credits();


int main(int argc, char** argv) {
   std::string option = "0";
   
   // Check for milestone 2 or 3
   if (argc < 5) {
      std::cout << "\nProgram terminated.\n"
               << "Please type y/n for each enhancement:\n"
               << "> ./scrabble <Help!> <Better Invalid Input> <Colour>\n"
               << "\nFor example:\n"
               << "> ./scrabble y y n y\n"
      << std::endl;
      return EXIT_SUCCESS;
   }
   // Allocate empty tile bag
   LinkedList* tileBag = new LinkedList;
   // Allocate memory for players
   Player* player1 = new Player;
   Player* player2 = new Player;
   // A 2D Vector that will serve as the board
   std::vector<std::vector<Tile*>> board(15,std::vector<Tile*>(15, NULL));
   std::vector<std::vector<Tile*>> *boardPtr = &board;
   // String the record the users current input
   std::shared_ptr<std::string> curOpt = std::make_shared<std::string>("");

   do {
      // Open main menu
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
      if (option == "1") {
         newGame(player1, player2, tileBag, curOpt, argv);
      }
      // Load a game from a save file
      else if (option == "2") {
         loadGame(player1, player2, boardPtr, tileBag, curOpt);
      }
      // Show credits
      else if (option == "3") {
         credits();
      }
      // Quit
      else if (option == "4"){
         std::cout << "Goodbye" << std::endl;
         return EXIT_SUCCESS;
      }
   } while ( option != "1" && option != "2");
   
   // Add colour to players, defaults to white
   if (*argv[3] == 'y') {
      player1->colour = "\u001b[31m";
      player2->colour = "\u001b[36m";
   }
   else {
      player1->colour = "\u001b[0m";
      player2->colour = "\u001b[0m";
   }
   // Enter playGame to play scrabble
   playGame(player1, player2, boardPtr, tileBag, curOpt, argv);
   std::cout << "Goodbye" << std::endl;
   delete tileBag;
   delete player1;
   delete player2;
   return EXIT_SUCCESS;
}

void playGame(Player* player1, Player* player2, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, char** argv) {
   Player* curPlayer;
   std::string letter;
   std::vector<Tile*> word;
   std::string curOption = *curOpt;
   int bingoCounter = 0;

   // Get list of words for correctness checking
   std::ifstream wordFile("web2");
   std::unordered_set<std::string> wordList;
   std::string line;
   while(std::getline(wordFile, line)) {
      wordList.insert(line);
   }

   // Keep changing player turns until quit
   while (curOption != "Quit") {
      // Boolean array to track if a player passes twice
      bool passTracker[4] = {false};
      // For loop running 4 times to alternate between players turns and track if a player passes twice
      for (int i = 0; i < 4 && curOption != "Quit"; i++) { 
         // If statements to set the current players information
         if ((player1)->turn) {
            curPlayer = player1;
         }
         else {
            curPlayer = player2;
         }
         // Print the name, score, board and hand
         printNameAndScore(player1, player2);
         printBoard(boardPtr);

         std::cout << "\nYour hand is" << std::endl;
         // Add some colour!
         std::cout << curPlayer->colour;
         curPlayer->hand->print();
         std::cout << "\u001b[0m";

         std::cin.clear();
         std::cin.sync();

         // If a player passes, record it in the boolean array
         if (curOption == "pass") {
            passTracker[i] = true;
            // If player passes halfway through placing a word
            if(bingoCounter != 0) {
               word.clear();
            }
         }
         // Set curOption to player input switch
         curOption = "next";
         // Enter a do while loop after getting user input and running the command according to user input
         do {
            // If user input is a replace command
            if (curOption.find("replace") != std::string::npos) {
               curPlayer->replaceTile(tileBag, curOption[8]);
               curOption = "done";
            }
            else if (curOption.find("place Done") != std::string::npos) {
               bool placed;
               if (*argv[4] == 'y') {
                  placed = curPlayer->placeWord(word, boardPtr, 'y', wordList);
               }
               else {
                  placed = curPlayer->placeWord(word, boardPtr, 'n', wordList);
               }
               // If placing word fails, clear board, reset tile pos and append back to player hand
               if (!placed) {
                  for (Tile* tile : word) {
                     boardPtr->at(tile->row).at(tile->col) = NULL;
                     tile->setPos("reset");
                     curPlayer->hand->append(tile);
                  }
                  curOption = "next";
               }
               // If placing succeeds, append correct number of tiles from bag
               else {
                  for (int i = 0; i < int(word.size()); ++i) {
                     curPlayer->hand->append(tileBag->pop());
                  }
                  // If player enters all 7 tiles from their hand, print bingo 
                  if (bingoCounter == 7) {
                     std::cout << std::endl;
                     std::cout << "BINGO!!!" << std::endl;
                     curPlayer->score += 50;
                  }
                  curOption = "done";
               }
               // Clear word
               word.clear();
            }            
            else if (curOption.find("help") != std::string::npos) {
               if (*argv[1] == 'y') {
                  help();
                  curOption = "next";
               }
               else {
                  std::cout << "Invalid input" << std::endl;
                  curOption = "next";
               }
            }
            // If user input is a place command
            else if (curOption.find("place") != std::string::npos) {
               Tile* tile;
               if (curPlayer->placeCheck(curOption[6], curOption.substr(11), boardPtr)) {
                  bingoCounter += 1;
                  // Set the position of tile after removing from player hand, add to "word" vector before placing
                  tile = curPlayer->hand->remove(curOption[6]);
                  tile->setPos(curOption.substr(11));
                  word.push_back(tile);
               }
               curOption = "next";
            }
            // If user input is a save command
            else if (curOption.find("save") != std::string::npos) {
               std::string fileName = "saveFiles/" + curOption.substr(5) + ".save";
               saveGame(player1, player2, boardPtr, tileBag, fileName);
               curOption = "next";
            }
            // If command does not signal end of turn
            else if (curOption == "next") {
               std::cout << "> ";
               std::getline(std::cin, curOption);
               if (std::cin.eof()) {
                  curOption = "Quit";
               }
               if (*argv[2] == 'y') {
                  while (!betterInputCheck(curOption) && !std::cin.eof()) {
                     std::cout << "> ";
                     std::getline(std::cin, curOption);
                     if (std::cin.eof()) {
                        curOption = "Quit";
                     }
                  }     
               }
               else {
                  while (!inputCheck(curOption) && !std::cin.eof()) {
                     std::cout << "> ";
                     std::getline(std::cin, curOption);
                     if (std::cin.eof()) {
                        curOption = "Quit";
                     }
                  }        
               }       
            }
         // Conditional checks   
         } while (curOption != "pass" && curOption != "Quit" && curOption != "done");
         // End the game if tile bag is empty, a player has no more tiles in their hand or a player passes twice
         if ((tileBag->peek() == NULL) && ((player1->hand->peek() == NULL) || (player2->hand->peek() == NULL) ||
             (passTracker[0] == true && passTracker[2] == true) || (passTracker[1] == true and passTracker[3] == true))) {
            endGame(player1, player2);
            curOption = "Quit";
         }
         std::cout << std::endl;
         bingoCounter = 0;
         
      // Change turn state bool
      player1->changeTurn();
      player2->changeTurn();
      }
   }
}

bool inputCheck(std::string curOption) {
   int length = curOption.length();
   int loc = 0;
   
   if (curOption == "Quit" || curOption == "pass" || curOption == "help") {
      return true;
   }
   else if (curOption.find("save") != std::string::npos) {
      if (curOption[4] == ' ' && length > 5) {  
         for(int i = 5; i < length; ++i) {
            if(!isalpha(curOption[i]) && !isdigit(curOption[i])) {
               std::cout << "Save file must only contain letters and/or numbers.\n";
               return false;
            }
         }  
         return true;
      }
   }
   else if (curOption.find("replace") != std::string::npos) {
      if (curOption[7] == ' ' && length < 10) {
         if (curOption[8] >= 'A' && curOption[8] <= 'Z') {
            return true;
         }
      }
   }
   else if (curOption.find("place") != std::string::npos) {
      if (curOption.find("place Done") != std::string::npos) {
         return true;
      }
      else if (curOption[5] == ' ' && curOption[8] == 'a' && curOption[9] == 't' && curOption[10] == ' ' && length > 12) {
            if (curOption[11] >= 'A' && curOption[11] <= 'O') {
               loc = stoi(curOption.substr(12));
               if (loc >= 0 && loc <= 14) {
                  return true;
               }
            }
      }
   }
   std::cout << "Invalid input" << std::endl;
   return false;
}

bool betterInputCheck(std::string curOption) {
   int length = curOption.length();
   int loc = 0;
   
   if (curOption == "Quit" || curOption == "pass" || curOption == "help") {
      return true;
   }
   else if (curOption.find("save") != std::string::npos) {
      if (curOption[4] == ' ' && length > 5) {  
         for(int i = 5; i < length; ++i) {
            if(!isalpha(curOption[i]) && !isdigit(curOption[i])) {
               std::cout << "Save file must only contain letters and/or numbers.\n";
               return false;
            }
         }  
         return true;
      }
      std::cout << "Invalid input. The correct syntax for <save> is:\n"
               << "> save <fileName>"
      << std::endl;
      return false;
   }
   else if (curOption.find("replace") != std::string::npos) {
      if (curOption[7] == ' ' && length < 10) {
         if (curOption[8] >= 'A' && curOption[8] <= 'Z') {
            return true;
         }
      }
      std::cout << "Invalid input. The correct syntax for <replace> is:\n"
               << "> replace <letter>"
      << std::endl;
      return false;
   }
   else if (curOption.find("place") != std::string::npos) {
      if (curOption.find("place Done") != std::string::npos) {
         return true;
      }
      else if (curOption[5] == ' ' && curOption[8] == 'a' && curOption[9] == 't' && curOption[10] == ' ' && length > 12) {
            if (curOption[11] >= 'A' && curOption[11] <= 'O') {
               loc = stoi(curOption.substr(12));
               if (loc >= 0 && loc <= 14) {
                  return true;
               }
            }
      }
      std::cout << "Invalid input. The correct syntax for <place> is:\n"
               << "> place <letter> at <row><column>"
      << std::endl;
      return false;
   }
   std::cout << "Invalid input. Type \"help\" for a list of commands." << std::endl;
   return false;
}

void endGame(Player* player1, Player* player2) {
   std::cout << std::endl;
   std::cout << "Game over\n" <<
                "Score for " << player1->name << ": " << player1->score <<
                "\nScore for " << player2->name << ": " << player2->score << 
   std::endl;
   if (player1->score > player2->score) {
      std::cout << "Player " << player1->name << " won!" << std::endl;
   }
   else if (player1->score < player2->score) {
      std::cout << "Player " << player2->name << " won!" << std::endl;
   }
   else  {
      std::cout << "It's a draw!" << std::endl; 
   }
}

void printNameAndScore(Player* player1, Player* player2) {
   if(player1->turn) {
      std::cout << player1-> colour << player1->name << "\u001b[0m, it's your turn" << std::endl;
   }
   else {
      std::cout << player2-> colour << player2->name << "\u001b[0m, it's your turn" << std::endl;
   }
   std::cout << "Score for " << player1-> colour << player1->name << "\u001b[0m: " << player1->score << std::endl;
   std::cout << "Score for " << player2-> colour << player2->name << "\u001b[0m: " << player2->score << std::endl;
}

void printBoard(std::vector<std::vector<Tile*>>* boardPtr) {
   std::cout << "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  " << std::endl;
   std::cout << "  -------------------------------------------------------------" << std::endl;
   char a = 'A';
   for (int i = 0; i < 15; i++){
      std::cout << a << " |";
      for (int j = 0; j < 15; j++) {
         if (boardPtr->at(i).at(j) == NULL) {
            std::cout << "   |";
         }
         else {
            std::cout << " " << boardPtr->at(i).at(j)->colour << boardPtr->at(i).at(j)->letter << "\u001b[0m |";
         }
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

void newGame(Player* player1, Player* player2, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, char** argv) {
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
      if (!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); }))) {
         std::cout << "Name must be uppercase characters only.\n" << std::endl;
      }
   } while(!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); })));
   std::cout << std::endl;
   player1->setName(input);

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
         std::cout << "Name must be uppercase characters only.\n" << std::endl;
      }
   } while(!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); })));
   player1->setName(input);
   
   std::cout << "\nLet's Play!\n" << std::endl;

   // Generate new tile bag and player hands
   *tileBag = *createTileBag();
   player1->createHand(tileBag);
   player2->createHand(tileBag);
   // Set turn to player 1
   player1->turn = true;
   std::cin.ignore();
}

void loadGame(Player* player1, Player* player2, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::shared_ptr<std::string> curOpt) {
   std::string fileName;
   std::ifstream saveFile;   
   std::string input;
   Tile* tile = NULL;

   // Get file name
   std::cout << "Enter the filename from which to load a game" << std::endl;
   std::getline(std::cin, fileName);
   do {
      std::cout << "> ";
      std::getline(std::cin, fileName);
      if (std::cin.eof()) {
         *curOpt = "Quit";
         std::cout << std::endl;
         return;
      }
      saveFile.open("saveFiles/" + fileName + ".save");
      if (!saveFile) {
         std::cout << "File not found or invalid." << std::endl;
      }
   } while(!saveFile);

   // Player 1 name and score
   saveFile >> (player1)->name;
   saveFile >> (player1)->score;
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
      player1->hand->append(tile);
   } while(input.find(',') != std::string::npos);

   // Player 2 name and score
   saveFile >> (player2)->name;
   saveFile >> (player2)->score;
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
      player2->hand->append(tile);
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
         if (input == " ") {
            boardPtr->at(i).at(j - 1) = NULL;
         }
         else {
            boardPtr->at(i).at(j - 1) = new Tile(input[0], 0);
         }
      }
   }
   // Get tile bag
   input = "resetting";
   saveFile >> input;
   // Check if tileBag not empty
   if(input.find(',') != std::string::npos) {
      do {
         // Exception for literally just the letters Z and Q
         if(input.length() == 5) {
            tile = new Tile(input[0], std::stoi(input.substr(2,3)));
         }
         else {
            tile = new Tile(input[0], input[2] - '0');
         }
         tileBag->append(tile);
         saveFile >> input;
      } while(input.find(',') != std::string::npos);
   }
   // Get player turn
   saveFile >> input;
   if(player1->name == input) {
      player1->turn = true;
   }
   else {
      player2->turn = true;
   }
   std::cout << "\nLoaded save file: " << fileName << "\n" << std::endl;
}

void saveGame(Player* player1, Player* player2, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::string fileName) {
   std::ofstream saveFile(fileName);
   Tile* tile = NULL;
   LinkedList* player1HandCopy = new LinkedList(*player1->hand);
   LinkedList* player2HandCopy = new LinkedList(*player2->hand);
   LinkedList* tileBagCopy = new LinkedList(*tileBag);

   // Write player 1 name and score
   saveFile << player1->name << std::endl;
   saveFile << player1->score << std::endl;
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
   saveFile << player2->name << std::endl;
   saveFile << player2->score << std::endl;
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
         if (boardPtr->at(i).at(j)== NULL) {
            saveFile << "   |";
         }
         else {
            saveFile << " " << boardPtr->at(i).at(j)->letter << " |";
         }
      }
      saveFile << std::endl;
      a++;
   }
   // Write tile bag
   if(tileBagCopy->peek() == NULL) {
      saveFile << std::endl;
   }
   else {
      do {
         tile = new Tile(*tileBagCopy->pop());
          // Check for last tile
         if(tileBagCopy->peek() == NULL) {
            saveFile << tile->letter << "-" << tile->value << std::endl;
         }
         else {
            saveFile << tile->letter << "-" << tile->value << ", ";
         }
         delete tile;
      }
      while(tileBagCopy->peek() != NULL);
   }
   // Write player turn
   if(player1->turn) {
      saveFile << player1->name;
   }
   else {
      saveFile << player2->name;
   }
   std::cout << "Successfully saved game to " << fileName << std::endl;

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

   // Read file into vector for shuffling
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

void help() {
   std::cout << "Some commands you can use:\n"
            << " > place - place a tile on the board\n"
            << " > replace - swap a tile in your hand\n"
            << " > pass - skip your turn\n"
            << " > save - save the current game\n"
            << " > Quit - exit the game"
   << std::endl;
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
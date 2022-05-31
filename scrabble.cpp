
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
void newGame(Player* player1, Player* player2, Player* player3, Player* player4, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, char** argv);
void loadGame(Player* player1, Player* player2, Player* player3, Player* player4, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, 
               std::shared_ptr<std::string> curOpt);
void saveGame(Player* player1, Player* player2, Player* player3, Player* player4, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::string fileName);
void playGame(Player* player1, Player* player2, Player* player3, Player* player4, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, 
               std::shared_ptr<std::string> curOpt, char** argv);
void getNewPlayer(Player* player, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, int playerNum);
void readHand(Player* player, std::string input);
void printBoard(std::vector<std::vector<Tile*>>* boardPtr);
void printNameAndScore(Player* player1, Player* player2, Player* player3, Player* player4);
void endGame(Player* player1, Player* player2, Player* player3, Player* player4);
bool inputCheck(std::string curOption);
bool betterInputCheck(std::string curOption);
LinkedList* createTileBag();
void help();
void credits();


int main(int argc, char** argv) {
   std::string option = "0";
   
   // Check for milestone 2 or 3
   if (argc < 6) {
      std::cout << "\nProgram terminated.\n"
               << "Please type y/n for each enhancement:\n"
               << "> ./scrabble <Help!> <Better Invalid Input> <Colour> <Word Checking> <3/4 Player>\n"
               << "\nFor example:\n"
               << "> ./scrabble y y n y n\n"
      << std::endl;
      return EXIT_SUCCESS;
   }
   // Allocate empty tile bag
   LinkedList* tileBag = new LinkedList;
   // Allocate memory for players
   Player* player1 = new Player;
   Player* player2 = new Player;
   Player* player3 = new Player;
   Player* player4 = new Player;
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
         newGame(player1, player2, player3, player4, tileBag, curOpt, argv);
      }
      // Load a game from a save file
      else if (option == "2") {
         loadGame(player1, player2, player3, player4, boardPtr, tileBag, curOpt);
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
      player3->colour = "\u001b[33m";
      player4->colour = "\u001b[32m";
   }
   else {
      player1->colour = "\u001b[0m";
      player2->colour = "\u001b[0m";
      player3->colour = "\u001b[0m";
      player4->colour = "\u001b[0m";
   }
   // Enter playGame to play scrabble
   playGame(player1, player2, player3, player4, boardPtr, tileBag, curOpt, argv);
   std::cout << "Goodbye" << std::endl;
   delete tileBag;
   delete player1;
   delete player2;
   delete player3;
   delete player4;
   return EXIT_SUCCESS;
}

void playGame(Player* player1, Player* player2, Player* player3, Player* player4, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag,
               std::shared_ptr<std::string> curOpt, char** argv) {
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
      // If statements to set the current players information
      if ((player1)->turn) {
         curPlayer = player1;
      }
      else if (player2->turn) {
         curPlayer = player2;
      }
      else if (player3->turn) {
         curPlayer = player3;
      }
      else {
         curPlayer = player4;
      }
      // Print the name, score, board and hand
      printNameAndScore(player1, player2, player3, player4);
      printBoard(boardPtr);

      std::cout << "\nYour hand is" << std::endl;
      // Add some colour!
      std::cout << curPlayer->colour;
      curPlayer->hand->print();
      std::cout << "\u001b[0m";

      std::cin.clear();
      std::cin.sync();

      // Set curOption to player input switch
      curOption = "next";
      // Enter a do while loop after getting user input and running the command according to user input
      do {
         // If user input is a replace command
         if (curOption.find("replace") != std::string::npos) {
            // If tile is in hand
            if (curPlayer->hand->get(curOption[8]) != NULL) {
               curPlayer->replaceTile(tileBag, curOption[8]);
               curOption = "done";
            }
            else {
               std::cout << "Tile not in hand" << std::endl;
               curOption = "next";
            }
         }
         else if (curOption.find("place Done") != std::string::npos) {
            bool placed;
            placed = curPlayer->placeWord(word, boardPtr, *argv[4], wordList);
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
                  if (tileBag->peek() != NULL) {
                     curPlayer->hand->append(tileBag->pop());
                  }
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
            saveGame(player1, player2, player3, player4, boardPtr, tileBag, fileName);
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
         // If a player passes, increase consecutive turns passed
         if (curOption == "pass") {
            curPlayer->pass += 1;
            // If player passes halfway through placing a word
            if(bingoCounter != 0) {
               word.clear();
            }
         }
         // Conditional checks   
      } while (curOption != "pass" && curOption != "Quit" && curOption != "done");
      // End the game if tile bag is empty, a player has no more tiles in their hand or a player passes twice
      if (player4->name != "") {
         if ((tileBag->peek() == NULL) && ((player1->hand->peek() == NULL) || (player2->hand->peek() == NULL) || (player3->hand->peek() == NULL) || 
         (player4->hand->peek() == NULL) || (curPlayer->pass == 2))) {
         endGame(player1, player2, player3, player4);
         curOption = "Quit";
         }
      }
      else if (player3->name != "") {
         if ((tileBag->peek() == NULL) && ((player1->hand->peek() == NULL) || (player2->hand->peek() == NULL) || (player3->hand->peek() == NULL) ||
          (curPlayer->pass == 2))) {
         endGame(player1, player2, player3, player4);
         curOption = "Quit";
         }
      }
      else {
         if ((tileBag->peek() == NULL) && ((player1->hand->peek() == NULL) || (player2->hand->peek() == NULL) ||
            (curPlayer->pass == 2))) {
            endGame(player1, player2, player3, player4);
            curOption = "Quit";
         }
      }
      
      std::cout << std::endl;
      bingoCounter = 0;
      // Check if a turn was not a pass
      if (curOption == "done") {
         curPlayer->pass = 0;
      }

      // Change turn state bool
      if (player4->name != "") {
         if (player1->turn) {
            player1->changeTurn();
            player2->changeTurn();
         }
         else if (player2->turn) {
            player2->changeTurn();
            player3->changeTurn();
         }
         else if (player3->turn) {
            player3->changeTurn();
            player4->changeTurn();
         }
         else {
            player4->changeTurn();
            player1->changeTurn();
         }
      }
      else if (player3->name != "") {
         if (player1->turn) {
            player1->changeTurn();
            player2->changeTurn();
         }
         else if (player2->turn) {
            player2->changeTurn();
            player3->changeTurn();
         }
         else if (player3->turn) {
            player3->changeTurn();
            player1->changeTurn();
         }
      }
      else {
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
               std::cout << "Invalid input. Placement must be within the scope of the playing board." << std::endl;
               return false;
            }
            std::cout << "Invalid input. Placement must be within the scope of the playing board." << std::endl;
            return false;
            
      }
      std::cout << "Invalid input. The correct syntax for <place> is:\n"
               << "> place <letter> at <row><column>"
      << std::endl;
      return false;
   }
   std::cout << "Invalid input. Type \"help\" for a list of commands." << std::endl;
   return false;
}

void endGame(Player* player1, Player* player2, Player* player3, Player* player4) {
   std::cout << std::endl;
   std::cout << "Game over\n" <<
                "Score for " << player1->colour << player1->name << "\u001b[0m: " << player1->score <<
                "\nScore for " << player2->colour << player2->name << "\u001b[0m: " << player2->score << std::endl;
   if (player3->name != "") {
      std::cout << "Score for " << player3->colour << player3->name << "\u001b[0m: " << player3->score << std::endl;
   }
   if (player4->name != "") {
      std::cout << "Score for " << player4->colour << player4->name << "\u001b[0m: " << player4->score << std::endl;
   }

   // Find highest score
   std::vector<Player*> players = {player1, player2, player3, player4};
   Player* bestPlayer = new Player();
   for(Player* player : players) {
      if (player->score > bestPlayer->score) {
         bestPlayer = player;
      }
   }
   if (player4->name != "" && (bestPlayer->score * 4) == (player1->score + player2->score + player3->score + player4->score)) {
      std::cout << "It's a draw!" << std::endl; 
   }
   else if (player3->name != "" && (bestPlayer->score * 3) == (player1->score + player2->score + player3->score)) {
      std::cout << "It's a draw!" << std::endl;
   }
   else if ((bestPlayer->score * 2) == (player1->score + player2->score)) {
      std::cout << "It's a draw!" << std::endl; 
   }
   else  {
      std::cout << "Player " << bestPlayer->colour << bestPlayer->name << "\u001b[0m won!" << std::endl;
   }
}

void printNameAndScore(Player* player1, Player* player2, Player* player3, Player* player4) {
   if(player1->turn) {
      std::cout << player1-> colour << player1->name << "\u001b[0m, it's your turn" << std::endl;
   }
   else if (player2->turn) {
      std::cout << player2-> colour << player2->name << "\u001b[0m, it's your turn" << std::endl;
   }
   else if (player3->turn) {
      std::cout << player3-> colour << player3->name << "\u001b[0m, it's your turn" << std::endl;
   }
   else {
      std::cout << player4-> colour << player4->name << "\u001b[0m, it's your turn" << std::endl;
   }
   std::cout << "Score for " << player1-> colour << player1->name << "\u001b[0m: " << player1->score << std::endl;
   std::cout << "Score for " << player2-> colour << player2->name << "\u001b[0m: " << player2->score << std::endl;
   if (player3->name != "") {
      std::cout << "Score for " << player3-> colour << player3->name << "\u001b[0m: " << player3->score << std::endl;
   }
   if (player4->name!= "") {
      std::cout << "Score for " << player4-> colour << player4->name << "\u001b[0m: " << player4->score << std::endl;
   }
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

void newGame(Player* player1, Player* player2, Player* player3, Player* player4, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, char** argv) {
   std::string input;
   int numPlayers = 0;
   std::cout << "Starting a New Game\n" << std::endl;
   // Generate tile bag
   *tileBag = *createTileBag();
   // 3/4 player option
   if (*argv[5] == 'y') {
      do {
         if (std::cin.eof()) {
            *curOpt = "Quit";
            return;
         }
         std::cout << "Enter the number of players (3/4):\n"
                  << "> ";
         std::cin >> input;
      } while (input < "3" || input > "4");
      numPlayers = stoi(input);
   }

   // Set player 1 name and hand
   getNewPlayer(player1, tileBag, curOpt, 1);

   // Set player 2 name and hand
   getNewPlayer(player2, tileBag, curOpt, 2);

   if (numPlayers > 2) {
      // Set player 3 name and hand
      getNewPlayer(player3, tileBag, curOpt, 3);
   }
   if (numPlayers > 3) {
      // Set player 4 name and hand
      getNewPlayer(player4, tileBag, curOpt, 4);
   }
   std::cout << "\nLet's Play!\n" << std::endl;

   // Set turn to player 1
   player1->turn = true;
   std::cin.ignore();
}

void getNewPlayer(Player* player, LinkedList* tileBag, std::shared_ptr<std::string> curOpt, int playerNum) {
   std::string input;
   do {
      if (std::cin.eof()) {
         *curOpt = "Quit";
         return;
      }
      std::cout << "Enter a name for player " << playerNum
               << "\n> ";
      std::cin >> input;
      if (!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); }))) {
         std::cout << "Name must be uppercase characters only.\n" << std::endl;
      }
      } while(!(std::all_of((input).begin(), (input).end(), [](unsigned char c){ return std::isupper(c); })));
      player->setName(input);
      player->createHand(tileBag);
}

void loadGame(Player* player1, Player* player2, Player* player3, Player* player4, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag,
               std::shared_ptr<std::string> curOpt) {
   std::string fileName;
   std::ifstream saveFile;   
   std::string input;
   std::string line;
   int numPlayers = 0;
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

   getline(saveFile, line);
   if (line[0] == '#') { // If 3/4 player
      saveFile >> (player1)->name;
      numPlayers = line[1] - '0';
   }
   else { // Default
      (player1)->name = line;
   }
   // Player 1 score
   saveFile >> (player1)->score;
   // Player 1 hand
   do {
      saveFile >> input;
      readHand(player1, input);
   } while(input.find(',') != std::string::npos);

   // Player 2 name and score
   saveFile >> (player2)->name;
   saveFile >> (player2)->score;
   // Player 2 hand
   input = "resetting";
   do {
      saveFile >> input;
      readHand(player2, input);
   } while(input.find(',') != std::string::npos);

   if (numPlayers > 2) {
      // Player 3 name and score
      saveFile >> (player3)->name;
      saveFile >> (player3)->score;
      // Player 3 hand
      input = "resetting";
      do {
         saveFile >> input;
         readHand(player3, input);
      } while(input.find(',') != std::string::npos);
   }
   if (numPlayers > 3) {
      // Player 4 name and score
      saveFile >> (player4)->name;
      saveFile >> (player4)->score;
      // Player 4 hand
      input = "resetting";
      do {
         saveFile >> input;
         readHand(player4, input);
      } while(input.find(',') != std::string::npos);
   }

   // Set getline to first row of board
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
            boardPtr->at(i).at(j - 1)->row = i;
            boardPtr->at(i).at(j - 1)->col = j - 1;
         }
      }
   }
   // Get tile bag
   input = "resetting";
   saveFile >> input;
   // Check if tileBag not empty
   if(input.find('-') != std::string::npos) {
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
   else if (player2->name == input) {
      player2->turn = true;
   }
   else if (player3->name == input) {
      player3->turn = true;
   }
   else {
      player4->turn = true;
   }
   std::cout << "\nLoaded save file: " << fileName << "\n" << std::endl;
}

void readHand(Player* player, std::string input) {
   Tile* tile;
   // Exception for literally just the letters Z and Q
   if(input.length() == 5) {
      tile = new Tile(input[0], std::stoi(input.substr(2,3)));
   }
   else {
      tile = new Tile(input[0], input[2] - '0');
   }
   player->hand->append(tile);
}

void saveGame(Player* player1, Player* player2, Player* player3, Player* player4, std::vector<std::vector<Tile*>>* boardPtr, LinkedList* tileBag, std::string fileName) {
   std::ofstream saveFile(fileName);
   Tile* tile = NULL;
   LinkedList* player1HandCopy = new LinkedList(*player1->hand);
   LinkedList* player2HandCopy = new LinkedList(*player2->hand);
   LinkedList* tileBagCopy = new LinkedList(*tileBag);

   // If more than 2 player
   if (player4->name != "") {
      saveFile << "#4 player" << std::endl;
   }
   else if (player3->name != "") {
      saveFile << "#3 player" << std::endl;
   }

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
   } while(player1HandCopy->peek() != NULL);

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
   } while(player2HandCopy->peek() != NULL);

   // If player3 exists, write
   if (player3->name != "") {
      LinkedList* player3HandCopy = new LinkedList(*player3->hand);
      // Write player 3
      saveFile << player3->name << std::endl;
      saveFile << player3->score << std::endl;
      // Write player2 hand
      do {
         tile = new Tile(*player3HandCopy->pop());
         // Check for last tile
         if(player3HandCopy->peek() == NULL) {
            saveFile << tile->letter << "-" << tile->value << std::endl;
         }
         else {
            saveFile << tile->letter << "-" << tile->value << ", ";
         }
         delete tile;
      } while(player3HandCopy->peek() != NULL);
   }
   // If player4 exists, write
   if (player4->name != "") {
      LinkedList* player4HandCopy = new LinkedList(*player4->hand);
      // Write player 3
      saveFile << player4->name << std::endl;
      saveFile << player4->score << std::endl;
      // Write player2 hand
      do {
         tile = new Tile(*player4HandCopy->pop());
         // Check for last tile
         if(player4HandCopy->peek() == NULL) {
            saveFile << tile->letter << "-" << tile->value << std::endl;
         }
         else {
            saveFile << tile->letter << "-" << tile->value << ", ";
         }
         delete tile;
      } while(player4HandCopy->peek() != NULL);
   }

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
   else if (player2->turn) {
      saveFile << player2->name;
   }
   else if (player3->turn) {
      saveFile << player3->name;
   }
   else {
      saveFile << player4->name;
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
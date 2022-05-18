
#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "LinkedList.h"
#include <iostream>
#include <vector>
#include <unordered_set>

class Player {
public:

   Player();
   ~Player();

   // Set player name
   void setName(std::string name);
   // Generate a hand of 7 tiles from the tile bag
   void createHand(LinkedList* tileBag);
   // Change turn
   void changeTurn();
   // Replace chosend tile in hand with tile from top of bag
   void replaceTile(LinkedList* tileBag, Letter letter);
   // Check if a tile can be placed on the board
   bool placeCheck(char letter, std::string pos, std::vector<std::vector<Tile*>>* boardPtr);
   // Place word on board
   bool placeWord(std::vector<Tile*> word, std::vector<std::vector<Tile*>>* boardPtr, char checker, std::unordered_set<std::string> wordList);

   std::string    name;
   int    score;
   bool turn;
   LinkedList* hand;
   std::string colour;
   // number of times a player passes
   int pass;
};

#endif // ASSIGN2_PLAYER_H
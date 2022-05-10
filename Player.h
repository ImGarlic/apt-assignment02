
#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "LinkedList.h"
#include <iostream>
#include <vector>

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
   // Place a tile on the board
   void placeTile(char replaceLetter, char letter, std::string number, std::vector<std::vector<std::string>>* boardPtr);

   std::string    name;
   int    score;
   bool turn;
   LinkedList* hand;
};

#endif // ASSIGN2_PLAYER_H

#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "LinkedList.h"
#include <iostream>

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

   std::string    name;
   int    score;
   bool turn;
   LinkedList* hand;
};

#endif // ASSIGN2_PLAYER_H
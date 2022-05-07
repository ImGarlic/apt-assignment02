#include "LinkedList.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>

#define EXIT_SUCCESS    0

LinkedList* createTileBag() {
   LinkedList* tileBag = new LinkedList();
   std::vector<Tile*>tiles;
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

void replaceTile(LinkedList* tileBag, LinkedList* hand, Letter letter) {
    Tile* tile = hand->remove(letter);
    if(tile == NULL) {
        return;
    }
    tileBag->append(tile);
    tile = tileBag->pop();
    hand->append(tile);
}

int main(void) {
    LinkedList* tileBag = createTileBag();
    LinkedList* hand = createHand(tileBag);
    Letter e = 'E';
    hand->print();
    replaceTile(tileBag, hand, e);
    hand->print();

    delete tileBag;
}
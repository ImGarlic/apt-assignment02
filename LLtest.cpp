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

int main(void) {
    LinkedList* list = new LinkedList();
    list->print();
    list = createTileBag();
    Tile* t = list->pop();
    std::cout << "Popped " << t->letter << "-" << t->value << std::endl;
    list->print();
    delete t;
    delete list;
}
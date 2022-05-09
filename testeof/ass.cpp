// g++ -Wall -Werror -std=c++14 -g -O -o ass Tile.cpp Node.cpp LinkedList.cpp Player.cpp ass.cpp

#include "LinkedList.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>

#define EXIT_SUCCESS    0

LinkedList* createTileBag() {
   LinkedList* tileBag = new LinkedList();
    for(int i = 0; i < 4; ++i) {
        tileBag->append(new Tile('A', 1));
    }
    tileBag->append(new Tile('E', 1));
    tileBag->append(new Tile('E', 1));
    tileBag->append(new Tile('E', 1));
    for(int i = 0; i < 40; ++i) {
        tileBag->append(new Tile('A', 1));
    }
    return tileBag;
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
    Player* player1 = new Player();
    player1->setName();
    std::cout << player1->name << std::endl;

    delete tileBag;
}
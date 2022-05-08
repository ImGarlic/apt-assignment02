
#include "Player.h"
#include <iostream>
#include <algorithm>

Player::Player() {

}

Player::~Player() {

}
void Player::setName() {
    std::cin >> name;
    while(!(std::all_of(name.begin(), name.end(), [](unsigned char c){ return std::isupper(c); }))) {
        std::cout << "\nName must be uppercase characters only\n"
                << "> ";
        std::cin >> name;
    }
}
void Player::createHand(LinkedList* tileBag) {
    hand = new LinkedList();
    Tile* tile = NULL;

    // Take first 7 tiles from bag and add to hand
    for(int i = 0; i < 7; ++i) {
      tile = tileBag->pop();
      this->hand->append(tile);
    }
}

void Player::changeTurn() {
    // Change turn from true to false or vice-versa
    if(turn) {
        turn = false;
        return;
    }
    turn = true;
}

void Player::replaceTile(LinkedList* tileBag, Letter letter) {
    // Remove tile from hand and get tile
    Tile* tile = hand->remove(letter);
    // Tile not found
    if(tile == NULL) {
        return;
    }
    // Add tile back to bag
    tileBag->append(tile);
    // Add bag's top tile to hand
    tile = tileBag->pop();
    hand->append(tile);
}

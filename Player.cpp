
#include "Player.h"
#include <iostream>
#include <algorithm>

Player::Player() {
    hand = new LinkedList();
    turn = false;
    score = 0;
}

Player::~Player() {

}
void Player::setName(std::string name) {
    this->name = name;
}
void Player::createHand(LinkedList* tileBag) {
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


#include "Player.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>

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

void Player::placeTile(char replaceLetter, char letter, std::string number, std::vector<std::vector<std::string>>* boardPtr) {
   std::unordered_map<char, int> alphabet = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, 
                                             {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}};
      std::string num1 = number;
      int row = alphabet[letter];
      int col = std::stoi(num1);
      boardPtr->at(row).at(col) = replaceLetter;
}

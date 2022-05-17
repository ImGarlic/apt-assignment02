
#include "Tile.h"
#include <iostream>
#include <unordered_map>

Tile::Tile(Letter letter, Value value) {
    this->letter = letter;
    this->value = value;
    this->row = -1;
    this->col = -1;
    this->colour = "\u001b[0m";
}

Tile::Tile(Tile& other) {
    this->letter = other.letter;
    this->value = other.value;
    this->row = other.row;
    this->col = other.col;
    this->colour = other.colour;
}

Tile::~Tile() {

}

void Tile::setPos(std::string pos) {
    if (pos == "reset") {
        row = -1;
        col = -1;
        return;
    }
    std::unordered_map<char, int> alphabet = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, 
                                             {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}};
    row = alphabet[pos[0]];
    col = std::stoi(pos.substr(1));
}

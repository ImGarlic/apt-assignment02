
#include "Player.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <fstream>

Player::Player() {
    hand = new LinkedList();
    turn = false;
    score = 0;
    pass = 0;
}

Player::~Player() {
    delete hand;
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

bool Player::placeCheck(char letter, std::string pos, std::vector<std::vector<Tile*>>* boardPtr) {
    std::unordered_map<char, int> alphabet = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, 
                                             {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}};
    int row = alphabet[pos[0]];
    int col = std::stoi(pos.substr(1));

    if (boardPtr->at(row).at(col) == NULL) {
        if (!hand->get(letter)) {
            std::cout << "Tile not in hand" << std::endl;
            return false;
        }
        return true;
    }
    std::cout << "Tile already exists at " << pos[0] << col << std::endl;
    return false;
}

bool Player::placeWord(std::vector<Tile*> word, std::vector<std::vector<Tile*>>* boardPtr, char checker, std::unordered_set<std::string> wordList) {
    int tempScore = 0;
    for (Tile* tile : word) {
        // Check if user entered the same pos twice
        if (boardPtr->at(tile->row).at(tile->col) != NULL) {
            std::cout << "Couldn't place word; board has been double-placed.\nPlease try again." << std::endl;
            return false;
        }
        // Place tile on board with colour
        tile->colour = this->colour;
        boardPtr->at(tile->row).at(tile->col) = tile;
        // Update score
        tempScore += tile->value;
    }
    // Word checker off
    if(checker == 'n') {
        score += tempScore;
        return true;
    }
    // Word checker on
    std::string curWord = "";
    Tile* curTile;
    std::vector<std::string> boardWords;
    bool reading;
    // Check tiles around positions for existing words on the board
    for (Tile* tile : word) {
        int row = tile->row;
        int col = tile->col;
        curTile = tile;
        reading = true;
        curWord = "";
        // Check vertical
        // Go all the way up
        while (row >= 0) {
            if (boardPtr->at(row).at(col) == NULL) {
                reading= false;
            }
            if (reading) {
                curTile = boardPtr->at(row).at(col);
            }
            row -= 1;
        }
        row = curTile->row;
        reading = true;
        // Go back down and make tiles into a word
        while (row <= 14) {
            if (boardPtr->at(row).at(col) == NULL) {
                reading = false;
            }
            if (reading) {
                curTile = boardPtr->at(row).at(col);
                curWord += curTile->letter;
            }
            row += 1;
        }
        // Lowercase for checking
        std::transform(curWord.begin(), curWord.end(), curWord.begin(), [](unsigned char c){ return std::tolower(c); });
        boardWords.push_back(curWord);
        // Reset
        curWord = "";
        curTile = tile;
        row = tile->row;
        col = tile->col;
        reading = true;

        // Check horizontal
        // Go all the way left
        while (col >= 0) {
            if (boardPtr->at(row).at(col) == NULL) {
                reading = false;
            }
            if (reading) {
                curTile = boardPtr->at(row).at(col);
            }
            col -= 1;
        }
        col = curTile->col;
        reading = true;
        // Go back right and make tiles into a word
        while (col <= 14) {
            if (boardPtr->at(row).at(col) == NULL) {
                reading = false;
            }
            if (reading) {
                curTile = boardPtr->at(row).at(col);
                curWord += curTile->letter;
            }
            col += 1;
        }
        // Lowercase for checking
        std::transform(curWord.begin(), curWord.end(), curWord.begin(), [](unsigned char c){ return std::tolower(c); });
        boardWords.push_back(curWord);
    }
    // Check if all words exist in word list
    for (std::string j : boardWords) {
        // Check list
        if(wordList.find(j) == wordList.end()) {
            if (wordList.find(j) == wordList.end()) {
                std::cout << j << " does not exist in the Scrabble Dictionary.\nPlease try again." << std::endl;
                return false;
            }
        }
    }
    score += tempScore;
    return true;
}

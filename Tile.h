
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H
#include <iostream>

// Define a Letter type
typedef char Letter;

// Define a Value type
typedef int Value;

class Tile {
public:

   Tile(Letter letter, Value value);
   Tile(Tile& other);
   ~Tile();
   void setPos(std::string pos);

   Letter letter;
   Value  value;
   std::string colour;
   int row;
   int col;
};

#endif // ASSIGN2_TILE_H


#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:

   LinkedList();
   ~LinkedList();

   // Print out all nodes
   void print();

   // Add tile to end of list
   void append(Tile* tile);

   // Remove first node from list and return tile
   Tile* pop();

private:
   Node* head;
};

#endif // ASSIGN2_LINKEDLIST_H

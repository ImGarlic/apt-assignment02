
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:

   LinkedList();
   ~LinkedList();
   LinkedList(LinkedList& other);

   // Print out all nodes
   void print();

   // Add tile to end of list
   void append(Tile* tile);

   // Remove first node from list and return respective tile
   Tile* pop();

   // Remove first node containing letter and return respective tile
   Tile* remove(Letter letter);

   // Return tile at the front of the list
   Tile* peek();

   // Find if letter exists in the list
   Tile* get(Letter letter);

private:
   Node* head;
};

#endif // ASSIGN2_LINKEDLIST_H

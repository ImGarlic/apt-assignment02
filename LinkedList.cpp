
#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList() {
   head = nullptr;

   // TODO
}

LinkedList::~LinkedList() {
}

void LinkedList::print() {
   Node* curNode = head;
   std::cout << "Your hand is" << std::endl;
   while(curNode->next != NULL) {
      std::cout << curNode->tile->letter << "-" << curNode->tile->value << ", ";
      curNode = curNode->next;
   }
   std::cout << curNode->tile->letter << "-" << curNode->tile->value << std::endl;
}

void LinkedList::append(Tile* tile) {
   Node* node = new Node(tile, NULL);
   Node* curNode = head;
   // Base case, list is empty
   if(head == nullptr) {
      head = node;
      return;
   }
   // Traverse to end
   while(curNode->next != NULL) {
      curNode = curNode->next;
   }
   // Assign next node to appended node
   curNode->next = node;
   return;
}

Tile* LinkedList::pop() {
   // Base case, list is empty
   if(head == nullptr) {
      return NULL;
   }
   // Assign head to next node and return removed node
   Tile* tile = head->tile;
   head = head->next;

   return tile;
}
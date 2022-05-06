
#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList() {
   head = nullptr;
}

LinkedList::~LinkedList() {
   Node* curNode = head;
   Node* temp;
   // Base case, list empty
   if(head == nullptr) {
      return;
   }
   // Delete nodes one by one
   while(curNode->next != NULL) {
      temp = curNode;
      curNode = curNode->next;
      delete temp;
   }
   delete curNode;
}

void LinkedList::print() {
   Node* curNode = head;
   int count = 0;
   std::cout << "Your hand is" << std::endl;
   // Base case, list is empty
   if(head == nullptr) {
      std::cout << "empty lol" << std::endl;
      return;
   }
   // Traverse and print all nodes
   while(curNode->next != NULL) {
      std::cout << curNode->tile->letter << "-" << curNode->tile->value << ", ";
      curNode = curNode->next;
      count += 1;
   }
   // Final node node counted, so add to count and print node
   count += 1;
   std::cout << curNode->tile->letter << "-" << curNode->tile->value << std::endl;
   std::cout << count << std::endl;
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
   // Assign head to next node and return removed tile
   Tile* tile = head->tile;
   head = head->next;
   return tile;
}

Tile* LinkedList::remove(Letter letter) {
   Node* curNode = head;
   Node* temp;
   int count = 0;
   // Base case, list is empty
   if(head == nullptr) {
      return NULL;
   }
   // Traverse and search for letter
   while(curNode->next != NULL) {
      if(curNode->next->tile->letter == letter) {
         // Letter found, delete node
         count += 2;
         std::cout << "Removed " << curNode->next->tile->letter << "-" << curNode->next->tile->value << " at position " << count << std::endl;
         temp = curNode->next;
         curNode->next = temp->next;
         return temp->tile;
      }
      curNode = curNode->next;
      count += 1;
   }
   return NULL;
}
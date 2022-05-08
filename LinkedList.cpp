
#include "LinkedList.h"
#include <iostream>
#include <vector>

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
   delete head;
}

LinkedList::LinkedList(LinkedList& other) {
   this->head = nullptr;
   Node* curNode = other.head;

   // Traverse node and copy 1 by 1
   while(curNode != NULL) {
      this->append(curNode->tile);
      curNode = curNode->next;
   }
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
   Node* prevNode = nullptr;
   Tile* tile;

   // Base case, list is empty
   if(head == nullptr) {
      std::cout << "head" << std::endl;
      return NULL;
   }
   // First letter found
   if(head->tile->letter == letter) {
      return this->pop();
   }
   // Traverse and search for letter
   while(curNode->next != NULL) {
      std::cout << "tile :" << curNode->tile->letter << std::endl;
      if(curNode->tile->letter == letter) {
         // Letter found, delete node
         tile = curNode->tile;
         std::cout << "tile :" << tile->letter << std::endl;
         prevNode->next = curNode->next;
         return tile;
      }
      prevNode = curNode;
      curNode = curNode->next;
   }
   return NULL;
}

Tile* LinkedList::peak() {
   if(head == nullptr) {
      return NULL;
   }
   return this->head->tile;
}
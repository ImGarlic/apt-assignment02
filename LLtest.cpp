#include "LinkedList.h"
#include <iostream>
#include <memory>

#define EXIT_SUCCESS    0

int main(void) {
    LinkedList* list = new LinkedList();
    Letter a = 'A';
    Letter b = 'B';
    Value x = 1;
    Value y = 2;
    Tile* tile = new Tile(a, x);
    Tile* tile2 = new Tile(b, y);
    list->append(tile);
    list->append(tile2);
    list->print();
    Tile* t = list->pop();
    std::cout << "Popped " << t->letter << "-" << t->value << std::endl;
    list->print();
}
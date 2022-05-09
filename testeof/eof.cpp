#include <iostream>

int main() {
    std::string line;
    int num;
    // while (std::getline(std::cin, line))
    // {
    //     std::cout << line << std::endl;
    // }
  for (;;) {

    std::cin >> num;
    if (std::cin.eof()) break;
    std::cout << "Number is " << num << std::endl;

  }
    std::cout << "Done" << num << std::endl;  
    
}
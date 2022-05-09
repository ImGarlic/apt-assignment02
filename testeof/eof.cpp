#include <iostream>

int main() {
    std::string curOption;
    std::getline(std::cin, curOption);
    int length = curOption.length();
    std::cout << length << std::endl;

    if (length == 9) {
        if (curOption.find("replace") != std::string::npos) {
            if (curOption[7] == ' ') {
                if (curOption[8] >= 'A' && curOption[8] <= 'Z') {
                std::cout << "True1" << std::endl;
                return true;
                }
            }
        }
    }

    else if (length == 13 || length == 14){
        if (curOption.find("place") != std::string::npos) {
            if (curOption[5] == ' ' && curOption[8] == 'a' && curOption[9] == 't' && curOption[5] == ' ') {
                if (curOption[11] >= 'A' && curOption[11] <= 'O') {
                    if (length == 13) {
                        if (curOption[12] >= '0' && curOption[12] <= '9') {
                            std::cout << "True2" << std::endl;
                            return true;
                        }
                    }
                    else if (length == 14) {
                        if (curOption[13] >= '0' && curOption[13] <= '9') {
                            std::cout << "True3" << std::endl;
                            return true;
                        }                        
                    }
                }
            }
        }
    }

    else if (length == 10) {
        if (curOption.find("place Done") != std::string::npos) {
            std::cout << "True4" << std::endl;
            return true;
        }
    }

    else if (length == 4) {
        if (curOption.find("pass") != std::string::npos) {
            std::cout << "True5" << std::endl;
            return true;
        }
    }

    else if (curOption.find("save") != std::string::npos) {
        if (curOption[4] == ' ') {
            std::cout << "True6" << std::endl;
            return true;
        }
    }
}
#include <iostream>
#include "hashmap.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    hashmap<char *> charmap(64,false);
    charmap.insertunit(1,"sadsd");
    char * x = charmap.getunit(1).unitvalue;
    printf("value is : %s",x );
    return 0;
}
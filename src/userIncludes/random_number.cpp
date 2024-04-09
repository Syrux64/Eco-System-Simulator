#include<stdlib.h>

#include "random_number.h"

// Functions -->
int random_number(int min, int max){
    return min + rand() % (max - min + 1);
}
#include <iostream>
#include "operations.hpp"

using namespace std;


int main()
{
    gen_operations(DIV | ADD | MUL | SUB, 5);
}

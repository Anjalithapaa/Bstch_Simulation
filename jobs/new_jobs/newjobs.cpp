#include <iostream>

int main()
{
    int number = 2; // The number for which we are printing the multiplication table

    std::cout << "Multiplication Table of 2" << std::endl;

    // Loop to print the multiplication table
    for (int i = 1; i <= 10; ++i)
    {
        std::cout << number << " x " << i << " = " << number * i << std::endl;
    }

    return 0;
}

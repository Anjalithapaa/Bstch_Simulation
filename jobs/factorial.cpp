#include <iostream>

int main()
{
    int factorial = 1;
    for (int i = 1; i <= 10; ++i)
    {
        factorial *= i;
        std::cout << "Factorial after multiplying " << i << " is: " << factorial << std::endl;
    }
    return 0;
}

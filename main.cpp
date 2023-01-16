#include <iostream>
#include <stdio.h>
#include <cmath>
#include <bitset>
#include <climits>
#include <stdlib.h>

struct NumberDescription
{
    double val = 0;
    bool sign;
    double intpart = 0;
    double fractpart = 0;
    uint32_t bin_val = 0;
};

int countBits(uint32_t val) // подсчёт количества значащих битов
{
    int count = 0;

    while (val != 0)
    {
        val = val / 2;
        count++;
    }
    std::cout << "Count: " << count << std::endl;
    return count;
}

void split(struct NumberDescription *number)
{
    if (number->val < 0)
    {
        number->sign = true;
    }
    else
        number->sign = false;

    number->fractpart = (modf(abs(number->val), &number->intpart)); // находим целую и дробную часть
}

void setSign(uint32_t *val)
{
    *val |= (1 << 31);
}

uint32_t getSign(uint32_t val)
{
    return val >> 31;
}

int main()
{
    struct NumberDescription number;

    std::cout << "Введите число: \n";
    std::cin >> number.val;

    split(&number);

    std::cout << number.val << " = "
              << number.intpart << " + "
              << number.fractpart << std::endl;
    std::cout << "Sign: " << number.sign << std::endl;
    printf("val = %.10f %f + %.10f\n", number.val, number.intpart, number.fractpart);

    return 0;
}
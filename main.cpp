#include <iostream>
#include <stdio.h>
#include <cmath>
#include <bitset>
#include <climits>
#include <stdlib.h>

uint32_t swap(uint32_t old_val);

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

void convert_float_part_to_binary(struct NumberDescription *number)
{
    uint32_t val = 0;

    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";

    int tmp_intpart = int(number->intpart);
    double tmp_fractpart = number->fractpart;
    std::cout << "NUM = " << tmp_intpart << std::endl;

    int result;

    int num_significant_digit;
    int offset = 0;

    if (tmp_intpart == 0)
    {
        val = val << 1;
        offset = 1;
    }

    for (; tmp_intpart > 0 && offset <= 31; offset++)
    {
        result = tmp_intpart % 2;
        val = (val << 1) | result;
        tmp_intpart = tmp_intpart / 2;
        std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
    }
    num_significant_digit = offset;
    std::cout << "num_significant_digit: " << num_significant_digit << std::endl;

    val = swap(val);
    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";

    for (offset = 31 - num_significant_digit; tmp_fractpart != 0 && offset >= 0; offset--)
    {
        tmp_fractpart = tmp_fractpart * 2;
        result = int(tmp_fractpart);
        val = val | (result << offset);
        tmp_fractpart = tmp_fractpart - result;
        // std::cout << tmp_fractpart << std::endl;
        //  std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
    }

    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
}

uint32_t swap(uint32_t old_val)
{
    uint32_t new_val;
    int result;
    for (int i = 0; i <= 31; i++)
    {
        result = old_val & 1;
        old_val = old_val >> 1;
        // std::cout << std::bitset<sizeof(old_val) * CHAR_BIT>(old_val) << "\n";
        new_val = new_val | result;
        if (i != 31)
            new_val = new_val << 1;
    }
    // std::cout << std::bitset<sizeof(old_val) * CHAR_BIT>(old_val) << "\n";
    // std::cout << std::bitset<sizeof(new_val) * CHAR_BIT>(new_val) << "\n";
    return new_val;
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

    // std::cout << "Введите число: \n";
    // std::cin >> number.val;
    number.val = 0.25;

    split(&number);

    std::cout << number.val << " = "
              << number.intpart << " + "
              << number.fractpart << std::endl;
    std::cout << "Sign: " << number.sign << std::endl;
    printf("val = %.10f %f + %.10f\n", number.val, number.intpart, number.fractpart);
    convert_float_part_to_binary(&number);

    return 0;
}
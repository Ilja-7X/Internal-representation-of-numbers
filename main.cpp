#include <iostream>
#include <stdio.h>
#include <cmath>
#include <bitset>
#include <climits>
#include <stdlib.h>

uint32_t reverse(uint32_t old_val);

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

int convert_float_part_to_binary(struct NumberDescription *number)
{
    uint32_t val = 0;

    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";

    int tmp_intpart = int(number->intpart);
    double tmp_fractpart = number->fractpart;
    std::cout << "NUM = " << tmp_intpart << std::endl;

    int result;

    int size_intpart;
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
    size_intpart = offset;
    std::cout << "Intpart = " << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
    std::cout << "size_intpart: " << size_intpart << std::endl;
    if (size_intpart > 24)
    {
        std::cout << "Число превышает допустимый диапазон\n";
        return -1;
    }

    val = reverse(val);
    std::cout << "Before reverse: " << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";

    std::cout << "Start\n";

    for (offset = 31 - size_intpart; tmp_fractpart != 0 && offset >= 0; offset--)
    {
        tmp_fractpart = tmp_fractpart * 2;
        result = int(tmp_fractpart);
        val = val | (result << offset);
        tmp_fractpart = tmp_fractpart - result;
        std::cout << tmp_fractpart << std::endl;
        std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
    }

    std::cout << "End\n";

    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";

    /*int exponent = 0;
    int res = 0;

    if (size_intpart == 1)
    {
        for (int i = 31; i >= 0; i--)
        {
            res = val >> 31;
            val = val << 1;
            if (res == 1)
            {
                break;
                ;
            }
            else
            {
                exponent--;
            }
            std::cout << "--------";
            std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
            std::cout << "Exponent = " << exponent << "\n";

            std::cout << "--------";
        }
    }
    else if (size_intpart > 1)
    {
        exponent = size_intpart - 1;
        val = val << 1;
    }
    std::cout << "Exponent = " << exponent << "\n";
    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";*/
    return 0;
}

uint32_t reverse(uint32_t old_val) // меняем порядок битов с прямого на обратный
{
    uint32_t new_val = 0;
    int result;
    for (int i = 0; i <= 31; i++)
    {
        result = old_val & 1;
        old_val = old_val >> 1;
        new_val = new_val | result;
        if (i != 31)
            new_val = new_val << 1;
    }

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
    number.val = 0.125;

    split(&number);

    std::cout << number.val << " = "
              << number.intpart << " + "
              << number.fractpart << std::endl;
    std::cout << "Sign: " << number.sign << std::endl;
    printf("val = %.10f %f + %.10f\n", number.val, number.intpart, number.fractpart);
    convert_float_part_to_binary(&number);

    return 0;
}
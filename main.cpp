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

uint32_t convert_float_part_to_binary(struct NumberDescription *number)
{
    uint32_t val = 0;

    int tmp_intpart = int(number->intpart);
    double tmp_fractpart = number->fractpart;

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
    }
    size_intpart = offset;
    if (size_intpart > 24)
    {
        std::cout << "Число превышает допустимый диапазон\n";
        std::exit;
    }

    val = reverse(val);

    for (offset = 31 - size_intpart; tmp_fractpart != 0 && offset >= 0; offset--)
    {
        tmp_fractpart = tmp_fractpart * 2;
        result = int(tmp_fractpart);
        val = val | (result << offset);
        tmp_fractpart = tmp_fractpart - result;
    }

    number->size_intpart = size_intpart;
    return val;
}

void shift_point(struct NumberDescription *number, uint32_t *val)
{
    int exponent = 0;
    int res = 0;

    if (number->size_intpart == 1)
    {
        for (int i = 31; i >= 0; i--)
        {
            res = *val >> 31;
            *val = *val << 1;
            if (res == 1)
                break;
            else
                exponent--;
        }
    }
    else if (number->size_intpart > 1)
    {
        exponent = number->size_intpart - 1;
        *val = *val << 1;
    }
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
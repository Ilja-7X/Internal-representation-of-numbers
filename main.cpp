#include <iostream>
#include <stdio.h>
#include <cmath>
#include <bitset>
#include <climits>
#include <stdlib.h>

uint32_t reverse(uint32_t old_val);

struct NumberDescription
{
    double input_val = 0;
    bool sign;
    double intpart = 0;
    double fractpart = 0;
    int size_intpart = 0;
    int exponent = 0;
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
    if (number->input_val < 0)
    {
        number->sign = true;
    }
    else
        number->sign = false;

    number->fractpart = (modf(abs(number->input_val), &number->intpart)); // находим целую и дробную часть
}

uint32_t convert_float_part_to_binary(struct NumberDescription *number) // перевод числа в двоичный вид
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

void shift_point(struct NumberDescription *number, uint32_t *val) // смещаем точку
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

void concatenate_parts(struct NumberDescription *number) // знак + характеристика + мантисса
{
    if (number->sign == true)
    {
        number->bin_val = number->bin_val | (1 << 31);
    }
    // std::cout << std::bitset<sizeof(number->exponent) * CHAR_BIT>(number->exponent) << "\n";
    uint32_t characteristic = number->exponent + 127;
    // std::cout << "Characteristic = " << characteristic << std::endl;
    // std::cout << std::bitset<sizeof(characteristic) * CHAR_BIT>(characteristic) << "\n";

    // characteristic = characteristic << 30;

    // number->bin_val = number->bin_val | (characteristic << 30);
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

    std::cout << "Введите число: \n";
    std::cin >> number.input_val;

    split(&number);

    std::cout << number.input_val << " = "
              << number.intpart << " + "
              << number.fractpart << std::endl;
    printf("val = %.10f %f + %.10f\n", number.input_val, number.intpart, number.fractpart);

    uint32_t val1 = convert_float_part_to_binary(&number);
    // std::cout << std::bitset<sizeof(val1) * CHAR_BIT>(val1) << "\n";

    shift_point(&number, &val1);
    // std::cout << std::bitset<sizeof(val1) * CHAR_BIT>(val1) << "\n";

    concatenate_parts(&number);
    // std::cout << std::bitset<sizeof(number.bin_val) * CHAR_BIT>(number.bin_val) << "\n";
    return 0;
}
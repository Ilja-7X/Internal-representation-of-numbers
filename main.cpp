#include <iostream>
#include <stdio.h>
#include <cmath>
#include <bitset>
#include <climits>
#include <stdlib.h>

uint32_t reverse(uint32_t old_val);

struct NumberDescription // структура, поля которой будут заполняться в течеине работы программы
{
    double input_val = 0; // вводимое число
    bool sign;            // знак числа (true - отриц, false - полож)
    double intpart = 0;   // целая часть
    double fractpart = 0; // вещественная часть
    int size_intpart = 0; // кол-во разрядов в целой части
    int exponent = 0;     // экспонента числа
    uint32_t bin_val = 0; // двоичное представлине числа
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

void split(struct NumberDescription *number) // разбиение числа на знак, цулую и вещественную части
{
    if (number->input_val < 0) // если введёное число меньше 0
    {
        number->sign = true; // устанавливаем флаг отрицательности
    }
    else
        number->sign = false; // иначе флаг остаётся нулевым, что значит число положительное

    number->fractpart = (modf(abs(number->input_val), &number->intpart)); // выделяем целую и дробную часть при помощи функции modf
}

uint32_t convert_float_part_to_binary(struct NumberDescription *number) // перевод вещественного числа в двоичный вид
{
    uint32_t val = 0;

    int tmp_intpart = int(number->intpart);   // локальня переменнная с целой частью
    double tmp_fractpart = number->fractpart; // локальня переменнная с вещественной частью

    int result;

    int size_intpart;
    int offset = 0;

    if (tmp_intpart == 0) // если целая часть содержит только один 0, т.е. 0.
    {
        val = val << 1;
        offset = 1;
    }

    for (; tmp_intpart > 0 && offset <= 31; offset++) // в цикле осуществяется перевод целой части в двоичный вид
    {
        result = tmp_intpart % 2;
        val = (val << 1) | result;
        tmp_intpart = tmp_intpart / 2;
    }
    size_intpart = offset;
    if (size_intpart > 24) // проверка целой части на допустимое значение (не должно превышать 24 бита)
    {
        std::cout << "Число превышает допустимый диапазон\n";
        std::exit;
    }

    val = reverse(val); // после перевода необходимо зеркально отрбразить набор полученных бит

    for (offset = 31 - size_intpart; tmp_fractpart != 0 && offset >= 0; offset--) // в цикле осуществяется перевод второй части - вещественнной - в двоичный вид
    {
        tmp_fractpart = tmp_fractpart * 2;
        result = int(tmp_fractpart);
        val = val | (result << offset);
        tmp_fractpart = tmp_fractpart - result;
    }

    number->size_intpart = size_intpart; // присваивание значения лок переменной полю структуры
    return val;
}

void shift_point(struct NumberDescription *number, uint32_t *val) // нормализация числа путем сдвина точки и увеличения/уменьшения соответственно экспоненты
{
    int exponent = 0;
    int res = 0;

    if (number->size_intpart == 1) // если целая часть состоит из одного бита
    {
        for (int i = 31; i >= 0; i--)
        {
            res = *val >> 31; // вытаскиваем первый бит числа
            *val = *val << 1; // делаем сдвиг на один бит влево
            if (res == 1)     // если "вытащенный" бит единица цикла for прекращаем, т.к. число уже нормализоно
                break;
            else            //
                exponent--; // если "вытащенный" бит не единица, уменьшаем значение экспоненты и продолжаем цикл for
        }
    }
    else if (number->size_intpart > 1) // если целая часть состоит из нескольких бит
    {
        exponent = number->size_intpart - 1; // значение экспоненты
        *val = *val << 1;                    // удаляём 24-ый бит мантиссы, который подразумевается по умолчанию
    }
    number->exponent = exponent;
    *val = *val >> 9;
}

void concatenate_parts(struct NumberDescription *number, uint32_t val) // собираем воедино: знак + характеристика + мантисса
{
    if (number->sign == true) // если число отрицательное
    {
        number->bin_val = number->bin_val | (1 << 31); // выставляем первый бит в единицу
    }

    uint32_t characteristic = number->exponent + 127; // для записи характеристки к экспонение нужно прибавить 127

    characteristic = characteristic << 23; // смещаем характеристику на 23 бита левее,

    number->bin_val = number->bin_val | characteristic; // и складываем с результирующим значение, тем самым накладываем характеристику помиио знака

    number->bin_val = number->bin_val >> 23; // два выражения для очистки крайних правых бит под мантиссу от муссора (на всякий случай)
    number->bin_val = number->bin_val << 23;

    number->bin_val = number->bin_val | val; // накладывание на результирующее значение также и мантиссы
}

uint32_t reverse(uint32_t old_val) // зеркально отображаем порядок бит
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

void setSign(uint32_t *val) // о
{
    *val |= (1 << 31);
}

uint32_t getSign(uint32_t val) // выделение знака числа
{
    return val >> 31;
}

void convert_int_part_to_binary(struct NumberDescription *number) // представление целого числа в двоичном виде
{
    uint32_t val = 0;
    int result, count = 0;
    int tmp_intpart = int(number->intpart);
    std::cout << "answer: " << tmp_intpart << std::endl;

    while (tmp_intpart != 0) // перевод в двоичное число
    {
        result = tmp_intpart % 2;
        val = (val << 1) | result;
        tmp_intpart = tmp_intpart / 2;
        count++;
    }

    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";

    val = reverse(val); // зеркальное отображение
    val = val >> 32 - count;

    if (number->sign == true) // если число было введено отрицательным, то переводим его в дополнитльный код
    {
        val = ~val; // делаем инверсию
        std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";

        val = (1 << 31); // старший бит,отвечающий за знак выставляем вединицу
        std::cout << val << std::endl;

        val = val + 1; // прибавляем единицу
        std::cout << val << std::endl;
        std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
    }
    std::cout << std::bitset<sizeof(val) * CHAR_BIT>(val) << "\n";
}

int main()
{
    struct NumberDescription number; // созда1м структуру, в которой будем хранить полезныю инфу

    std::cout << "Введите число: \n";
    std::cin >> number.input_val;

    split(&number); // разделине числа на части: знак, целую и вещественную части

    if (number.fractpart == 0) // в случае, когда вещественная чать равна 0 => введено было целое
    {
        convert_int_part_to_binary(&number); // представление числа в двоичном виде
    }
    else // // в случае, когда вещественная чать равна не 0 => введено вещественное число
    {
        uint32_t val1 = convert_float_part_to_binary(&number); // представление числа в двоичном виде (промежуточый вариант)

        shift_point(&number, &val1); // нормализация числа (сдвин точки и измение значения экспоненты)

        concatenate_parts(&number, val1); // объединение трёх частей: знака, характеристики и мантиссы
    }

    std::cout << std::bitset<sizeof(number.bin_val) * CHAR_BIT>(number.bin_val) << "\n"; // отображение двочиного представления числа

    return 0;
}
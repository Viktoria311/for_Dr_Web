#include "dictionary.h"
#include <iostream>


int main()
{
    my_map<int, char> d;

    d.set(20, 'j');
    d.set(10, 'g');
    d.set(40, 'h');
    d.set(5, 'z');
    d.set(15, 'd');
    d.set(25, 's');
    d.set(50, 'j');
    d.set(2, 'g');
    d.set(6, 'k');
    d.set(13, 'z');
    d.set(17, 'd');
    d.set(22, 'c');
    d.set(30, 'j');
    d.set(47, 'g');
    d.set(70, 'h');
    d.set(69, 'z');
    d.set(80, 'd');
    d.set(21, 's');
    d.set(24, 'j');
    d.set(14, 'g');
    d.set(3, 'k');

    d.print();
    auto v = std::move(d);
    std::cout << "d" << std::endl;
    d.print();
    std::cout << "v" << std::endl;
    v.print();

    std::cout << v.get(7) << std::endl;

    return 0;
}

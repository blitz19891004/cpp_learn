#include <iostream>
#include "Person.h"
#include "SmallInline.h"

int main() {
    Person p("Alice", 30);
    std::cout << p.toString() << std::endl;

    p.setAge(31);
    std::cout << "Age: " << p.getAge() << std::endl;

    SmallInline s(5);
    s.add(3);
    std::cout << "SmallInline value: " << s.get() << std::endl;

    return 0;
}

#include "Person.h"
#include <sstream>

Person::Person(std::string name, int age)
    : name_(std::move(name)), age_(age) {}

void Person::setName(const std::string& name) {
    name_ = name;
}

void Person::setAge(int age) {
    age_ = age;
}

const std::string& Person::getName() const {
    return name_;
}

int Person::getAge() const {
    return age_;
}

std::string Person::toString() const {
    std::ostringstream oss;
    oss << name_ << " (" << age_ << ")";
    return oss.str();
}

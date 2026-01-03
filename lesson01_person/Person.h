#pragma once

#include <string>

class Person {
public:
    // 构造与默认析构
    Person(std::string name, int age);
    ~Person() = default;

    // 使用默认拷贝/赋值
    Person(const Person&) = default;
    Person& operator=(const Person&) = default;

    // 接口
    void setName(const std::string& name);
    void setAge(int age);
    const std::string& getName() const;
    int getAge() const;
    std::string toString() const;

private:
    std::string name_;
    int age_;
};

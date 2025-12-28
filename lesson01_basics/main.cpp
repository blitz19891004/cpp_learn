#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

void basicTypes() {
    int num = 100;
    float f = 3.14f;
    double d = 3.14159;
    char c = 'A';
    bool flag = true;
    
    std::cout << "整数: " << num << std::endl;
    std::cout << "浮点: " << f << std::endl;
    std::cout << "字符: " << c << std::endl;
    std::cout << "布尔: " << flag << std::endl;
}

int add(int a, int b) {
    return a + b;
}

void pointerDemo() {
    int value = 42;
    int* ptr = &value;
    
    std::cout << "值: " << value << std::endl;
    std::cout << "指针地址: " << ptr << std::endl;
    std::cout << "指针指向的值: " << *ptr << std::endl;
    
    *ptr = 100;
    std::cout << "修改后值: " << value << std::endl;
}

void referenceDemo() {
    int value = 42;
    int& ref = value;
    
    std::cout << "值: " << value << std::endl;
    ref = 100;
    std::cout << "通过引用修改后: " << value << std::endl;
}

class Calculator {
private:
    int result;
    
public:
    Calculator() : result(0) {}
    
    void add(int num) {
        result += num;
    }
    
    void subtract(int num) {
        result -= num;
    }
    
    int getResult() const {
        return result;
    }
};

int main() {
    setupConsole();
    std::cout << "=== 第1课：C++基础语法 ===" << std::endl;
    
    std::cout << "\n1. 基本数据类型:" << std::endl;
    basicTypes();
    
    std::cout << "\n2. 函数调用:" << std::endl;
    int sum = add(10, 20);
    std::cout << "10 + 20 = " << sum << std::endl;
    
    std::cout << "\n3. 指针:" << std::endl;
    pointerDemo();
    
    std::cout << "\n4. 引用:" << std::endl;
    referenceDemo();
    
    std::cout << "\n5. 类和对象:" << std::endl;
    Calculator calc;
    calc.add(10);
    calc.add(20);
    calc.subtract(5);
    std::cout << "计算结果: " << calc.getResult() << std::endl;
    
    return 0;
}


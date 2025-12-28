#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

void vectorDemo() {
    std::vector<int> numbers;
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);
    
    std::cout << "Vector元素: ";
    for (size_t i = 0; i < numbers.size(); i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "使用迭代器: ";
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "范围for循环: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

void stringDemo() {
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string result = str1 + " " + str2;
    
    std::cout << "字符串: " << result << std::endl;
    std::cout << "长度: " << result.length() << std::endl;
    std::cout << "子串: " << result.substr(0, 5) << std::endl;
    
    if (result.find("World") != std::string::npos) {
        std::cout << "包含'World'" << std::endl;
    }
}

void mapDemo() {
    std::map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    
    std::cout << "Map内容:" << std::endl;
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    
    if (scores.find("Bob") != scores.end()) {
        std::cout << "Bob的分数: " << scores["Bob"] << std::endl;
    }
}

void algorithmDemo() {
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};
    
    std::cout << "原始数据: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;
    
    std::sort(nums.begin(), nums.end());
    std::cout << "排序后: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;
    
    auto it = std::find(nums.begin(), nums.end(), 8);
    if (it != nums.end()) {
        std::cout << "找到8，位置: " << (it - nums.begin()) << std::endl;
    }
}

int main() {
    std::cout << "=== 第2课：STL容器和字符串处理 ===" << std::endl;
    
    std::cout << "\n1. Vector容器:" << std::endl;
    vectorDemo();
    
    std::cout << "\n2. String字符串:" << std::endl;
    stringDemo();
    
    std::cout << "\n3. Map映射:" << std::endl;
    mapDemo();
    
    std::cout << "\n4. 算法库:" << std::endl;
    algorithmDemo();
    
    return 0;
}


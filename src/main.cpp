#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class Greeter {
public:
    explicit Greeter(std::string name) : name_(std::move(name)) {}
    void greet() const { std::cout << "Hello, " << name_ << "!\n"; }
private:
    std::string name_;
};

int main() {
    Greeter g("C++ Demo");
    g.greet();

    std::vector<int> v{4, 1, 3, 2};
    std::sort(v.begin(), v.end());
    std::cout << "Sorted:";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';

    return 0;
}

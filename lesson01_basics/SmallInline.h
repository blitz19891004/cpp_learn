#pragma once

class SmallInline {
public:
    explicit SmallInline(int v = 0) : value_(v) {}
    void add(int v) { value_ += v; }           // inline 简短实现
    int get() const { return value_; }
private:
    int value_;
};

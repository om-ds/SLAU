#pragma once
#include <iostream>
#include <vector>

template<typename T>

class Matrix
{
private:
    int rows;
    int cols;
    std::vector<std::vector<T>> data;

public:
    Matrix(int r, int c);
    void input();
    void print();
    T get(int i, int j);
    T set(T value, int i, int j);
    Matrix operator+ (Matrix& other);
    Matrix operator- (Matrix& other);
};

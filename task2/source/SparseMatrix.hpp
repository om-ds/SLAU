#pragma once
#include <iostream>
#include <vector>
#include <map>

template<typename T>

class SparseMatrix
{
private:
    int rows;
    int cols;
    std::map<std::pair<int, int>, T> data;
public:
    SparseMatrix(int r, int c) : rows(r), cols(c) {}
    void set(int i, int j, T value);
    T get(int i, int j);
    void print();
    SparseMatrix operator+ (SparseMatrix& other);
    std::vector<T> operator* (std::vector<T>& v);
}

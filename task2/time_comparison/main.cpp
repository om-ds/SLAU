#include "../source/Matrix.hpp"
#include "../source/SparseMatrix.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <random>

int getRandomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}


int main()
{
    std::ofstream out1;
    out1.open("mostly_zeros_usual.txt");
    std::ofstream out2;
    out2.open("mostly_zeros_sparse.txt");

    for (int i = 1000; i <= 5000; i += 500)
    {
        Matrix<int> A(i, i);
        SparseMatrix<int> B(i, i);
        std::vector<int> v(i, 0);

        for (int y = 0; y < v.size(); y++)
        {
            if (y % 10 == 0)
            {
                v[y] = 1;
            }
        }

        for (int j = 0; j < i * i / 20; j++)
        {
            A.set(getRandomInt(0, i - 1), getRandomInt(0, i - 1), 1);
            B.set(getRandomInt(0, i - 1), getRandomInt(0, i - 1), 1);
        }

        auto s1 = std::chrono::high_resolution_clock::now();
        std::vector<int> ans1 = A * v;
        auto f1 = std::chrono::high_resolution_clock::now();
        auto d1 = std::chrono::duration_cast<std::chrono::nanoseconds>(f1 - s1);
        out1 << d1.count() << " ";

        auto s2 = std::chrono::high_resolution_clock::now();
        std::vector<int> ans2 = B * v;
        auto f2 = std::chrono::high_resolution_clock::now();
        auto d2 = std::chrono::duration_cast<std::chrono::nanoseconds>(f2 - s2);
        out2 << d2.count() << " ";

        std::cout << i << std::endl;
    }

    out1.close();
    out2.close();
}

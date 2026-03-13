#ifndef TRIDIAGMATRIX_HPP
#define TRIDIAGMATRIX_HPP

#include <iostream>
#include <array>
using namespace std;

template<typename T, size_t N>
class tridiagMatrix
{
public:
    array<T, N - 1> a;
    array<T, N> b;
    array<T, N - 1> c;

    tridiagMatrix(array<T, N - 1> lower, array<T, N> main, array<T, N - 1> upper)
    {
        a = lower;
        b = main;
        c = upper;
    }

    T get(size_t i, size_t j)
    {
        if (i == 0 and j >= 0 and j < a.size())
        {
            return a[j];
        }

        else if (i == 1 and j >= 0 and j < b.size())
        {
            return b[j];
        }

        else if (i == 2 and j >= 0 and j < c.size())
        {
            return c[j];
        }

        else
        {
            cout << "No such element." << endl;
            return 0;
        }
    }

    void print()
    {
        if (a.size() != 0 )
        {
            cout << "Lower: " << a[0];
            for (int i = 1; i < a.size(); i++)
            {
                cout << "; " << a[i];
            }
            cout << endl;

            cout << "Main: " << b[0];
            for (int i = 1; i < b.size(); i++)
            {
                cout << "; " << b[i];
            }
            cout << endl;

            cout << "Upper: " << c[0];
            for (int i = 1; i < c.size(); i++)
            {
                cout << "; " << c[i];
            }
            cout << endl;
        }

        else
        {
            cout << "Main: " << b[0] << endl;
        }
    }

    array<T, N> ThomasAlgorithm(array<T, N> d)
    {
        array<T, N> result;

        for (size_t i = 0; i < N; i++)
        {
            T sum = 0;

            if (i > 0)
            {
                sum += abs(a[i - 1]);
            }

            if (i < N-1)
            {
                sum += abs(c[i]);
            }

            if (abs(b[i]) < sum)
            {
                cerr << "Incorrect matrix! Answer can be wrong." << endl;
            }
        }

        array<T, N - 1> p;
        array<T, N> q;

        p[0] = - c[0] / b[0];
        q[0] = d[0] / b[0];

        for (size_t i = 1; i < N; i++)
        {
            T denominator = b[i];

            if (i > 0)
            {
                denominator += a[i - 1] * p[i - 1];
            }

            if (i < N - 1)
            {
                p[i] = -c[i] / denominator;
            }

            q[i] = (d[i] - (i > 0 ? a[i - 1] * q[i - 1] : 0)) / denominator;
        }

        result[N - 1] = q[N - 1];

        for (int i = N - 2; i >= 0; i--) {
            result[i] = p[i] * result[i + 1] + q[i];
        }

        return result;
    }
};

bool testThomasAlgorithm()
{
    array<int, 3> L = {2, 3, 4};
    array<int, 4> M = {4, 5, 6, 7};
    array<int, 3> U = {1, 1, 1};
    array<int, 4> D = {5, 8, 10, 11};

    tridiagMatrix<int, 4> matrix(L, M, U);
    array<int, 4> result = matrix.ThomasAlgorithm(D);

    array<int, 4> answer = {1, 1, 1, 1};

    if (result == answer)
    {
        cout << "ThomasAlgorithm works." << endl;
        return true;
    }

    return false;
}

bool testGet()
{
    array<int, 3> L = {1, 2, 3};
    array<int, 4> M = {4, 5, 6, 7};
    array<int, 3> U = {8, 9, 10};

    tridiagMatrix<int, 4> matrix(L, M, U);

    if (matrix.get(0, 0) == 1 and matrix.get(1, 2) == 6)
    {
        cout << ".get() works." << endl;
        return true;
    }

    return false;
}

void testPrint()
{
    array<int, 3> L = {2, 3, 4};
    array<int, 4> M = {4, 5, 6, 7};
    array<int, 3> U = {1, 1, 1};

    tridiagMatrix<int, 4> matrix(L, M, U);

    cout << "Testing print():" << endl;
    matrix.print();
    cout << endl;
}

void test()
{
    cout << "Test:" << endl;

    testPrint();

    if (testGet() == true and testThomasAlgorithm() == true)
    {
        cout << "Everything works." << endl;
    }
}

#endif

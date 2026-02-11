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
        if (i >= N || j >= N)
        {
            throw out_of_range("Too big index");
        }

        if (i == j)
        {
            return b[i];
        }

        else if (i == j + 1)
        {
            return a[j];
        }

        else if (i + 1 == j)
        {
            return c[i];
        }

        else
        {
            return 0;
        }
    }

    array<T, N> ThomasMethod(array<T, N> d)
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
                cerr << "Incorrect matrix!";
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


int main()
{
    array<int, 3> L = {2, 3, 4};
    array<int, 4> M = {4, 5, 6, 7};
    array<int, 3> U = {1, 1, 1};
    array<int, 4> D = {5, 8, 10, 11};

    tridiagMatrix<int, 4> test(L, M, U);

    array<int, 4> ans = test.ThomasMethod(D);

    for (int i = 0; i < ans.size(); i++)
    {
        cout << ans[i] << " ";
    }
    cout << endl;
}

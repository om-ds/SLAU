#include "./tridiagMatrix.cpp"
#include <iostream>
using namespace std;

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

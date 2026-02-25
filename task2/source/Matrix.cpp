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
    Matrix(int r, int c) : rows(r), cols(c)
    {
        data.assign(rows, std::vector<T>(cols, T(0)));
    }

    void input()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                std::cin >> data[i][j];
            }
        }
    }

    void print()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    T get(int i, int j)
    {
        return data[i][j];
    }

    T set(T value, int i, int j)
    {
        data[i][j] = value;
    }

    Matrix operator+ (Matrix& other)
    {
        if (rows != other.rows or cols != other.cols)
        {
            throw std::invalid_argument("Different sizes!");
        }

        Matrix result(rows, cols);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }

        return result;
    }

    Matrix operator- (Matrix& other)
    {
        if (rows != other.rows or cols != other.cols)
        {
            throw std::invalid_argument("Different sizes!");
        }

        Matrix result(rows, cols);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }

        return result;
    }
};

int main()
{
    Matrix<double> A(3, 3);
    Matrix<double> B(3, 3);
    A.input();
    B.input();
    Matrix<double> C = A + B;
    C.print();
}

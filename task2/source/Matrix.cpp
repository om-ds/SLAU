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

    T set(int i, int j, T value)
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
            throw std::invalid_argument("Wrong sizes!");
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

    std::vector<T> operator* (std::vector<T>& v)
    {
        if (v.size() != cols)
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        std::vector<T> result(rows, 0);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result[i] += data[i][j] * v[j];
            }
        }

        return result;
    }
};

template<typename T>

T operator* (std::vector<T>& a, std::vector<T>& b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Wrong sizes!");
    }

    T result = 0;

    for (int i = 0; i < a.size(); i++)
    {
        result += a[i] * b[i];
    }

    return result;
}

template<typename T>

std::vector<T> operator+ (std::vector<T>& a, std::vector<T>& b)
{
    if (a.size() != b.size())
    {
        throw std::invalid_argument("Wrong sizes!");
    }

    std::vector<T> result(a.size(), 0);

    for (int i = 0; i < a.size(); i++)
    {
        result[i] = a[i] + b[i];
    }

    return result;
}

template<typename T>

std::vector<T> operator* (std::vector<T>& v, T value)
{
    for (int i = 0; i < v.size(); i++)
    {
        std::vector<T> result(v.size(), 0);

        for (int i = 0; i < v.size(); i++)
        {
            result[i] = v[i] * value;
        }

        return result;
    }
}


/*int main()
{
    Matrix<double> A(3, 2);
    A.input();
    std::vector<double> v = {1, 2};
    std::vector<double> ans = A * v;
}*/

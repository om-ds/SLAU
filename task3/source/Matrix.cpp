#include <iostream>
#include <vector>
#include <cmath>

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

    void set(int i, int j, T value)
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

    Matrix<T> operator* (Matrix<T>& other)
    {
        if (cols != other.rows)
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        Matrix<T> result(rows, other.cols);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < other.cols; j++)
            {
                for (int k = 0; k < cols; k++)
                {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }

        return result;
    }

    std::pair<Matrix<T>, Matrix<T>> qr()
    {
        if (rows < cols)
        {
            throw std::invalid_argument("Wrong sizes!!");
        }

        Matrix<T> R(rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                R.data[i][j] = data[i][j];
            }
        }

        Matrix<T> Q(rows, rows);
        for (int i = 0; i < rows; i++)
        {
            Q.data[i][i] = T(1);
        }

        int min_dim;
        if (rows < cols)
        {
            min_dim = rows;
        }
        else
        {
            min_dim = cols;
        }

        for (int k = 0; k < min_dim; k++)
        {
            T norm = 0;
            for (int i = k; i < rows; i++)
            {
                norm += R.data[i][k] * R.data[i][k];
            }
            norm = std::sqrt(norm);

            if (norm == T(0))
            {
                continue;
            }

            T sign = (R.data[k][k] >= 0) ? T(1) : T(-1);
            T alpha = -sign * norm;

            std::vector<T> v(rows, 0);
            v[k] = R.data[k][k] - alpha;
            for (int i = k + 1; i < rows; i++)
            {
                v[i] = R.data[i][k];
            }

            T v_norm = 0;
            for (int i = 0; i < rows; i++)
            {
                v_norm += v[i] * v[i];
            }
            v_norm = std::sqrt(v_norm);

            if (v_norm == T(0))
            {
                continue;
            }

            for (int i = 0; i < rows; i++)
            {
                v[i] = v[i] / v_norm;
            }

            for (int j = k; j < cols; j++)
            {
                T dot = 0;
                for (int i = 0; i < rows; i++)
                {
                    dot += v[i] * R.data[i][j];
                }
                for (int i = 0; i < rows; i++)
                {
                    R.data[i][j] = R.data[i][j] - 2 * v[i] * dot;
                }
            }

            for (int i = 0; i < rows; i++)
            {
                T dot = 0;
                for (int j = 0; j < rows; j++)
                {
                    dot += Q.data[i][j] * v[j];
                }
                for (int j = 0; j < rows; j++)
                {
                    Q.data[i][j] = Q.data[i][j] - 2 * dot * v[j];
                }
            }
        }

        return std::make_pair(Q, R);
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
    std::vector<T> result(v.size(), 0);

    for (int i = 0; i < v.size(); i++)
    {
        result[i] = v[i] * value;
    }

    return result;
}


/*int main()
{
    Matrix<double> A(3, 2);
    A.input();
    std::vector<double> v = {1, 2};
    std::vector<double> ans = A * v;
}*/

#include <iostream>
#include <vector>
#include "VectorOperations.hpp"

template<typename T>

class SparseMatrix
{
public:
    int rows;
    int cols;
    std::vector<T> values;
    std::vector<int> col_indices;
    std::vector<int> row_ptr;

    int findInRow(int row, int col) const
    {
        int start = row_ptr[row];
        int end = row_ptr[row + 1];

        for (int k = start; k < end; k++)
        {
            if (col_indices[k] == col)
            {
                return k;
            }
        }
        return -1;
    }

    int findInsertPosition(int row, int col) const
    {
        int start = row_ptr[row];
        int end = row_ptr[row + 1];

        for (int k = start; k < end; k++)
        {
            if (col_indices[k] >= col)
            {
                return k;
            }
        }
        return end;
    }

    void insertSorted(int row, int col, T value)
    {
        if (value == T(0))
        {
            int position = findInRow(row, col);
            if (position != -1)
            {
                values.erase(values.begin() + position);
                col_indices.erase(col_indices.begin() + position);
                for (int r = row + 1; r <= rows; r++)
                    row_ptr[r]--;
            }
        }

        int position = findInRow(row, col);
        if (position != -1)
        {
            values[position] = value;
        }
        else
        {
            int insert_position = findInsertPosition(row, col);

            values.insert(values.begin() + insert_position, value);
            col_indices.insert(col_indices.begin() + insert_position, col);

            for (int r = row + 1; r <= rows; r++)
                row_ptr[r]++;
        }
    }

    SparseMatrix(int r, int c) : rows(r), cols(c)
    {
        row_ptr.resize(rows + 1, 0);
    }

    void set(int i, int j, T value)
    {
        if (i < 0 or i >= rows or j < 0 or j >= cols)
            throw std::out_of_range("Wrong index!");
        insertSorted(i, j, value);
    }

    T get(int i, int j)
    {
        if (i < 0 or i >= rows or j < 0 or j >= cols)
            throw std::out_of_range("Wrong index!");

        int position = findInRow(i, j);
        return (position != -1) ? values[position] : T(0);
    }

    void print()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                std::cout << get(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    SparseMatrix operator+ (SparseMatrix& other)
    {
        if (rows != other.rows or cols != other.cols)
        {
            throw std::invalid_argument("Wrong sizes!");
        }
        SparseMatrix<T> result(rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                T val = get(i, j) + other.get(i, j);
                if (val != T(0))
                    result.set(i, j, val);
            }
        }
        return result;
    }

    std::vector<T> operator* (std::vector<T>& v)
    {
        if (static_cast<int>(v.size()) != cols)
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        std::vector<T> result(rows, T(0));

        for (int i = 0; i < rows; i++)
        {
            int start = row_ptr[i];
            int end = row_ptr[i + 1];
            for (int k = start; k < end; k++)
            {
                int j = col_indices[k];
                T val = values[k];
                result[i] += val * v[j];
            }
        }
        return result;
    }

    std::vector<T> solveSimpleIteration(std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6))
    {
        if ((rows != cols) or (b.size() != rows))
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        T maxDiag = T(0);
        for (int i = 0; i < rows; ++i)
        {
            T diag = std::abs(get(i, i));
            if (diag > maxDiag) maxDiag = diag;
        }
        T tau = T(1) / maxDiag;

        std::vector<T> x(rows, T(0));

        for (int iter = 0; iter < maxIterations; iter++)
        {
            std::vector<T> Ax = (*this) * x;
            std::vector<T> r = b - Ax;

            std::vector<T> x_new = x + r * tau;

            T diff = T(0);
            for (int i = 0; i < rows; i++)
            {
                diff += std::abs(x_new[i] - x[i]);
            }
            x = x_new;
            if (diff < tolerance)
            {
                break;
            }
        }
        return x;
    }

    std::vector<T> solveJacobi(std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6))
    {
        if ((rows != cols) or (b.size() != rows))
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        std::vector<T> x(rows, T(0));
        std::vector<T> x_new(rows, T(0));

        for (int iter = 0; iter < maxIterations; iter++)
        {
            for (int i = 0; i < rows; i++)
            {
                x_new[i] = b[i];
                int start = row_ptr[i];
                int end = row_ptr[i + 1];
                for (int k = start; k < end; k++)
                {
                    int j = col_indices[k];
                    T val = values[k];
                    if (j != i)
                    {
                        x_new[i] -= val * x[j];
                    }
                }

                T diag = get(i, i);
                if (diag == T(0))
                {
                    throw std::runtime_error("Zero on diagonal!");
                }
                x_new[i] /= diag;
            }

            T diff = T(0);
            for (int i = 0; i < rows; i++)
            {
                diff += std::abs(x_new[i] - x[i]);
            }

            x = x_new;
            if (diff < tolerance)
            {
                break;
            }
        }

        return x;
    }

    std::vector<T> solveGaussSeidel(std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6))
    {
        if ((rows != cols) or (b.size() != rows))
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        std::vector<T> x(rows, T(0));
        std::vector<T> x_old = x;

        for (int iter = 0; iter < maxIterations; iter++)
        {
            x_old = x;

            for (int i = 0; i < rows; i++)
            {
                T sum = b[i];
                int start = row_ptr[i];
                int end = row_ptr[i + 1];
                for (int k = start; k < end; k++)
                {
                    int j = col_indices[k];
                    T val = values[k];
                    if (j != i)
                    {
                        sum -= val * x[j];
                    }
                }

                T diag = get(i, i);
                if (diag == T(0))
                {
                    throw std::runtime_error("Zero on diagonal!");
                }
                x[i] = sum / diag;
            }

            T diff = T(0);
            for (int i = 0; i < rows; i++)
            {
                diff += std::abs(x[i] - x_old[i]);
            }

            if (diff < tolerance)
            {
                break;
            }
        }

        return x;
    }

    std::vector<T> solveSimpleIterationChebyshev(std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6))
    {
        if ((rows != cols) or (b.size() != rows))
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        std::vector<T> v(rows, T(1));
        T norm = T(0);

        for (int i = 0; i < rows; i++)
        {
            norm += v[i] * v[i];
        }
        norm = std::sqrt(norm);

        for (int i = 0; i < rows; i++)
        {
            v[i] /= norm;
        }
        T maxEigen = T(0);

        for (int iter = 0; iter < 50; iter++)
        {
            std::vector<T> Av = (*this) * v;
            maxEigen = T(0);

            for (int i = 0; i < rows; i++)
            {
                maxEigen += v[i] * Av[i];
            }
            norm = T(0);

            for (int i = 0; i < rows; i++)
            {
                norm += Av[i] * Av[i];
                }
            norm = std::sqrt(norm);

            if (norm == T(0))
            {
                throw std::runtime_error("Zero norm in power method!");
            }

            for (int i = 0; i < rows; i++)
            {
                v[i] = Av[i] / norm;
            }
        }

        T minEigen = maxEigen / T(10);
        if (minEigen <= T(0))
        {
            minEigen = maxEigen / T(100);
        }

        int n = maxIterations;
        std::vector<int> perm = {0, 1};
        int m = 2;
        while (m < n)
        {
            std::vector<int> new_perm;
            for (int idx : perm)
            {
                new_perm.push_back(idx);
                new_perm.push_back(2 * m - 1 - idx);
            }
            perm = new_perm;
            m *= 2;
        }

        std::vector<T> x(rows, T(0));
        T theta = T(3.1415) / T(n);
        T center = (maxEigen + minEigen) / T(2);
        T halfDiff = (maxEigen - minEigen) / T(2);

        for (int cycle = 0; cycle < 10; cycle++)
        {
            for (int iter = 0; iter < n; iter++)
            {
                int k = perm[iter % perm.size()];
                T t_k = std::cos(theta * (T(2) * k + T(1)) / T(2));
                T t_prime = center + halfDiff * t_k;
                T tau = T(1) / t_prime;

                std::vector<T> Ax = (*this) * x;
                std::vector<T> r = b - Ax;
                for (int i = 0; i < rows; i++)
                {
                    x[i] += tau * r[i];
                }
            }

            T diff = T(0);
            std::vector<T> Ax = (*this) * x;
            for (int i = 0; i < rows; i++)
            {
                diff += std::abs(b[i] - Ax[i]);
            }
            if (diff < tolerance)
            {
                break;
            }
        }

        return x;
    }
};

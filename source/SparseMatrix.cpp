#include <iostream>
#include <vector>
#include "VectorOperations.hpp"

template<typename T>

class SparseMatrix
{
private:
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

public:
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
};

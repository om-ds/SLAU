#include <iostream>
#include <vector>
#include <cmath>
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

    T get(int i, int j) const
    {
        if (i < 0 or i >= rows or j < 0 or j >= cols)
            throw std::out_of_range("Wrong index!");
        int position = findInRow(i, j);
        return (position != -1) ? values[position] : T(0);
    }

    void print() const
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

    SparseMatrix operator+ (const SparseMatrix& other) const
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

    std::vector<T> operator* (const std::vector<T>& v) const
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

    std::vector<T> solveSimpleIteration(const std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6)) const
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

    std::vector<T> solveJacobi(const std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6)) const
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

    std::vector<T> solveGaussSeidel(const std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6)) const
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

    std::vector<T> solveSimpleIterationChebyshev(const std::vector<T>& b, int maxIterations = 1000, T tolerance = T(1e-6), int n_roots = 16) const
    {
        if ((rows != cols) or (b.size() != rows))
        {
            throw std::invalid_argument("Wrong sizes!");
        }

        std::vector<T> x_eig(rows, T(1));
        T lambda_max = T(0);
        T lambda_prev = T(0);
        T pi = T(3.1415926535);

        for (int iter = 0; iter < 100; ++iter)
        {
            std::vector<T> Ax = (*this) * x_eig;
            T numerator = T(0);
            T denominator = T(0);
            for (int i = 0; i < rows; ++i)
            {
                numerator += x_eig[i] * Ax[i];
                denominator += x_eig[i] * x_eig[i];
            }
            if (denominator > T(0))
            {
                lambda_prev = lambda_max;
                lambda_max = numerator / denominator;
            }
            T norm = T(0);
            for (int i = 0; i < rows; ++i)
            {
                norm += Ax[i] * Ax[i];
            }
            norm = std::sqrt(norm);
            if (norm < T(1e-15))
                break;
            for (int i = 0; i < rows; ++i)
            {
                x_eig[i] = Ax[i] / norm;
            }
            if (std::abs(lambda_max - lambda_prev) < T(1e-10))
                break;
        }

        T lambda_min = lambda_max * T(0.012);
        std::vector<T> x(rows, T(0));
        int total_iterations = 0;



        for (int cycle = 0; cycle < maxIterations / n_roots; ++cycle)
        {
            std::vector<T> cheb_roots(n_roots);

            T theta_0 = pi / T(2 * n_roots);
            T delta = pi / T(n_roots);

            T cos_theta = std::cos(theta_0);
            T sin_theta = std::sin(theta_0);
            T cos_delta = std::cos(delta);
            T sin_delta = std::sin(delta);

            for (int k = 0; k < n_roots; ++k)
            {
                cheb_roots[k] = T(0.5) * (lambda_max + lambda_min) +
                               T(0.5) * (lambda_max - lambda_min) * cos_theta;

                if (k < n_roots - 1)
                {
                    T new_cos = cos_theta * cos_delta - sin_theta * sin_delta;
                    T new_sin = sin_theta * cos_delta + cos_theta * sin_delta;
                    cos_theta = new_cos;
                    sin_theta = new_sin;
                }
            }

            std::vector<T> permuted_roots(n_roots);
            int left = 0, right = n_roots - 1;
            for (int i = 0; i < n_roots; ++i)
            {
                if (i % 2 == 0)
                {
                    permuted_roots[i] = cheb_roots[left++];
                }
                else
                {
                    permuted_roots[i] = cheb_roots[right--];
                }
            }

            for (int i = 0; i < n_roots; ++i)
            {
                T tau_i = T(1) / permuted_roots[i];
                std::vector<T> Ax = (*this) * x;
                std::vector<T> residual = b - Ax;
                std::vector<T> x_new = x + residual * tau_i;
                T diff = T(0);
                for (int j = 0; j < rows; ++j)
                {
                    diff += std::abs(x_new[j] - x[j]);
                }
                x = x_new;
                total_iterations++;
                if (diff < tolerance)
                {
                    return x;
                }
            }

            std::vector<T> Ax = (*this) * x;
            std::vector<T> residual = b - Ax;
            T residual_norm = T(0);
            for (int j = 0; j < rows; ++j)
            {
                residual_norm += std::abs(residual[j]);
            }
            if (residual_norm < tolerance)
            {
                return x;
            }
        }
        return x;
    }
};

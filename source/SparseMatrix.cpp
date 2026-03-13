#include <iostream>
#include <vector>

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
};

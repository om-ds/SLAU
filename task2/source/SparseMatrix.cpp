#include <iostream>
#include <vector>
#include <map>

template<typename T>

class SparseMatrix
{
private:
    int rows;
    int cols;
    std::map<std::pair<int, int>, T> data;
public:
    SparseMatrix(int r, int c) : rows(r), cols(c) {}

    void set(int i, int j, T value)
    {
        data[{i, j}] = value;
    }

    T get(int i, int j)
    {
        if (data.count({i, j}) == 1)
        {
            return data[{i, j}];
        }

        else
        {
            return T(0);
        }
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

        result.data.insert(data.begin(), data.end());

        for (const auto& pair : other.data)
        {
            if (data.count(pair.first) == 1)
            {
                result.data[pair.first] += other.data[pair.first];
            }

            else
            {
                result.data[pair.first] = other.data[pair.first];
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
                result[i] += get(i, j) * v[j];
            }
        }

        return result;
    }
};

/*int main()
{
    SparseMatrix<double> A(3, 3);
    A.set(0, 0, 1);
    A.set(1, 0, 2);
    A.set(2, 2, 9);
    A.print();
    std::vector<double> v = {1, 2, 3};
    auto ans = A * v;
    for (int i = 0; i < ans.size(); i++)
    {
        std::cout << ans[i] << " ";
    }
    std::cout << std::endl;
}*/

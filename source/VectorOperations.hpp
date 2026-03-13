#include <vector>
#include <stdexcept>

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

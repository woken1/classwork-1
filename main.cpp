#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

struct IntArray
{
    IntArray(int i) :
        k(1),
        a(new int[1])
    {
        *a = i;
    }

    ~IntArray()
    {
        delete[] a;
    }

    IntArray(const IntArray& rhs);
    IntArray& operator=(const IntArray& rhs);

    IntArray(IntArray&& rhs) :
        a(rhs.a),
        k(rhs.getsize())
    {
        rhs.a = nullptr;
    }

    IntArray& operator=(IntArray&& rhs)
    {
        delete[] a;
        a = rhs.a;
        k = rhs.getsize();

        rhs.a = nullptr;
        return *this;
    }

    int add(int i);
    int get(size_t id) const noexcept;
    size_t getsize() const noexcept;
    int last() const noexcept;

    int* a;
    size_t k;
};

IntArray::IntArray(const IntArray& rhs) :
    a(new int[rhs.getsize()]),
    k(rhs.getsize())
{
    for (size_t i = 0; i < rhs.getsize(); ++i)
        a[i] = rhs.get(i);
}

int IntArray::get(size_t id) const noexcept
{
    return a[id];
}

size_t IntArray::getsize() const noexcept
{
    return k;
}

int IntArray::last() const noexcept
{
    return get(getsize() - 1);
}

int IntArray::add(int i)
{
    int* dint = new int[getsize() + 1];

    for (size_t i = 0; i < getsize(); ++i)
    {
        dint[i] = get(i);
    }

    delete[] a;
    a = dint;
    ++k;
    return i;
}

IntArray& IntArray::operator=(const IntArray& rhs)
{
    if (this == &rhs) return *this;
    int* p = new int[rhs.getsize()];

    for (size_t i = 0; i < rhs.getsize(); ++i)
        p[i] = rhs.get(i);

    delete[] a;
    a = p;
    k = rhs.getsize();

    return *this;
}

class IntMatrix : public IntArray
{
    size_t rows_, cols_;

public:
    IntMatrix(const char* filename) : IntArray(0)
    {
        std::ifstream f(filename);
        if (!f) throw std::runtime_error("File open error");

        f >> rows_ >> cols_;
        if (rows_ == 0 || cols_ == 0)
            throw std::runtime_error("Invalid dimensions");

        delete[] a;
        k = rows_ * cols_;
        a = new int[k];

        for (size_t i = 0; i < k; ++i)
            f >> a[i];

        if (!f) throw std::runtime_error("Read error");
    }

    int get(size_t i, size_t j) const
    {
        return IntArray::get(i * cols_ + j);
    }

    void set(size_t i, size_t j, int val)
    {
        a[i * cols_ + j] = val;
    }

    size_t getRows() const { return rows_; }
    size_t getCols() const { return cols_; }

    void commandI(size_t r, int v)
    {
        if (r > rows_) throw std::out_of_range("Invalid row index");

        size_t newRows = rows_ + 1;
        size_t newCols = cols_;
        int* newData = new int[newRows * newCols];

        size_t idx = 0;
        for (size_t i = 0; i < r; ++i)
            for (size_t j = 0; j < cols_; ++j)
                newData[idx++] = get(i, j);

        for (size_t j = 0; j < newCols; ++j)
            newData[idx++] = v;

        for (size_t i = r; i < rows_; ++i)
            for (size_t j = 0; j < cols_; ++j)
                newData[idx++] = get(i, j);

        delete[] a;
        a = newData;
        k = newRows * newCols;
        rows_ = newRows;
        cols_ = newCols;
    }

    void commandII(size_t c, int v)
    {
        if (c > cols_) throw std::out_of_range("Invalid column index");

        size_t newRows = rows_;
        size_t newCols = cols_ + 1;
        int* newData = new int[newRows * newCols];

        size_t idx = 0;
        for (size_t i = 0; i < rows_; ++i)
        {
            for (size_t j = 0; j < c; ++j)
                newData[idx++] = get(i, j);
            newData[idx++] = v;
            for (size_t j = c; j < cols_; ++j)
                newData[idx++] = get(i, j);
        }

        delete[] a;
        a = newData;
        k = newRows * newCols;
        rows_ = newRows;
        cols_ = newCols;
    }

    void print() const
    {
        for (size_t i = 0; i < rows_; ++i)
        {
            for (size_t j = 0; j < cols_; ++j)
            {
                std::cout << get(i, j);
                if (j + 1 < cols_) std::cout << ' ';
            }
            std::cout << '\n';
        }
    }
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 4)
        {
            std::cerr << "Usage: " << argv[0] << " <file> <I|II> <pos>\n";
            return 1;
        }

        std::string cmd = argv[2];
        IntMatrix m(argv[1]);

        int value;
        std::cin >> value;
        if (!std::cin) return 1;

        if (cmd == "I")
        {
            size_t r = static_cast<size_t>(std::stoi(argv[3]));
            m.commandI(r, value);
            m.print();
        }
        else if (cmd == "II")
        {
            size_t c = static_cast<size_t>(std::stoi(argv[3]));
            m.commandII(c, value);
            m.print();
        }
        else
        {
            std::cerr << "Unknown command: " << cmd << "\n";
            return 1;
        }
    }
    catch (...)
    {
        return 1;
    }
}
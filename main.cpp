#include <iostream>

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

int main()
{
    try
    {
        int next = 0;
        std::cin >> next;

        IntArray a(next);

        while (std::cin >> next)
        {
            a.add(next);
        }

        if (!std::cin && !std::cin.eof())
        {
            return 1;
        }

        size_t count = 1;

        for (size_t i = 0; i < a.getsize() - 1; ++i)
        {
            int d = a.get(i);

            count += !(d % a.last()) ? 1 : 0;
        }

        std::cout << count << "\n";
    }
    catch (...)
    {
        return 1;
    }
}
// Iterator Design Pattern
// Intent: Lets you traverse elements of a collection without exposing its
// underlying representation (list, stack, tree, etc.).

#include <iostream>
#include <vector>

// C++ has its own implementation of iterator that works with a
// different generic containers defined by the standard library.
template <typename T, typename U>
class Iterator
{
public:
    using iterType = typename std::vector<T>::iterator;
    Iterator(U* const container)
        : m_container(container)
        , m_it(m_container->m_data.begin())
    { }
    void first() { m_it = m_container->m_data.begin(); }
    void next() { m_it++; }
    bool isDone() const { return m_it == m_container->m_data.end(); }
    iterType current() const { return m_it; }

private:
    U* const m_container;
    iterType m_it;
};

// Generic Collections/Containers provides one or several methods for retrieving
// fresh iterator instances, compatible with the collection class.
template <class T>
class Container
{
    friend class Iterator<T, Container<T>>;

public:
    void add(T a) { m_data.push_back(std::move(a)); }
    Iterator<T, Container<T>>* createIterator() { return new Iterator<T, Container<T>>(this); }

private:
    std::vector<T> m_data;
};

class Data
{
public:
    Data(const int a = 0)
        : m_data(a)
    { }
    int data() const { return m_data; }

private:
    int m_data;
};

// The client code may or may not know about the Concrete Iterator or Collection classes, for this
// implementation the container is generic so you can used with an int or with a custom class.
int main()
{
    std::cout << "________________Iterator with int________________\n";
    Container<int> cont1;
    for (int i = 0; i < 10; i++)
    {
        cont1.add(i);
    }
    Iterator<int, Container<int>>* const it1 = cont1.createIterator();
    for (it1->first(); !it1->isDone(); it1->next())
    {
        std::cout << *it1->current() << '\n';
    }

    std::cout << "________________Iterator with Data________________\n";
    Container<Data> cont2;
    Data a(100), b(1000), c(10000);
    cont2.add(a);
    cont2.add(b);
    cont2.add(c);
    Iterator<Data, Container<Data>>* const it2 = cont2.createIterator();
    for (it2->first(); !it2->isDone(); it2->next())
    {
        std::cout << it2->current()->data() << '\n';
    }

    delete it1;
    delete it2;
}

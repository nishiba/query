#pragma once

#include <iterator>
#include "query/closure_traits.h"


template <typename Iterator, typename Pred>
class WhereIterator :
    public std::iterator<std::forward_iterator_tag, typename Iterator::value_type> {
public:
    WhereIterator(Iterator& t, Pred& p, int i, int size)
        : _i(i), _t(t), _p(p), _size(size)
    {
    }
    bool operator!=(WhereIterator& other)
    {
        return _i != other._i
            || _t != other._t;
    }
    WhereIterator& operator++()
    {
        ++_i;
        ++_t;
        while (_i != _size && !_p(*_t, _i)) {
            ++_i;
            ++_t;
        }
        return *this;
    }
    auto& operator*()
    {
        return *_t;
    }
private:
    int _i;
    int _size;
    Iterator _t;
    Pred& _p;
};

template <typename T, typename Pred>
class Where {
public:
    typedef typename T::value_type value_type;
public:
    Where(T& t, Pred& p) : _t(t), _p(p)
    {
    }
    bool operator!=(Where& other)
    {
        return _i != other._i
            || _t != other._t;
    }
    auto& operator*()
    {
        return _t[_i];
    }
    auto begin()
    {
        int i = 0;
        auto b = _t.begin();
        while (i != _t.size() && !_p(*b, i)) {
            ++i;
            ++b;
        }
        return this->makeIterator(b, i);
    }
    auto end()
    {
        return this->makeIterator(_t.end(), _t.size());
    }
private:
    template <typename Iterator>
    auto makeIterator(Iterator& t, std::size_t i) {
        return WhereIterator<Iterator, Pred>(t, _p, i, _t.size());
    }
private:
    typename closure_traits<T>::type _t;
    Pred& _p;
};


template <typename T, typename F>
auto where(T& t, F& f) {
    auto p = [&](auto& x, std::size_t index) {return f(x); };
    return query(Where<T, decltype(p)>(t, p));
}


template <typename T, typename F>
auto whereWithIndex(T& t, F& f) {
    return query(Where<T, F>(t, f));
}

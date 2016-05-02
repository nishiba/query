#pragma once

#include <iterator>
#include "query/closure_traits.h"
#include "query/utility.h"



template <typename Iterator, typename Pred>
class SelectIterator :
    public std::iterator<std::forward_iterator_tag, typename Iterator::value_type> {
public:
    SelectIterator(Iterator& t, Pred& p, std::size_t i)
        : _t(t), _i(i), _p(p)
    {
    }
    bool operator!=(SelectIterator& other)
    {
        return _t != other._t
            || _i != other._i
            || &_p != &other._p;
    }
    SelectIterator& operator++()
    {
        ++_i;
        ++_t;
        return *this;
    }
    auto operator*()
    {
        return _p(*_t, _i);
    }
private:
    int _i;
    Iterator _t;
    Pred& _p;
};

template <typename T, typename Pred>
class Select {
public:
    typedef typename T::value_type value_type;
public:
    Select(T& t, Pred& p)
        : _t(t), _p(p)
    {
    }
    auto begin()
    {
        return this->makeIterator(_t.begin(), 0);
    }
    auto end()
    {
        return this->makeIterator(_t.end(), _t.size());
    }
    std::size_t size() const
    {
        return _t.size();
    }
private:
    template <typename Iterator>
    auto makeIterator(Iterator& t, std::size_t i) {
        return SelectIterator<Iterator, Pred>(t, _p, i);
    }
private:
    typename closure_traits<T>::type _t;
    Pred& _p;
};


template <typename T, typename F>
auto select(T& t, F& f) {
    auto p = [&](const auto& x, std::size_t index) {return f(x); };
    return query(Select<T, decltype(p)>(t, p));
}

template <typename T, typename F>
auto selectWithIndex(T& t, F& f) {
    return query(Select<T, F>(t, f));
}

template <typename T, typename F>
auto selectUnzip(T& t, F& f) {
    auto p = [&](auto& x, std::size_t index) {return applyTuple(f, x); };
    return query(Select<T, decltype(p)>(t, p));
}

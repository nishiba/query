#pragma once

#include <iterator>
#include "query/closure_traits.h"


template <typename T, typename Pred>
class where_iterator :
    public std::iterator<std::forward_iterator_tag, typename T::value_type> {
public:
    where_iterator(T& t, Pred& p, int i = 0)
        : _i(i), _t(t), _p(p)
    {
        update();
    }
    bool operator!=(where_iterator& other)
    {
        return _i != other._i;
    }
    where_iterator& operator++()
    {
        ++_i;
        update();
        return *this;
    }
    auto& operator*()
    {
        return _t[_i];
    }
    where_iterator begin()
    {
        return where_iterator(_t, _p, 0);
    }
    where_iterator end()
    {
        return where_iterator(_t, _p, _t.size());
    }
private:
    void update()
    {
        while (_i != _t.size() && !_p(_t[_i], _i)) ++_i;
    }
private:
    int _i;
    typename closure_traits<T>::type _t;
    Pred& _p;
};

template <typename T, typename F>
auto where(T& t, F& f) {
    auto p = [&](auto& x, std::size_t index) {return f(x); };
    return query(where_iterator<T, decltype(p)>(t, p));
}


template <typename T, typename F>
auto where_with_index(T& t, F& f) {
    return query(where_iterator<T, F>(t, f));
}

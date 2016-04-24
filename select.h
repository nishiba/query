#pragma once

#include <iterator>
#include "closure_traits.h"



template <typename T, typename Pred>
class select_iterator :
    public std::iterator<std::forward_iterator_tag, typename T::value_type> {
public:
    select_iterator(T& t, Pred& p, int i = 0)
        : _i(i), _t(t), _p(p)
    {
    }
    bool operator!=(select_iterator& other)
    {
        return _i != other._i;
    }
    select_iterator& operator++()
    {
        ++_i;
        return *this;
    }
    auto operator*()
    {
        return _p(_t[_i], _i);
    }
    select_iterator begin()
    {
        return select_iterator(_t, _p, 0);  
    }
    select_iterator end()
    {
        return select_iterator(_t, _p, _t.size());
    }
private:
    int _i;
    typename closure_traits<T>::type _t;
    Pred& _p;
};

template <typename T, typename F>
auto select(T& t, F& f) {
    auto p = [&](auto& x, std::size_t index) {return f(x); };
    return select_iterator<T, decltype(p)>(t, p);
}

template <typename T, typename F>
auto select_with_index(T& t, F& f) {
    return select_iterator<T, F>(t, f);
}

template <typename T, typename F, int N = boost::tuples::length<typename T::value_type>::value>
struct select_unzip_traits;

//TODO
template <typename T, typename F>
struct select_unzip_traits<T, F, 2> 
{
    static auto apply(T& t, F& f) 
    {
        return [&](auto& x, std::size_t index) {return f(x.get<0>(), x.get<1>()); };
    }
};


template <typename T, typename F>
auto select_unzip(T& t, F& f) {
    auto p = select_unzip_traits<T, F>::apply(t, f);
    return select_iterator<T, decltype(p)>(t, p);
}

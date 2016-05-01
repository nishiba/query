#pragma once

#include <iterator>
#include "query/closure_traits.h"


template <typename T, typename F> inline
void apply(T& t, F& f) {
    for (auto& a : t) {
        f(a);
    }
}

template <typename T, typename F> inline
void applyWithIndex(T& t, F& f) {
    std::size_t i = 0;
    for (auto& a : t) {
        f(a, i);
        ++i;
    }
}

template <typename T, typename F, int N = boost::tuples::length < typename T::value_type > ::value>
struct ApplyUnzipTraits;

//TODO
template <typename T, typename F>
struct ApplyUnzipTraits<T, F, 2>
{
    static void apply(typename T::value_type& t, F& f)
    {
        f(t.get<0>(), t.get<1>());
    }
};

template <typename T, typename F> inline
void applyUnzip(T& t, F& f) {
    for (auto& a : t) {
        ApplyUnzipTraits<T, F>::apply(a, f);
    }
}



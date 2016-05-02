#pragma once

#include <iterator>
#include "query/closure_traits.h"
#include "query/utility.h"


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

template <typename T, typename F> inline
void applyUnzip(T& t, F& f) {
    for (auto& a : t) {
        applyTuple(f, a);
    }
}



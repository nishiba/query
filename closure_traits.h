#pragma once


#include "query/query.h"

template <typename T>
struct closure_traits {
    typedef T& type;
};


template <typename T>
struct closure_traits<Query<T>> {
    typedef Query<T> type;
};


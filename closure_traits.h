#pragma once


#include "query.h"

template <typename T>
struct closure_traits {
    typedef T& type;
};


template <typename T>
struct closure_traits<query<T>> {
    typedef query<T> type;
};


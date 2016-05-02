#pragma once

#include <boost/tuple/tuple.hpp>


template <int N>
struct ApplyTuple {
    template <typename F, typename Tuple, typename... Args>
    static auto apply(F& f, Tuple& t, Args&... args)
    {
        return ApplyTuple<N - 1>::apply(f, t, t.get<N - 1>(), args...);
    }
};

template <>
struct ApplyTuple<0> {
    template <typename F, typename Tuple, typename... Args>
    static auto apply(F& f, Tuple& t, Args&... args)
    {
        return f(args...);
    }
};

template <typename F, typename Tuple>
auto applyTuple(F& f, Tuple& t)
{
    return ApplyTuple< boost::tuples::length<Tuple>::value>::apply(f, t);
}

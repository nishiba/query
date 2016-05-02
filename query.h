#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <boost/optional.hpp>


template<typename T>
class Query {
public:
    typedef typename std::remove_reference<T>::type::value_type value_type;
public:
    Query(T t) : _t(t) {}

    template <typename F>
    auto where(F& f) { return ::where(*this, f); }
    template <typename F>
    auto whereWithIndex(F& f) { return ::whereWithIndex(*this, f); }

    template <typename F>
    auto select(F& f) { return ::select(*this, f); }
    template <typename F>
    auto selectWithIndex(F& f) { return ::selectWithIndex(*this, f); }
    template <typename F>
    auto selectUnzip(F& f) { return ::selectUnzip(*this, f); }
    
    template <typename F>
    void apply(F& f) { ::apply(*this, f); }
    template <typename F>
    void applyWithIndex(F& f) { ::applyWithIndex(*this, f); }
    template <typename F>
    void applyUnzip(F& f) { ::applyUnzip(*this, f); }

    template <typename F>
    bool any(F& f) { return ::any(*this, f); }
    template <typename V>
    V average(V initial) { return ::average(*this, initial); }
    template <typename V>
    V sum(V initial) { return ::sum(*this, initial); }


    auto skip(std::size_t n) { return ::skip(_t, n); }
    auto take(std::size_t n) { return ::take(_t, n); }
    template <typename F>
    auto takeWhile(F& f) { return ::takeWhile(*this, f); }
    template <typename F>
    auto takeWhileWithIndex(F& f) { return ::takeWhileWithIndex(*this, f); }


    auto begin() { return _t.begin();}
    auto end() { return _t.end();}
    auto begin() const { return _t.begin(); }
    auto end() const { return _t.end(); }
    auto size() const { return _t.size(); }
    auto front() { return *begin(); }

    auto toStdVector() 
    {
        typedef 
            typename std::remove_const<
                typename std::remove_reference<decltype(*_t.begin())>::type
            >::type value_type;
        return std::vector<value_type>(_t.begin(), _t.end());
    }

private:
    T _t;
};

template<typename T>
Query<T&> query(T& t)
{
    return Query<T&>(t);
}

template<typename T>
Query<T> query(T&& t)
{
    return Query<T>(t);
}


#include "query/where.h"
#include "query/select.h"
#include "query/apply.h"
#include "query/TakeWhile.h"
#include <boost/tuple/tuple.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <tuple>

template <typename ... T>
auto zip(T&... t)
{
    auto b = boost::make_zip_iterator(boost::make_tuple(std::begin(t)...));
    auto e = boost::make_zip_iterator(boost::make_tuple(std::end(t)...));
    return query(boost::make_iterator_range(b, e));
}

inline auto range(int begin, int end) {
    assert(begin < end);
    auto b = boost::make_counting_iterator(begin);
    auto e = boost::make_counting_iterator(end);
    return query(boost::make_iterator_range(b, e));
}

template <typename T, typename V> inline
V sum(T& t, V x)
{
    return std::accumulate(t.begin(), t.end(), x);
}

template <typename T, typename V> inline
V average(T& t, V x) 
{
    return sum(t, x) / t.size();
}


template<typename T> inline
auto skip(T& t, std::size_t n) {
    assert(n < t.size());
    auto b = std::begin(t);
    std::advance(b, n);
    auto e = std::end(t);
    return query(boost::make_iterator_range(b, e));
}

template<typename T> inline
auto take(T& t, std::size_t n) {
    assert(n < t.size());
    auto b = std::begin(t);
    auto e = std::begin(t);
    std::advance(e, n);
    return query(boost::make_iterator_range(b, e));
}


template<typename T, typename F> inline
bool any(T& t, F& f) {
    return std::find_if(t.begin(), t.end(), f) != t.end();
}

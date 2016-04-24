#pragma once


template<typename T>
class query : public T {
public:
    query(const T& t) : T(t) {}

    template <typename F>
    auto where(F& f) {
        return ::where(*this, f);
    }
    template <typename F>
    auto select(F& f) {
        return ::select(*this, f);
    }
    auto skip(std::size_t n)
    {
        return ::skip(base(), n);
    }
    auto take(std::size_t n)
    {
        return ::take(base(), n);
    }
private:
    T& base() { return *this; }
};

template<typename T>
query<T> make_query(const T& t)
{
    return query<T>(t);
}


#include "where.h"
#include "select.h"


template <typename ... T>
auto zip(T&... t)
{
    auto b = boost::make_zip_iterator(boost::make_tuple(std::begin(t)...));
    auto e = boost::make_zip_iterator(boost::make_tuple(std::end(t)...));
    return make_query(boost::make_iterator_range(b, e));
}

template<typename T>
auto skip(T& t, std::size_t n) {
    assert(n < t.size());
    auto b = std::begin(t);
    std::advance(b, n);
    auto e = std::end(t);
    return make_query(boost::make_iterator_range(b, e));
}

template<typename T>
auto take(T& t, std::size_t n) {
    assert(n < t.size());
    auto b = std::begin(t);
    auto e = std::begin(t);
    std::advance(e, n);
    return make_query(boost::make_iterator_range(b, e));
}


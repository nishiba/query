#pragma once

#include <iterator>


template <typename Iterator, typename Pred>
class TakeWhileIterator :
    public std::iterator<std::forward_iterator_tag, typename Iterator::value_type> {
public:
    TakeWhileIterator(Iterator& t, Pred& p, int i, int size)
        : _i(i), _t(t), _p(p), _size(size)
    {
    }
    bool operator!=(TakeWhileIterator& other)
    {
        return _i != other._i;
    }
    TakeWhileIterator& operator++()
    {
        ++_i;
        ++_t;
        if (!_p(*_t, _i)) {
            _i = _size;
        }
        return *this;
    }
    auto& operator*()
    {
        return *_t;
    }
private:
    int _i;
    int _size;
    Iterator _t;
    Pred& _p;
};

template <typename T, typename Pred>
class TakeWhile {
public:
    typedef typename T::value_type value_type;
public:
    TakeWhile(T& t, Pred& p) : _t(t), _p(p)
    {
    }
    bool operator!=(TakeWhile& other)
    {
        return _t != other._t;
    }
    auto begin()
    {
        bool isValid = _p(*_t.begin(), 0);
        int i = isValid ? 0 : _t.size();
        auto b = isValid ? _t.begin() : _t.end();
        return this->makeIterator(b, i);
    }
    auto end()
    {
        return this->makeIterator(_t.end(), _t.size());
    }
private:
    template <typename Iterator>
    auto makeIterator(Iterator& t, std::size_t i) {
        return TakeWhileIterator<Iterator, Pred>(t, _p, i, _t.size());
    }
private:
    typename closure_traits<T>::type _t;
    Pred& _p;
};


template <typename T, typename F>
auto takeWhile(T& t, F& f) {
    auto p = [&](auto& x, std::size_t index) {return f(x); };
    return query(TakeWhile<T, decltype(p)>(t, p));
}


template <typename T, typename F>
auto takeWhileWithIndex(T& t, F& f) {
    return query(TakeWhile<T, F>(t, f));
}

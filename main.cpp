// query.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <tuple>
#include <iostream>


struct Buzz {
    explicit Buzz(double x) : _x(x) { std::cout << "Buzz(double x)" << std::endl; }
    Buzz(const Buzz& x) : _x(x._x) { std::cout << "Buzz(const Buzz& x)" << std::endl; }
    Buzz(const Buzz&& x) : _x(x._x) { std::cout << "Buzz(const Buzz&& x)" << std::endl; }
    double _x;
};


template <int N>
struct Expand {
    template <typename F, typename Tuple, typename... Args>
    static void apply(F& f, Tuple& t, Args&... args)
    {
        std::cout << N << std::endl;
        Expand<N - 1>::apply(f, t, std::get<N - 1>(t), args...);
    }
};

template <>
struct Expand<0> {
    template <typename F, typename Tuple, typename... Args>
    static void apply(F& f, Tuple& t, Args&... args)
    {
        f(args...);
    }
};

template <typename F, typename Tuple>
void apply(F& f, Tuple& t)
{
    std::cout << "apply" << std::endl;
    Expand<std::tuple_size<Tuple>::value>::apply(f, t);
}


int main()
{
    Buzz three(3.0);
    auto x = std::make_tuple(1, 2.0, three);
    auto f = [](auto& a, auto& b, auto& c) {
        std::cout << typeid(a).name() << std::endl;
        std::cout << typeid(b).name() << std::endl;
        std::cout << typeid(c).name() << std::endl;
    };
    std::cout << "---" << std::endl;
    apply(f, x);

    return 0;
}

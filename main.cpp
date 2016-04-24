// query.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <type_traits>

#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <boost/type_traits.hpp>

#include "query.h"









int main()
{
    std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> y = { 0, 1, 2, 3, 4, 5, 6, 7 };
    for (auto& x : where(v, [](int x) -> bool {return (x % 2) == 0; })) {
        std::cout << x << std::endl;
    }

    std::cout << std::endl;
    for (auto& x : where_with_index(v, [](int x, std::size_t index) -> bool {return (index < 4); })) {
        std::cout << x << std::endl;
    }

    std::cout << std::endl;
    for (auto& x : skip(v, 1).where([](int x) {return x < 6; })) {
        std::cout << x << std::endl;
    }

    std::cout << std::endl;
    for (auto x : skip(v, 1).select([](int x) {return x < 6; })) {
        std::cout << x << std::endl;
    }

    std::cout << std::endl;
    for (auto& x : zip(v, y).take(2)) {
        std::cout << x.get<0>() << "," << x.get<1>() << std::endl;
    }

    std::cout << std::endl;
    for (auto&& x : select_unzip(zip(v, y), [](auto& x, auto& y) {return x + y; })) {
        std::cout << x << std::endl;
    }

    std::cout << std::endl;
    for (auto& x : skip(v, 1).take(3)) {
        std::cout << x << std::endl;
    }

    return 0;
}

#pragma once
#pragma once

#include <iostream>
#include <variant>
#include "operations.h"
#include <map>


namespace full{



namespace MyEnumTyped {

struct A{};
struct B{};
struct C{};

using Value = std::variant<A,B,C>;

}

constexpr int foo(MyEnumTyped::A){return 1;}
constexpr int foo(MyEnumTyped::B){return 2;}
constexpr int foo(MyEnumTyped::C){return 3;}

constexpr MyEnumTyped::Value v = MyEnumTyped::B{};

constexpr auto i = std::visit([](auto  e){ return foo(e); },v);

int main() {


    std::cout<<"i=" << i <<" \n";

    std::cout << " v = " << toString(v) << std::endl;

    MyEnumTyped::Value x;

    fromString(x,"full::MyEnumTyped::C");
    std::cout<< toString(x) << "\n";



    const auto all = asList(x);

    for(auto const& e:all)
        std::cout << " " << toString(e);

    std::cout<<std::endl;



    std::map<MyEnumTyped::Value, std::string> map;
    map[MyEnumTyped::B{}] = "mapB";
    map[MyEnumTyped::C{}] = "mapC";

    for(const auto x:map)
        std::cout<<toString(x.first) << ":" << x.second << " ";
    std::cout<<std::endl;


    return 0;
}






}

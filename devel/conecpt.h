#pragma once

#include <iostream>
#include <variant>

namespace conecpt{


enum class MyEnumClassic {
    A,
    B,
    C
};


namespace MyEnumTyped {

struct A{};
struct B{};
struct C{};

using Value = std::variant<A,B,C>;

}



int foo(MyEnumClassic v){
    switch(v){

    case MyEnumClassic::A: return 1;
    case MyEnumClassic::B: return 2;
    case MyEnumClassic::C: return 3;
    default:
        break;
    }

    return -1;


}

int foo(MyEnumTyped::A){return 1;}
int foo(MyEnumTyped::B){return 2;}
int foo(MyEnumTyped::C){return 3;}



int main() {

    MyEnumClassic m = MyEnumClassic::B;
    MyEnumTyped::Value v = MyEnumTyped::B{};

    const auto j = foo(m);
    const auto i = std::visit([](auto  e){ return foo(e); },v);

    std::cout<<"i=" << i << " j=" << j << " \n";

    return 0;
}






}

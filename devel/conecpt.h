#pragma once

#include <iostream>
#include <variant>

namespace conecpt{



/////////////////////////////////////

namespace classic {

enum class MyEnum
{
    A,
    B,
    C
};

int main()
{
    MyEnum e = MyEnum::B;

    switch(e){

    case MyEnum::A: return 1;
    case MyEnum::B: return 2;
    case MyEnum::C: return 3;
    default:
        break;
    }

    return -1;
}

} // ns classic




/////////////////////////////////////

namespace typed {

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

int case_(MyEnum::A){ return 1; }
int case_(MyEnum::B){ return 2; }
int case_(MyEnum::C){ return 3; }

int main()
{
    MyEnum::Value e = MyEnum::B{};
    return std::visit([](auto  e_){ return case_(e_); },e);
}

} //ns typed




/////////////////////////////////////

int main() {
    return classic::main() == typed::main();
}



}

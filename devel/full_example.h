#pragma once

#include <iostream>
#include <variant>
#include "variant_helper.h"
#include <map>
#include <cassert>

namespace full{

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

namespace printExample {

void main()
{
    MyEnum::Value v = MyEnum::B{};
    std::cout<<"v=" << vahlp::toString(v) <<"\n"; // prints 'v=MyEnum::B'
}


} // ns

namespace parseExample {

void main()
{
    MyEnum::Value v = vahlp::fromString<MyEnum::Value>("full::MyEnum::C");
    std::cout<< "parsed v=" << vahlp::toString(v) << "\n";   // prints 'parsed v=MyEnum::C'
}


} // ns

namespace ifExample {

int main()
{
    MyEnum::Value v = MyEnum::B{};

    if (std::holds_alternative<MyEnum::B>(v)) // returns 1
        return 1;

    return 0;
}


} // ns


namespace constexprExample {

constexpr int foo(MyEnum::A){return 1;}
constexpr int foo(MyEnum::B){return 2;}
constexpr int foo(MyEnum::C){return 3;}

constexpr MyEnum::Value v = MyEnum::B{};

constexpr auto i = std::visit([](auto  e){ return foo(e); },v); // i is set to 2

} // ns


namespace indexExample {

void main() {

    std::map<MyEnum::Value, std::string> map;
    map[MyEnum::B{}] = "XXX";
    map[MyEnum::C{}] = "YYY";

    //prints 'MyEnum::B=XXX MyEnum::C=YYY'
    for(const auto x:map)
        std::cout<<vahlp::toString(x.first) << "=" << x.second << " ";

    std::cout<<std::endl;
}


} // ns

namespace iterationExample {

void main() {

    const auto instances = vahlp::makeInstances<MyEnum::Value>();

    for(auto const& e:instances)
        std::cout << " " << vahlp::toString(e); // prints 'MyEnum::A MyEnum::B MyEnum::C'

    std::cout<<std::endl;
}


} // ns



namespace extensionExample {

namespace Log
{
    struct ToConsole
    {
        void log(const char* msg){ std::cout << "logging to console: '" << msg << "'\n"; }
    };

    struct ToFile
    {
        ToFile(){}
        ToFile(std::string filename):m_filename(filename){}
        std::string m_filename;
        void log(const char* msg){ std::cout << "logging to file '" << m_filename << ": '" << msg << "'\n"; }
    };

    struct ToNetwork
    {
        void log(const char* msg){ std::cout << "logging to network: '" << msg << "'\n"; }
    };

    using Value = std::variant<ToConsole,ToFile,ToNetwork>;
}


void main()
{
    Log::Value log = Log::ToFile{"myLogFile.txt"};
    const char* msg = "some msg";
    std::visit([&](auto  log_){ log_.log(msg); },log);  // prints "logging to file 'myLogFile.txt: 'some msg'"
}


} // ns

constexpr int foo(MyEnum::A){return 1;}
constexpr int foo(MyEnum::B){return 2;}
constexpr int foo(MyEnum::C){return 3;}

constexpr MyEnum::Value v = MyEnum::B{};

constexpr auto i = std::visit([](auto  e){ return foo(e); },v);

int main() {

    printExample::main();
    parseExample::main();
    indexExample::main();
    extensionExample::main();

    if (std::holds_alternative<MyEnum::B>(v)){
        std::cout<<"holds B\n";
    }

    std::cout<<"i=" << i <<" \n";

    const std::string str = vahlp::toString(v);

    std::cout << " v = " << str << std::endl;

    const MyEnum::Value x = vahlp::fromString<MyEnum::Value>(str);
    std::cout<< vahlp::toString(x) << "\n";

    assert(str==vahlp::toString(x));
    assert(v.index()==x.index());



    const auto instances = vahlp::makeInstances<MyEnum::Value>();

    for(auto const& e:instances)
        std::cout << " " << vahlp::toString(e);

    std::cout<<std::endl;



    std::map<MyEnum::Value, std::string> map;
    map[MyEnum::B{}] = "mapB";
    map[MyEnum::C{}] = "mapC";

    for(const auto x:map)
        std::cout<<vahlp::toString(x.first) << ":" << x.second << " ";

    std::cout<<std::endl;


    return 0;
}






}

# TypeBasedEnum
An alternative realization of an enum is demonstrated below which is based on types instead of enum values. It makes use of std::variant to represent different enum values as shown in the example below:

Classical representation:

```cpp
enum class MyEnum 
{
    A,
    B,
    C
};
```

Type-base representation:

```cpp
#include <variant>

namespace MyEnum 
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}
```

## Typical Use-Cases 

In this section some standard uses cases of enums are considered how they can be realized with typebased enums.

### Switch Statement

A standard switch statement like the following:

```cpp

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
```

A typebased version could look as follows:

```cpp
#include <variant>

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
```
Note that the type `Value` must contain all types. If a type is not included in `Value` it cannot be used in combination with it. Also note that `case_()` functions must be provied for all types in order to have successful compilation. 

### If Statement

The if-statement looks the following:

```cpp
#include <variant>

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

int main()
{
    MyEnum::Value v = MyEnum::B{};

    if (std::holds_alternative<MyEnum::B>(v)) // returns 1
        return 1;

    return 0;
}
```
### Stringify

The stringification of typebased enums can be done fully generic by employing the type-information provided by `std::variant`: 

```cpp
#include <iostream>
#include <variant>
#include "variant_helper.h"

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

void main()
{
    MyEnum::Value v = MyEnum::B{};
    std::cout<<"v=" << vahlp::toString(v) <<"\n"; //prints 'v=MyEnum::B'
}
```

The file `variant_helper.h` contains the function `vahlp::toString()` which simply prints the type typename of the argument.

### Unstringify

The unstringification of typebased enums can also be done fully generic by employing the type-information provided by `std::variant`: 

```cpp
#include <iostream>
#include <variant>
#include "variant_helper.h"

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

void main()
{
    MyEnum::Value v = vahlp::fromString<MyEnum::Value>("MyEnum::C");
    std::cout<< "parsed v=" << vahlp::toString(v) << "\n";   //prints 'parsed v=MyEnum::C'
}
```
The file `variant_helper.h` contains the function `vahlp::fromString()` which iterates over all typenames of the variant to determine the resulting enum type.

### Constexpr

Typed enums can be used in constexpr context: 

```cpp
#include <iostream>
#include <variant>
#include "variant_helper.h"

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

constexpr int foo(MyEnum::A){return 1;}
constexpr int foo(MyEnum::B){return 2;}
constexpr int foo(MyEnum::C){return 3;}

constexpr MyEnum::Value v = MyEnum::B{};

constexpr auto i = std::visit([](auto  e){ return foo(e); },v); // i is set to 2
```

### Use as Index

Typed enums can be used as index in eg. maps:

```cpp
#include <iostream>
#include <variant>
#include <map>
#include "variant_helper.h"

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

void main() {

    std::map<MyEnum::Value, std::string> map;
    map[MyEnum::B{}] = "XXX";
    map[MyEnum::C{}] = "YYY";

    //prints 'MyEnum::B=XXX MyEnum::C=YYY'
    for(const auto x:map)
        std::cout<<vahlp::toString(x.first) << "=" << x.second << " ";  

    std::cout<<std::endl;
}
```

Note that `operator<()` is overloaded here for variants  (see `variant_helper.h`). The implementation of `operator<()` makes use of the type-index within the variant. 

### Iteratibilty

All typed enums can be iterated over:

```cpp
#include <iostream>
#include <variant>
#include <map>
#include "variant_helper.h"

namespace MyEnum
{
    struct A{};
    struct B{};
    struct C{};

    using Value = std::variant<A,B,C>;
}

void main() {

    const auto instances = vahlp::makeInstances<MyEnum::Value>();

    for(auto const& e:instances)
        std::cout << " " << vahlp::toString(e); // prints 'MyEnum::A MyEnum::B MyEnum::C'

    std::cout<<std::endl;
}
```

The function `vahlp::makeInstances<MyEnum::Value>()` returns an instance of each type contained in `Value` within a vector of type `std::vector<MyEnum::Value>`.

### Extendablity

Since the enum types are just regular types, they can be extended arbitrarily, as long as they are default constructible, eg.: 

```cpp
#include <iostream>
#include <variant>
#include <map>
#include "variant_helper.h"

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
```

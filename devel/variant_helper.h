#pragma once
#include <variant>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <cxxabi.h>
#include <sstream>
#include <vector>

namespace std {

template<typename... Types>
struct less<std::variant<Types...>>
{
   bool operator()(const std::variant<Types...>& l, const std::variant<Types...>& r) const
   {
      return l.index() < r.index();
   }
};

}


namespace vahlp{

namespace detail {

std::string demangle( const char* mangled_name ) {

    std::size_t len = 0 ;
    int status = 0 ;
    std::unique_ptr< char, decltype(&std::free) > ptr(
                __cxxabiv1::__cxa_demangle( mangled_name, nullptr, &len, &status ), &std::free ) ;
    return ptr.get() ;
}


template<typename ...>
struct TypeDeductionHelper{};

template<typename Cond, typename Found, typename NotFound, typename First, typename ... Remaining>
auto find_if(Cond cond, Found found, NotFound notFound,TypeDeductionHelper<First, Remaining...>){
    const First first;
    if (cond(first)) return found(first);

    TypeDeductionHelper<Remaining...> r;
    return find_if(cond,found,notFound,r);
}

template<typename Cond, typename Found, typename NotFound>
auto find_if(Cond, Found, NotFound notFound,TypeDeductionHelper<>){
    return notFound();
}

template<typename R, typename First, typename... Remaining>
R make(int idx, TypeDeductionHelper<First, Remaining...>){
    First f;
    R r{f};
    TypeDeductionHelper<Remaining...> h;
    return idx==0 ? r : make<R,Remaining...>(idx-1,h);
}

template<typename R>
R make(int,TypeDeductionHelper<>){
    return  R{};
}

template<typename T>
struct VariantTypesResolver{};

template<typename... Types>
struct VariantTypesResolver<std::variant<Types...>> {

    using variant = std::variant<Types...>;
    static constexpr int type_count = std::variant_size<variant>::value;

    static variant fromString(std::string const& s){

        auto cond = [&](auto enum_value){return s == detail::demangle(typeid(enum_value).name());};
        auto found = [](auto enum_value)->variant{return enum_value;};
        auto nofound = []()->variant{return {};};

        constexpr detail::TypeDeductionHelper<Types...> h;

        return detail::find_if(cond,found,nofound,h);
    }



    static std::vector<variant> makeInstances(){
        constexpr detail::TypeDeductionHelper<Types...> h;

        std::vector<variant> r(type_count);

        for(int i=0;i<type_count;++i)
            r[i] = detail::make<variant, Types... >(i,h);

        return r;
    }


};


} // ns detail

template<typename... Types>
std::string toString(std::variant<Types...> const&v){
    return std::visit([](auto enum_value){
        return detail::demangle(typeid(enum_value).name());
    },v);
}

template<typename Variant>
Variant fromString(std::string const& s){
    return detail::VariantTypesResolver<Variant>::fromString(s);
}


template<typename Variant>
std::vector<Variant> makeInstances(){
    return detail::VariantTypesResolver<Variant>::makeInstances();
}




}

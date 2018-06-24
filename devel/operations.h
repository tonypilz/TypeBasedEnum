#pragma once
#include <variant>
#include <iostream>
#include <typeinfo>
#include <memory>
#include <cxxabi.h>
#include <sstream>
#include <vector>

namespace v_enum { //variant based enum

  template<typename... _Types>
  class variant : public std::variant<_Types...>{};


}



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


namespace full{

namespace detail {

std::string demangle( const char* mangled_name ) {

    std::size_t len = 0 ;
    int status = 0 ;
    std::unique_ptr< char, decltype(&std::free) > ptr(
                __cxxabiv1::__cxa_demangle( mangled_name, nullptr, &len, &status ), &std::free ) ;
    return ptr.get() ;
}

//transfers the types of a variant to another type, eg 'detail::VariantTypes<MyEnumTyped::Value>::types<std::tuple> tup;'
template<typename Variant>
struct VariantTypes{};

template<typename ... Types>
struct VariantTypes<std::variant<Types...>>{

    template<template<class ...> typename T>
    using types = T<Types...>; //all types
};



template<typename ...>
struct TypeDeductionHelper{};

template<typename Cond, typename Found, typename NotFound, typename First, typename ... Remaining>
auto find_if(Cond cond, Found found, NotFound notFound,TypeDeductionHelper<First, Remaining...>){
    const First first;
    if (cond(first)) return found(first);

    TypeDeductionHelper<Remaining...> r;
    return find_if(cond,found,notFound,r );
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



} // ns detail

template<typename... Types>
std::ostream& toString(std::ostream& s, std::variant<Types...> const&v){

    return std::visit([&](auto enum_value)->std::ostream&{ return s <<  detail::demangle(typeid(enum_value).name());},v);

}


template<typename... Types>
std::string toString(std::variant<Types...> const&v){



    std::ostringstream s;
    toString(s,v);
    return s.str();
}



template<typename... Types>
void fromString(std::variant<Types...> &v, std::string const& s){

    auto cond = [&](auto enum_value){return s == detail::demangle(typeid(enum_value).name());};
    auto found = [](auto enum_value)->std::variant<Types...>{return enum_value;};
    auto nofound = []()->std::variant<Types...>{return {};};

    detail::TypeDeductionHelper <Types...> h;

    v = detail::find_if(cond,found,nofound,h);
}


template<typename... Types>
std::vector<std::variant<Types...>> asList(std::variant<Types...> const&){

    using Variant = std::variant<Types...>;
    constexpr int n = std::variant_size<Variant>::value;

    std::vector<std::variant<Types...>> r(n);

    detail::TypeDeductionHelper<Types...> h;

    for(int i=0;i<n;++i)
        r[i] = detail::make<std::variant<Types...>, Types... >(i,h);
    return r;

}




}

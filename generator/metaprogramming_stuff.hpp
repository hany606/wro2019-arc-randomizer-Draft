#pragma once

#include <type_traits>

template<bool State, typename T = void>
using EnableIf = typename std::enable_if<State, T>::type;


template<bool State, typename TrueCaseT, typename FalseCaseT>
using Conditional = typename std::conditional<State, TrueCaseT, FalseCaseT>::type;

template<typename T>
static constexpr auto IsArithmetic = std::is_arithmetic<T>::value;

template<typename T>
static constexpr auto IsIntegral = std::is_integral<T>::value;

template<typename T>
static constexpr auto IsIntegral = std::is_floating_point<T>::value;

template<typename T>
using MakeUnsigned = typename std::make_unsigned<T>::type;

template<typename T>
using MakeSigned = typename std::make_signed<T>::type;


template<typename E>
using UnderlyingType = typename std::underlying_type<E>::type;


template<size_t N, typename Current, typename ... Variants>
struct Select: Select<N-1, Variants...> {

};

template<typename Current, typename ... Variants>
struct Select<0, Current, Variants...> {
    using type = Current;
};

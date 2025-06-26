#include <concepts>

#include <iostream>

#pragma once

/*
==================================================================================================================================================================================
TEMPLATE CONCEPTS

    C++ 'concepts' that restrict template types.

==================================================================================================================================================================================
*/

template<typename T>
concept Streamable = requires(std::ostream& os, const T& value) {
    { os << value } -> std::same_as<std::ostream&>;
};

template<typename T>
concept EqualityCompatible = requires(const T& a, const T& b) {
    {a == b} -> std::convertible_to<bool>;
};

template<typename T>
concept Numerical = requires(T x, const T& y) {

    //Arithmetic
    {x + y} -> std::convertible_to<T>;
    {x - y} -> std::convertible_to<T>;
    {x / y} -> std::convertible_to<T>;
    {x * y} -> std::convertible_to<T>;

    //Compound Assignment
    {x += y} -> std::convertible_to<T>;
    {x -= y} -> std::convertible_to<T>;
    {x /= y} -> std::convertible_to<T>;
    {x *= y} -> std::convertible_to<T>;

    //Comparison Operators 
    {x > y} -> std::convertible_to<bool>;
    {x < y} -> std::convertible_to<bool>;
    {x >= y} -> std::convertible_to<bool>;
    {x <= y} -> std::convertible_to<bool>;
    {x == y} -> std::convertible_to<bool>;

};

template<typename T>
concept Integral = requires(const T& x, const T& y) {

    {x % y} -> std::convertible_to<T>;

};

template<typename T>
concept IntegralNumerical = Numerical<T> && Integral<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;


template<typename ClassType, typename MemberType>
concept HasAccessibleMember = 
    std::is_class_v<ClassType> &&
    std::is_member_object_pointer_v<MemberType ClassType::*>;
#include <algorithm>
#include <limits>
#include <vector>

#include <cstdlib>

#include "SegLibConcepts.h"

#pragma once

namespace SLN {

/*
==================================================================================================================================================================================
PREDICATE FUNCTIONS

    Functions checking conditions about numbers

==================================================================================================================================================================================
*/

    /**
     * @brief Checks if a numerical type is equal to itself.
     *
     * @tparam T Any numerical type.
     * @param Value The value to be checked against itself.
     * @return True if the number is itself, otherwise false.
     * @note Outside of being objectively hilarious, this function can actually be used to identify NaN in non-integral types.
     */
    template <Numerical T>
    bool IsItself(const T Value) {
        return Value == Value;
    }

    /**
     * @brief Checks if an integral type is even. 
     *
     * @tparam T Any integral type (has modulo operator% definitions).
     * @param Value The value to be checked.
     * @return True if the number is even, otherwise false.
     */
    template <IntegralNumerical T>
    bool IsEven(const T Value) {
        return (Value % 2 == 0);
    }

    /**
     * @brief Checks if an integral type is odd. 
     *
     * @tparam T Any integral type (has modulo operator% definitions).
     * @param Value The value to be checked.
     * @return True if the number is odd, otherwise false.
     */
    template <IntegralNumerical T>
    bool IsOdd(const T Value) {
        return !(Value % 2 == 0);
    }

    /**
     * @brief Checks if a numerical type is positive. 
     *
     * @tparam T Any numerical type.
     * @param Value The value to be checked.
     * @return True if the number is above zero, otherwise false.
     */
    template <Numerical T>
    bool IsPositive(const T Value) {
        return (Value > 0); 
    }

    /**
     * @brief Checks if a numerical type is negative. 
     *
     * @tparam T Any numerical type.
     * @param Value The value to be checked.
     * @return True if the number is above zero, otherwise false.
     */
    template <Numerical T>
    bool IsNegative(const T Value) {
        return (Value < 0);
    }

    /**
     * @brief Checks if an integral type is prime (is greater than 1 and has no factors outside itself). 
     *
     * @tparam T Any integral numerical type.
     * @param Value The value to be checked.
     * @return True if the number is prime, otherwise false.
     */
    template <IntegralNumerical T>
    bool IsPrime(T Value) {
        
        if (Value <= 1) return false;
        if (Value <= 3) return true;

        if (IsEven(Value)) {
            return false;
        }
        if (Value % 3 == 0) {
            return false;
        }

        for (long long i  = 5; i * i <= Value; i = i + 6) {
            if (Value % i == 0 || Value % (i + 2) == 0) return false;
        }

        return true;

    }

    /**
     * @brief Checks if an integral type is composite (is above 1 and has factors outside itself). 
     *
     * @tparam T Any integral numerical type.
     * @param Value The value to be checked.
     * @return True if the number is composite, otherwise false.
     */
    template <IntegralNumerical T>
    bool IsComposite(T Value) {
        return Value > 1 && !IsPrime(Value);
    }

/*
==================================================================================================================================================================================
COMPARTIVE FUNCTIONS

    Functions that compare numbers

==================================================================================================================================================================================
*/

    /**
     * @brief Checks if two values are equal to an expected sum. 
     *
     * @tparam T Any numerical type.
     * @param Value1 The first value to be checked.
     * @param Value2 The second value to be checked.
     * @param ExpectedSum The expected sum.
     * @return True if the number is sum aligns with the expected sum, otherwise false.
     */
    template <Numerical T>
    bool IsThisRight(const T Value1, const T Value2, const T ExpectedSum) {
        return (Value1 + Value2) == ExpectedSum;
    }

    /**
     * @brief Checks if a value is within a given range inclusive of the bounds.
     *
     * @tparam T Any numerical type.
     * @param Value The value to be checked.
     * @param LowerBound The lower bound of the prescribed range.
     * @param UpperBound The upper bound of the prescribed range.
     * @return True if the number is within the defined range, otherwise false.
     */
    template <Numerical T>
    bool InRange(const T Value, const T LowerBound, const T UpperBound) {
        if (Value >= LowerBound && Value <= UpperBound) return true;
        return false;
    }

    /**
     * @brief Checks if a value is within a given range exclusive of the bounds.
     *
     * @tparam T Any numerical type.
     * @param Value The value to be checked.
     * @param LowerBound The lower bound of the prescribed range.
     * @param UpperBound The upper bound of the prescribed range.
     * @return True if the number is within the defined range, exclsuive, otherwise false.
     */
    template <Numerical T>
    bool InRangeExclusive(const T Value, const T LowerBound, const T UpperBound) {
        if (Value > LowerBound && Value < UpperBound) return true;
        return false;
    }

    /**
     * @brief Checks if two floating point types are approximately equal.
     *
     * @tparam T Any floating point type.
     * @param Value1 The first value to be checked.
     * @param Value2 The second value to be checked.
     * @return True if the numbers are approximately equal, otherwise false.
     */
    template <FloatingPoint T>
    bool IsApproximatelyEqual(T Value1, T Value2) {

        T absEpsilon = std::numeric_limits<T>::epsilon() * 100;
        T relEpsilon = std::numeric_limits<T>::epsilon() * 10;

        T Threshold = std::max(absEpsilon, relEpsilon * std::max(std::abs(Value1), std::abs(Value2)));

        return(InRange(Value1, Value2 - Threshold, Value2 + Threshold));

    }

    /**
     * @brief Checks if one number divides into another without a remainder.
     *
     * @tparam T Any integral type.
     * @param Numerator The numerator.
     * @param Denominator The denominator.
     * @return True if the numbers are factors of the each other, otherwise false.
     */
    template <Integral T>
    bool IsDivisibleBy(T Numerator, T Denominator) {
        return Numerator % Denominator == 0;
    }

    /**
     * @brief 
     *
     * @tparam T Any integral type.
     * @param Value The number to be factorised from.
     * @param Factor Factor to be checked against Value.
     * @return Returns the quotient of the two values, else zero.
     */
    template <Integral T>
    T GetQuotient(T Value, T Factor) {
        if (!IsDivisibleBy(Value, Factor)) {
            return 0;
        }

        return Value / Factor;

    }

/*
==================================================================================================================================================================================
OPERATIVE FUNCTIONS

    Functions that change numbers.

==================================================================================================================================================================================
*/

    /**
     * @brief 
     *
     */
    template <Numerical T>
    T Add(T Value1, T Value2) {

        return Value1 + Value2;

    }

    /**
     * @brief 
     *
     */
    template <Numerical T>
    T Square(T Value) {

        return Value * Value;

    }


/*
==================================================================================================================================================================================
EVALUATIVE FUNCTIONS

    Functions that evaluate nummbers.

==================================================================================================================================================================================
*/

    template <IsSimpleNumericalContainer Container>
    float Average(const Container& NumericalContainer) {

        using NumericalType = typename Container::value_type;

        float Sum = 0;
        size_t Iterator = 0;
        
        for (const NumericalType Element : NumericalContainer) {
            Sum += Element;
            Iterator++;
        }

        if (Iterator == 0) return 0;

        return Sum / Iterator;

    }

    template <IsSimpleNumericalContainer Container>
    typename Container::value_type AverageType(const Container& NumericalContainer) {

        using NumericalType = typename Container::value_type;

        NumericalType Sum = 0;
        size_t Iterator = 0;
        
        for (const NumericalType Element : NumericalContainer) {
            Sum += Element;
            Iterator++;
        }

        if (Iterator == 0) return 0;

        return Sum / Iterator;

    }


/*
==================================================================================================================================================================================
GENERATIVE FUNCTIONS

    Functions that produce numbers.

==================================================================================================================================================================================
*/

    float RandFloatInRange(float Minimum, float Maximum);
    
    std::vector<int> GeneratePrimes(size_t Limit);

    std::vector<int> GenerateComposites(size_t Limit);

}




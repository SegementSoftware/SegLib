#include <vector>
#include <algorithm>
#include <iostream>
#include <concepts>

#include "SegLibConcepts.h"

namespace SLV {

/*
==================================================================================================================================================================================
MODIFICATION FUNCTIONS

    Functions centred around generic vector modifications

==================================================================================================================================================================================
*/

    /**
     * @brief Appends one vector to another
     *
     * @tparam T Vector element type.
     * @param Vector1 A constant reference to the vector that will be appended to.
     * @param Vector2 A constant reference to the vector that will be appended.
     * @return The resulting vector of appending Vector2 to Vector1
     */
    template <typename T>
    std::vector<T> Append(const std::vector<T>& Vector1, const std::vector<T>& Vector2) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector1.size() + Vector2.size());

        ReturnVector.insert(ReturnVector.end(), Vector1.begin(), Vector1.end());
        ReturnVector.insert(ReturnVector.end(), Vector2.begin(), Vector2.end());

        return ReturnVector;

    }

    /**
     * @brief Erases a given index from a given vector.
     *
     * @tparam T Vector element type.
     * @param Vector A constant reference to the vector that will be erased from.
     * @param Index The index for erasure.
     * @return The resulting vector of erasing Vector[Index]. If Index is out of range, an unedited copy will be returned.
     * @note To avoid undefined behaviour this function checks if Index is within the bounds of Vector, this is unoptimised.
     */
    template <typename T>
    std::vector<T> Erase(const std::vector<T>& Vector, size_t Index) {

        if (Index >= Vector.size() || Vector.empty()) {
            return Vector;
        }

        std::vector<T> ReturnVector = Vector;
        ReturnVector.erase(ReturnVector.begin() + Index);
        return ReturnVector;


    }

    template <typename T>
    void Erase_p(std::vector<T>& Vector, size_t Index) {

        Vector.erase(Vector.begin() + Index);

    }

    /**
     * @brief Removes internal duplicate elements from a vector, preserving the order of first appearance.
     *
     * @tparam T Any type with an equality (operator==) definition. The type does not require operator< definition.
     * @param Vector A reference to the vector to be processed.
     * Its contents will be modified in-place to contain only unique elements.
     * @return The number of elements that were removed from the vector (i.e., the count of duplicates found).
     * @note MakeUniqueInPlace is designed to be as generic as possible, it contains a nested loop and may create bottlenecks in massive datasets.
     * @warning The input vector 'Vector' is modified directly.
     */
    template <EqualityCompatible T>
    size_t MakeUniqueInPlace(std::vector<T>& Vector) {

        std::vector<T> ResultVector;
        ResultVector.reserve(Vector.size());

        size_t RemovedElements = 0;

        for (const T& CurrentItem : Vector) {

            bool Duplicate = false;

            for (const T& ExistingItem : ResultVector) {

                if (CurrentItem == ExistingItem) {
                    Duplicate = true;
                    RemovedElements++;
                    break;
                } 

            }

            if (!Duplicate) {
                ResultVector.emplace_back(CurrentItem);
            }
            
        }

        Vector = std::move(ResultVector);

        return RemovedElements;
        
    }

/*
==================================================================================================================================================================================
CREATE FUNCTIONS

    Functions centred around vector comparison and the creation of new vectors

==================================================================================================================================================================================
*/


    /**
     * @brief Combines two vectors, and removes any duplicates. Order will not be maintained in the resulting vector.
     *
     * @tparam T Any type with an equality (operator==) definition. The type does not require operator< definition.
     * @param Vector1 A constant reference to the first source vector.
     * @param Vector2 A constant reference to the second source vector.
     * @return The unified vector.
     * @note CreateUnion is designed to be as generic as possible, it contains a nested loop and may create bottlenecks in massive datasets.
     */
    template <EqualityCompatible T>
    std::vector<T> CreateUnion(const std::vector<T>& Vector1, const std::vector<T>& Vector2) {

        std::vector<T> UnionVector = Append(Vector1, Vector2);
        MakeUniqueInPlace(UnionVector);
        return UnionVector;

    }

    /**
     * @brief Combines like elements within two vectors. Order is from Vector1 is maintained.
     * 
     *
     * @tparam T Any type with an equality (operator==) definition. The type does not require operator< definition.
     * @param Vector1 A constant reference to the first source vector, the element order from this vector will be preserved.
     * @param Vector2 A constant reference to the second source vector.
     * @return A vector that contains elements present in both datasets.
     * @note CreateIntersectional is designed to be as generic as possible, it contains a nested loop and may create bottlenecks in massive datasets.
     */
    template <EqualityCompatible T>
    std::vector<T> CreateIntersectional(const std::vector<T>& Vector1, const std::vector<T>& Vector2) {

        std::vector<T> IntersectionalVector;
        IntersectionalVector.reserve(std::min(Vector1.size(), Vector2.size()));

        for (const T& CurrentElement : Vector1) {

            bool Intersection = false;

            for (const T& ExistingElement : Vector2) {

                if (CurrentElement == ExistingElement) {
                    Intersection = true;
                    break;
                }

            }

            if (Intersection) {
                IntersectionalVector.emplace_back(CurrentElement);
            }

        }

        MakeUniqueInPlace(IntersectionalVector);

        return IntersectionalVector;

    }

    /**
     * @brief Creates a vector of elements present in BaseVector, but not in ComparisonVector.
     * 
     *
     * @tparam T Any type with an equality (operator==) definition. The type does not require operator< definition.
     * @param BaseVector A constant reference to the vector that the differential will be derived from.
     * @param ComparisonVector A constant reference to the vector the base vector will be compared against.
     * @return A vector that contains elements present in BaseVector, and not in ComparisonVector.
     * @note CreateDifferential is designed to be as generic as possible, it contains a nested loop and may create bottlenecks in massive datasets.
     */
    template <EqualityCompatible T>
    std::vector<T> CreateDifferential(const std::vector<T>& BaseVector, const std::vector<T>& ComparisonVector) {

        std::vector<T> DifferentialVector;
        DifferentialVector.reserve(BaseVector.size() + ComparisonVector.size());

        for (const T& CurrentElement : BaseVector) {

            bool Difference = true;

            for (const T& ExistingElement : ComparisonVector) {

                if (CurrentElement == ExistingElement) {
                    Difference = false;
                    break;
                }

            }

            if (Difference) {
                DifferentialVector.emplace_back(CurrentElement);
            }

        }

        MakeUniqueInPlace(DifferentialVector);

        return DifferentialVector;

    }

    template <EqualityCompatible T>
    std::vector<T> CreateSymmeticalDifference(const std::vector<T>& Vector1, const std::vector<T>& Vector2) {
        std::vector<T> SymmeticalDifferenceVector = Append(CreateDifferential(Vector1, Vector2), CreateDifferential(Vector2, Vector1));
        MakeUniqueInPlace(SymmeticalDifferenceVector);
        return SymmeticalDifferenceVector;
    }




/*
==================================================================================================================================================================================
QUERY FUNCTIONS

    Functions centred around finding out information about vectors

==================================================================================================================================================================================
*/

    /**
     * @brief Checks if a vector contains an instance of a given value.
     *
     * @tparam T Any type with an equality (operator==) definition.
     * @param Vector A constant reference to the source vector.
     * @param Element A constant reference to the value to be checked.
     * @return True if the vector does contain the element, otherwise false.
     */
    template <EqualityCompatible T>
    bool ContainsElement(const std::vector<T>& Vector, const T& Element) {
        
        for (const T& CurrentElement : Vector) {
            if (CurrentElement == Element) {
                return true;
            }
        }

        return false;

    }

    /**
     * @brief Finds the index of a given element within a vector.
     *
     * @tparam T Any type with an equality (operator==) definition.
     * @param Vector A constant reference to the source vector.
     * @param Element A constant reference to the value to be checked.
     * @return The index of the element, otherwise -1.
     */
    template <EqualityCompatible T>
    int FindElement(const std::vector<T>& Vector, const T& Element) {

        for (int i = 0; i < Vector.size(); i++) {
            if (Vector[i] == Element) {
                return i;
            }
        }

        return -1;
        
    }

    /**
     * @brief Finds the indices of a given element within a vector.
     *
     * @tparam T Any type with an equality (operator==) definition.
     * @param Vector A constant reference to the source vector.
     * @param Element A constant reference to the value to be checked.
     * @return A vector containing all indices that are equal to the given element.
     */
    template <EqualityCompatible T>
    std::vector<int> FindAllElement(const std::vector<T>& Vector, const T& Element) {

        std::vector<int> Positions = {};

        for (int i = 0; i < Vector.size(); i++) {
            if (Vector[i] == Element) {
                Positions.push_back(i);
            }
        }

        return Positions;
        
    }

    /**
     * @brief Counts the instances of a given element within a vector.
     *
     * @tparam T Any type with an equality (operator==) definition.
     * @param Vector A constant reference to the source vector.
     * @param Element A constant reference to the value to be checked.
     * @return An integer equal to the number of times a given element appears within a vector.
     */
    template <EqualityCompatible T>
    int CountElement(const std::vector<T>& Vector, const T& Element) {

        int Counter = 0;

        for (int i = 0; i < Vector.size(); i++) {
            if (Vector[i] == Element) {
                Counter++;
            }
        }

        return Counter;
        
    }

/*
==================================================================================================================================================================================
DELETION FUNCTIONS

    Functions centred around removing elements from vectors

==================================================================================================================================================================================
*/


    /**
     * @brief Creates a vector of elements that ConditionalFunc evaluates as true.
     * 
     *
     * @tparam T Any type compatible with ConditionalFunc.
     * @tparam Condition Any function that returns a boolean.
     * @param Vector A constant reference to the vector that the conditional tests will be performed on.
     * @param ConditionalFunc The function that will establish the predicate for an element to be included.
     * @return A vector that contains elements present in Vector that fulfill the conditions of ConditionalFunc.
     * @note ConditionalFunc must return true for an element from Vector to be included in the returned vector.
     */
    template <typename T, typename Condition>
    std::vector<T> ConditionalInclusion(const std::vector<T>& Vector, Condition ConditionalFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            if (ConditionalFunc(CurrentElement)) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }


    /**
     * @brief Edits a vector to only include elements ConditionalFunc evaluates as true.
     * 
     *
     * @tparam T Any type compatible with ConditionalFunc.
     * @tparam Condition Any function that returns a boolean.
     * @param Vector A mutable reference to the vector that the conditional tests will be performed on.
     * @param ConditionalFunc The function that will establish the predicate for an element to be included.
     * @return The number of elements removed from Vector.
     * @note ConditionalFunc must return true for an element from Vector remain within the vector.
     */
    template <typename T, typename Condition>
    size_t ConditionalInclusion_p(std::vector<T>& Vector, Condition ConditionalFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        size_t ElementsRemoved = 0;

        for (const T& CurrentElement : Vector) {

            if (ConditionalFunc(CurrentElement)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        Vector = std::move(ReturnVector);

        return ElementsRemoved;

    }


    /**
     * @brief Creates a vector of elements that ConditionalFunc evaluates as false.
     * 
     *
     * @tparam T Any type compatible with ConditionalFunc.
     * @tparam Condition Any function that returns a boolean.
     * @param Vector A constant reference to the vector that the conditional tests will be performed on.
     * @param ConditionalFunc The function that will establish the predicate for an element to be excluded.
     * @return A vector that contains elements present in Vector that ConditionalFunc evaluates to false.
     * @note ConditionalFunc must return false for an element from Vector to be included in the returned vector.
     */
    template <typename T, typename Condition>
    std::vector<T> ConditionalExclusion(const std::vector<T>& Vector, Condition ConditionalFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            if (!ConditionalFunc(CurrentElement)) {
                ReturnVector.emplace_back(CurrentElement);
            }

        }

        return ReturnVector;

    }


    /**
     * @brief Edits a vector to only include elements that ConditionalFunc evaluates as false.
     * 
     *
     * @tparam T Any type compatible with ConditionalFunc.
     * @tparam Condition Any function that returns a boolean.
     * @param Vector A mutable reference to the vector that the conditional tests will be performed on.
     * @param ConditionalFunc The function that will establish the predicate for an element to be excluded.
     * @return  The number of elements removed from Vector.
     * @note ConditionalFunc must return false for an element from Vector to remain within the vector.
     */
    template <typename T, typename Condition>
    size_t ConditionalExclusion_p(std::vector<T>& Vector, Condition ConditionalFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        size_t ElementsRemoved = 0;

        for (const T& CurrentElement : Vector) {

            if (!ConditionalFunc(CurrentElement)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }

        }

        Vector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    /**
     * @brief Creates a vector of elements that ConditionalFunc evaluates as true.
     * 
     *
     * @tparam T Any type compatible with ConditionalFunc.
     * @tparam Comparison Any function compatible with type T that returns a boolean.
     * @param Vector A constant reference to the vector that the conditional tests will be performed on.
     * @param CompVar A constant reference to the variable to compare Vector[i] against.
     * @param ComparativeFunc The function that will establish the predicate for an element to be included.
     * @return A vector that contains elements present in Vector that fulfill the conditions of ComparativeFunc.
     * @note ComparativeFunc must return true for an element from Vector to be included in the returned vector.
     */
    template <typename T, typename Comparison>
    std::vector<T> ComparativeInclusion(const std::vector<T>& Vector, const T& CompVar, Comparison ComparativeFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            if (ComparativeFunc(CurrentElement, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }


    template <typename T, typename Comparison>
    size_t ComparativeInclusion_p(std::vector<T>& Vector, const T& CompVar, Comparison ComparativeFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        size_t ElementsRemoved = 0;

        for (const T& CurrentElement : Vector) {

            if (ComparativeFunc(CurrentElement, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            } 
 
        }

        Vector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    /**
     * @brief Creates a vector of elements that ConditionalFunc evaluates as false.
     * 
     *
     * @tparam T Any type compatible with ConditionalFunc.
     * @tparam Comparison Any function compatible with type T that returns a boolean.
     * @param Vector A constant reference to the vector that the conditional tests will be performed on.
     * @param CompVar A constant reference to the variable to compare Vector[i] against.
     * @param ComparativeFunc The function that will establish the predicate for an element to be included.
     * @return A vector that contains elements present in Vector that do not fulfill the conditions of ComparativeFunc.
     * @note ComparativeFunc must return false for an element from Vector to be included in the returned vector.
     */
    template <typename T, typename Comparison>
    std::vector<T> ComparativeExclusion(const std::vector<T>& Vector, const T& CompVar, Comparison ComparativeFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            if (!ComparativeFunc(CurrentElement, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template <typename T, typename Comparison>
    size_t ComparativeExclusion_p(std::vector<T>& Vector, const T& CompVar, Comparison ComparativeFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        size_t ElementsRemoved = 0;

        for (const T& CurrentElement : Vector) {

            if (!ComparativeFunc(CurrentElement, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        Vector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    /**
     * @brief Creates a vector of elements that are equal to a given variable.
     * 
     *
     * @tparam T Any type with an equality operator
     * @param Vector A constant reference to the vector that the equality checks will be performed on.
     * @param CompVar A constant reference to the variable to compare Vector[i] against.
     * @return A vector that contains elements present in Vector that are equal to CompVar.
     */
    template <EqualityCompatible T>
    std::vector<T> EqualityInclusion(const std::vector<T>& Vector, const T& CompVar) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            if (CurrentElement == CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template <EqualityCompatible T>
    size_t EqualityInclusion_p(std::vector<T>& Vector, const T& CompVar) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        size_t ElementsRemoved = 0;

        for (const T& CurrentElement : Vector) {

            if (CurrentElement == CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        Vector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    /**
     * @brief Creates a vector of elements that are not equal to a given variable.
     * 
     *
     * @tparam T Any type with an equality operator
     * @param Vector A constant reference to the vector that the equality checks will be performed on.
     * @param CompVar A constant reference to the variable to compare Vector[i] against.
     * @return A vector that contains elements present in Vector that are not equal to CompVar.
     */
    template <EqualityCompatible T, typename Comparison>
    std::vector<T> EqualityExclusion(const std::vector<T>& Vector, const T& CompVar) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            if (CurrentElement != CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template <EqualityCompatible T, typename Comparison>
    size_t EqualityExclusion_p(std::vector<T>& Vector, const T& CompVar) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        size_t ElementsRemoved = 0;

        for (const T& CurrentElement : Vector) {

            if (CurrentElement != CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        Vector = std::move(ReturnVector);

        return ElementsRemoved;

    }



/*
==================================================================================================================================================================================
TRANSFORMATIONAL FUNCTIONS

    Functions centred around changing elements within a given vector.

==================================================================================================================================================================================
*/

    /**
     * @brief Creates a vector of elements based on Vector, transformed by TransformationalFunc.
     * 
     *
     * @tparam T Any type compatible with TransformationFunc.
     * @tparam Transformation Any function that can accept T as an argument.
     * @param Vector A constant reference to the vector providing a base case for the returned vector.
     * @param TransformationFunc The function that will perform the transformational opperation.
     * @return A vector that contains elements present in Vector after being transformed by TransformationFunc.
     */
    template <typename T, typename R, typename Transformation>
    std::vector<R> Transform(const std::vector<T>& Vector, Transformation TransformationFunc) {

        std::vector<R> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            ReturnVector.emplace_back(TransformationFunc(CurrentElement));

        }

        return ReturnVector;

    }

    template <typename T, typename OperationVariable, typename Operation>
    std::vector<T> Operate(const std::vector<T>& Vector, const OperationVariable& OperativeVar, Operation OperativeFunc) {

        std::vector<T> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            ReturnVector.emplace_back(OperativeFunc(CurrentElement, OperativeVar));

        }

        return ReturnVector;

    }

    template <typename T, typename OperationVariable, typename Operation>
    void Operate_p(std::vector<T>& Vector, const OperationVariable& OperativeVar, Operation OperativeFunc) {

        for (T& CurrentElement : Vector) {

           CurrentElement = OperativeFunc(CurrentElement, OperativeVar);

        }

    }

    template <typename T, typename OperationVariable, typename R, typename Operation>
    std::vector<R> OperativeTransform(const std::vector<T>& Vector, const OperationVariable& OperativeVar, Operation OperativeFunc) {

        std::vector<R> ReturnVector;
        ReturnVector.reserve(Vector.size());

        for (const T& CurrentElement : Vector) {

            ReturnVector.emplace_back(OperativeFunc(CurrentElement, OperativeVar));

        }

        return ReturnVector;

    }

/*
==================================================================================================================================================================================
VECTOR DISTRIBUTION FUNCTIONS

    Functions that accomodate vectors

==================================================================================================================================================================================
*/

    template<typename T>
    std::vector<std::vector<T>>Distribute(const std::vector<T>& Vector, size_t Distributions, bool ForceEqualDistribution) {
        
        std::vector<std::vector<T>> ReturnVector;
        ReturnVector.reserve(Distributions);

        if (Distributions <= 1) {
            ReturnVector.emplace_back(Vector);
            return ReturnVector;
        }

        size_t Indices = Vector.size() / Distributions;
        size_t Index = 0;

        for (size_t i = 0; i < Distributions; i++) {

            std::vector<T> CurrentDistribution;
            CurrentDistribution.reserve(Indices);
            
            for (size_t x = 0; x < Indices; x++) {

                CurrentDistribution.emplace_back(Vector[Index]);
                Index++;

            }

            ReturnVector.emplace_back(CurrentDistribution);

        }

        if (ForceEqualDistribution) {
            return ReturnVector;
        }

        size_t Remainder = Vector.size() % Distributions;

        for (size_t i = 0; i < Remainder; i++) {

            ReturnVector[i].push_back(Vector[Index]);
            Index++;

        }

        return ReturnVector;

    }

    template<typename T>
    std::vector<std::vector<T>>Distribute(const std::vector<T>& Vector, size_t Distributions) {
        return Distribute(Vector, Distributions, false);
    }


/*
==================================================================================================================================================================================
IOSTREAM FUNCTIONS

    Functions centred around printing elements in a vector.

==================================================================================================================================================================================
*/

    /**
     * @brief Prints all the elements within a vector.
     * 
     *
     * @tparam T Any type compatible with an iostream compatible pipe operator<<.
     * @param Vector A constant reference to the vector that will be printed.
     */
    template <Streamable T>
    void Print(const std::vector<T>& Vector) {

        std::cout << std::endl;

        for (const T& Element : Vector) {
            std::cout << Element << "\n";
        }

        std::cout << std::endl;

    }

}
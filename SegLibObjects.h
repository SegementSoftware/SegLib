#include "SegLibConcepts.h"

#include <iostream>

namespace SLO {

/*
==================================================================================================================================================================================
    
    These functions are a bit more difficult to use than most other SegLib functions, but are also SegLib's most powerful functions. 

    

        Remember that MemberTypes (attribute pointers) are invoked thusly:

            &ObjectType::AttributeName

==================================================================================================================================================================================

==================================================================================================================================================================================
STRUCTURES AND CLASSES

    Seglib's built in structures and classes

==================================================================================================================================================================================
*/

    template <typename ClassType, typename MemberType>
    requires HasAccessibleMember<ClassType, MemberType>
    class LinkedMember {

        private:

        ClassType& ClassPtr;
        MemberType ClassType::*MemberTypePtr;

        public:

        MemberType Member;

        LinkedMember(ClassType& ParentClassPtr, MemberType ClassType::*PtrToMember)
        
        :   ClassPtr(ParentClassPtr),
            MemberTypePtr(PtrToMember),
            Member(ClassPtr.*MemberTypePtr)
        
        
        {

        }

        ClassType* GetClass() {
            return &ClassPtr;
        }

        ClassType CopyClass() {
            return ClassPtr;
        }

        void Restore() {
            Member = ClassPtr.*MemberTypePtr;
        }

        void Commit() {
            ClassPtr.*MemberTypePtr = Member;
        }

    };

/*
==================================================================================================================================================================================
COMPARISON FUNCTIONS

    Functions centred around comparing one class to another

==================================================================================================================================================================================
*/


    /**
     * @brief Checks if two class members are equal.
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be equality comparable with the type it will be checked against (not necessarily identical).
     * @param Object1 An instance of the object MemberType1 will be derived from.
     * @param Member1 A generic pointer to the desired attribute from Object1.
     * @param Object2 An instance of the object MemberType2 will be derived from.
     * @param Member2 A generic pointer to the desired attribute from Object2.
     * @return True if the attribute's equality check returns true, else false.
     */
    template<typename ClassType1, typename MemberType1,
            typename ClassType2, typename MemberType2>
        requires HasAccessibleMember<ClassType1, MemberType1> &&
                HasAccessibleMember<ClassType2, MemberType2> &&
                std::equality_comparable_with<MemberType1, MemberType2>
    bool Compare(
        const ClassType1& Object1, MemberType1 ClassType1::*Member1,
        const ClassType2& Object2, MemberType2 ClassType2::*Member2) {

            return (Object1.*Member1) == (Object2.*Member2);

    }

    /**
     * @brief Checks if two class members satisfy a predicate function.
     * 
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be compatible with the Predicate function.
     * @tparam Predicate A function that is compatible with both MemberTypes and evaluates to a value that can be converted to a boolean.
     * @param Object1 An instance of theobject MemberType1 will be derived from.
     * @param Member1 A generic pointer to the desired attribute from Object1.
     * @param Object2 An instance of the object MemberType2 will be derived from.
     * @param Member2 A generic pointer to the desired attribute from Object2.
     * @param ConditionalFunc A function that can take Member1 and Member2 as arguments and evaluate to a boolean.
     * @return True if the predicate evaluates true, else false.
     * 
     * @note Technically, writing a specific function saves a function call, so for thousands of checks, I would recommend against this function.
     */
    template<typename ClassType1, typename MemberType1,
            typename ClassType2, typename MemberType2,
            typename Predicate>
        requires HasAccessibleMember<ClassType1, MemberType1> &&
                HasAccessibleMember<ClassType2, MemberType2> &&
                std::invocable<Predicate, const MemberType1&, const MemberType2&> &&
                std::convertible_to<std::invoke_result_t<Predicate, const MemberType1&, const MemberType2&>, bool>
    bool ComparePredicate(
        const ClassType1& Object1, MemberType1 ClassType1::*Member1,
        const ClassType2& Object2, MemberType2 ClassType2::*Member2,
        Predicate ConditionalFunc) {

            return ConditionalFunc(Object1.*Member1, Object2.*Member2);

    }

    /**
     * @brief Checks a class attribute is equal to a given variable.
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be equality comparable with the type it will be checked against (not necessarily identical).
     * @tparam ComparisonVariable A variable that is equality comparable with MemberType.
     * @param Object1 An instance of the object MemberType1 will be derived from.
     * @param Member1 A generic pointer to the desired attribute from Object1.
     * @param CompVar The comparison variable, equality comparable with the type of Object1.
     * @return True if the attribute's equality check returns true, else false.
     */
    template<typename ClassType1, typename MemberType1,
             typename ComparisonVariable>
    requires HasAccessibleMember<ClassType1, MemberType1> &&
             std::equality_comparable_with<MemberType1, ComparisonVariable>
    bool CompareVariable(const ClassType1& Object1, MemberType1 ClassType1::*Member1, const ComparisonVariable& CompVar) {
        return (Object1.*Member1) == (CompVar);
    }

    /**
     * @brief Checks if a class member and an arbitrary variable satisfy a predicate function.
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be compatible with the Predicate function.
     * @tparam ComparisonVariable A variable that is compatible with the given predicate function.
     * @tparam Predicate A function that is compatible with MemberType and ComparisonVariable and evaluates to a value that can be converted to a boolean.
     * @param Object1 An instance of the object MemberType1 will be derived from.
     * @param Member1 A generic pointer to the desired attribute from Object1.
     * @param CompVar The comparison variable, equality comparable with the type of Object1.
     * @param ConditionalFunc A function that can take Member1 and CompVar as arguments and evaluate to a boolean.
     * @return True if the predicate evaluates true, else false.
     * 
     * @note Technically, writing a specific function saves a function call, so for thousands of checks, I would recommend against this function.
     */
    template<typename ClassType1, typename MemberType1,
             typename ComparisonVariable,
             typename Predicate>
    requires HasAccessibleMember<ClassType1, MemberType1> &&
             std::invocable<Predicate, const MemberType1&, const ComparisonVariable&> &&
             std::convertible_to<std::invoke_result_t<Predicate, const MemberType1&, const ComparisonVariable&>, bool>
    bool CompareVariablePredicate(const ClassType1& Object1, MemberType1 ClassType1::*Member1, const ComparisonVariable& CompVar, Predicate ConditionalFunc) {
        return ConditionalFunc(Object1.*Member1, CompVar);
    }

    template<typename ClassType, typename MemberType,
             typename OperationVariable,
             typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&, const OperationVariable&> &&
             std::convertible_to<std::invoke_result_t<Operation, const MemberType&, const OperationVariable&>, MemberType>
    ClassType Operate(const ClassType& Object, MemberType ClassType::*Member, const OperationVariable& OperationVar, Operation OperativeFunc) {
        
        ClassType Copy = Object;

        Copy.*Member = OperativeFunc(Copy.*Member, OperationVar);


        return Copy;

    }

    template<typename ClassType, typename MemberType,
             typename OperationVariable,
             typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&, const OperationVariable&> &&
             std::convertible_to<std::invoke_result_t<Operation, MemberType&, const OperationVariable&>, MemberType>
    void Operate_p(ClassType& ObjectVector, MemberType ClassType::*Member, const OperationVariable& OperationVar, Operation OperativeFunc) {

        ObjectVector.*Member = OperativeFunc(ObjectVector.*Member, OperationVar);

    }

    template<typename ClassType, typename MemberType, typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Operation, const MemberType&>, MemberType>
    ClassType Operate(const ClassType& Object, MemberType ClassType::*Member, Operation OperativeFunc) {
        
        ClassType Copy = Object;

        Copy.*Member = OperativeFunc(Copy.*Member);


        return Copy;

    }

    template<typename ClassType, typename MemberType, typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Operation, MemberType&>, MemberType>
    void Operate_p(ClassType& ObjectVector, MemberType ClassType::*Member, Operation OperativeFunc) {

        ObjectVector.*Member = OperativeFunc(ObjectVector.*Member);

    }

/*
==================================================================================================================================================================================
VECTOR FUNCTIONS

    Functions that accomodate vectors

==================================================================================================================================================================================
*/

    /**
     * @brief Creates a vector of ClassType objects with a prescribed member equal to CompVar.
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be equality comparable with ComparisonVariable.
     * @tparam ComparisonVariable A variable that is equality comparable with ClassType::MemberType.
     * @param ObjectVector A constant reference to a vector of type ClassType.
     * @param Member A generic pointer to the desired attribute from instances of ClassType.
     * @param CompVar The comparison variable, equality comparable with with ClassType::MemberType.
     * @return A vector of type ClassType containing objects with a prescribed member equal to Compvar.
     */
    template<typename ClassType, typename MemberType,
             typename ComparisonVariable>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::equality_comparable_with<MemberType, ComparisonVariable>
    std::vector<ClassType> EqualityInclusion(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            if (CurrentElement.*Member == CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    /**
     * @brief Edits a vector of ClassType objects to include only objects with ClassObject::MemberType equal to Compvar
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be equality comparable with ComparisonVariable.
     * @tparam ComparisonVariable A variable that is equality comparable with ClassType::MemberType.
     * @param ObjectVector A intentionally mutable reference to a vector of type ClassType.
     * @param Member A generic pointer to the desired attribute from instances of ClassType.
     * @param CompVar The comparison variable, equality comparable with with ClassType::MemberType.
     * @return The number of elements removed from ObjectVector.
     */
    template<typename ClassType, typename MemberType,
             typename ComparisonVariable>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::equality_comparable_with<MemberType, ComparisonVariable>
    size_t EqualityInclusion_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        size_t ElementsRemoved = 0;

        for (const ClassType& CurrentElement : ObjectVector) {

            if (CurrentElement.*Member == CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        ObjectVector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    /**
     * @brief Creates a vector of ClassType objects with a prescribed member that is not equal to CompVar.
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be equality comparable with ComparisonVariable.
     * @tparam ComparisonVariable A variable that is equality comparable with ClassType::MemberType.
     * @param ObjectVector A constant reference to a vector of type ClassType.
     * @param Member A generic pointer to the desired attribute from instances of ClassType.
     * @param CompVar The comparison variable, equality comparable with with ClassType::MemberType.
     * @return A vector of type ClassType containing objects with a prescribed member not equal to Compvar.
     */
    template<typename ClassType, typename MemberType,
             typename ComparisonVariable>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::equality_comparable_with<MemberType, ComparisonVariable>
    std::vector<ClassType> EqualityExclusion(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            if (CurrentElement.*Member != CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType,
             typename ComparisonVariable>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::equality_comparable_with<MemberType, ComparisonVariable>
    size_t EqualityExclusion_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        size_t ElementsRemoved = 0;

        for (const ClassType& CurrentElement : ObjectVector) {

            if (CurrentElement.*Member != CompVar) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        ObjectVector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    /**
     * @brief Creates a vector of ClassType objects that satisfy the condition of ConditionalFunc.
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be compatible with Predicate.
     * @tparam ComparisonVariable A variable that is compatible with Predicate.
     * @param ObjectVector A constant reference to a vector of type ClassType.
     * @param Member A generic pointer to the desired attribute from instances of ClassType.
     * @param CompVar The comparison variable, compatible with Predicate.
     * @return A vector of type ClassType containing objects with a prescribed member not equal to Compvar.
     */
    template<typename ClassType, typename MemberType, typename Predicate>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Predicate, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Predicate, const MemberType&>, bool>
    std::vector<ClassType> ConditionalInclusion(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, Predicate ConditionalFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            if (ConditionalFunc(CurrentElement.*Member)) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType, typename Predicate>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Predicate, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Predicate, const MemberType&>, bool>
    size_t ConditionalInclusion_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, Predicate ConditionalFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        size_t ElementsRemoved = 0;

        for (const ClassType& CurrentElement : ObjectVector) {

            if (ConditionalFunc(CurrentElement.*Member)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        ObjectVector = std::move(ReturnVector);

        return ElementsRemoved;

    }


    /**
     * @brief Creates a vector of ClassType objects that satisfy the condition of ConditionalFunc.
     *
     * @tparam ClassType The class a given attribute is read from, must have MemberType as an accessible member (not private).
     * @tparam MemberType The type of a given attribute, must be compatible with Predicate.
     * @tparam ComparisonVariable A variable that is compatible with Predicate.
     * @param ObjectVector A constant reference to a vector of type ClassType.
     * @param Member A generic pointer to the desired attribute from instances of ClassType.
     * @param CompVar The comparison variable, compatible with Predicate.
     * @return A vector of type ClassType containing objects with a prescribed member not equal to Compvar.
     */
    template<typename ClassType, typename MemberType,
             typename ComparisonVariable,
             typename Comparative>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Comparative, const MemberType&, const ComparisonVariable&> &&
             std::convertible_to<std::invoke_result_t<Comparative, const MemberType&, const ComparisonVariable&>, bool>
    std::vector<ClassType> ComparativeInclusion(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar, Comparative ComparativeFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            if (ComparativeFunc(CurrentElement.*Member, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType,
             typename ComparisonVariable,
             typename Comparative>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Comparative, const MemberType&, const ComparisonVariable&> &&
             std::convertible_to<std::invoke_result_t<Comparative, const MemberType&, const ComparisonVariable&>, bool>
    size_t ComparativeInclusion_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar, Comparative ComparativeFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        size_t ElementsRemoved = 0;

        for (const ClassType& CurrentElement : ObjectVector) {

            if (ComparativeFunc(CurrentElement.*Member, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        ObjectVector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    template<typename ClassType, typename MemberType, typename Predicate>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Predicate, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Predicate, const MemberType&>, bool>
    std::vector<ClassType> ConditionalExclusion(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, Predicate ConditionalFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            if (!ConditionalFunc(CurrentElement.*Member)) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType, typename Predicate>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Predicate, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Predicate, const MemberType&>, bool>
    size_t ConditionalExclusion_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, Predicate ConditionalFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        size_t ElementsRemoved = 0;

        for (const ClassType& CurrentElement : ObjectVector) {

            if (!ConditionalFunc(CurrentElement.*Member)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        ObjectVector = std::move(ReturnVector);

        return ElementsRemoved;

    }

    template<typename ClassType, typename MemberType,
             typename ComparisonVariable,
             typename Comparative>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Comparative, const MemberType&, const ComparisonVariable&> &&
             std::convertible_to<std::invoke_result_t<Comparative, const MemberType&, const ComparisonVariable&>, bool>
    std::vector<ClassType> ComparativeExclusion(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar, Comparative ComparativeFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            if (!ComparativeFunc(CurrentElement.*Member, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            }
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType,
             typename ComparisonVariable,
             typename Comparative>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Comparative, const MemberType&, const ComparisonVariable&> &&
             std::convertible_to<std::invoke_result_t<Comparative, const MemberType&, const ComparisonVariable&>, bool>
    size_t ComparativeExclusion_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const ComparisonVariable& CompVar, Comparative ComparativeFunc) {
        
        std::vector<ClassType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        size_t ElementsRemoved = 0;

        for (const ClassType& CurrentElement : ObjectVector) {

            if (!ComparativeFunc(CurrentElement.*Member, CompVar)) {
                ReturnVector.emplace_back(CurrentElement);
            } else {
                ElementsRemoved++;
            }
 
        }

        ObjectVector = std::move(ReturnVector);

        return ElementsRemoved;

    }


    template<typename ClassType, typename MemberType,
             typename OperationVariable,
             typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&, const OperationVariable&> &&
             std::convertible_to<std::invoke_result_t<Operation, const MemberType&, const OperationVariable&>, MemberType>
    std::vector<ClassType> Operate(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const OperationVariable& OperationVar, Operation OperativeFunc) {
        
        std::vector<ClassType> ReturnVector = ObjectVector;

        for (ClassType& CurrentElement : ReturnVector) {

            CurrentElement.*Member = OperativeFunc(CurrentElement.*Member, OperationVar);
 
        }

        return ReturnVector;

    }



    template<typename ClassType, typename MemberType,
             typename OperationVariable,
             typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&, const OperationVariable&> &&
             std::convertible_to<std::invoke_result_t<Operation, MemberType&, const OperationVariable&>, MemberType>
    void Operate_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const OperationVariable& OperationVar, Operation OperativeFunc) {

        for (ClassType& CurrentElement : ObjectVector) {

            CurrentElement.*Member = OperativeFunc(CurrentElement.*Member, OperationVar);
 
        }


    }

    template<typename ClassType, typename MemberType, typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Operation, const MemberType&>, MemberType>
    std::vector<ClassType> Operate(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, Operation OperativeFunc) {
        
        std::vector<ClassType> ReturnVector = ObjectVector;

        for (ClassType& CurrentElement : ReturnVector) {

            CurrentElement.*Member = OperativeFunc(CurrentElement.*Member);
 
        }

        return ReturnVector;

    }



    template<typename ClassType, typename MemberType, typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&> &&
             std::convertible_to<std::invoke_result_t<Operation, MemberType&>, MemberType>
    void Operate_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, Operation OperativeFunc) {

        for (ClassType& CurrentElement : ObjectVector) {

            CurrentElement.*Member = OperativeFunc(CurrentElement.*Member);
 
        }


    }

    template<typename ClassType, typename ClassMethod>
    void Operate_p(std::vector<ClassType>& ObjectVector, ClassMethod ClassType::*Method) {
        
        for (ClassType& CurrentElement : ObjectVector) {

            CurrentElement.*Method;
 
        }


    }

/*
==================================================================================================================================================================================
VECTOR EXTRACTION & CREATION FUNCTIONS

    Functions that accomodate vectors

==================================================================================================================================================================================
*/

    template<typename ClassType, typename MemberType>
    requires HasAccessibleMember<ClassType, MemberType>
    std::vector<MemberType>Extract(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member) {
        
        std::vector<MemberType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            ReturnVector.emplace_back(CurrentElement.*Member);
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType>
    requires HasAccessibleMember<ClassType, MemberType>
    std::vector<LinkedMember<ClassType, MemberType>> ExtractLinked(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member) {
        
        std::vector<LinkedMember<ClassType, MemberType>> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (ClassType& CurrentElement : ObjectVector) {

            ReturnVector.emplace_back(LinkedMember(CurrentElement, Member));
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType, typename Transformation, 
             typename T = std::invoke_result_t<Transformation, const MemberType&>>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Transformation, const MemberType&>
    std::vector<T>ExtractTransform(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, Transformation TransformationFunc) {
        
        std::vector<T> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            ReturnVector.emplace_back(TransformationFunc(CurrentElement.*Member));
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType,
             typename OperationVariable,
             typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&, const OperationVariable&> &&
             std::convertible_to<std::invoke_result_t<Operation, const MemberType&, const OperationVariable&>, MemberType>
    std::vector<MemberType>ExtractOperate(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const OperationVariable& OperationVar, Operation OperativeFunc) {
        
        std::vector<MemberType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            ReturnVector.emplace_back(OperativeFunc(CurrentElement.*Member, OperationVar));
 
        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType,
             typename OperationVariable,
             typename Operation>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&, const OperationVariable&> &&
             std::convertible_to<std::invoke_result_t<Operation, const MemberType&, const OperationVariable&>, MemberType>
    std::vector<MemberType>ExtractOperate_p(std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const OperationVariable& OperationVar, Operation OperativeFunc) {
        
        std::vector<MemberType> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (ClassType& CurrentElement : ObjectVector) {

            CurrentElement.*Member = OperativeFunc(CurrentElement.*Member, OperationVar);
            ReturnVector.emplace_back(CurrentElement.*Member);
 
        }

        return ReturnVector;

    }


    template<typename ClassType, typename MemberType,
             typename OperationVariable,
             typename Operation,
             typename T = std::invoke_result_t<Operation, const MemberType&, const OperationVariable&>>
    requires HasAccessibleMember<ClassType, MemberType> &&
             std::invocable<Operation, const MemberType&, const OperationVariable&>
    std::vector<T>ExtractOperativeTransform(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, const OperationVariable& OperationVar, Operation OperativeFunc) {
        
        std::vector<T> ReturnVector;
        ReturnVector.reserve(ObjectVector.size());

        for (const ClassType& CurrentElement : ObjectVector) {

            ReturnVector.emplace_back(OperativeFunc(CurrentElement.*Member, OperationVar));
 
        }

        return ReturnVector;

    }

/*
==================================================================================================================================================================================
VECTOR DISTRIBUTION FUNCTIONS

    Functions that accomodate vectors

==================================================================================================================================================================================
*/

    template<typename ClassType, typename MemberType>
    requires HasAccessibleMember<ClassType, MemberType>
    std::vector<std::vector<MemberType>>DistributeMember(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, size_t Distributions, bool ForceEqualDistribution) {
        
        std::vector<std::vector<MemberType>> ReturnVector;
        ReturnVector.reserve(Distributions);

        if (Distributions <= 1) {
            ReturnVector.emplace_back(Extract(ObjectVector, Member));
            return ReturnVector;
        }

        size_t Indices = ObjectVector.size() / Distributions;
        size_t Index = 0;

        for (size_t i = 0; i < Distributions; i++) {

            std::vector<MemberType> CurrentDistribution;
            CurrentDistribution.reserve(Indices);
            
            for (size_t x = 0; x < Indices; x++) {

                CurrentDistribution.emplace_back(ObjectVector[Index].*Member);
                Index++;

            }

            ReturnVector.emplace_back(CurrentDistribution);

        }

        if (ForceEqualDistribution) {
            return ReturnVector;
        }

        size_t Remainder = ObjectVector.size() % Distributions;

        for (size_t i = 0; i < Remainder; i++) {

            ReturnVector[i].push_back(ObjectVector[Index].*Member);
            Index++;

        }

        return ReturnVector;

    }

    template<typename ClassType, typename MemberType>
    requires HasAccessibleMember<ClassType, MemberType>
    std::vector<std::vector<MemberType>>DistributeMember(const std::vector<ClassType>& ObjectVector, MemberType ClassType::*Member, size_t Distributions) {
        return DistributeMember(ObjectVector, Member, Distributions, false);
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
    template <typename ClassType, Streamable MemberType>
    void Print(const std::vector<ClassType>& Vector, MemberType ClassType::*Member) {

        std::cout << std::endl;

        for (const ClassType& Element : Vector) {
            std::cout << Element.*Member << '\n';
        }

        std::cout << std::endl;

    }


}


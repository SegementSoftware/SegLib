# SegLib

SegLib is a C++20 template library that attempts to facilitate faster prototyping of projects by abstracting common std::vector and object operations to generic functions. It also possesses superficial templated numerical functions, these will be expanded upon. Ultimately,
SegLib will aim to provide generic functions to assist in prototyping games.

Currently, the general design philosophy of SegLib's design is:
* The more generic a function, the better.
* Functions should be easily invoked.
* Whilst speed is important, other libraries already do speed incredibly well.

## What does SegLib do well?

Performing operations on arbitrary objects, particularly std::vectors of objects, is what SegLib excels at. For example, imagine we were making a cardgame and had a card structure:

```cpp
struct Card {
    std::string CardID;
    int Value;
    int Suit;
};
```

If we had a std::vector<Card>, using member pointers, SegLib could quickly extract a given member into a seperate vector:

```cpp
auto Values = SLO::Extract(Cards, &Card::Value);
```

Alternatively, it can create "linked members" that store references to their parent object, either by directly invoking `SLO::LinkedMember`, or with `SLO::ExtractLinked`. The members can be edited, then "commited" back to their parent object.
Extracted members can also be transformed during extraction with `SLO::ExtractTransform`, they can change types during this process.

In a game like Hearts, its important to know how many cards of a given suit a player has, and to ensure plays only play from that suit. A sub-hand could easily be created like this:

```cpp
int HeartsSuit = 4;
auto Hearts = SLO::EqualityInclusion(Cards, &Card::Suit, HeartsSuit);
```
Of course, equality is not the only condition important for Inclusion/Exclusion, conditions (`SLO::ConditionalExclusion`) that evaluate a member, or compare (`SLO::ComparativeExclusion`) it with an arbitrary variable can also be used to create or edit vectors of objects.
```cpp
SLO::ConditionalExclusion(Cards, &Card::Value, SLN::IsEven<int>);
```

If your game has dynamic card values, objects can also be subject to `SLO::Operate`, either individually or in a container. Like most transformative SegLib functions an in place version can be invoked with the `_p` suffix.
```cpp
SLO::Operate_p(Cards, &Card::Value, 12, SLN::Add<int>);
```

The namespace `SLV` works similarly, containing most of the same functions. Although creating copies of std::vectors most C++ programmers unhappy, it allows for SegLib functions to be piped into each other, creating cursed ways to pratice 8 times tables.
Experience the weird one-liners of Python, in the comfort of your own C++: 

```cpp
SLV::Print(SLV::Operate(SLV::ComparativeInclusion(SLV::ConditionalExclusion(SLN::GenerateComposites(240), SLN::IsOdd<int>), 24, SLN::IsDivisibleBy<int>), 3, SLN::GetQuotient<int>));
```

## Installation
SegLib is header only, save for SegLibNumerical.cpp. If you decide to use SegLibNumerical.cpp be sure to include it as an added executable in your build. Otherwise, simply include the desired SegLib[module].h file in your project.

## Future Updates
SegLib is far from finished, but here's the general direction:

* Make SLO functions less painful.
* Expand upon SLN, at this point it mostly serves as a series of functions to test SLV and SLO with.
* Expand upon SegLibConcepts to improve compilation error detection.
* Support more container types, either by refactoring what already exists or with another namespace.

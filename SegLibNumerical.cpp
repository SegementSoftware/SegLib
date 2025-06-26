#include "SegLibNumerical.h"

/*
==================================================================================================================================================================================
GENERATIVE FUNCTIONS

    Functions that produce numbers.

==================================================================================================================================================================================
*/

namespace SLN {

    float RandFloatInRange(float Minimum, float Maximum) {

    float Rand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return Minimum + (Rand * (Maximum - Minimum));

    }

    std::vector<int> GeneratePrimes(size_t Limit) {

        std::vector<int> Primes;
        Primes.reserve(Limit);

        size_t Index = 5;

        while (Primes.size() != Limit) {
            if (IsPrime(Index)) Primes.emplace_back(Index);
            Index++;
        }

        return Primes;

    }

    std::vector<int> GenerateComposites(size_t Limit) {

        std::vector<int> Composites;
        Composites.reserve(Limit);

        size_t Index = 0;

        while (Composites.size() != Limit) {
            if (IsComposite(Index)) Composites.emplace_back(Index);
            Index++;
        }

        return Composites;

    }


}


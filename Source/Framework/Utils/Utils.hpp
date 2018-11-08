//
// Utils.hpp
//
// @author Roberto Cano
//

#pragma once

#include <random>

namespace Framework
{
    namespace Utils
    {
        class RandomGenerator
        {
        public:
            RandomGenerator(int includedMin, int excludedMax);
            RandomGenerator(const std::vector<float>& weights);
            ~RandomGenerator();

            int getValue() const;

        private:
            class Generator;
            class RangeGeneratorImpl;
            class WeightedRangeGeneratorImpl;
            std::unique_ptr<Generator> _generator;
        };

        template<typename T>
        const std::string GetComponentIdFromThis(const T& thiz)
        {
            return typeid(thiz).name();
        }
        template<typename T>
        const std::string GetComponentIdFromClass()
        {
            return typeid(T).name();
        }
    }
}
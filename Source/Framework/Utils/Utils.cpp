//
// Utils.hpp
//
// @author Roberto Cano
//

#pragma once

#include"Utils.hpp"

#include <ctime>

using namespace Framework::Utils;

#pragma region - Internal implementation
class RandomGenerator::Generator
{
public:
    Generator() = default;
    virtual ~Generator() = default;

    virtual int getValue() const = 0;
};

class RandomGenerator::RangeGeneratorImpl : public RandomGenerator::Generator
{
public:
    RangeGeneratorImpl(int includedMin, int excludedMax)
        : _rng(_rd())
        , _distribution(includedMin, excludedMax)
    {
        // Empty on purpose
    }


    int getValue() const override
    {
        return _distribution(_rng);
    }

private:
    mutable std::random_device _rd;
    mutable std::mt19937_64 _rng;
    std::uniform_int_distribution<int> _distribution;
};

class RandomGenerator::WeightedRangeGeneratorImpl : public RandomGenerator::Generator
{
public:
    WeightedRangeGeneratorImpl(const std::vector<float>& weights)
        : _rng(_rd())
        , _distribution(weights.begin(), weights.end())
    {
        // Empty on purpose
    }


    int getValue() const override
    {
        return _distribution(_rng);
    }

private:
    mutable std::random_device _rd;
    mutable std::mt19937_64 _rng;
    std::discrete_distribution<int> _distribution;
};

RandomGenerator::RandomGenerator(int includedMin, int excludedMax)
{
    _generator = std::make_unique<RangeGeneratorImpl>(includedMin, excludedMax);
}

RandomGenerator::RandomGenerator(const std::vector<float>& weights)
{
    _generator = std::make_unique<WeightedRangeGeneratorImpl>(weights);
}

RandomGenerator::~RandomGenerator()
{
    // Satisfy unique_ptr requirements by defining the destructor here
}

int RandomGenerator::getValue() const
{
    return _generator->getValue();
}


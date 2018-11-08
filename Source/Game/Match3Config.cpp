//
// Match3Config.cpp
//
// @author Roberto Cano
//

#include "Match3Config.hpp"
#include <algorithm>

using namespace Match3::Game;

void Config::validate() const
{
    engineSettings.validate();

    // Check that the drop rate of symbols adds up to 1.0
    float totalDropRate = 0.0f;
    for (const auto& symbolDefinition : symbolDefinitions)
    {
        totalDropRate += symbolDefinition.second.dropRate;
    }

    bool isDropRateValid = totalDropRate >= (1.0f - std::numeric_limits<float>::epsilon()) &&
        totalDropRate <= (1.0f + std::numeric_limits<float>::epsilon());

    assert(isDropRateValid);
}

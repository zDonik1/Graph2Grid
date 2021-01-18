/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#pragma once

#include <memory>
#include <random>
#include <type_traits>
#include <iostream>

namespace zg2g {

using namespace std;

template <typename Condition>
using EnableIf = typename enable_if<Condition::value>::type;

/**
 * @brief Holds a range of some numeric type
 *
 * Holds a range of either integers or real numbers, and produces a random
 * number from the range on demand. The template argument should be of an
 * arithmetic type (either integer or real types).
 */
template <typename Numeric, typename = EnableIf<is_arithmetic<Numeric>>>
class Range
{
    Numeric m_min;
    Numeric m_max;

private:
    enum class IntSpecialization {};
    static constexpr IntSpecialization intDummy = {};
    enum class FloatSpecialization {};
    static constexpr FloatSpecialization floatDummy = {};

public:
    Range(Numeric min, Numeric max)
        : m_min(min)
        , m_max(max)
    {
    }

    /**
     * @brief Returns a random number in range
     * @return Numeric type
     *
     * Generates a uniformly distritbuted random number in the range,
     * which is inclusive - [min, max];
     */
    template<IntSpecialization dummy = intDummy, typename = EnableIf<is_integral<Numeric>>>
    Numeric getRandom() const
    {
        default_random_engine generator;
        uniform_int_distribution<Numeric> distribution(m_min, m_max);
        return distribution(generator);
    }

    /**
     * @brief Returns a random number in range
     * @return Numeric type
     *
     * Generates a uniformly distritbuted random number in the range,
     * where min value is inclusive, and max value is exclusive - [min, max).
     */
    template<FloatSpecialization dummy = floatDummy, typename = EnableIf<is_floating_point<Numeric>>>
    Numeric getRandom() const
    {
        default_random_engine generator;
        uniform_real_distribution<Numeric> distribution(m_min, m_max);
        return distribution(generator);
    }

    /**
     * @brief Returns minimum value in range
     * @return Numeric type
     */
    Numeric min() const
    {
        return m_min;
    }

    /**
     * @brief Returns maximum value in range
     * @return Numeric type
     */
    Numeric max() const
    {
        return m_max;
    }
};

}

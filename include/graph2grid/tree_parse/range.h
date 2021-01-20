/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#pragma once

#include <memory>
#include <random>
#include <type_traits>

namespace zg2g {

using namespace std;

template <typename Condition, typename Default = void>
using EnableIf = typename enable_if<Condition::value, Default>::type;

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

    template <typename Condition>
    using NumericEnableIf = EnableIf<Condition, Numeric>;

public:

    /**
     * @brief Constructor for one number.
     * @param num
     *
     * Ranges can hold a single number, which works as if it was just a number.
     * Note: Ranges can hold one number only if it is of integral type!
     */
    template<typename Integral = NumericEnableIf<is_integral<Numeric>>>
    Range(Integral num)
        : m_min(num)
        , m_max(num)
    {
    }

    /**
     * @brief Constructor for min and max in range
     * @param min
     * @param max
     */
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
    template<IntSpecialization dummy = intDummy,
             typename Integral = NumericEnableIf<is_integral<Numeric>>>
    Integral getRandom() const
    {
        if (m_min == m_max) {
            return m_min;
        }

        default_random_engine generator;
        uniform_int_distribution<Integral> distribution(m_min, m_max);
        return distribution(generator);
    }

    /**
     * @brief Returns a random number in range
     * @return Numeric type
     *
     * Generates a uniformly distritbuted random number in the range,
     * where min value is inclusive, and max value is exclusive - [min, max).
     */
    template<FloatSpecialization dummy = floatDummy,
             typename Real = NumericEnableIf<is_floating_point<Numeric>>>
    Real getRandom() const
    {
        default_random_engine generator;
        uniform_real_distribution<Real> distribution(m_min, m_max);
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

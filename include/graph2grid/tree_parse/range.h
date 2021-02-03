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

// wrapper for std::enable_if to make it shorter
template <typename Condition, typename Default = void>
using EnableIf = typename std::enable_if<Condition::value, Default>::type;

/**
 * @brief Holds a range of some numeric type
 *
 * Holds a range of either integers or real numbers, and produces a random
 * number from the range on demand. The template argument should be of an
 * arithmetic type (either integer or real types).
 */
template <typename Numeric, typename = EnableIf<std::is_arithmetic<Numeric>>>
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
     * @brief Constructor for min and max in range
     * @param min Numeric to be saved as minimum of range
     * @param max Numeric to be saved as maximum of range
     */
    Range(Numeric min = 0, Numeric max = 0)
        : m_min(min)
        , m_max(max)
    {
        if (m_max < m_min) {
            m_max = m_min;
        }
    }

    /**
     * @brief Returns a random number in range
     * @return Random number in range of Integral type
     *
     * Generates a uniformly distritbuted random number in the range,
     * which is inclusive - [min, max];
     */
    template<IntSpecialization dummy = intDummy,
             typename Integral = NumericEnableIf<std::is_integral<Numeric>>>
    inline auto getRandom() const
    {
        if (m_min == m_max) {
            return m_min;
        }

        std::default_random_engine generator;
        std::uniform_int_distribution<Integral> distribution(m_min, m_max);
        return distribution(generator);
    }

    /**
     * @brief Returns a random number in range
     * @return  Random number in range of Real type
     *
     * Generates a uniformly distritbuted random number in the range,
     * where min value is inclusive, and max value is exclusive - [min, max).
     */
    template<FloatSpecialization dummy = floatDummy,
             typename Real = NumericEnableIf<std::is_floating_point<Numeric>>>
    inline auto getRandom() const
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<Real> distribution(m_min, m_max);
        return distribution(generator);
    }

    /**
     * @brief Returns minimum value in range
     * @return Minimum of range of Numeric type
     */
    inline auto min() const
    {
        return m_min;
    }

    /**
     * @brief Returns maximum value in range
     * @return Maximum of range of Numeric type
     */
    inline auto max() const
    {
        return m_max;
    }

    /**
     * @brief Compares ranges for equality
     * @param rhs Range on the right hand side of == operator
     * @return Equality result of bool type
     */
    inline auto operator ==(const Range<Numeric> &rhs) const
    {
        return m_min == rhs.min() && m_max == rhs.max();
    }

    inline auto &operator =(const Range<Numeric> &rhs)
    {
        m_min = rhs.min();
        m_max = rhs.max();
        return *this;
    }
};

} // namespace zg2g

/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <doctest/doctest.h>

#include <graph2grid/tree_parse/range.h>
#include <string>

using namespace zg2g;

TEST_CASE("Range") {
    SUBCASE("min and max") {
        SUBCASE("integer") {
            const Range<int> range(10, 20);
            CHECK(range.min() == 10);
            CHECK(range.max() == 20);
        }
        SUBCASE("real") {
            const Range<double> rangeDouble(5.1, 20.1);
            CHECK(rangeDouble.min() == 5.1);
            CHECK(rangeDouble.max() == 20.1);

            const Range<float> rangeFloat(5.1f, 20.1f);
            CHECK(rangeFloat.min() == 5.1f);
            CHECK(rangeFloat.max() == 20.1f);
        }
    }
    SUBCASE("random in range") {
        constexpr int randomIterations = 100;

        SUBCASE("integer") {
            Range<int> range(10, 20);
            for (int i = 0; i < randomIterations; ++i) {
                auto random = range.getRandom();
                CHECK(random >= 10);
                CHECK(random <= 20);
            }
        }
        SUBCASE("real") {
            Range<double> range(5.1, 20.1);
            for (int i = 0; i < randomIterations; ++i) {
                auto random = range.getRandom();
                CHECK(random >= 5.1);
                CHECK(random < 20.1);
            }
        }
    }
}

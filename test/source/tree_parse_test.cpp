/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <doctest/doctest.h>

#include <graph2grid/tree_parse/range.h>
#include <string>

using namespace std;
using namespace zg2g;

TEST_CASE("Range") {
    SUBCASE("one value") {
        const Range range(10);
        CHECK(range.min() == 10);
        CHECK(range.max() == 10);
        CHECK(range.getRandom() == 10);
    }

    const Range rangeInt(10, 20);
    const Range rangeDouble(5.1, 20.1);

    SUBCASE("min and max") {
        CHECK(rangeInt.min() == 10);
        CHECK(rangeInt.max() == 20);

        CHECK(rangeDouble.min() == 5.1);
        CHECK(rangeDouble.max() == 20.1);
    }
    SUBCASE("random in range") {
        constexpr int randomIterations = 100;

        for (int i = 0; i < randomIterations; ++i) {
            auto random = rangeInt.getRandom();
            CHECK(random >= 10);
            CHECK(random <= 20);
        }

        for (int i = 0; i < randomIterations; ++i) {
            auto random = rangeDouble.getRandom();
            CHECK(random >= 5.1);
            CHECK(random < 20.1);
        }
    }
    SUBCASE("equality") {
        CHECK(rangeInt == Range{ 10, 20 });
        CHECK(rangeDouble == Range{ 5.1, 20.1 });
    }
}

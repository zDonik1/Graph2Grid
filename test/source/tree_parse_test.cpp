/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <doctest/doctest.h>

#include <graph2grid/tree_parse/range.h>
#include <graph2grid/tree_parse/near_component.h>
#include <graph2grid/tree_parse/tree_node_component.h>

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

TEST_CASE("NearComponent") {
    SUBCASE("normal") {
        NearComponent nearComponent{ "SomeComponent", { 3 }, 10, 2, 20 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 10, 2, 20 });
    }
    SUBCASE("min distance less than 1") {
        NearComponent nearComponent{ "SomeComponent", { 3 }, 2, 0, 20 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 2, 1, 20 });
    }
    SUBCASE("max distance less than min distance") {
        NearComponent nearComponent{ "SomeComponent", { 3 }, 2, 20, 2 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 2, 20, 20 });
    }
    SUBCASE("max distance less than min distance") {
        NearComponent nearComponent{ "SomeComponent", { 3 }, 2, 20, 2 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 20, 20, 20 });
    }
    SUBCASE("preferred distance less") {
        NearComponent nearComponent{ "SomeComponent", { 3 }, 1, 2, 20 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 2, 2, 20 });
    }
    SUBCASE("preferred distance more") {
        NearComponent nearComponent{ "SomeComponent", { 3 }, 25, 2, 20 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 20, 2, 20 });
    }
}

TEST_CASE("TreeNodeComponent") {
    // checking name and count
    TreeNodeComponent component("MyNewComponent", { 10, 60 });
    CHECK(component.name() == "MyNewComponent");
    CHECK(component.count() == Range{ 10, 60 });

    // checking addSize and sizes
    component.addSize({{ 10, 20 }, { 10 }});
    component.addSize({{ 5 }, { 2, 4 }});
    CHECK(component.sizes().at(0) == Size{{ 10, 20 }, { 10 }});
    CHECK(component.sizes().at(1) == Size{{ 5 }, { 2, 4 }});

    // checking addNear and near
    NearComponent nearComponent{ "AnotherComponent", { 3 }, 10, 2, 20 };
    component.addNear(nearComponent);                   // l-value reference
    component.addNear({ "ThirdComponent", { 1, 4 }});   // r-value reference

    CHECK(component.near().at(0) == NearComponent{ "AnotherComponent", { 3 }, 10, 2, 20 });
    auto second = component.near().at(1);
    NearComponent equivalent{ "ThirdComponent", { 1, 4 }, second.minDistance(), 1,
                            std::numeric_limits<decltype(second.maxDistance())>::max() };
    CHECK(second == equivalent);
}

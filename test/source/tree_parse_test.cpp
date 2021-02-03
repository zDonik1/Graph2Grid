/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <doctest/doctest.h>

#include <sstream>
#include <fstream>

#include <graph2grid/tree_parse/range.h>
#include <graph2grid/tree_parse/near_component.h>
#include <graph2grid/tree_parse/tree_node_component.h>
#include <graph2grid/tree_parse/tree_node_system.h>
#include <graph2grid/tree_parse/tree_node_system_stack.h>
#include <graph2grid/tree_parse/ggdl_lexer.h>

using namespace std;
using namespace zg2g;

TEST_CASE("Range") {
    SUBCASE("default value") {
        const Range<int> rangeInt;
        const Range<double> rangeDouble;

        CHECK(rangeInt.min() == 0);
        CHECK(rangeInt.max() == 0);

        CHECK(rangeDouble.min() == 0.0);
        CHECK(rangeDouble.max() == 0.0);
    }
    SUBCASE("one value") {
        const Range rangeInt(10);
        const Range rangeDouble(10.5);

        CHECK(rangeInt.min() == 10);
        CHECK(rangeInt.max() == 10);
        CHECK(rangeInt.getRandom() == 10);

        CHECK(rangeDouble.min() == 10.5);
        CHECK(rangeDouble.max() == 10.5);
        CHECK(rangeDouble.getRandom() == 10.5);
    }
    SUBCASE("two values") {
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
}

TEST_CASE("NearComponent") {
    SUBCASE("normal") {
        const NearComponent nearComponent{ "SomeComponent", { 3 }, 10, 2, 20 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 10, 2, 20 });
    }
    SUBCASE("min distance less than 1") {
        const NearComponent nearComponent{ "SomeComponent", { 3 }, 2, 0, 20 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 2, 1, 20 });
    }
    SUBCASE("max distance less than min distance") {
        const NearComponent nearComponent{ "SomeComponent", { 3 }, 2, 20, 2 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 2, 20, 20 });
    }
    SUBCASE("max distance less than min distance") {
        const NearComponent nearComponent{ "SomeComponent", { 3 }, 2, 20, 2 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 20, 20, 20 });
    }
    SUBCASE("preferred distance less") {
        const NearComponent nearComponent{ "SomeComponent", { 3 }, 1, 2, 20 };
        CHECK(nearComponent == NearComponent{ "SomeComponent", { 3 }, 2, 2, 20 });
    }
    SUBCASE("preferred distance more") {
        const NearComponent nearComponent{ "SomeComponent", { 3 }, 25, 2, 20 };
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
    const NearComponent nearComponent{ "AnotherComponent", { 3 }, 10, 2, 20 };
    component.addNear(nearComponent);                   // l-value reference
    component.addNear({ "ThirdComponent", { 1, 4 }});   // r-value reference

    CHECK(component.near().at(0) == NearComponent{ "AnotherComponent", { 3 }, 10, 2, 20 });
    auto second = component.near().at(1);
    const NearComponent equivalent{ "ThirdComponent", { 1, 4 }, second.minDistance(), 1,
                            std::numeric_limits<decltype(second.maxDistance())>::max() };
    CHECK(second == equivalent);
}

TEST_CASE("TreeNodeSystem") {
    SUBCASE("default range for root systems") {
        TreeNodeSystem system("MySystem");
        CHECK(system.name() == "MySystem");
        CHECK(system.count() == Range{ 0, 0 });
    }
    SUBCASE("assigned range") {
        TreeNodeSystem system("MySystem", Range{ 3, 5 });
        CHECK(system.name() == "MySystem");
        CHECK(system.count() == Range{ 3, 5 });
    }
    SUBCASE("addComponent and components") {
        TreeNodeSystem system("MySystem");

        SUBCASE("adding component by l-value reference") {
            auto componentPtr = make_unique<TreeNodeComponent>("SomeComponent", Range{ 3 });
            system.addComponent(move(componentPtr));

            const auto &component = system.components().at(0);
            CHECK(component->name() == "SomeComponent");
            CHECK(component->count() == Range{ 3 });
        }
        SUBCASE("adding system by r-value reference") {
            system.addComponent(make_unique<TreeNodeSystem>("SomeSystem", Range{ 3, 4 }));

            const auto &component = system.components().at(0);
            CHECK(component->name() == "SomeSystem");
            CHECK(component->count() == Range{ 3, 4 });
        }
    }
}

TEST_CASE("TreeNodeSystemStack") {
    SUBCASE("default range for root system stacks") {
        TreeNodeSystemStack stack("MySystemStack");
        CHECK(stack.name() == "MySystemStack");
        CHECK(stack.count() == Range{ 0, 0 });
    }
    SUBCASE("assigned range") {
        TreeNodeSystemStack stack("MySystemStack", Range{ 3, 5 });
        CHECK(stack.name() == "MySystemStack");
        CHECK(stack.count() == Range{ 3, 5 });
    }
    SUBCASE("addSystem and systems") {
        TreeNodeSystemStack stack("MySystemStack");

        SUBCASE("adding by l-value reference") {
            auto systemPtr = make_unique<TreeNodeSystem>("FirstSystem", Range{ 3 });
            stack.addSystem(systemPtr);
            CHECK(systemPtr.get() == nullptr); // check if moved

            const auto &system = stack.systems().at(0);
            CHECK(system->name() == "FirstSystem");
            CHECK(system->count() == Range{ 3 });
        }
        SUBCASE("adding by r-value reference") {
            stack.addSystem(make_unique<TreeNodeSystem>("SecondSystem", Range{ 3, 4 }));

            const auto &system = stack.systems().at(0);
            CHECK(system->name() == "SecondSystem");
            CHECK(system->count() == Range{ 3, 4 });
        }
    }
}

TEST_CASE("GddlLexer") {
    SUBCASE("setStream string stream") {
        const string str = "Component MyComponent\n    count: 1-2";
        const vector<Token> testTable = {
            { Token::Type::Keyword,    "Component",    0 },
            { Token::Type::Identifier, "MyComponent", -1 },
            { Token::Type::Syntax,     "\n",          -1 },
            { Token::Type::Keyword,    "count",        1 },
            { Token::Type::Syntax,     ":",           -1 },
            { Token::Type::Literal,    "1",           -1 },
            { Token::Type::Syntax,     "-",           -1 },
            { Token::Type::Literal,    "2",           -1 },
            { Token::Type::Eof,        "",            -1 },
        };

        auto stream = make_shared<stringstream>(str);
        GgdlLexer lexer(stream);

        SUBCASE("peek and eof") {
            int counter = 0;
            while (!lexer.eof()) {
                const auto token = lexer.peek(counter);
                CHECK(token.type == testTable.at(counter).type);
                CHECK(token.chars == testTable.at(counter).chars);
                CHECK(token.indent == testTable.at(counter).indent);
                ++counter;
            }
        }
        SUBCASE("consume and eof") {
            int counter = 0;
            while (!lexer.eof()) {
                const auto token = lexer.consume();
                CHECK(token.type == testTable.at(counter).type);
                CHECK(token.chars == testTable.at(counter).chars);
                CHECK(token.indent == testTable.at(counter).indent);
                ++counter;
            }
        }
        SUBCASE("peek and consume") {
            {
                const auto token = lexer.peek();
                CHECK(token.type == testTable.at(0).type);
                CHECK(token.chars == testTable.at(0).chars);
                CHECK(token.indent == testTable.at(0).indent);
            }
            {
                const auto token = lexer.consume();
                CHECK(token.type == testTable.at(0).type);
                CHECK(token.chars == testTable.at(0).chars);
                CHECK(token.indent == testTable.at(0).indent);
            }
            {
                const auto token = lexer.peek();
                CHECK(token.type == testTable.at(1).type);
                CHECK(token.chars == testTable.at(1).chars);
                CHECK(token.indent == testTable.at(1).indent);
            }
        }
        SUBCASE("operator >> and operator bool") {
            int counter = 0;
            Token token;
            while (lexer >> token) {
                CHECK(token.type == testTable.at(counter).type);
                CHECK(token.chars == testTable.at(counter).chars);
                CHECK(token.indent == testTable.at(counter).indent);
                ++counter;
            }
        }
    }
    SUBCASE("setStream file stream") {
        const vector<Token> testTable = {
            { Token::Type::Keyword,    "System",               0 },
            { Token::Type::Identifier, "City",                -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "Component",            1 },
            { Token::Type::Identifier, "ResidentialBuilding", -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "count",                2 },
            { Token::Type::Syntax,     ":",                   -1 },
            { Token::Type::Literal,    "30",                  -1 },
            { Token::Type::Syntax,     "-",                   -1 },
            { Token::Type::Literal,    "60",                  -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "size",                 2 },
            { Token::Type::Syntax,     ":",                   -1 },
            { Token::Type::Syntax,     "(",                   -1 },
            { Token::Type::Literal,    "6",                   -1 },
            { Token::Type::Syntax,     "-",                   -1 },
            { Token::Type::Literal,    "10",                  -1 },
            { Token::Type::Syntax,     ",",                   -1 },
            { Token::Type::Literal,    "4",                   -1 },
            { Token::Type::Syntax,     "-",                   -1 },
            { Token::Type::Literal,    "8",                   -1 },
            { Token::Type::Syntax,     ")",                   -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "near",                 2 },
            { Token::Type::Syntax,     ":",                   -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Identifier, "Market",               3 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "preferredDistance",    4 },
            { Token::Type::Syntax,     ":",                   -1 },
            { Token::Type::Literal,    "1",                   -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "clusterCount",         4 },
            { Token::Type::Syntax,     ":",                   -1 },
            { Token::Type::Literal,    "5",                   -1 },
            { Token::Type::Syntax,     "-",                   -1 },
            { Token::Type::Literal,    "10",                  -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "Component",            1 },
            { Token::Type::Identifier, "Market",              -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "count",                2 },
            { Token::Type::Syntax,     ":",                   -1 },
            { Token::Type::Literal,    "6",                   -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Keyword,    "size",                 2 },
            { Token::Type::Syntax,     ":",                   -1 },
            { Token::Type::Syntax,     "[",                   -1 },
            { Token::Type::Syntax,     "(",                   -1 },
            { Token::Type::Literal,    "10",                  -1 },
            { Token::Type::Syntax,     ",",                   -1 },
            { Token::Type::Literal,    "15",                  -1 },
            { Token::Type::Syntax,     ")",                   -1 },
            { Token::Type::Syntax,     ",",                   -1 },
            { Token::Type::Syntax,     "(",                   -1 },
            { Token::Type::Literal,    "6",                   -1 },
            { Token::Type::Syntax,     ",",                   -1 },
            { Token::Type::Literal,    "10",                  -1 },
            { Token::Type::Syntax,     ")",                   -1 },
            { Token::Type::Syntax,     "]",                   -1 },
            { Token::Type::Syntax,     "\n",                  -1 },
            { Token::Type::Eof,        "",                    -1 },
        };

        auto stream = make_shared<ifstream>("city_config.ggdl");
        REQUIRE(stream->is_open());

        GgdlLexer lexer(stream);

        SUBCASE("peek and eof") {
            int counter = 0;
            while (!lexer.eof()) {
                const auto token = lexer.peek(counter);
                CHECK(token.type == testTable.at(counter).type);
                CHECK(token.chars == testTable.at(counter).chars);
                CHECK(token.indent == testTable.at(counter).indent);
                ++counter;
            }
        }
        SUBCASE("consume and eof") {
            int counter = 0;
            while (!lexer.eof()) {
                const auto token = lexer.consume();
                CHECK(token.type == testTable.at(counter).type);
                CHECK(token.chars == testTable.at(counter).chars);
                CHECK(token.indent == testTable.at(counter).indent);
                ++counter;
            }
        }
        SUBCASE("operator >> and operator bool") {
            int counter = 0;
            Token token;
            while (lexer >> token) {
                CHECK(token.type == testTable.at(counter).type);
                CHECK(token.chars == testTable.at(counter).chars);
                CHECK(token.indent == testTable.at(counter).indent);
                ++counter;
            }
        }
    }
}

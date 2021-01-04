#include <doctest/doctest.h>
#include <graph2grid/system.h>
#include <graph2grid/version.h>

#include <string>

TEST_CASE("System") {
  using namespace zg2g;
}

TEST_CASE("Graph2Grid version") {
  static_assert(std::string_view(GRAPH2GRID_VERSION) == std::string_view("0.1.0"));
  CHECK(std::string(GRAPH2GRID_VERSION) == std::string("0.1.0"));
}

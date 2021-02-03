/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <graph2grid/tree_parse/tree_node_system_stack.h>

using namespace std;
using namespace spimpl;
using namespace zg2g;

struct TreeNodeSystemStack::PImpl
{
    Systems systems;

public:
    PImpl()
    {
    }
};

TreeNodeSystemStack::TreeNodeSystemStack(const string &name, const Range<int> &count)
    : TreeNodeComponent(name, count)
    , impl(make_unique_impl<PImpl>())
{
}

TreeNodeSystemStack::TreeNodeSystemStack(string &&name, const Range<int> &count)
    : TreeNodeComponent(move(name), count)
    , impl(make_unique_impl<PImpl>())
{
}

void TreeNodeSystemStack::addSystem(std::unique_ptr<TreeNodeSystem> system)
{
    impl->systems.emplace_back(move(system));
}

const Systems &TreeNodeSystemStack::systems() const
{
    return impl->systems;
}

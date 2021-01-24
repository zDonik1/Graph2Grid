/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <graph2grid/tree_parse/tree_node_system.h>

using namespace std;
using namespace spimpl;
using namespace zg2g;

struct TreeNodeSystem::PImpl
{
    Components components;

public:
    PImpl()
    {
    }
};

TreeNodeSystem::TreeNodeSystem(const string &name, const Range<int> &count)
    : TreeNodeComponent(name, count)
    , impl(make_unique_impl<PImpl>())
{
}

void TreeNodeSystem::addComponent(std::unique_ptr<TreeNodeComponent> &component)
{
    addComponent(move(component));
}

void TreeNodeSystem::addComponent(std::unique_ptr<TreeNodeComponent> &&component)
{
    impl->components.emplace_back(move(component));
}

const Components &TreeNodeSystem::components() const
{
    return impl->components;
}

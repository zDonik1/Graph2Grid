/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <graph2grid/tree_parse/tree_node_component.h>

#include <graph2grid/tree_parse/range.h>
#include <graph2grid/tree_parse/near_component.h>

using namespace std;
using namespace spimpl;
using namespace zg2g;

struct TreeNodeComponent::PImpl
{
    string name;
    Range<int> count;
    Sizes sizes;
    vector<NearComponent> near;

public:
    PImpl(const string &_name, const Range<int> &_count)
        : name(_name)
        , count(_count)
    {
    }

    PImpl(string &&_name, const Range<int> &_count)
        : name(move(_name))
        , count(_count)
    {
    }
};

TreeNodeComponent::TreeNodeComponent(const string &name, const Range<int> &count)
    : impl(make_impl<PImpl>(name, count))
{
}

TreeNodeComponent::TreeNodeComponent(string &&name, const Range<int> &count)
    : impl(make_impl<PImpl>(move(name), count))
{
}

void TreeNodeComponent::setName(const string &name)
{
    impl->name = name;
}

void TreeNodeComponent::setName(string &&name)
{
    impl->name = move(name);
}

void TreeNodeComponent::setCount(const Range<int> &count)
{
    impl->count = count;
}

void TreeNodeComponent::addSize(const Size &size)
{
    impl->sizes.emplace_back(size);
}

void TreeNodeComponent::addNear(const NearComponent &near)
{
    impl->near.push_back(near);
}

void TreeNodeComponent::addNear(NearComponent &&near)
{
    impl->near.emplace_back(std::move(near));
}

const string &TreeNodeComponent::name() const
{
    return impl->name;
}

const Range<int> &TreeNodeComponent::count() const
{
    return impl->count;
}

const Sizes &TreeNodeComponent::sizes() const
{
    return impl->sizes;
}

const std::vector<NearComponent> &TreeNodeComponent::near() const
{
    return impl->near;
}

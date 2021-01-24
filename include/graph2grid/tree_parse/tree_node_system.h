/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#pragma once

#include <memory>
#include <vector>

#include <spimpl.h>

#include "tree_node_component.h"

namespace zg2g {

using Components = std::vector<std::unique_ptr<TreeNodeComponent>>;

class TreeNodeSystem : public TreeNodeComponent
{
    struct PImpl;
    spimpl::unique_impl_ptr<PImpl> impl;

public:
    TreeNodeSystem(const std::string &name, const Range<int> &count = { 0, 0 });

    void addComponent(std::unique_ptr<TreeNodeComponent> &component);
    void addComponent(std::unique_ptr<TreeNodeComponent> &&component);

    const Components &components() const;
};

} // namespace zg2g

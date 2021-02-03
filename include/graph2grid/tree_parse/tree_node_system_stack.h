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
#include "tree_node_system.h"

namespace zg2g {

using Systems = std::vector<std::unique_ptr<TreeNodeSystem>>;

class TreeNodeSystemStack : public TreeNodeComponent
{
    struct PImpl;
    spimpl::unique_impl_ptr<PImpl> impl;

public:
    TreeNodeSystemStack(const std::string &name, const Range<int> &count = {});
    TreeNodeSystemStack(std::string &&name = "", const Range<int> &count = {});

    void addSystem(std::unique_ptr<TreeNodeSystem> system);

    const Systems &systems() const;
};

} // namespace zg2g

/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#pragma once

#include <memory>
#include <vector>
#include <utility>

#include <spimpl.h>

#include "range.h"
#include "near_component.h"

namespace zg2g {

using Size = std::pair<const Range<int>, const Range<int>>;
using Sizes = std::vector<Size>;

/**
 * @brief The TreeNodeComponent holds information about a component parsed
 * from the ggdl file
 *
 * TreeNodeSystem and TreeNodeSystemStack inherit from TreeNodeComponent,
 * and while parsing the ggdl file, a tree is made which will hold all the
 * necessary information about the input paramters for the generation of
 * the graph. This tree consists of Components, and the root component of
 * the tree will always be either a TreeNodeSystem and TreeNodeSystemStack.
 */
class TreeNodeComponent
{
    struct PImpl;
    spimpl::impl_ptr<PImpl> impl;

public:
    /**
     * @brief Constructs a tree node component with name and count
     * @param name std::string to be saved as component name
     * @param count Range<int> to be saved as count of instances of this component
     */
    TreeNodeComponent(const std::string &name, const Range<int> &count);

    /**
     * @brief Adds a size paramter to the component
     * @param size Size to be added into the possible sizes list
     *
     * The sizes represents all the possible sizes that this component
     * can have. Type Size is a pair of Ranges, first being the width
     * and second being the height. During generation, any possible size
     * is selected for the component and used.
     *
     * @sa TreeNodeComponent::sizes()
     */
    void addSize(const Size &size);

    /**
     * @brief Adds a near component to this component by l-value (copies it)
     * @param near Near component to be added to be clustered around
     * @sa NearComponent
     */
    void addNear(const NearComponent &near);

    /**
     * @brief Adds a near component to this component by r-value (moves it)
     * @param near Near component to be added to be clustered around
     * @sa NearComponent
     */
    void addNear(NearComponent &&near);

    /**
     * @brief Returns the name of the component
     * @return Name of component as std::string
     *
     * Names uniquely identify components and help to reference them
     * in "near" property in other components.
     */
    const std::string &name() const;

    /**
     * @brief Returns the number of components possible as Range
     * @return Count of component instances as Range<int>
     *
     * The count signifies the maximum and the minimum possible number
     * of intances that get generated from this component in the whole
     * system that it is in. This property always overrides the "clusterCount"
     * of NearComponent.
     *
     * @sa NearComponent::clusterCount
     */
    const Range<int> &count() const;

    /**
     * @brief Returns the list of all possible sizes for this component
     * @return List of sizes of Sizes type
     * @sa TreeNodeComponent::addSize()
     */
    const Sizes &sizes() const;

    /**
     * @brief Returns the list of near components
     * @return List of near components as std::vector<NearComponent>
     * @sa NearComponent
     */
    const std::vector<NearComponent> &near() const;
};

} // namespace zg2g

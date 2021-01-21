/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#pragma once

#include <string>
#include <limits>

#include <spimpl.h>

#include "range.h"

namespace zg2g {

/**
 * @brief The NearComponent is a helper struct for TreeNodeComponent
 *
 * Its purpose is to hold information about other components which should
 * be near the component the "near" property is defined in. It is used
 * during the generation to create links (edges) between components in
 * the graph, and to ensure that some properties are enforced. An example
 * could be a number of ResidentialBuildings near a Market, where "near"
 * property will be defined for ResidentialBuildings to cluster around
 * Markets.
 *
 * @sa TreeNodeComponent::near()
 */
class NearComponent
{
    struct PImpl;
    spimpl::impl_ptr<PImpl> impl;

public:
    /**
     * @brief Constructs a NearComponent
     * @param _name Name of other component
     * @param _clusterCount Number of this component around another component
     * @param _preferredDistance The preferred distance to start spawning in
     * @param _minDistance Minimum distance spawn limit
     * @param _maxDistance Maximum distance spawn limit
     *
     * preferredDistance defaults to minDistance if not set.
     * minDistance defaults to 1 if not set.
     * maxDistance defaults to the max limit of uint32_t if not set.
     */
    NearComponent(const std::string &name,
                  const Range<int> &clusterCount,
                  uint32_t preferredDistance = 0,
                  uint32_t minDistance = 1,
                  uint32_t maxDistance = std::numeric_limits<uint32_t>::max());

    /**
     * @brief Name of the other component. Used to identify other component
     * @return Retuns name of component as std::string
     */
    const std::string &name() const;

    /**
     * @brief The count range of this component which should be clustered
     * around another component.
     * @return Returns cluster count as Range<int>
     *
     * This property will be overriden by "count" of TreeNodeComponent
     * during generation if the count max is less than the totalClusterCount,
     * or if count min is greater than totalClusterCount, where
     * totalClusterCount = clusterCount * count of other component.
     */
    const Range<int> &clusterCount() const;

    /**
     * @brief The preferred distance between this component and the other
     * component
     * @return Returns preferred distance as uint32_t
     *
     * During generation, the components are generated around preferredDistance
     * and branch out inwards and outwards from preferredDistance up to the
     * minimum and maximum distances. Defaults to minimum distance, meaning
     * as close as possible.
     *
     * @sa NearComponent::minDistance()
     * @sa NearComponent::maxDistance()
     */
    uint32_t preferredDistance() const;

    /**
     * @brief The minimum distance between this component and the other
     * component
     * @return Returns minimum distance as uint32_t
     *
     * The minimum distance is used to signify the limit at which this branching
     * will have to stop. This property overrides "clusterCount" if there is no
     * space left between the min and max distances. Defaults to 1 meaning
     * generation is allowed to be as close as possible.
     *
     * @sa NearComponent::preferredDistance()
     * @sa NearComponent::maxDistance()
     */
    uint32_t minDistance() const;

    /**
     * @brief The maximum distance between this component and the other
     * component
     * @return Returns maximum distance as uint32_t
     *
     * The maximum distance is used to signify the limit at which this branching
     * will have to stop. This property overrides "clusterCount" if there is no
     * space left between the min and max distances. Defaults to max 32-bit
     * unsigned integer limit meaning generation is allowed to be as far as possible.
     *
     * @sa NearComponent::preferredDistance
     * @sa NearComponent::minDistance
     */
    uint32_t maxDistance() const;

    /**
     * @brief Compares near components for equality
     * @param rhs Near component on the right hand side of == operator
     * @return Equality result of bool type
     */
    bool operator ==(const NearComponent &rhs) const;
};

} // namespace zg2g

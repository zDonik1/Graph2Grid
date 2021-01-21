/****************************************************************************
 *
 * @author Doniyor Tokhirov <tokhirovdoniyor(at)gmail.com>
 *
 ****************************************************************************/

#include <graph2grid/tree_parse/near_component.h>

using namespace spimpl;
using namespace zg2g;

struct NearComponent::PImpl
{
    std::string name;
    const Range<int> clusterCount;
    uint32_t preferredDistance;
    uint32_t minDistance;
    uint32_t maxDistance;

public:
    PImpl(const std::string &_name, const Range<int> &_clusterCount,
          uint32_t _preferredDistance, uint32_t _minDistance, uint32_t _maxDistance)
        : name(_name)
        , clusterCount(_clusterCount)
        , preferredDistance(_preferredDistance)
        , minDistance(_minDistance)
        , maxDistance(_maxDistance)
    {
        if (minDistance < 1) {
            minDistance = 1;
        }

        if (maxDistance < minDistance) {
            maxDistance = minDistance;
        }

        if (preferredDistance < minDistance) {
            preferredDistance = minDistance; // also ensures default is minDistance
        } else if (preferredDistance > maxDistance) {
            preferredDistance = maxDistance;
        }
    }
};

NearComponent::NearComponent(const std::string &name, const Range<int> &clusterCount,
                             uint32_t preferredDistance, uint32_t minDistance,
                             uint32_t maxDistance)
    : impl(make_impl<PImpl>(name, clusterCount, preferredDistance,
                            minDistance, maxDistance))
{
}

const std::string &NearComponent::name() const
{
    return impl->name;
}

const Range<int> &NearComponent::clusterCount() const
{
    return impl->clusterCount;
}

uint32_t NearComponent::preferredDistance() const
{
    return impl->preferredDistance;
}

uint32_t NearComponent::minDistance() const
{
    return impl->minDistance;
}

uint32_t NearComponent::maxDistance() const
{
    return impl->maxDistance;
}

bool NearComponent::operator ==(const NearComponent &rhs) const
{
    return impl->name == rhs.name()
            && impl->clusterCount == rhs.clusterCount()
            && impl->preferredDistance == rhs.preferredDistance()
            && impl->minDistance == rhs.minDistance()
            && impl->maxDistance == rhs.maxDistance();
}

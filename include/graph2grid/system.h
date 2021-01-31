#pragma once

#include <spimpl.h>

namespace zg2g {

class System
{
    struct PImpl;
    spimpl::impl_ptr<PImpl> impl;

public:
    System();
};

} // namespace zg2g

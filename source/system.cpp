#include <graph2grid/system.h>

using namespace spimpl;
using namespace zg2g;

struct System::PImpl
{
    PImpl()
    {
    }
};

System::System()
    : impl(make_impl<PImpl>())
{
}

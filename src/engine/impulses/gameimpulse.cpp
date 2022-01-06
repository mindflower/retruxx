#include <stdexcept>
#include <impulses/i_impulses.h>

namespace m3d
{
    void AuxImpulseInfo::UnpackXy(float*, float*, float*, float*) const
    {
        throw std::logic_error("Not implemented");
    }

    AuxImpulseInfo::AuxImpulseInfo(int, bool, int, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    float AuxImpulseInfo::UnpackWheel() const
    {
        throw std::logic_error("Not implemented");
    }
}

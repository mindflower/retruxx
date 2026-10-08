#include "i_renderer.h"

namespace m3d
{
    namespace rend
    {
        ShaderMacro::ShaderMacro(const char* n, const char* d) :
            name(n),
            definition(d)
        {
            // RVA 0x70B550
        }

        void LightSource::init(LightType const type, CVector const& pos)
        {
            // RVA 0x6FDB60 - type, position (used as the direction too) and a 1000 range; the colours
            // and the attenuation fields are left as they are.
            m_type = type;
            m_direction = pos;
            m_origin = pos;
            m_range = 1000.0f;
        }
    }  // namespace rend
}  // namespace m3d

// The inline members of the original renderer interface headers (engine/renderer/i_renderer_resource.h,
// i_renderer_query.h, i_renderer.h) that HTA's lib/include/renderer headers declare but do not
// define. The executable never instantiates these classes, so the driver defines them.
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace rend
    {
        // orig 0x642940 i_renderer_resource.h:20
        IRenderResource::IRenderResource() :
            m_refCount(0)
        {
        }

        // orig 0x642950 i_renderer_resource.h:35
        IRenderResource::~IRenderResource() {}

        // orig 0x642990 i_renderer_resource.h:43
        int IRenderResource::AddRef()
        {
            return ++m_refCount;
        }

        // orig 0x6429a0 i_renderer_resource.h:51
        int IRenderResource::Release()
        {
            int refCount = --m_refCount;
            if (refCount <= 0)
            {
                delete this;
            }
            return refCount;
        }

        // orig 0x6429c0 i_renderer_resource.h:63
        int IRenderResource::GetRefCount()
        {
            return m_refCount;
        }

        // orig 0x642a00 i_renderer_query.h:247
        IQuery::~IQuery() {}

        // orig 0x64acc0 i_renderer_shader.h:113
        IHlslShader::~IHlslShader() {}

        // orig 0x64c500 i_renderer_shader.h:46
        IAsmShader::~IAsmShader() {}

        // orig 0x645290 i_renderer_shader.h:285
        IEffect::~IEffect() {}

        const unsigned int IHlslShader::INVALID_PARAM = 0xffffffff;

        // orig 0x6429e0 i_renderer_query.h:156
        QueryReturnValue::QueryReturnValue() :
            m_type(NotValid)
        {
        }

        // orig 0x6429f0 i_renderer_query.h:163
        QueryReturnValue::~QueryReturnValue() {}

        bool QueryReturnValue::operator==(QueryReturnValue const& rhs) const
        {
            return m_type == rhs.m_type && i64 == rhs.i64;
        }

        // orig 0x6429d0 i_renderer_query.h:91
        void QueryReturnValue::SetType(Type t)
        {
            m_type = t;
        }

        QueryReturnValue::Type QueryReturnValue::GetType() const
        {
            return m_type;
        }

        bool QueryReturnValue::IsValid() const
        {
            return m_type != NotValid;
        }

        bool QueryReturnValue::GetBool() const
        {
            return b;
        }

        // i_renderer_query.h:103
        unsigned long QueryReturnValue::GetDword() const
        {
            return d;
        }

        uint64_t QueryReturnValue::GetUInt64() const
        {
            return i64;
        }

        BANDWIDTHTIMINGS const& QueryReturnValue::GetBandWidthTimings() const
        {
            return bw;
        }

        CACHEUTILIZATION const& QueryReturnValue::GetCacheUtilization() const
        {
            return cu;
        }

        INTERFACETIMINGS const& QueryReturnValue::GetInterfaceTimings() const
        {
            return it;
        }

        PIPELINETIMINGS const& QueryReturnValue::GetPipelineTimings() const
        {
            return pt;
        }

        STAGETIMINGS const& QueryReturnValue::GetStageTimings() const
        {
            return st;
        }

        VCACHE const& QueryReturnValue::GetVCash() const
        {
            return vc;
        }

        // orig 0x63a2d0 i_renderer.h:635
        ShaderMacro::ShaderMacro() {}

        ShaderMacro::ShaderMacro(ShaderMacro const& rhs) :
            name(rhs.name),
            definition(rhs.definition)
        {
        }
    }  // namespace rend
}  // namespace m3d

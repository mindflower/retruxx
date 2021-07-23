#pragma once

namespace m3d
{
    namespace rend
    {
        class QueryReturnValue
        {
        public:
            enum Type
            {
                NotValid = 0x0,
                Bool = 0x1,
                Dword = 0x2,
                Uint64 = 0x3,
                BandWidthTimings = 0x4,
                CacheUtilization = 0x5,
                InterfaceTimings = 0x6,
                PipelineTimings = 0x7,
                StageTimings = 0x8,
                VCache = 0x9,
            };

        public:
            unsigned long GetDword() const;

        private:
            Type m_type;
            //$57FB59F6B747DB7209BE92194A8F631C ___u1;
        };
    }
}

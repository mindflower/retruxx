#pragma once

namespace m3d
{
    namespace rend
    {
        struct BANDWIDTHTIMINGS
        {
            /* 0x0000 */ float MaxBandwidthUtilized;
            /* 0x0004 */ float FrontEndUploadMemoryUtilizedPercent;
            /* 0x0008 */ float VertexRateUtilizedPercent;
            /* 0x000c */ float TriangleSetupRateUtilizedPercent;
            /* 0x0010 */ float FillRateUtilizedPercent;
        }; /* size: 0x0014 */


        struct CACHEUTILIZATION
        {
            /* 0x0000 */ float TextureCacheHitRate;
            /* 0x0004 */ float PostTransformVertexCacheHitRate;
        }; /* size: 0x0008 */

        struct INTERFACETIMINGS
        {
            /* 0x0000 */ float WaitingForGPUToUseApplicationResourceTimePercent;
            /* 0x0004 */ float WaitingForGPUToAcceptMoreCommandsTimePercent;
            /* 0x0008 */ float WaitingForGPUToStayWithinLatencyTimePercent;
            /* 0x000c */ float WaitingForGPUExclusiveResourceTimePercent;
            /* 0x0010 */ float WaitingForGPUOtherTimePercent;
        }; /* size: 0x0014 */

        struct PIPELINETIMINGS
        {
            /* 0x0000 */ float VertexProcessingTimePercent;
            /* 0x0004 */ float PixelProcessingTimePercent;
            /* 0x0008 */ float OtherGPUProcessingTimePercent;
            /* 0x000c */ float GPUIdleTimePercent;
        }; /* size: 0x0010 */

        struct STAGETIMINGS
        {
            /* 0x0000 */ float MemoryProcessingPercent;
            /* 0x0004 */ float ComputationProcessingPercent;
        }; /* size: 0x0008 */

        struct VCACHE
        {
            /* 0x0000 */ unsigned long Pattern;
            /* 0x0004 */ unsigned long OptMethod;
            /* 0x0008 */ unsigned long CacheSize;
            /* 0x000c */ unsigned long MagicNumber;
        }; /* size: 0x0010 */

        class QueryReturnValue
        {
            enum Type
            {
                NotValid = 0,
                Bool = 1,
                Dword = 2,
                Uint64 = 3,
                BandWidthTimings = 4,
                CacheUtilization = 5,
                InterfaceTimings = 6,
                PipelineTimings = 7,
                StageTimings = 8,
                VCache = 9,
            };

        public:
            QueryReturnValue();
            ~QueryReturnValue();
            bool operator==(const m3d::rend::QueryReturnValue&) const;
            void SetType(m3d::rend::QueryReturnValue::Type);
            m3d::rend::QueryReturnValue::Type GetType() const;
            bool IsValid() const;
            bool GetBool() const;
            unsigned long GetDword() const;
            uint64_t GetUInt64() const;
            const m3d::rend::BANDWIDTHTIMINGS& GetBandWidthTimings() const;
            const m3d::rend::CACHEUTILIZATION& GetCacheUtilization() const;
            const m3d::rend::INTERFACETIMINGS& GetInterfaceTimings() const;
            const m3d::rend::PIPELINETIMINGS& GetPipelineTimings() const;
            const m3d::rend::STAGETIMINGS& GetStageTimings() const;
            const m3d::rend::VCACHE& GetVCash() const;

        private:
            /* 0x0000 */ m3d::rend::QueryReturnValue::Type m_type;
            /* 0x0004 */ char Padding_204[4];
            union
            {
                /* 0x0008 */ bool b;
                /* 0x0008 */ unsigned long d;
                /* 0x0008 */ uint64_t i64;
                m3d::rend::BANDWIDTHTIMINGS bw;
                m3d::rend::CACHEUTILIZATION cu;
                m3d::rend::INTERFACETIMINGS it;
                m3d::rend::PIPELINETIMINGS pt;
                m3d::rend::STAGETIMINGS st;
            }; /* size: 0x0014 */
            m3d::rend::VCACHE vc;
        }; /* size: 0x0020 */
    }
}

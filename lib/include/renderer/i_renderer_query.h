#pragma once

#include <cstdint>

namespace m3d
{
    class Landscape;

    namespace rend
    {
        // Query results are filled in by the renderer DLL. The union holds a uint64_t, so
        // natural 8-byte packing keeps the original 0x20-byte layout.
#pragma pack(push, 8)
        struct BANDWIDTHTIMINGS
        {
            float MaxBandwidthUtilized;
            float FrontEndUploadMemoryUtilizedPercent;
            float VertexRateUtilizedPercent;
            float TriangleSetupRateUtilizedPercent;
            float FillRateUtilizedPercent;
        };


        struct CACHEUTILIZATION
        {
            float TextureCacheHitRate;
            float PostTransformVertexCacheHitRate;
        };

        struct INTERFACETIMINGS
        {
            float WaitingForGPUToUseApplicationResourceTimePercent;
            float WaitingForGPUToAcceptMoreCommandsTimePercent;
            float WaitingForGPUToStayWithinLatencyTimePercent;
            float WaitingForGPUExclusiveResourceTimePercent;
            float WaitingForGPUOtherTimePercent;
        };

        struct PIPELINETIMINGS
        {
            float VertexProcessingTimePercent;
            float PixelProcessingTimePercent;
            float OtherGPUProcessingTimePercent;
            float GPUIdleTimePercent;
        };

        struct STAGETIMINGS
        {
            float MemoryProcessingPercent;
            float ComputationProcessingPercent;
        };

        struct VCACHE
        {
            uint32_t Pattern;
            uint32_t OptMethod;
            uint32_t CacheSize;
            uint32_t MagicNumber;
        };

        class QueryReturnValue
        {
            // Landscape::QueryWaterVisibility reads the result directly.
            friend class m3d::Landscape;

            enum Type : int32_t
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
            m3d::rend::QueryReturnValue::Type m_type;
            char Padding_204[4];
            union
            {
                bool b;
                uint32_t d;
                uint64_t i64;
                m3d::rend::BANDWIDTHTIMINGS bw;
                m3d::rend::CACHEUTILIZATION cu;
                m3d::rend::INTERFACETIMINGS it;
                m3d::rend::PIPELINETIMINGS pt;
                m3d::rend::STAGETIMINGS st;
                m3d::rend::VCACHE vc;
            };
        };
#pragma pack(pop)

        static_assert(sizeof(BANDWIDTHTIMINGS) == 0x0014);
        static_assert(sizeof(CACHEUTILIZATION) == 0x0008);
        static_assert(sizeof(INTERFACETIMINGS) == 0x0014);
        static_assert(sizeof(PIPELINETIMINGS) == 0x0010);
        static_assert(sizeof(STAGETIMINGS) == 0x0008);
        static_assert(sizeof(VCACHE) == 0x0010);
        static_assert(sizeof(QueryReturnValue) == 0x0020);
    }
}

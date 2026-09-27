#pragma once

#include <cstdint>

namespace m3d
{
    class Application;
    class Landscape;

    namespace cmn
    {
        // The original driver DLLs read the timer directly (the renderer reads +0x14), so the
        // layout is fixed: natural 8-byte packing for the double and the 64-bit counter.
#pragma pack(push, 8)
        class Timer
        {
            // Application::OneFrame reads the frame statistics directly.
            friend class m3d::Application;
            // Landscape::DrawWaterLayer reads the current time without notching it.
            friend class m3d::Landscape;

        public:
            Timer();
            unsigned int GetLastFrameTime() const;
            void SetActiveState(int);
            unsigned int GetFrameStartTimeUnscaled() const;
            unsigned int GetLastFrameTimeUnscaled() const;
            void NewFrame();
            unsigned int GetFrameStartTime() const;
            float GetRawFPS() const;
            unsigned int _GetCurTimeUnscaled() const;
            float GetFPS() const;
            float GetTimeScale();
            double GetFrameStartTimeSec() const;
            int GetCurFrame() const;
            unsigned int GetCurTimeUnscaled() const;
            unsigned int _GetCurTime() const;
            void SetTimeScale(float);
            unsigned int GetCurTime() const;

        private:
            void NotchCurTime() const;

        private:
            mutable uint32_t m_curTime = 0;
            mutable uint32_t m_prevTime = 0;
            mutable uint32_t m_curTimeUnscaled = 0;
            uint32_t m_frameStartTime = 0;
            uint32_t m_lastFrameTime = 0;
            uint32_t m_frameStartTimeUnscaled = 0;
            uint32_t m_lastFrameTimeUnscaled = 0;
            float m_fps = 0.0;
            uint32_t m_curFrame = 0;
            uint32_t m_fpsFrame = 0;
            uint32_t m_fpsTime = 0;
            double m_frameStartTimeSec = 0.0;
            float m_timescale = 0.0;
            int64_t m_performanceCounterFrequency = 0;
            bool m_bIsNewFrame = false;
            bool m_bJustActivated = false;
        };
#pragma pack(pop)

        static_assert(sizeof(Timer) == 0x0050);
    }
}

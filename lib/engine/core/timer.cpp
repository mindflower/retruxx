#include <core/timer.h>

#include <config.h>
#include <core/kernel.h>
// QueryPerformanceCounter and timeGetTime.
#include <Windows.h>
#include <mmsystem.h>

namespace m3d
{
    namespace cmn
    {
        // Frame length assumed for the first frame after the timer is (re)activated, in ms.
        static constexpr uint32_t kActivationFrameTime = 10;

        Timer::Timer()
        {
            // RVA 0x644900 - the timer starts at scale 1 and marked as just activated, so the
            // first frame gets the fixed activation length rather than the time since start-up.
            m_timescale = 1.0f;

            LARGE_INTEGER frequency;
            if (!::QueryPerformanceFrequency(&frequency))
            {
                frequency.QuadPart = 0;
            }
            m_performanceCounterFrequency = frequency.QuadPart;

            LARGE_INTEGER counter;
            counter.QuadPart = 0;
            if (m_performanceCounterFrequency != 0 && ::QueryPerformanceCounter(&counter))
            {
                m_prevTime = static_cast<uint32_t>(1000 * counter.QuadPart / m_performanceCounterFrequency);
            }

            m_bIsNewFrame = false;
            m_bJustActivated = true;
        }

        int Timer::GetCurFrame() const
        {
            return m_curFrame;
        }

        unsigned int Timer::GetCurTime() const
        {
            NotchCurTime();
            return m_curTime;
        }

        void Timer::NotchCurTime() const
        {
            // RVA 0x6449B0 - advances the clocks to now. With a fixed time step the clock only
            // moves, by m_timer_timestepvalue, when a new frame starts.
            uint32_t now;
            if (g_Kernel->GetEngineCfg().m_timer_fixedtimestep.GetB())
            {
                now = m_prevTime;
                if (m_bIsNewFrame)
                {
                    now += g_Kernel->GetEngineCfg().m_timer_timestepvalue.GetI();
                }
            }
            else
            {
                LARGE_INTEGER counter;
                counter.QuadPart = 0;
                if (m_performanceCounterFrequency != 0 && ::QueryPerformanceCounter(&counter))
                {
                    now = static_cast<uint32_t>(1000 * counter.QuadPart / m_performanceCounterFrequency);
                }
                else
                {
                    now = ::timeGetTime();
                }
            }

            uint32_t delta = now - m_prevTime;
            if (m_bJustActivated && !g_Kernel->GetEngineCfg().m_timer_fixedtimestep.GetB())
            {
                // Right after activation the real elapsed time (a pause, a load) is replaced
                // by one nominal frame, and intermediate notches do not advance the clock.
                delta = m_bIsNewFrame ? kActivationFrameTime : 0;
            }

            m_curTimeUnscaled += delta;
            m_prevTime = now;
            // The scaled step is truncated to whole milliseconds on its own, so fractions are
            // lost on every notch rather than accumulated.
            m_curTime += static_cast<int>(static_cast<double>(delta) * m_timescale);
        }

        unsigned int Timer::GetCurTimeUnscaled() const
        {
            NotchCurTime();
            return m_curTimeUnscaled;
        }

        float Timer::GetFPS() const
        {
            return m_timescale * m_fps;
        }

        unsigned int Timer::GetFrameStartTime() const
        {
            return m_frameStartTime;
        }

        double Timer::GetFrameStartTimeSec() const
        {
            return m_frameStartTimeSec;
        }

        unsigned int Timer::GetFrameStartTimeUnscaled() const
        {
            return m_frameStartTimeUnscaled;
        }

        unsigned int Timer::GetLastFrameTime() const
        {
            return m_lastFrameTime;
        }

        unsigned int Timer::GetLastFrameTimeUnscaled() const
        {
            return m_lastFrameTimeUnscaled;
        }

        void Timer::NewFrame()
        {
            // RVA 0x644B00
            m_bIsNewFrame = true;
            NotchCurTime();
            m_bIsNewFrame = false;

            if (m_bJustActivated)
            {
                m_lastFrameTimeUnscaled = kActivationFrameTime;
                m_lastFrameTime = static_cast<int>(m_timescale * 10.0f);
            }
            else
            {
                m_lastFrameTime = m_curTime - m_frameStartTime;
                m_lastFrameTimeUnscaled = m_curTimeUnscaled - m_frameStartTimeUnscaled;
            }

            ++m_fpsFrame;
            ++m_curFrame;
            m_bJustActivated = false;
            m_frameStartTime = m_curTime;
            m_frameStartTimeUnscaled = m_curTimeUnscaled;
            m_frameStartTimeSec = static_cast<double>(m_curTime) * 0.001;

            // The frame rate is re-measured once more than a second has passed.
            if (m_fpsTime == 0)
            {
                m_fpsTime = m_curTime;
            }
            uint32_t const elapsed = m_curTime - m_fpsTime;
            if (elapsed > 1000)
            {
                uint32_t const frames = m_fpsFrame;
                m_fpsTime = m_curTime;
                m_fpsFrame = 0;
                m_fps = static_cast<float>(static_cast<double>(frames) * 1000.0f / static_cast<double>(elapsed));
            }
        }

        float Timer::GetRawFPS() const
        {
            return m_fps;
        }

        float Timer::GetTimeScale()
        {
            return m_timescale;
        }

        void Timer::SetActiveState(int state)
        {
            // RVA 0x6449A0 - only activation is recorded. Deactivating does nothing: the flag
            // is cleared by the next NewFrame.
            if (state)
            {
                m_bJustActivated = true;
            }
        }

        void Timer::SetTimeScale(float timeScale)
        {
            m_timescale = timeScale;
        }

        unsigned int Timer::_GetCurTime() const
        {
            return m_curTime;
        }

        unsigned int Timer::_GetCurTimeUnscaled() const
        {
            return m_curTimeUnscaled;
        }
    }
}

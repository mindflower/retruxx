#include <core/timer.h>

//TODO: windows.h
#include <config.h>
#include <stdexcept>
#include <Windows.h>
#include <core/kernel.h>

namespace m3d
{
    namespace cmn
    {
        Timer::Timer()
        {
            LARGE_INTEGER largeInt;
            if (::QueryPerformanceFrequency(&largeInt))
            {
                m_performanceCounterFrequency = largeInt.QuadPart;
            }
            if (m_performanceCounterFrequency != 0 && ::QueryPerformanceCounter(&largeInt))
            {
                m_prevTime = 1000 * (largeInt.QuadPart / m_performanceCounterFrequency);
            }
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
            unsigned int frameTime = 0;
            if (g_Kernel->GetEngineCfg().m_timer_fixedtimestep.GetB())
            {
                if (m_bIsNewFrame)
                {
                    frameTime = m_prevTime + g_Kernel->GetEngineCfg().m_timer_timestepvalue.GetI();
                }
                else
                {
                    frameTime = m_prevTime;
                }
            }
            else
            {
                LARGE_INTEGER counter;
                if (m_performanceCounterFrequency == 0 || FALSE == ::QueryPerformanceCounter((&counter)))
                {
                    frameTime = ::timeGetTime();
                }
                else
                {
                    frameTime = 1000 * counter.QuadPart / m_performanceCounterFrequency;
                }
            }
            auto deltaTime = frameTime - m_prevTime;
            if (m_bJustActivated)
            {
                if (!g_Kernel->GetEngineCfg().m_timer_fixedtimestep.GetB())
                {
                    //TODO: magic number
                    deltaTime = m_bIsNewFrame ? 0xA : 0;
                }
            }
            m_curTimeUnscaled += deltaTime;
            m_prevTime = frameTime;
            //TODO: check correctness
            m_curTime += deltaTime * m_timescale;
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
            throw std::logic_error("Not implemented");
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
            //TODO: check correctness
            if (state != 0)
            {
                m_bJustActivated = true;
            }
            else
            {
                m_bJustActivated = false;
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

#include <core/timer.h>

//TODO: windows.h
#include <Windows.h>

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

        unsigned int Timer::GetCurTimeUnscaled() const
        {
            NotchCurTime();
            return m_curTimeUnscaled;
        }

        float Timer::GetFPS() const
        {
            return m_timescale * m_fps;
        }

        unsigned Timer::GetFrameStartTime() const
        {
            return m_frameStartTime;
        }

        double Timer::GetFrameStartTimeSec() const
        {
            return m_frameStartTimeSec;
        }

        unsigned Timer::GetFrameStartTimeUnscaled() const
        {
            return m_frameStartTimeUnscaled;
        }

        unsigned Timer::GetLastFrameTime() const
        {
            return m_lastFrameTime;
        }

        unsigned Timer::GetLastFrameTimeUnscaled() const
        {
            return m_lastFrameTimeUnscaled;
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

        unsigned Timer::_GetCurTime() const
        {
            return m_curTime;
        }

        unsigned Timer::_GetCurTimeUnscaled() const
        {
            return m_curTimeUnscaled;
        }
    }
}

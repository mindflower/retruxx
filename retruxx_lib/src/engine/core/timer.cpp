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
            //TODO: check this and refactor
            bool v2; // zf
            double v3; // st7
            unsigned int v4; // edx
            unsigned int v5; // eax
            unsigned int v6; // ecx
            unsigned int v7; // edx
            unsigned int v8; // ecx

            this->m_bIsNewFrame = 1;
            m3d::cmn::Timer::NotchCurTime();
            v2 = !this->m_bJustActivated;
            this->m_bIsNewFrame = 0;
            if (v2)
            {
                v4 = this->m_curTimeUnscaled - this->m_frameStartTimeUnscaled;
                this->m_lastFrameTime = this->m_curTime - this->m_frameStartTime;
                this->m_lastFrameTimeUnscaled = v4;
            }
            else
            {
                v3 = this->m_timescale;
                this->m_lastFrameTimeUnscaled = 10;
                this->m_lastFrameTime = (v3 * 10.0);
            }
            v5 = this->m_curTime;
            ++this->m_fpsFrame;
            ++this->m_curFrame;
            v6 = this->m_curTimeUnscaled;
            v7 = this->m_fpsFrame;
            this->m_bJustActivated = 0;
            this->m_frameStartTime = v5;
            this->m_frameStartTimeUnscaled = v6;
            v2 = this->m_fpsTime == 0;
            this->m_frameStartTimeSec = v5 * 0.001;
            if (v2)
                this->m_fpsTime = v5;
            v8 = v5 - this->m_fpsTime;
            if (v8 > 0x3E8)
            {
                this->m_fpsTime = v5;
                this->m_fpsFrame = 0;
                this->m_fps = v7 * 1000.0 / v8;
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

#pragma once

namespace m3d
{
    namespace cmn
    {
        class Timer
        {
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
            mutable unsigned int m_curTime = 0;
            mutable unsigned int m_prevTime = 0;
            mutable unsigned int m_curTimeUnscaled = 0;
            unsigned int m_frameStartTime = 0;
            unsigned int m_lastFrameTime = 0;
            unsigned int m_frameStartTimeUnscaled = 0;
            unsigned int m_lastFrameTimeUnscaled = 0;
            float m_fps = 0.0;
            unsigned int m_curFrame = 0;
            unsigned int m_fpsFrame = 0;
            unsigned int m_fpsTime = 0;
            long double m_frameStartTimeSec = 0.0;
            float m_timescale = 0.0;
            __int64 m_performanceCounterFrequency = 0;
            bool m_bIsNewFrame = false;
            bool m_bJustActivated = false;
        };
    }
}

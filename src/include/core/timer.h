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
            unsigned int m_curTime;
            unsigned int m_prevTime;
            unsigned int m_curTimeUnscaled;
            unsigned int m_frameStartTime;
            unsigned int m_lastFrameTime;
            unsigned int m_frameStartTimeUnscaled;
            unsigned int m_lastFrameTimeUnscaled;
            float m_fps;
            unsigned int m_curFrame;
            unsigned int m_fpsFrame;
            unsigned int m_fpsTime;
            long double m_frameStartTimeSec;
            float m_timescale;
            __int64 m_performanceCounterFrequency;
            bool m_bIsNewFrame;
            bool m_bJustActivated;
        };
    }
}

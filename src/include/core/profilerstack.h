#pragma once
#include <string>
#include <vector>

namespace m3d
{
    class Profiler
    {
    public:
        Profiler(char const*);
        Profiler();

        void EndCountdown();
        double GetAverageTime() const;
        void EndFrame();
        void SetAverageVal(unsigned int);
        void StartFrame();
        void SetName(char const*);
        char const* GetName() const;
        void StartCountdown();

    private:
        //std::mem_fun<void, m3d::Profiler>(void (*)());
        void Init();
        //std::mem_fun_t<void, m3d::Profiler>::mem_fun_t<void, m3d::Profiler>(void (*)());

    private:
        std::string m_name;
        unsigned int m_curFrame;
        unsigned int m_numFramesToRecalculate;
        float m_performanceCounterFrequency;
        __int64 m_totalClocks;
        __int64 m_totalClocksPerFrame;
        __int64 m_lastClocks;
        __int64 m_totalClocksForRecalcFrames;
        __int64 m_averageClocks;
    };

    class ProfilerStack
    {
    public:
        ProfilerStack();
        ~ProfilerStack();

        Profiler* GetProfiler(unsigned int);
        void Clear();
        unsigned int GetNumProfilers() const;
        void StartFrame();
        unsigned int AddProfiler(char const*, unsigned int);
        char const* GetName(unsigned int) const;
        void EndFrame();

    private:
        std::vector<m3d::Profiler*> m_stack;
    };

    class FrameProfilerPtr
    {
    public:
        FrameProfilerPtr(Profiler*);
        ~FrameProfilerPtr();

    private:
        Profiler* m_counter;
    };
}

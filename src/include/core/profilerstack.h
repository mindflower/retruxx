#pragma once
#include <string>
#include <vector>

namespace m3d
{
    class Profiler
    {
    public:
        Profiler();
        Profiler(char const* name);

        void EndCountdown();
        double GetAverageTime() const;
        void EndFrame();
        void SetAverageVal(unsigned int numFrames);
        void StartFrame();
        void SetName(char const* name);
        char const* GetName() const;
        void StartCountdown();

    private:
        //std::mem_fun<void, m3d::Profiler>(void (*)());
        void Init();
        //std::mem_fun_t<void, m3d::Profiler>::mem_fun_t<void, m3d::Profiler>(void (*)());

    private:
        std::string m_name = "default";
        unsigned int m_curFrame = 0;
        unsigned int m_numFramesToRecalculate = 30;
        float m_performanceCounterFrequency;
        __int64 m_totalClocks = 0;
        __int64 m_totalClocksPerFrame = 0;
        __int64 m_lastClocks = 0;
        __int64 m_totalClocksForRecalcFrames = 0;
        __int64 m_averageClocks = 0;
    };

    class ProfilerStack
    {
    public:
        ~ProfilerStack();

        Profiler* GetProfiler(unsigned int);
        void Clear();
        unsigned int GetNumProfilers() const;
        void StartFrame();
        unsigned int AddProfiler(char const*, unsigned int);
        char const* GetName(unsigned int) const;
        void EndFrame();

    private:
        std::vector<Profiler*> m_stack;
    };

    class FrameProfilerPtr
    {
    public:
        FrameProfilerPtr(Profiler* pCounter);
        ~FrameProfilerPtr();

    private:
        Profiler* m_counter;
    };
}

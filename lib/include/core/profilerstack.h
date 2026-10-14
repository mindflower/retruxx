#pragma once
#include "retruxx/common.h"

namespace m3d
{
    class Application;

    class Profiler
    {
        // Application::OneFrame reads the timings directly.
        friend class Application;

    public:
        Profiler(const m3d::Profiler&);
        Profiler(const char* name);
        Profiler();
        virtual ~Profiler() = default;
        void SetName(const char* name);
        const char* GetName() const;
        void StartFrame();
        void EndFrame();
        void StartCountdown();
        void EndCountdown();
        void Reset();
        unsigned long GetTotalTime() const;
        unsigned long GetLastFrameTime() const;
        void SetAverageVal(unsigned int numFrames);
        double GetAverageTime() const;

    private:
        retruxx::string m_name;
        unsigned int m_curFrame;
        unsigned int m_numFramesToRecalculate;
        float m_performanceCounterFrequency;
        int64_t m_totalClocks;
        int64_t m_totalClocksPerFrame;
        int64_t m_lastClocks;
        int64_t m_totalClocksForRecalcFrames;
        int64_t m_averageClocks;
        void Init();
    };

    class Application;

    class ProfilerStack
    {
        friend class Application;

    public:
        ProfilerStack(const m3d::ProfilerStack&);
        ProfilerStack();
        ~ProfilerStack();
        unsigned int AddProfiler(const char* name, unsigned int averageVal);
        const char* GetName(unsigned int id) const;
        m3d::Profiler* GetProfiler(unsigned int id);
        m3d::Profiler* GetProfilerByName(const char*);
        void StartFrame();
        void EndFrame();
        void Clear();
        unsigned int GetNumProfilers() const;

    private:
        retruxx::vector<m3d::Profiler*, retruxx::allocator<m3d::Profiler*> > m_stack;
        unsigned int m_numProfilers;
    };

    class ProfilerPtr
    {
    public:
        ProfilerPtr(m3d::Profiler*);
        ~ProfilerPtr();
        m3d::Profiler* GetProfiler() const;

    private:
        m3d::Profiler* m_counter;
    };

    class FrameProfilerPtr
    {
    public:
        FrameProfilerPtr(m3d::ProfilerPtr&);
        FrameProfilerPtr(m3d::Profiler* pCounter);
        ~FrameProfilerPtr();

    private:
        m3d::Profiler* m_counter;
    };
}

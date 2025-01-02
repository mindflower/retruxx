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
        /* 0x0000 */ retruxx::vector<m3d::Profiler*, retruxx::allocator<m3d::Profiler*> > m_stack;
        /* 0x0010 */ unsigned int m_numProfilers;
    }; /* size: 0x0014 */

    static_assert(sizeof(ProfilerStack) == 0x0014);

    class FrameProfilerPtr
    {
    public:
        FrameProfilerPtr(Profiler* pCounter);
        ~FrameProfilerPtr();

    private:
        Profiler* m_counter;
    };
}

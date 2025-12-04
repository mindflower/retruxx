#pragma once
#include "retruxx/common.h"

namespace m3d
{
    class Profiler
    {
    public:
        Profiler(const m3d::Profiler&);
        Profiler(const char* name);
        Profiler();
        virtual ~Profiler() = default /* 0x00 */;
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
        /* 0x0008 */ retruxx::string m_name;
        /* 0x0024 */ unsigned int m_curFrame;
        /* 0x0028 */ unsigned int m_numFramesToRecalculate;
        /* 0x002c */ float m_performanceCounterFrequency;
        /* 0x0030 */ int64_t m_totalClocks;
        /* 0x0038 */ int64_t m_totalClocksPerFrame;
        /* 0x0040 */ int64_t m_lastClocks;
        /* 0x0048 */ int64_t m_totalClocksForRecalcFrames;
        /* 0x0050 */ int64_t m_averageClocks;
        void Init();
    }; /* size: 0x0058 */

    static_assert(sizeof(Profiler) == 0x0058);

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

    class ProfilerPtr
    {
    public:
        ProfilerPtr(m3d::Profiler*);
        ~ProfilerPtr();
        m3d::Profiler* GetProfiler() const;

    private:
        /* 0x0000 */ m3d::Profiler* m_counter;
    }; /* size: 0x0004 */

    class FrameProfilerPtr
    {
    public:
        FrameProfilerPtr(m3d::ProfilerPtr&);
        FrameProfilerPtr(m3d::Profiler* pCounter);
        ~FrameProfilerPtr();

    private:
        /* 0x0000 */ m3d::Profiler* m_counter;
    }; /* size: 0x0004 */
}

#include <algorithm>
#include <m3dapp.h>
#include <core/profilerstack.h>

namespace m3d
{
    Profiler::Profiler() :
        m_performanceCounterFrequency(Application::g_pApp->GetMeasuredCpuFrequency())
    {
    }

    Profiler::Profiler(char const* name) :
        m_name(name),
        m_performanceCounterFrequency(Application::g_pApp->GetMeasuredCpuFrequency())
    {
    }

    void Profiler::Init()
    {
        m_curFrame = 0;
        m_numFramesToRecalculate = 30;
        m_performanceCounterFrequency = 0.0;
        m_totalClocks = 0;;
        m_totalClocksPerFrame = 0;
        m_lastClocks = 0;
        m_totalClocksForRecalcFrames = 0;
        m_averageClocks = 0;
        m_performanceCounterFrequency = Application::g_pApp->GetMeasuredCpuFrequency();
    }

    void Profiler::StartCountdown()
    {
        m_lastClocks = __rdtsc();
    }

    void Profiler::StartFrame()
    {
        m_totalClocksPerFrame = 0;
        m_lastClocks = 0;
    }

    void Profiler::EndCountdown()
    {
        auto const diffTime = __rdtsc() - m_lastClocks;
        m_totalClocksPerFrame += diffTime;
        m_totalClocksForRecalcFrames += diffTime;
    }

    void Profiler::EndFrame()
    {
        //TODO: check correctness
        ++m_curFrame;
        m_totalClocks += m_totalClocksPerFrame;
        if (m_curFrame >= m_numFramesToRecalculate || m_totalClocksForRecalcFrames > 1500000000)
        {
            m_averageClocks = m_totalClocksForRecalcFrames / m_curFrame;
            m_totalClocksForRecalcFrames = 0;
            m_curFrame = 0;
        }
    }

    double Profiler::GetAverageTime() const
    {
        return m_averageClocks / m_performanceCounterFrequency * 1000.0;
    }

    char const* Profiler::GetName() const
    {
        return m_name.c_str();
    }

    void Profiler::SetAverageVal(unsigned int numFrames)
    {
        m_numFramesToRecalculate = numFrames;
    }

    void Profiler::SetName(char const* name)
    {
        m_name = name;
    }

    void ProfilerStack::StartFrame()
    {
        std::for_each(m_stack.begin(), m_stack.end(), [](auto* profiler)
        {
            profiler->StartFrame();
        });
    }

    void ProfilerStack::EndFrame()
    {
        std::for_each(m_stack.begin(), m_stack.end(), [](auto* profiler)
        {
            profiler->EndFrame();
        });
    }

    FrameProfilerPtr::FrameProfilerPtr(Profiler* pCounter) :
        m_counter(pCounter)
    {
        m_counter->StartCountdown();
    }

    FrameProfilerPtr::~FrameProfilerPtr()
    {
        m_counter->EndCountdown();
    }

    unsigned int ProfilerStack::AddProfiler(char const* name, unsigned int averageVal)
    {
        //TODO: check correctness
        auto* profiler = new Profiler(name);
        profiler->SetAverageVal(averageVal);
        m_stack.push_back(profiler);
        return m_stack.size() - 1;
    }

    void ProfilerStack::Clear()
    {
        std::for_each(m_stack.begin(), m_stack.end(), [](auto* profiler)
        {
            delete profiler;
        });
        m_stack.clear();
    }

    char const* ProfilerStack::GetName(unsigned int id) const
    {
        return m_stack.at(id)->GetName();
    }

    unsigned int ProfilerStack::GetNumProfilers() const
    {
        return m_stack.size();
    }

    ProfilerStack::ProfilerStack() :
        m_numProfilers(0)
    {
    }

    ProfilerStack::~ProfilerStack()
    {
        Clear();
    }

    Profiler* ProfilerStack::GetProfiler(unsigned int id)
    {
        return m_stack.at(id);
    }
}

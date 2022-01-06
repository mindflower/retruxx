#include "processmanager.h"
#include <stdexcept>
#include <core/aiparam.h>
#include "event.h"

namespace ai
{
    ProcessManager* theProcessManager = nullptr;

    int ProcessManager::GetNumProcessedEvents() const
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::Update(float, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    ProcessManager::ProcessManager()
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& ProcessManager::GetEventName(eGameEvent) const
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    ProcessManager::~ProcessManager()
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    eGameEvent ProcessManager::GetEventId(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    int ProcessManager::GetNumEventsInQueue() const
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::UpdateDieEvents()
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::PostMessageA(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::PostMessageA(int, int, int, float, m3d::AIParam, m3d::AIParam, int)
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::_RegisterEvents()
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::_RegisterEvent(CStr const&, eGameEvent)
    {
        throw std::logic_error("Not implemented");
    }

    void ProcessManager::_EnsureAllEventsAreRegistered() const
    {
        throw std::logic_error("Not implemented");
    }
}

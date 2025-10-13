#include "radiomanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RadioManager)
        RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RadioManager);

    ai::Obj* RadioManagerPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    RadioManager::~RadioManager()
    {
        throw std::logic_error("Not implemented");
    }

    RadioManager::RadioManager(const ai::RadioManagerPrototypeInfo& prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RadioManager::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RadioManager::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RadioManager::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RadioManager::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::RadioManagerPrototypeInfo* RadioManager::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    int RadioManager::OnEvent(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::_OnObjectDie(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::_OnSomeoneAtSight(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::_OnUnderAttack(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::_OnRelationChanged(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::_OnPartBroken(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::_OnPlayerVehicleChanged(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::ReadyMessage(ai::RadioManager::RM_ENUM type, int belong, const CStr& mes)
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::EnableRadio()
    {
        throw std::logic_error("Not implemented");
    }

    void RadioManager::DisableRadio()
    {
        throw std::logic_error("Not implemented");
    }
}

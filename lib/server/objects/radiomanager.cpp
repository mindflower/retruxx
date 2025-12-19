#include "radiomanager.h"
#include "server/event.h"
#include "server/processmanager.h"
#include <game/m3dgame.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RadioManager)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RadioManager);

    ai::Obj* RadioManagerPrototypeInfo::CreateTargetObject() const
    {
        return new RadioManager(*this);
    }

    RadioManager::~RadioManager() = default;

    RadioManager::RadioManager(ai::RadioManagerPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_bRadioEnabled = true;
    }

    m3d::Object* RadioManager::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RadioManager::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RadioManager::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::Class* RadioManager::GetClass() const
    {
        return RT_CLASS_LOCAL(RadioManager);
    }

    ai::RadioManagerPrototypeInfo const* RadioManager::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RadioManager::OnEvent(ai::Event const& evn)
    {
        int result = Obj::OnEvent(evn);
        switch (evn.m_eventId)
        {
        case GE_OBJECT_DIE:
            ai::RadioManager::_OnObjectDie(evn);
            result = 1;
            break;
        case GE_PART_BROKEN:
            ai::RadioManager::_OnPartBroken(evn);
            result = 1;
            break;
        case GE_RELATION_CHANGED:
            ai::RadioManager::_OnRelationChanged(evn);
            result = 1;
            break;
        case GE_NOTICE_SOMEONE:
            ai::RadioManager::_OnSomeoneAtSight(evn);
            result = 1;
            break;
        case GE_UNDER_ATTACK:
            ai::RadioManager::_OnUnderAttack(evn);
            result = 1;
            break;
        case GE_PLAYER_VEHICLE_CHANGED:
            ai::RadioManager::_OnPlayerVehicleChanged(evn);
            result = 1;
            break;
        default:
            return result;
        }
        return result;
    }

    void RadioManager::_OnObjectDie(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RadioManager::_OnSomeoneAtSight(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RadioManager::_OnUnderAttack(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RadioManager::_OnRelationChanged(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RadioManager::_OnPartBroken(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RadioManager::_OnPlayerVehicleChanged(ai::Event const& evn)
    {
        if (m_bRadioEnabled)
        {
            M3D_APP->EnqueueMessage(SM_SOUND_MESSAGE, -1, 0, 0, 0, {}, {});
        }
    }

    void RadioManager::ReadyMessage(ai::RadioManager::RM_ENUM type, int belong, CStr const& mes)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RadioManager::EnableRadio()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RadioManager::DisableRadio()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

#include "dynamicquestdestroy.h"
#include <stdexcept>

#include "core/ini.h"

#include "base/prototypemanager.h"
#include "base/objcontainer.h"
#include "server/dynamicscene.h"
#include "server/processmanager.h"
#include "vehicle.h"

#include <core/kernel.h>
#include <core/log.h>
#include <m3dapp.h>
#include <server/utils.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestDestroy)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestDestroy);

    bool DynamicQuestDestroyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_targetSchwarzPart, xmlNode, "TargetSchwarzPart");
            return 1;
        }
        return result;
    }

    DynamicQuestDestroyPrototypeInfo::DynamicQuestDestroyPrototypeInfo()
    {
        m_targetSchwarzPart = 0.0;
    }

    DynamicQuest* DynamicQuestDestroyPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x82D830
        return new DynamicQuestDestroy(*this);
    }

    DynamicQuestDestroy::DynamicQuestDestroy(DynamicQuestDestroyPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        // RVA 0x82D750 - nothing of its own to set up.
    }

    void DynamicQuestDestroy::LoadNamesForTargetsFromXml(m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x82DF50 - the pool of names bandit leaders are given, restored from a save.
        m_namesForTargets.clear();
        if (!xmlNode->IsEmpty())
        {
            CStr strNames;
            m3d::SafeStrAttrib(strNames, xmlNode, "Names");
            ai::StrToStringVector(strNames, m_namesForTargets);
        }
    }

    m3d::Class* DynamicQuestDestroy::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    m3d::Class* DynamicQuestDestroy::GetClass() const
    {
        // RVA 0x82D740
        return RT_CLASS_LOCAL(DynamicQuestDestroy);
    }

    bool DynamicQuestDestroy::FreeNameForTargetExists()
    {
        // RVA 0x82DC60
        return !m_namesForTargets.empty();
    }

    DynamicQuestDestroyPrototypeInfo const* DynamicQuestDestroy::GetPrototypeInfo() const
    {
        // RVA 0x82DCA0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), DynamicQuestDestroyPrototypeInfo const);
    }

    void DynamicQuestDestroy::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x82DCD0 - the quest is forgotten once the hirer is dead, already being removed or inside a
        // repository, or once the target is gone.
        Obj::Update(elapsedTime, workTime);

        Obj* const hirer = theObjects->GetEntityByObjId(GetHirerObjId());
        Obj* const target = theObjects->GetEntityByObjId(GetTargetObjId());
        if (!hirer || (hirer->GetFlags() & 8) != 0 || (hirer->GetFlags() & 2) != 0 ||
            hirer->GetParentRepository() || !target)
        {
            _SetStatus(STATUS_FORGOTTEN);
        }
    }

    void DynamicQuestDestroy::SaveNamesForTargetsToXml(m3d::cmn::XmlNode* xmlNode)
    {
        // RVA 0x82D8A0
        xmlNode->SetAttribute("Names", ai::StringVectorToStr(m_namesForTargets).c_str());
    }

    DynamicQuestManager::QuestType DynamicQuestDestroy::GetQuestType() const
    {
        // RVA 0x82D770
        return DynamicQuestManager::TYPE_DESTROY;
    }

    int DynamicQuestDestroy::OnEvent(Event const& evn)
    {
        // RVA 0x82E160
        int const result = DynamicQuest::OnEvent(evn);
        if (evn.m_eventId == GE_OBJECT_DIE)
        {
            _OnObjectDie(evn);
            return 1;
        }
        return result;
    }

    CStr DynamicQuestDestroy::GetRandomNameForTarget()
    {
        // RVA 0x82DE30 - a name is taken out of the pool so that two bandit leaders never share one. Once
        // the pool runs dry the names stop being pretty.
        if (m_namesForTargets.empty())
        {
            return CStr("DynamicQuestDestroyTarget") + CStr(rand());
        }
        unsigned const index = static_cast<unsigned>(m_namesForTargets.size()) * rand() / 0x8000;
        CStr const res = m_namesForTargets[index];
        m_namesForTargets.erase(m_namesForTargets.begin() + index);
        return res;
    }

    int DynamicQuestDestroy::_CalcReward()
    {
        // RVA 0x82DD80 - the tougher the target, the better the pay.
        DynamicQuestDestroyPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (!gDynamicScene->GetVehicleControlledByPlayer())
        {
            return prototypeInfo->m_minReward;
        }
        // NOTE: the target is used without a null check.
        Obj* const target = theObjects->GetEntityByObjId(GetTargetObjId());
        int const reward = static_cast<int>(target->GetSchwarz() * prototypeInfo->m_targetSchwarzPart);
        return reward > prototypeInfo->m_minReward ? reward : prototypeInfo->m_minReward;
    }

    void DynamicQuestDestroy::_OnCreate()
    {
        // RVA 0x82E000 - wait for the target to die.
        if (GetTargetObjId() != -1)
        {
            m3d::AIParam param(static_cast<int>(GE_OBJECT_DIE));
            theProcessManager->PostMessageA(GE_SUBSCRIBE, GetTargetObjId(), GetId(), 0.0, param, {}, 1);
        }
    }

    DynamicQuestDestroy::~DynamicQuestDestroy()
    {
        // RVA 0x82D780 - the base class does all the work.
    }

    m3d::Object* DynamicQuestDestroy::Clone()
    {
        // RVA 0x82D8E0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void DynamicQuestDestroy::_OnObjectDie(Event const&)
    {
        // RVA 0x82E0A0 - the target is dead: the quest is done if anyone had taken it, and forgotten if
        // nobody had.
        if (GetQuestStatus() == STATUS_NOT_TAKEN || GetQuestStatus() == STATUS_FORGOTTEN)
        {
            _SetStatus(STATUS_FORGOTTEN);
        }
        else
        {
            _SetStatus(STATUS_COMPLETE);
        }
        m3d::AIParam param(static_cast<int>(GE_OBJECT_DIE));
        theProcessManager->PostMessageA(GE_UNSUBSCRIBE, GetTargetObjId(), GetId(), 0.0, param, {}, 1);
    }

    m3d::Object* DynamicQuestDestroy::CreateObject()
    {
        // RVA 0x82DAA0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}

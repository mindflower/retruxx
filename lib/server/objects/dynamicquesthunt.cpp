#include "dynamicquesthunt.h"
#include <stdexcept>

#include "core/ini.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/player.h"
#include "server/relationship.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(DynamicQuestHunt)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(DynamicQuestHunt);

    ai::DynamicQuest* DynamicQuestHuntPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestHuntPrototypeInfo::DynamicQuestHuntPrototypeInfo()
    {
        m_playerSchwarzPart = 0.0;
    }

    bool DynamicQuestHuntPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::DynamicQuestPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_playerSchwarzPart, xmlNode, "PlayerSchwarzPart");
            m3d::SafeFloatAttrib(m_huntSeasonLength, xmlNode, "HuntSeasonLength");
            return 1;
        }
        return result;
    }

    DynamicQuestHunt::DynamicQuestHunt(DynamicQuestHuntPrototypeInfo const& prototype) : DynamicQuest(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::ConsiderPlayerKill(int victimBelong)
    {
        // RVA 0x741C00 - a kill counts when the victim is hostile to the player and not of the hirer's own belong.
        if (m_questStatus != STATUS_PROCESSING)
        {
            return;
        }
        Obj* const hirer = theObjects->GetEntityByObjId(m_hirerObjId);
        if (!hirer || !thePlayer)
        {
            return;
        }
        int const hirerBelong = hirer->GetBelong();
        if (theRelationship->CheckTolerance(victimBelong, thePlayer->GetBelong()) <= RS_ENEMY && victimBelong != hirerBelong)
        {
            ++m_fragsAtStart;
        }
    }

    DynamicQuestManager::QuestType DynamicQuestHunt::GetQuestType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::UpdateBeforeShowInConversation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float DynamicQuestHunt::GetTimeBeforeHuntEnd() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestHunt::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::ProcessHuntCompletion()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestHuntPrototypeInfo const* DynamicQuestHunt::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float DynamicQuestHunt::GetTimeFromHuntStart() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* DynamicQuestHunt::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DynamicQuest);
    }

    void DynamicQuestHunt::PassToAnotherMap()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::_OnTake()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DynamicQuestHunt::_CalcReward()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuestHunt::~DynamicQuestHunt()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DynamicQuestHunt::_OnCreate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestHunt::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* DynamicQuestHunt::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

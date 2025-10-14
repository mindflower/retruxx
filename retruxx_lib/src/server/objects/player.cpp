#include "player.h"
#include "thirdparty/injecttools.h"
#include <stdexcept>
#include <client.h>

#include "m3dapp.h"
#include "vehicle.h"
#include "world.h"
#include "base/globalproperties.h"
#include "core/ini.h"
#include "server/infocone.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "core/log.h"
#include "server/event.h"
#include "server/utils.h"
#include <server/processmanager.h>

#include "radiomanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMoney)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddMoney)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetHealth)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMaxHealth)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetFuel)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMaxFuel)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetVehicle)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto vehicle = player->GetVehicle();
    context->pushObject(vehicle);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddItemsToRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, RemoveItemsFromRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, HasAmountOfItemsInRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, CanPlaceItemsToRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, IsQuestItemPresent)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddQuestItem)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, RemoveQuestItem)
{
    throw std::logic_error("Not implemented");
}

namespace ai
{
    namespace 
    {
        class TestPlayerVisibility : public m3d::IsNodeTransparent
        {
        public:
            virtual bool test(m3d::SgNode*, float) override /* 0x00 */;
            virtual float getTransparentRadius() override /* 0x04 */;
            virtual bool setPermanentTransparency(m3d::SgNode*) override /* 0x08 */;
        }; /* size: 0x0004 */

        bool TestPlayerVisibility::test(m3d::SgNode*, float)
        {
            throw std::logic_error("Not implemented");
        }

        float TestPlayerVisibility::getTransparentRadius()
        {
            auto* vehicle = ai::gDynamicScene->GetVehicleControlledByPlayer();
            if (!vehicle)
                return 0.0;

            const auto center = vehicle->GetGeometricCenter();
            auto invMat = M3D_RENDERER->MatGetOrgInv();

            CVector v9;
            v9.x = invMat.x - center.x;
            v9.y = invMat.y - center.y;
            v9.z = invMat.z - center.z;
            auto size = vehicle->GetSize().z * 0.5;
            return v9.length() + size;
        }

        bool TestPlayerVisibility::setPermanentTransparency(m3d::SgNode*)
        {
            throw std::logic_error("Not implemented");
        }
    }

    RT_CLASS_EXPORTS_BEGIN(Player)
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetMoney, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, AddMoney, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetHealth, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetMaxHealth, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetFuel, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetMaxFuel, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetVehicle, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, AddItemsToRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, RemoveItemsFromRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, HasAmountOfItemsInRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, CanPlaceItemsToRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, IsQuestItemPresent, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, AddQuestItem, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, RemoveQuestItem, "", "", "")
    RT_CLASS_EXPORTS_END;

    RT_CLASS_DEFINE(Player);

    CStr const& PlayerPrototypeInfo::GetModelName() const
    {
        return m_modelName;
    }

    PlayerPrototypeInfo::PlayerPrototypeInfo()
    {
        this->m_skinNumber = 0;
        this->m_cfgNumber = 0;
        m3d::pClient->GetWorld().GetGraph().SetTransparencyTest(new TestPlayerVisibility);
    }

    ai::Obj* PlayerPrototypeInfo::CreateTargetObject() const
    {
        return new Player(*this);
    }

    unsigned PlayerPrototypeInfo::GetSkinNumber() const
    {
        return m_skinNumber;
    }

    unsigned PlayerPrototypeInfo::GetCfgNumber() const
    {
        return m_cfgNumber;
    }

    bool PlayerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(this->m_modelName, xmlNode, "ModelFile");
            m3d::SafeUintAttrib(this->m_skinNumber, xmlNode, "SkinNum");
            m3d::SafeUintAttrib(this->m_cfgNumber, xmlNode, "CfgNum");
            return 1;
        }
        return result;
    }

    m3d::Class* Player::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    RETRUXX_DLL_INJECT_FUNCTION(0x0064FA80, Player::GetMoney)
    int Player::GetMoney() const
    {
        return m_money.value().get();
    }

    void Player::ChangeVehicleByExisting(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::Update(float elapsedTime, unsigned)
    {
        // TODO: generated code
        // Early return for very small time steps
        if (elapsedTime < 0.001f)
            return;

        PlayerFightState currentFightState = m_playerFightState;

        // Handle transition from battle state
        if (currentFightState != FIGHT_BATTLE)
        {
            PlayerFightState prevState = m_prevPlayerFightState;
            if (prevState == FIGHT_BATTLE || prevState == FIGHT_BATTLE_JUST_FINISHED)
            {
                m_playerFightState = FIGHT_BATTLE_JUST_FINISHED;
                if (prevState == FIGHT_BATTLE)
                {
                    m_timeOfNoBattle.setToMin();
                }
            }
        }

        // Handle fight state changes and messaging
        PlayerFightState newFightState = m_playerFightState;
        if (newFightState >= FIGHT_CLEAR && newFightState <= FIGHT_BATTLE)
        {
            if (newFightState != m_prevPlayerFightState)
            {
                // Send state change message
                M3D_APP->EnqueueMessage(66555, // Message ID for fight state change
                    newFightState,
                    0, 0, 0,
                    {},
                    {}
                );
            }
        }
        else if (newFightState == FIGHT_BATTLE_JUST_FINISHED)
        {
            // Regenerate cooldown timer after battle
            m_timeOfNoBattle.regenerate(elapsedTime);

            // Check if cooldown is complete
            if (m_timeOfNoBattle.value().get() >= m_timeOfNoBattle.maxValue().get())
            {
                bool stateChanged = (currentFightState != m_prevPlayerFightState);
                m_playerFightState = currentFightState;

                if (stateChanged)
                {
                    // Send state reversion message
                    M3D_APP->EnqueueMessage(66555, // Message ID for fight state change
                        currentFightState,
                        0, 0, 0,
                        {},
                        {}
                    );
                }
            }
        }

        // Update state tracking
        PlayerFightState finalState = m_playerFightState;
        m_playerFightState = FIGHT_CLEAR;
        m_prevPlayerFightState = finalState;

        // Update info object tracking with timeout
        if (ai::theGlobProp.m_infoObjUpdateTimeout <= m_timeInfoObjTimeout)
        {
            m_timeInfoObjTimeout = 0.0f;
            m_infoObjId = -1;

            // Update vehicle info if we have a valid vehicle
            if (m_vehicleObjId != -1 && m_infoCone->SetVehicleId(m_vehicleObjId))
            {
                m_infoObjId = m_infoCone->GetInfoObjId();
            }
        }
        else
        {
            m_timeInfoObjTimeout += elapsedTime;
        }
    }

    retruxx::vector<CStr> const& Player::GetQuestItemPrototypeNames() const
    {
        throw std::logic_error("Not implemented");
    }

    float Player::GetFuel() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::HuntQuestIsTaken() const
    {
        throw std::logic_error("Not implemented");
    }

    int Player::RemoveQuestItem(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::GetPropertiesNames(retruxx::set<CStr>&) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Player::GetCfgNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Player::GetModelName() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::CanPlaceItemsToRepository(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    Player::Player(PlayerPrototypeInfo const& prototypeInfo) :
        Obj(prototypeInfo),
        m_money{ 0, 0 },
        m_timeOfNoBattle{ 0.0, 0.0, 7.0, 1.0 }
    {
        m_radioManager = 0;
        this->m_vehicleObjId = -1;
        this->m_playerFightState = NUM_FIGHT_STATES;
        this->m_prevPlayerFightState = NUM_FIGHT_STATES;
        m_lastSaveDir = "saves\\__tmp__save\\";

        m_money.m_AfterValueChange = new ai::MemberFunctionOneArg<ai::Player, int, void>(*this, &Player::_OnMoneyValueAfterChange);
        m_infoObjId = -1;

        m_infoCone = new InfoCone;
        this->m_timeInfoObjTimeout = 0.0;
        this->m_modelName = prototypeInfo.GetModelName();
        this->m_skinNumber = prototypeInfo.GetSkinNumber();
        this->m_cfgNumber = prototypeInfo.GetCfgNumber();
        this->m_huntQuestIsTaken = 0;
        this->m_curNumForVehicleWithoutName = 0;
    }

    Vehicle* Player::GetVehicle() const
    {
        auto* vehicle = dynamic_cast<Vehicle*>(theObjects->GetEntityByObjId(m_vehicleObjId));
        if (vehicle)
        {
            if ((vehicle->GetFlags() & 8) != 0 || (vehicle->GetFlags() & 2) != 0 || vehicle->GetParentRepository())
                return nullptr;
        }
        return vehicle;
    }

    int Player::OnEvent(Event const& evn)
    {
        auto result = ai::Obj::OnEvent(evn);
        switch (evn.m_eventId)
        {
        case GE_OBJECT_DIE:
            ai::Player::_OnObjectDie(evn);
            result = 1;
            break;
        case GE_UNDER_ATTACK:
            if (this->m_playerFightState >= FIGHT_BATTLE)
            {
                result = 1;
                break;
            }
            this->m_playerFightState = FIGHT_BATTLE;
            result = 1;
            break;
        case GE_NOTICE_ENEMY:
            result = 1;
            if (this->m_playerFightState < FIGHT_ALARM)
                this->m_playerFightState = FIGHT_ALARM;
            break;
        case GE_DYNAMIC_QUEST_TAKEN:
            ai::Player::_OnDynamicQuestTaken(evn);
            result = 1;
            break;
        case GE_DYNAMIC_QUEST_COMPLETE:
            ai::Player::_OnDynamicQuestComplete(evn);
            result = 1;
            break;
        case GE_DYNAMIC_QUEST_FORGOTTEN:
            ai::Player::_OnDynamicQuestForgotten(evn);
            result = 1;
            break;
        case GE_DYNAMIC_QUEST_FAILED:
            ai::Player::_OnDynamicQuestFailed(evn);
            result = 1;
            break;
        default:
            return result;
        }
        return result;
    }

    void Player::AddMoney(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::IsQuestItemPresent(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::ChangeVehicleByNew(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int Player::GetPropertyId(char const* propName) const
    {
        auto it = Player::m_propertiesMap.find(propName);
        if (it != Player::m_propertiesMap.end())
        {
            return it->second;
        }

        return Obj::GetPropertyId(propName);
    }

    IzvratRepository* Player::GetRepository() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        m_questItemPrototypeNames.clear();

        ref_ptr questItemsNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(questItemsNode, "QuestItems");
        if (!questItemsNode->IsEmpty())
        {
            retruxx::vector<CStr> questItemPrototypeNames;
            CStr prototypeNames;
            m3d::SafeStrAttrib(prototypeNames, questItemsNode, "PrototypeNames");
            StrToStringVector(prototypeNames, questItemPrototypeNames);
            for (auto& name : questItemPrototypeNames)
            {
                if (!AddQuestItem(name))
                {
                    M3D_LOG_ERR("Player::LoadFromXML error - fail to add quest item for prototype " + name);
                }
            }
        }
    }

    float Player::GetMaxHealth() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Player::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        switch (propertyId)
        {
        case 11:
            m_money.value().set(newValue.GetAsID());
            return true;

        case 44:
            m_modelName = newValue.GetAsStr();
            return true;

        case 45:
            m_skinNumber = newValue.GetAsID();
            return true;

        case 46:
            this->m_cfgNumber = newValue.GetAsID();
            return true;

        default:
            return Obj::SetPropertyById(propertyId, newValue);
        }
        return false;
    }

    void Player::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float Player::GetHealth() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::AddChild(Obj* obj)
    {
        Obj::AddChild(obj);
        if (obj)
        {
            if (obj->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                if (m_vehicleObjId == -1)
                {
                    m_vehicleObjId = obj->GetId();

                    auto newVehicle = dynamic_cast<Vehicle*>(theObjects->GetEntityByObjId(m_vehicleObjId));
                    newVehicle->m_bIsControlledByPlayer = true;
                    newVehicle->SetMoveStatus(Vehicle::MOVE_IDLE);
                    newVehicle->SetAttackStatus(Vehicle::ATTACK_IDLE);
                    newVehicle->SetUpdatingByODE(true);
                    newVehicle->UnsubscribeRadioManagerFromAllNearbyObjIds();
                    if (CStr(newVehicle->GetName()).empty())
                    {
                        newVehicle->SetName("PlayerVehicle_" + CStr(m_vehicleObjId));
                        ++m_curNumForVehicleWithoutName;
                    }

                    M3D_APP->ImmediateMessage(66544, 0, 0, 0, 0, {}, {});
                    CauseEvent(GE_PLAYER_VEHICLE_CHANGED, 0.0, {}, {});
                    if (m_radioManager)
                    {
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, { 9 }, {}, 1);
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, { 44 }, {}, 1);
                    }
                }
            }
            else
            {
                if (!obj->IsKindOf(&ai::RadioManager::m_classRadioManager))
                {
                    return;
                }
                if (!m_radioManager)
                {
                    m_radioManager = dynamic_cast<RadioManager*>(obj);
                    if (m_vehicleObjId != -1)
                    {
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, { 9 }, {}, 1);
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, { 44 }, {}, 1);
                    }
                    theProcessManager->PostMessageA(2, GetId(), m_radioManager->GetId(), 0.0, { 65 }, {}, 1);
                }
            }
            LinkToParent(GetId(), HIERARCHY_CHILD);
        }
    }

    float Player::GetMaxFuel() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::RemoveItemsFromRepository(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    PlayerPrototypeInfo const* Player::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::AddItemsToRepository(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::Registration()
    {
        m_propertiesMap["Money"] = 11;
        m_propertiesMap["ModelName"] = 44;
        m_propertiesMap["skin"] = 45;
        m_propertiesMap["cfg"] = 46;
    }

    void Player::GetPropertiesIDs(retruxx::set<int>&) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Player::GetSchwarz() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Player::GetClass() const
    {
        return RT_CLASS_LOCAL(Player);
    }

    InfoCone const& Player::GetInfoCone() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Player::AddQuestItem(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::HasAmountOfItemsInRepository(char const*, int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Player::GetLastSaveDir() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SetLastSaveDir(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Player::GetInfoObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Player::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    int Player::GetRadioManagerId() const
    {
        throw std::logic_error("Not implemented");
    }

    NumericBoundedBelow<int>& Player::Money()
    {
        throw std::logic_error("Not implemented");
    }

    NumericBoundedBelow<int> const& Player::Money() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Player::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::CanChildBeAdded(m3d::Class* pClass) const
    {
        return ai::Obj::CanChildBeAdded(pClass) || pClass->m3d::Class::IsKindOf(&ai::Vehicle::m_classVehicle);
    }

    unsigned Player::GetSkinNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    Player::~Player()
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_InternalPostLoad()
    {
        auto protoId = thePrototypeManager->GetPrototypeId("radioManager");
        auto radioManagerId = theObjects->CreateNewObject(protoId, "", this->GetId(), -1);
        auto radioManager = theObjects->GetEntityByObjId(radioManagerId);
        AddChild(radioManager);
    }

    void Player::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestForgotten(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Player::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestFailed(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnUnderAttack(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Player::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestTaken(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnMoneyValueAfterChange(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnNoticeEnemy(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnObjectDie(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestComplete(Event const&)
    {
        throw std::logic_error("Not implemented");
    }
}

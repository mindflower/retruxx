#include "player.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <client.h>

#include <config.h>
#include <level.h>
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
#include <server/resourcemanager.h>
#include <server/server.h>
#include <server/statistic/statisticmanager.h>
#include <server/statistic/intstatistic.h>
#include <scene/nodes/sgnode.h>
#include <skelmodel.h>
#include <game/m3dgame.h>

#include "dynamicquesthunt.h"
#include "radiomanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMoney)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    context->pushInt(player->GetMoney());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddMoney)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    player->AddMoney(context->asInt(1));
    return 0;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetHealth)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    context->pushFloat(player->GetHealth());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMaxHealth)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    context->pushFloat(player->GetMaxHealth());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetFuel)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    context->pushFloat(player->GetFuel());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMaxFuel)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    context->pushFloat(player->GetMaxFuel());
    return 1;
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
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto res = player->AddItemsToRepository(context->asString(1), context->asInt(2));
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, RemoveItemsFromRepository)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto res = player->RemoveItemsFromRepository(context->asString(1), context->asInt(2));
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, HasAmountOfItemsInRepository)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto res = player->HasAmountOfItemsInRepository(context->asString(1), context->asInt(2));
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, CanPlaceItemsToRepository)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto res = player->CanPlaceItemsToRepository(context->asString(1), context->asInt(2));
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, IsQuestItemPresent)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto res = player->IsQuestItemPresent(context->asString(1));
    context->pushBool(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddQuestItem)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto name = context->asString(1);
    auto res = player->AddQuestItem(name);
    context->pushInt(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, RemoveQuestItem)
{
    auto* player = (ai::Player*)context->asObject(0, "Player");
    auto res = player->RemoveQuestItem(context->asString(1));
    context->pushInt(res);
    return 1;
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

        float Clamp01(float v)
        {
            if (v < 0.0f)
            {
                return 0.0f;
            }
            if (v > 1.0f)
            {
                return 1.0f;
            }
            return v;
        }

        bool TestPlayerVisibility::test(m3d::SgNode* n, float distCoeff)
        {
            // RVA 0x24F310 - fades a scene node out while it occludes the player's
            // vehicle in follow-camera mode.
            ai::Vehicle* veh = ai::gDynamicScene->GetVehicleControlledByPlayer();
            if (M3D_APP->m_player.m_cameraMode != CM_FOLLOWMODE || !veh || M3D_APP->GetCurGameMode())
            {
                return false;
            }

            Aabb const box = n->GetAabb();

            // Project the eight corners of the node's world-space bounding box.
            CVector projectedBox[8];
            int idx = 0;
            for (int ix = 0; ix < 2; ++ix)
            {
                for (int iy = 0; iy < 2; ++iy)
                {
                    for (int iz = 0; iz < 2; ++iz)
                    {
                        CVector corner{box.m_box[3 * ix], box.m_box[3 * iy + 1], box.m_box[3 * iz + 2]};
                        corner = n->GetCurrentMatrix().vecMul(corner);
                        CVector projected;
                        M3D_RENDERER->ProjectWorldAbs(&projected, &corner);
                        projectedBox[idx++] = projected;
                    }
                }
            }

            float minX = projectedBox[0].x;
            float maxX = projectedBox[0].x;
            float minY = projectedBox[0].y;
            float maxY = projectedBox[0].y;
            for (int i = 1; i < 8; ++i)
            {
                if (minX > projectedBox[i].x)
                {
                    minX = projectedBox[i].x;
                }
                if (projectedBox[i].x > maxX)
                {
                    maxX = projectedBox[i].x;
                }
                if (minY > projectedBox[i].y)
                {
                    minY = projectedBox[i].y;
                }
                if (projectedBox[i].y > maxY)
                {
                    maxY = projectedBox[i].y;
                }
            }

            PointBase<float> const center{(maxX + minX) * 0.5f, (maxY + minY) * 0.5f};
            PointBase<float> const hsize{(maxX - minX) * 0.5f, (maxY - minY) * 0.5f};

            CVector const vehPos = veh->GetGeometricCenter();
            CVector projOrg;
            M3D_RENDERER->ProjectWorldAbs(&projOrg, &vehPos);
            CVector const sz = veh->GetSize();
            float const onScreenSize = M3D_APP->GetOnScreenSize(vehPos, sz.x * 0.80000001f);

            float const deltaX = std::fabs(center.x - projOrg.x) - hsize.x - onScreenSize;
            float const deltaY = std::fabs(center.y - projOrg.y) - hsize.y - onScreenSize;
            float const delta = (deltaY <= deltaX) ? deltaX : deltaY;
            if (onScreenSize <= delta)
            {
                return false;
            }

            float const distanceFade = Clamp01(delta / onScreenSize);

            float transparency = M3D_ENGINE_CFG.m_g_transparencyValue.GetF();
            if (distCoeff > 0.80000001f)
            {
                transparency = ((transparency - 1.0f) * distCoeff + (0.80000001f - transparency)) * -5.0000005f;
            }
            transparency = Clamp01(transparency);

            n->GetTransparencyParams().value = Clamp01(transparency + distanceFade);
            return true;
        }

        float TestPlayerVisibility::getTransparentRadius()
        {
            // RVA 0x24F820
            auto* vehicle = ai::gDynamicScene->GetVehicleControlledByPlayer();
            if (!vehicle)
                return 0.0;

            auto const center = vehicle->GetGeometricCenter();
            auto invMat = M3D_RENDERER->MatGetOrgInv();

            CVector v9;
            v9.x = invMat.x - center.x;
            v9.y = invMat.y - center.y;
            v9.z = invMat.z - center.z;
            float const size = vehicle->GetSize().z * 0.5f;
            return v9.length() + size;
        }

        bool TestPlayerVisibility::setPermanentTransparency(m3d::SgNode* node)
        {
            // RVA 0x24F8D0 - precomputes the distance at which a node starts fading,
            // from its model width and the vertical field of view.
            if (M3D_ENGINE_CFG.m_r_multiSamplesNum.GetI() > 2)
            {
                return false;
            }

            m3d::AnimatedModel* mdl = nullptr;
            node->GetServerItemProperty(0x400Au, &mdl);
            if (!mdl)
            {
                return false;
            }

            m3d::rend::Viewport const port = M3D_RENDERER->GetViewport();
            Aabb const& box = mdl->GetAabb();
            float const objectWidth = box.m_box[3] - box.m_box[0];
            node->GetTransparencyParams().objectWidth = objectWidth;

            double const halfFovTan = std::tan(M3D_APP->m_curCamera.m_fovY * 0.5 * 0.017453292);
            node->GetTransparencyParams().startDist =
                static_cast<float>(objectWidth / ((halfFovTan + halfFovTan) / static_cast<double>(port.m_height)));
            return true;
        }
    }  // namespace

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

    m3d::Class* Player::GetClass() const
    {
        return RT_CLASS_LOCAL(Player);
    }

    m3d::Object* Player::Clone()
    {
        // RVA 0x2500D0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Player::CreateObject()
    {
        // RVA 0x250290
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    Player::Player(PlayerPrototypeInfo const& prototypeInfo) :
        Obj(prototypeInfo),
        m_money{0, 0},
        m_timeOfNoBattle{0.0, 0.0, 7.0, 1.0}
    {
        // RVA 0x253630
        m_radioManager = 0;
        this->m_vehicleObjId = -1;
        this->m_playerFightState = NUM_FIGHT_STATES;
        this->m_prevPlayerFightState = NUM_FIGHT_STATES;
        m_lastSaveDir = "saves\\__tmp__save\\";

        m_money.m_AfterValueChange =
            new ai::MemberFunctionOneArg<ai::Player, int, void>(*this, &Player::_OnMoneyValueAfterChange);
        m_infoObjId = -1;

        m_infoCone = new InfoCone;
        this->m_timeInfoObjTimeout = 0.0;
        this->m_modelName = prototypeInfo.GetModelName();
        this->m_skinNumber = prototypeInfo.GetSkinNumber();
        this->m_cfgNumber = prototypeInfo.GetCfgNumber();
        this->m_huntQuestIsTaken = 0;
        this->m_curNumForVehicleWithoutName = 0;
    }

    Player::~Player()
    {
        // RVA 0x251C70 - every other member releases itself.
        delete m_infoCone;
        m_infoCone = nullptr;
    }

    PlayerPrototypeInfo const* Player::GetPrototypeInfo() const
    {
        // RVA 0x251160
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), PlayerPrototypeInfo const);
    }

    // ------------------------------------------------------------------------
    //  Vehicle
    // ------------------------------------------------------------------------

    Vehicle* Player::GetVehicle() const
    {
        // RVA 0x251190
        auto* vehicle = dynamic_cast<Vehicle*>(theObjects->GetEntityByObjId(m_vehicleObjId));
        if (vehicle)
        {
            if ((vehicle->GetFlags() & 8) != 0 || (vehicle->GetFlags() & 2) != 0 || vehicle->GetParentRepository())
                return nullptr;
        }
        return vehicle;
    }

    void Player::ChangeVehicleByNew(int newVehiclePrototypeId, bool bDeleteOldVehicle)
    {
        // RVA 0x251BF0
        int newVehicleObjId = -1;
        if (newVehiclePrototypeId != -1)
        {
            newVehicleObjId = theObjects->CreateNewObject(newVehiclePrototypeId, "Player1", -1, -1);
        }
        ChangeVehicleByExisting(newVehicleObjId, bDeleteOldVehicle);
    }

    void Player::ChangeVehicleByExisting(int newVehicleObjId, bool bDeleteOldVehicle)
    {
        // RVA 0x2511F0
        Vehicle* oldVehicle = GetVehicle();
        RemoveChild(oldVehicle);
        if (oldVehicle && bDeleteOldVehicle)
        {
            oldVehicle->Remove();
        }

        auto* newVehicle = dynamic_cast<Vehicle*>(theObjects->GetEntityByObjId(newVehicleObjId));
        if (!newVehicle)
        {
            return;
        }

        newVehicle->SetUpdatingByODE(true);
        AddChild(newVehicle);
        if (oldVehicle)
        {
            // Inherit the outgoing vehicle's placement, allegiance and god mode.
            newVehicle->SetPosition(oldVehicle->GetPosition());
            newVehicle->SetRotation(oldVehicle->GetRotation());
            newVehicle->SetBelong(oldVehicle->GetBelong());
            newVehicle->setGodMode(oldVehicle->getGodMode());
            oldVehicle->setGodMode(false);
        }
    }

    // ------------------------------------------------------------------------
    //  Vehicle-backed accessors
    // ------------------------------------------------------------------------

    float Player::GetHealth() const
    {
        // RVA 0x2512D0
        auto* vehicle = GetVehicle();
        return vehicle ? vehicle->GetHealth() : 0.0f;
    }

    float Player::GetMaxHealth() const
    {
        // RVA 0x2512F0
        auto* vehicle = GetVehicle();
        return vehicle ? vehicle->GetMaxHealth() : 0.0f;
    }

    float Player::GetFuel() const
    {
        // RVA 0x251310
        auto* vehicle = GetVehicle();
        return vehicle ? vehicle->GetFuel() : 0.0f;
    }

    float Player::GetMaxFuel() const
    {
        // RVA 0x251330
        auto* vehicle = GetVehicle();
        return vehicle ? vehicle->GetMaxFuel() : 0.0f;
    }

    unsigned Player::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x251350
        unsigned price = static_cast<unsigned>(GetMoney());
        if (auto* vehicle = GetVehicle())
        {
            price += vehicle->GetPrice(priceCoeffProvider);
        }
        return price;
    }

    unsigned Player::GetSchwarz() const
    {
        // RVA 0x251380
        unsigned schwarz = static_cast<unsigned>(GetMoney());
        if (auto* vehicle = GetVehicle())
        {
            schwarz += vehicle->GetSchwarz();
        }
        return schwarz;
    }

    bool Player::AddItemsToRepository(char const* prototypeName, int amount)
    {
        // RVA 0x2513A0
        auto* vehicle = GetVehicle();
        return vehicle && vehicle->AddItemsToRepository(prototypeName, amount);
    }

    bool Player::RemoveItemsFromRepository(char const* prototypeName, int amount)
    {
        // RVA 0x2513C0
        auto* vehicle = GetVehicle();
        return vehicle && vehicle->RemoveItemsFromRepository(prototypeName, amount);
    }

    bool Player::HasAmountOfItemsInRepository(char const* prototypeName, int amount) const
    {
        // RVA 0x2513E0
        auto* vehicle = GetVehicle();
        return vehicle && vehicle->HasAmountOfItemsInRepository(prototypeName, amount);
    }

    bool Player::CanPlaceItemsToRepository(char const* prototypeName, int amount)
    {
        // RVA 0x251400
        auto* vehicle = GetVehicle();
        return vehicle && vehicle->CanPlaceItemsToRepository(prototypeName, amount);
    }

    IzvratRepository* Player::GetRepository() const
    {
        // RVA 0x251420
        if (auto* vehicle = GetVehicle())
        {
            return vehicle->GetRepository();
        }

        M3D_LOG_INFO("Warning: cannot get repository cause the player hasn't got any vehicle");
        return nullptr;
    }

    // ------------------------------------------------------------------------
    //  Money
    // ------------------------------------------------------------------------

    NumericBoundedBelow<int>& Player::Money()
    {
        // RVA 0x24EF10
        return m_money;
    }

    NumericBoundedBelow<int> const& Player::Money() const
    {
        // RVA 0x24EF20
        return m_money;
    }

    int Player::GetMoney() const
    {
        return m_money.value().get();
    }

    void Player::AddMoney(int amount)
    {
        // RVA 0x250670
        int newValue;
        if (amount < 0)
        {
            // Spending never takes the balance below zero.
            int const current = m_money.value().get();
            int spent = -amount;
            if (current < -amount)
            {
                spent = current;
            }
            newValue = current - spent;
        }
        else
        {
            // Income feeds both the global and the per-level GDP statistic.
            auto* gdp = static_cast<IntStatistic*>(theStatisticManager->GetStatistic(STATISTIC_GDP, "IntStatistic"));
            gdp->SetGlobalFlag(true);
            gdp->Increase(amount);

            auto* levelGdp = static_cast<IntStatistic*>(theStatisticManager->GetStatistic(
                STATISTIC_GDP + pServer->GetWorld()->m_level->m_levelName, "IntStatistic"));
            levelGdp->SetGlobalFlag(false);
            levelGdp->Increase(amount);

            newValue = amount + m_money.value().get();
        }

        m_money.value().set(newValue);
    }

    void Player::_OnMoneyValueAfterChange(int oldMoneyValue)
    {
        // RVA 0x252C90
        M3D_APP->EnqueueMessage(66559, m_money.value().get(), oldMoneyValue, 0, 0, {}, {});
    }

    // ------------------------------------------------------------------------
    //  Quest items
    // ------------------------------------------------------------------------

    retruxx::vector<CStr> const& Player::GetQuestItemPrototypeNames() const
    {
        // RVA 0x24EF80
        return m_questItemPrototypeNames;
    }

    bool Player::IsQuestItemPresent(CStr const& itemPrototypeName) const
    {
        // RVA 0x250810
        for (auto const& name : m_questItemPrototypeNames)
        {
            if (name == itemPrototypeName)
            {
                return true;
            }
        }
        return false;
    }

    int Player::AddQuestItem(CStr const& itemPrototypeName)
    {
        // RVA 0x252D30
        int const prototypeId = thePrototypeManager->GetPrototypeId(itemPrototypeName);
        PrototypeInfo const* prototypeInfo = thePrototypeManager->GetPrototypeInfo(prototypeId);
        if (!prototypeInfo)
        {
            M3D_LOG_ERR("Player::AddQuestItem error - cannot find prototype for prototype " + itemPrototypeName);
            return 0;
        }

        int const questItemResourceId = theResourceManager->GetResourceId("QUEST_ITEM");
        if (!theResourceManager->bResourceIsKindOf(prototypeInfo->m_resourceId, questItemResourceId))
        {
            M3D_LOG_ERR(
                "Player::AddQuestItem error - invalid resource for quest item with prototype " +
                prototypeInfo->m_prototypeName + ": " +
                theResourceManager->GetResourceName(prototypeInfo->m_resourceId));
            return 0;
        }

        m_questItemPrototypeNames.push_back(itemPrototypeName);
        M3D_APP->EnqueueMessage(66562, 0, 0, 0, 0, {}, {});
        return 1;
    }

    int Player::RemoveQuestItem(CStr const& itemPrototypeName)
    {
        // RVA 0x253030
        for (auto it = m_questItemPrototypeNames.begin(); it != m_questItemPrototypeNames.end(); ++it)
        {
            if (*it == itemPrototypeName)
            {
                m_questItemPrototypeNames.erase(it);
                M3D_APP->EnqueueMessage(66562, 0, 0, 0, 0, {}, {});
                return 1;
            }
        }

        M3D_LOG_ERR(
            "Player::RemoveQuestItem error - quest item with prototype " + itemPrototypeName + " does not exist");
        return 0;
    }

    // ------------------------------------------------------------------------
    //  Misc accessors
    // ------------------------------------------------------------------------

    CStr const& Player::GetLastSaveDir() const
    {
        // RVA 0x24EF30
        return m_lastSaveDir;
    }

    void Player::SetLastSaveDir(CStr const& saveDir)
    {
        // RVA 0x24FA90
        m_lastSaveDir = saveDir;
    }

    InfoCone const& Player::GetInfoCone() const
    {
        return *m_infoCone;
    }

    int Player::GetInfoObjId() const
    {
        return m_infoObjId;
    }

    int Player::GetRadioManagerId() const
    {
        if (m_radioManager)
        {
            return m_radioManager->GetId();
        }
        return -1;
    }

    CStr const& Player::GetModelName() const
    {
        // RVA 0x24EFB0
        return m_modelName;
    }

    unsigned Player::GetSkinNumber() const
    {
        return m_skinNumber;
    }

    unsigned Player::GetCfgNumber() const
    {
        return m_cfgNumber;
    }

    bool Player::HuntQuestIsTaken() const
    {
        // RVA 0x24EFE0
        return m_huntQuestIsTaken;
    }

    void Player::RenderDebugInfo() const
    {
        // RVA 0x24F000 - the shipped build draws nothing here.
    }

    // ------------------------------------------------------------------------
    //  Properties
    // ------------------------------------------------------------------------

    void Player::Registration()
    {
        // RVA 0x252BA0
        m_propertiesMap["Money"] = 11;
        m_propertiesMap["ModelName"] = 44;
        m_propertiesMap["skin"] = 45;
        m_propertiesMap["cfg"] = 46;
    }

    void Player::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x251FA0
        m_propertiesMap[CStr(Name)] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
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

    eGObjPropertySaveStatus Player::GetPropertySaveStatus(int id) const
    {
        // RVA 0x251D50
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it == m_propertiesSaveStatesMap.end())
        {
            return Obj::GetPropertySaveStatus(id);
        }
        return it->second;
    }

    void Player::GetPropertiesNames(retruxx::set<CStr>& Props) const
    {
        // RVA 0x251E00
        for (auto const& entry : m_propertiesMap)
        {
            Props.insert(entry.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    void Player::GetPropertiesIDs(retruxx::set<int>& Props) const
    {
        // RVA 0x251E80
        for (auto const& entry : m_propertiesMap)
        {
            Props.insert(entry.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    CStr Player::GetPropertyName(int id) const
    {
        // RVA 0x251F00
        for (auto const& entry : m_propertiesMap)
        {
            if (entry.second == id)
            {
                return entry.first;
            }
        }
        return Obj::GetPropertyName(id);
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

    bool Player::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x252000
        switch (propertyId)
        {
        case 11:
            retVal = m_money.value().get();
            return true;

        case 44:
            retVal = m_modelName;
            return true;

        case 45:
            retVal = CStr(static_cast<int>(m_skinNumber));
            return true;

        case 46:
            retVal = CStr(static_cast<int>(m_cfgNumber));
            return true;

        default:
            return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    bool Player::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x252100 - defaults come from the prototype the player was spawned from.
        PlayerPrototypeInfo const* prototypeInfo = nullptr;
        switch (propertyId)
        {
        case 11:
            retVal = 0;
            return true;

        case 44:
            prototypeInfo = GetPrototypeInfo();
            retVal = prototypeInfo ? prototypeInfo->GetModelName() : CStr{};
            return true;

        case 45:
            prototypeInfo = GetPrototypeInfo();
            retVal = prototypeInfo ? CStr(static_cast<int>(prototypeInfo->GetSkinNumber())) : CStr("0");
            return true;

        case 46:
            prototypeInfo = GetPrototypeInfo();
            retVal = prototypeInfo ? CStr(static_cast<int>(prototypeInfo->GetCfgNumber())) : CStr("0");
            return true;

        default:
            return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    // ------------------------------------------------------------------------
    //  Hierarchy
    // ------------------------------------------------------------------------

    bool Player::CanChildBeAdded(m3d::Class* pClass) const
    {
        return ai::Obj::CanChildBeAdded(pClass) || pClass->m3d::Class::IsKindOf(&ai::Vehicle::m_classVehicle);
    }

    void Player::AddChild(Obj* obj)
    {
        // RVA 0x252290
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
                        theObjects->SetObjName(
                            m_vehicleObjId,
                            "PlayerVehicle_" + CStr(GetId()) + "_" + CStr(m_curNumForVehicleWithoutName));
                        ++m_curNumForVehicleWithoutName;
                    }

                    M3D_APP->ImmediateMessage(66544, 0, 0, 0, 0, {}, {});
                    CauseEvent(GE_PLAYER_VEHICLE_CHANGED, 0.0, {}, {});
                    if (m_radioManager)
                    {
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, {9}, {}, 1);
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, {44}, {}, 1);
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
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, {9}, {}, 1);
                        theProcessManager->PostMessageA(2, m_vehicleObjId, m_radioManager->GetId(), 0.0, {44}, {}, 1);
                    }
                    theProcessManager->PostMessageA(2, GetId(), m_radioManager->GetId(), 0.0, {65}, {}, 1);
                }
            }
            obj->LinkToParent(GetId(), HIERARCHY_CHILD);
        }
    }

    bool Player::RemoveChild(Obj* pChild)
    {
        // RVA 0x2527E0
        Obj::RemoveChild(pChild);
        if (!pChild)
        {
            return false;
        }

        if (IS_KIND_OF(pChild, Vehicle) && pChild->GetId() == m_vehicleObjId)
        {
            auto* vehicle = RT_DYNCAST(pChild, Vehicle);
            vehicle->SetHorn(false);
            vehicle->m_bIsControlledByPlayer = false;
            vehicle->UnsubscribeRadioManagerFromAllNearbyObjIds();
            m_vehicleObjId = -1;

            M3D_APP->ImmediateMessage(66544, 0, 0, 0, 0, {}, {});
            CauseEvent(GE_PLAYER_VEHICLE_CHANGED, 0.0, {}, {});
            pChild->SetParentInvalid();
            return true;
        }

        if (IS_KIND_OF(pChild, RadioManager) && pChild == m_radioManager)
        {
            m_radioManager = nullptr;
            pChild->SetParentInvalid();
            return true;
        }

        return false;
    }

    void Player::Remove()
    {
        Obj::Remove();
        auto vehicle = GetVehicle();
        if (vehicle)
        {
            vehicle->Remove();
        }
    }

    void Player::SetPassedToAnotherMapStatus()
    {
        // RVA 0x251BD0
        Obj::SetPassedToAnotherMapStatus();
        if (auto* vehicle = GetVehicle())
        {
            vehicle->SetPassedToAnotherMapStatus();
        }
    }

    void Player::_InternalPostLoad()
    {
        // RVA 0x2514A0
        auto protoId = thePrototypeManager->GetPrototypeId("radioManager");
        auto radioManagerId = theObjects->CreateNewObject(protoId, "", -1, -1);
        auto radioManager = theObjects->GetEntityByObjId(radioManagerId);
        AddChild(radioManager);
    }

    // ------------------------------------------------------------------------
    //  Events
    // ------------------------------------------------------------------------

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

    void Player::_OnObjectDie(Event const& evn)
    {
        // RVA 0x251550
        RemoveChild(theObjects->GetEntityByObjId(evn.m_senderObjId));
    }

    void Player::_OnNoticeEnemy(Event const&)
    {
        // RVA 0x24EF40
        if (m_playerFightState < FIGHT_ALARM)
        {
            m_playerFightState = FIGHT_ALARM;
        }
    }

    void Player::_OnUnderAttack(Event const&)
    {
        // RVA 0x24EF60
        if (m_playerFightState < FIGHT_BATTLE)
        {
            m_playerFightState = FIGHT_BATTLE;
        }
    }

    void Player::_OnDynamicQuestTaken(Event const& evn)
    {
        // RVA 0x2515A0
        Obj* sender = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (sender && sender->IsKindOf(&ai::DynamicQuestHunt::m_classDynamicQuestHunt))
        {
            m_huntQuestIsTaken = true;
        }
    }

    void Player::_OnDynamicQuestComplete(Event const& evn)
    {
        // RVA 0x251600
        Obj* sender = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (sender && sender->IsKindOf(&ai::DynamicQuestHunt::m_classDynamicQuestHunt))
        {
            m_huntQuestIsTaken = false;
        }
    }

    void Player::_OnDynamicQuestForgotten(Event const& evn)
    {
        // RVA 0x251660
        Obj* sender = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (sender && sender->IsKindOf(&ai::DynamicQuestHunt::m_classDynamicQuestHunt))
        {
            m_huntQuestIsTaken = false;
        }
    }

    void Player::_OnDynamicQuestFailed(Event const& evn)
    {
        // RVA 0x2516C0
        Obj* sender = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (sender && sender->IsKindOf(&ai::DynamicQuestHunt::m_classDynamicQuestHunt))
        {
            m_huntQuestIsTaken = false;
        }
    }

    // ------------------------------------------------------------------------
    //  Update
    // ------------------------------------------------------------------------

    void Player::Update(float elapsedTime, unsigned)
    {
        // RVA 0x252990
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
                M3D_APP->EnqueueMessage(
                    66555,  // Message ID for fight state change
                    newFightState,
                    0,
                    0,
                    0,
                    {},
                    {});
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
                    M3D_APP->EnqueueMessage(
                        66555,  // Message ID for fight state change
                        currentFightState,
                        0,
                        0,
                        0,
                        {},
                        {});
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

    // ------------------------------------------------------------------------
    //  Serialization
    // ------------------------------------------------------------------------

    void Player::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x2532C0
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

    void Player::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x250850
        Obj::SaveToXML(xmlFile, xmlNode);
        if (m_questItemPrototypeNames.empty())
        {
            return;
        }

        ref_ptr questItemsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "QuestItems");
        xmlNode->AddChild(questItemsNode);
        CStr const strQuestItemPrototypeNames = StringVectorToStr(m_questItemPrototypeNames);
        questItemsNode->SetAttribute("PrototypeNames", strQuestItemPrototypeNames.c_str());
    }

    void Player::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x24FAC0
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_huntQuestIsTaken, xmlNode, "HuntQuestIsTaken");
        m3d::SafeIntAttrib(m_curNumForVehicleWithoutName, xmlNode, "NumVehicleWithoutName");
    }

    void Player::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x24FB20
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("HuntQuestIsTaken", CStr(static_cast<int>(m_huntQuestIsTaken)).c_str());
        xmlNode->SetAttribute("NumVehicleWithoutName", CStr(m_curNumForVehicleWithoutName).c_str());
    }
}  // namespace ai

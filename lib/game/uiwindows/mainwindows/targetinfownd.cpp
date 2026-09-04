#include "targetinfownd.h"

#include "maingameinterfacewnd.h"
#include "core/log.h"
#include "core/timer.h"
#include "game/m3dgame.h"
#include "game/uimanager/uidefs.h"
#include "game/uimisc/guihelper.h"
#include "server/server.h"
#include "server/objects/base/physicobj.h"
#include "server/objects/player.h"
#include "server/objects/staticautogun.h"
#include "server/objects/vehicle.h"
#include "server/objects/monsters/boss04drone.h"
#include "ui/image.h"
#include "ui/progressbarwnd.h"

RT_CLASS_EXPORTS_BEGIN(TargetInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetInfoWnd);

TargetInfoWnd::AuxInfo::AuxInfo()
{
    // Set default UI element names
    m_pbHealthName = "pbHealth";
    m_lblHealthName = "lblHealth";
    m_pbDurabilityName = "pbDurability";
    m_lblDurabilityName = "lblDurability";
    m_wndDistanceName = "wndDistance";
    m_wndNameName = "wndName";

    // Set resistance icon names
    m_wndResistanceName[0] = "wndPiercingIco";
    m_wndResistanceName[1] = "wndBlastIco";
    m_wndResistanceName[2] = "wndEnergyIco";

    // Set default colors (assuming ARGB format)
    m_colorEnemy = 0xFFFF0000;   // Red
    m_colorFriend = 0xFF88FF00;  // Green-ish
}

m3d::Object* TargetInfoWnd::CreateObject()
{
    return new TargetInfoWnd;
}

m3d::Class* TargetInfoWnd::GetClass() const
{
    return RT_CLASS_LOCAL(TargetInfoWnd);
}

bool TargetInfoWnd::IsObjClassValidForInfo(m3d::Class const* cl)
{
    CStr className = cl->m_className;
    return className == "Vehicle" || className == "StaticAutoGun" || className == "Boss04Drone";
}

m3d::Object* TargetInfoWnd::Clone()
{
    // RVA 0x135140
    return new TargetInfoWnd(*this);
}

int TargetInfoWnd::GetTargetObjId() const
{
    return m_targetObjId;
}

bool TargetInfoWnd::NeedShow() const
{
    // RVA 0x136F90
    return m_targetObjId != -1;
}

TargetInfoWnd::~TargetInfoWnd()
{
    // RVA 0x1355E0 - m_aif's CStr members and the Wnd base clean up
    // automatically.
}

m3d::Class* TargetInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void TargetInfoWnd::UpdateName()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndName->SetText({});

        if (auto* targetObj = GetTargetObj())
        {
            auto const objFullName = ai::pServer->GetFullNameByObjID(m_targetObjId);
            auto const clanName = help::GetClanNameByBelong(targetObj->GetBelong());
            auto clanAbr = help::GetClanAbbreviationByName(clanName);
            if (!clanAbr.empty())
            {
                clanAbr = " (" + clanAbr + ")";
            }

            m_wndName->SetText(objFullName + clanAbr);
        }
    }
}

void TargetInfoWnd::UpdateControlsOnNewFrame()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        UpdateHealth();
        UpdateDurability();
        UpdateDistance();
        UpdateToleranceColor();
    }
}

int TargetInfoWnd::OnBeforeRemoveFromWndStation()
{
    Wnd::OnBeforeRemoveFromWndStation();
    if (!m_fadeStartTime)
    {
        m_fadeStartTime = 0;
        SetAlpha(0xFFu);
        m_fadeStartTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    }
    return 0;
}

int TargetInfoWnd::GameDataSetup()
{
    using namespace m3d::ui;

    if ((m_gameDataFlags & 2) != 0)
    {
        int res = 1;

        auto wnd = GetChildByName(m_aif.m_pbHealthName);
        if (wnd && IS_KIND_OF(wnd, ProgressBarWnd))
        {
            m_pbHealth = RT_DYNCAST(wnd, ProgressBarWnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_pbHealthName + " is not found or incorrect type");
            res = 0;
        }

        wnd = GetChildByName(m_aif.m_lblHealthName);
        if (wnd && IS_KIND_OF(wnd, Wnd))
        {
            m_lblHealth = RT_DYNCAST(wnd, Wnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_lblHealthName + " is not found or incorrect type");
            res = 0;
        }

        wnd = GetChildByName(m_aif.m_pbDurabilityName);
        if (wnd && IS_KIND_OF(wnd, ProgressBarWnd))
        {
            m_pbDurability = RT_DYNCAST(wnd, ProgressBarWnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_pbDurabilityName + " is not found or incorrect type");
            res = 0;
        }

        wnd = GetChildByName(m_aif.m_lblDurabilityName);
        if (wnd && IS_KIND_OF(wnd, Wnd))
        {
            m_lblDurability = RT_DYNCAST(wnd, Wnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_lblDurabilityName + " is not found or incorrect type");
            res = 0;
        }

        wnd = GetChildByName(m_aif.m_wndDistanceName);
        if (wnd && IS_KIND_OF(wnd, Wnd))
        {
            m_wndDistance = RT_DYNCAST(wnd, Wnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndDistanceName + " is not found or incorrect type");
            res = 0;
        }

        wnd = GetChildByName(m_aif.m_wndNameName);
        if (wnd && IS_KIND_OF(wnd, Wnd))
        {
            m_wndName = RT_DYNCAST(wnd, Wnd);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndNameName + " is not found or incorrect type");
            res = 0;
        }

        for (int i = 0; i < 3; ++i)
        {
            wnd = GetChildByName(m_aif.m_wndResistanceName[i]);
            if (wnd && IS_KIND_OF(wnd, ImageWnd))
            {
                m_wndResistance[i] = RT_DYNCAST(wnd, ImageWnd);
            }
            else
            {
                M3D_LOG_INFO(
                    "Get control error: control " + m_aif.m_wndResistanceName[i] + " is not found or incorrect type");
                res = 0;
            }
        }

        if (res)
        {
            CheckAndShow();
            if (!m_fadeStartTime)
            {
                UpdateName();
                UpdateResistance();
            }
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_ERR("TargetInfoWnd: error - fail to init because of a bad resource");
    return 0;
}

int TargetInfoWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 90)
    {
        OnNewFrameForce();
    }
    return 1;
}

TargetInfoWnd::TargetInfoWnd()
{
    m_pbHealth = 0;
    m_lblHealth = 0;
    m_pbDurability = 0;
    m_lblDurability = 0;
    m_wndDistance = 0;
    m_wndName = 0;
    m_fadeStartTime = 0;
    m_targetObjId = -1;
    m_wndResistance[0] = 0;
    m_wndResistance[1] = 0;
    m_wndResistance[2] = 0;
}

TargetInfoWnd::TargetInfoWnd(TargetInfoWnd const&) : TargetInfoWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x1355C0) default-constructs the base
    // and re-inits the AuxInfo, but leaves m_pbHealth/m_lblHealth/
    // m_pbDurability/m_lblDurability/m_wndDistance/m_wndName/
    // m_wndResistance/m_targetObjId/m_fadeStartTime uninitialized; delegating
    // to the default ctor here avoids reading uninitialized pointers while
    // still copying nothing from the source.
}

void TargetInfoWnd::StopFade()
{
    if (m_fadeStartTime)
    {
        M3D_APP->EnqueueMessage(42, (int)this, 0, 0, 0, {}, {});
    }
}

void TargetInfoWnd::SetTargetObj(int objId)
{
    auto oldTargetObjId = m_targetObjId;
    m_targetObjId = objId;
    if (objId != oldTargetObjId)
    {
        CheckAndShow();
        if (!m_fadeStartTime)
        {
            UpdateName();
            UpdateResistance();
        }
    }
}

void TargetInfoWnd::UpdateToleranceColor()
{
    // RVA 0x136B30
    if ((m_gameDataFlags & 1) == 0)
        return;

    if (m_targetObjId == -1)
        return;

    ai::Obj const* targetObj = GetTargetObj();
    if (!targetObj)
        return;

    // NOTE: the shipped code falls back to the literal color value 5 when
    // there is no local player to evaluate tolerance against (or, in the
    // unreachable case, a tolerance value outside RS_ENEMY..RS_OWN);
    // reproduced faithfully rather than guessing at an intended constant.
    unsigned int textColor = 5;

    if (ai::thePlayer)
    {
        ai::eTolerance const tolerance =
            ai::pServer->CheckTolerance(ai::thePlayer->GetBelong(), targetObj->GetBelong());

        if (tolerance == ai::RS_ENEMY)
        {
            textColor = m_aif.m_colorEnemy;
        }
        else if (tolerance > ai::RS_ENEMY && tolerance <= ai::RS_OWN)
        {
            textColor = m_aif.m_colorFriend;
        }
    }

    m_wndName->SetTextColor(textColor);
}

void TargetInfoWnd::UpdateDurability()
{
    using namespace ai;
    if ((m_gameDataFlags & 1) != 0)
    {
        m_pbDurability->ShowWindow(0);
        m_lblDurability->SetText({});
        if (m_targetObjId != -1)
        {
            auto* targetObj = GetTargetObj();
            if (targetObj)
            {
                if (ai::thePlayer && ai::thePlayer->GetVehicle())
                {
                    if (auto* vehicle = RT_DYNCAST(targetObj, Vehicle const))
                    {
                        float const dur = vehicle->GetFullDurability();
                        float const maxDur = vehicle->GetMaxFullDurability();
                        m_pbDurability->ShowWindow(true);
                        m_pbDurability->SetMaxValue(maxDur);
                        m_pbDurability->SetCurValue(dur);

                        int const roundDur = help::RoundHealth(dur);
                        m_lblDurability->SetText(CStr(roundDur));
                    }
                }
            }
        }
    }
}

void TargetInfoWnd::UpdateOnChangeTargetObj(int, int)
{
    // RVA 0x136150
    CheckAndShow();
    if (!m_fadeStartTime)
    {
        UpdateName();
        UpdateResistance();
    }
}

int TargetInfoWnd::OnAfterRemoveFromWndStation()
{
    auto const res = Wnd::OnAfterRemoveFromWndStation();
    m_fadeStartTime = 0;
    SetAlpha(0xFFu);

    auto oldTarget = m_targetObjId;
    m_targetObjId = -1;
    if (oldTarget != -1)
    {
        CheckAndShow();
        if (!m_fadeStartTime)
        {
            UpdateName();
            UpdateResistance();
        }
    }
    return res;
}

int TargetInfoWnd::OnBeforeAddToWndStation()
{
    if (m_targetObjId == -1)
    {
        return 0;
    }
    if (m_fadeStartTime)
    {
        m_fadeStartTime = 0;
        SetAlpha(0xFFu);
    }
    return Wnd::OnBeforeAddToWndStation();
}

void TargetInfoWnd::StartFade()
{
    // RVA 0x136C70
    m_fadeStartTime = 0;
    SetAlpha(0xFFu);
    m_fadeStartTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
}

ai::Obj const* TargetInfoWnd::GetTargetObj() const
{
    auto* obj = ai::theObjects->GetEntityByObjId(m_targetObjId);
    if (!obj || !obj->IsAlive())
    {
        return nullptr;
    }

    return obj;
}

void TargetInfoWnd::OnNewFrameForce()
{
    if ((m_gameDataFlags & 1) != 0 && NeedUpdate())
    {
        UpdateTargetObj();
        if (IsChildOf(M3D_APP))
        {
            if (m_fadeStartTime)
            {
                ProcessFade();
                return;
            }
            UpdateControlsOnNewFrame();
        }
        if (!m_fadeStartTime)
        {
            return;
        }
        ProcessFade();
    }
}

unsigned char TargetInfoWnd::CalcAlpha() const
{
    // RVA 0x136DA0
    if (!m_fadeStartTime)
    {
        return 0xFFu;
    }
    unsigned int const curTimeUnscaled = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    float const alpha = 255.0f - (curTimeUnscaled - m_fadeStartTime) * 0.001f * 254.0f;
    if (alpha < 1.0f)
    {
        return 1u;
    }
    if (alpha > 255.0f)
    {
        return 255u;
    }
    return static_cast<unsigned char>(alpha);
}

bool TargetInfoWnd::NeedUpdate() const
{
    auto parentWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_MAIN_GAME_INTERFACE);
    if (parentWnd)
    {
        if (IS_KIND_OF(parentWnd, MainGameInterfaceWnd))
        {
            return parentWnd->IsChildOf(M3D_APP);
        }
    }
    return false;
}

void TargetInfoWnd::UpdateResistance()
{
    // TODO: generated code TargetInfoWnd::UpdateResistance
    using namespace ai;

    // Only update if game data flag is set
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // First hide all resistance windows
    for (int i = 0; i < 3; ++i)
    {
        m_wndResistance[i]->ShowWindow(false);
    }

    // Check if we have a valid target
    if (m_targetObjId == -1)
    {
        return;
    }

    // Get the target vehicle object
    auto* target = GetTargetObj();
    if (!target)
    {
        return;
    }

    // Only show resistance for vehicles when player exists and has a vehicle
    if (!ai::thePlayer || !ai::thePlayer->GetVehicle())
    {
        return;
    }

    if (!IS_KIND_OF(target, Vehicle))
    {
        return;
    }

    auto* targetVehicle = RT_DYNCAST(target, Vehicle const);

    // Update visibility for each damage type based on durability coefficient
    for (ai::DamageType damageType = DAMAGE_PIERCING; damageType < DAMAGE_WATER;
         damageType = static_cast<ai::DamageType>(damageType + 1))
    {
        int resistanceIndex = damageType - DAMAGE_PIERCING;
        double durabilityCoeff = targetVehicle->GetFullDurabilityCoeffForDamageType(damageType);

        // Show resistance window only if durability coefficient is significant
        bool shouldShow = (durabilityCoeff >= 10.0);
        m_wndResistance[resistanceIndex]->ShowWindow(shouldShow);
    }
}

void TargetInfoWnd::CheckAndShow()
{
    auto parentWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_MAIN_GAME_INTERFACE);
    if (parentWnd)
    {
        if (IS_KIND_OF(parentWnd, MainGameInterfaceWnd))
        {
            auto* parentWndCasted = RT_DYNCAST(parentWnd.get(), MainGameInterfaceWnd);
            parentWndCasted->CheckAndShowTargetInfoWnd(false);
        }
    }
}

bool TargetInfoWnd::IsFading() const
{
    // RVA 0x136C60
    return m_fadeStartTime != 0;
}

void TargetInfoWnd::ProcessFade()
{
    if (m_fadeStartTime)
    {
        auto const curTimeUnscaled = m3d::g_Kernel->GetTimer().GetCurTimeUnscaled();
        auto const fadeDelta = m_fadeStartTime + 1000;
        if (curTimeUnscaled < fadeDelta)
        {
            auto const alpha = CalcAlpha();
            SetAlpha(alpha);
        }
        else
        {
            StopFade();
        }
    }
}

void TargetInfoWnd::RestoreFromFade()
{
    // RVA 0x136CA0
    m_fadeStartTime = 0;
    SetAlpha(0xFFu);
}

void TargetInfoWnd::SetAlpha(unsigned char alpha)
{
    // RVA 0x136E40
    auto color = alpha << 24;
    auto curColor = GetGfxServer()->GetColor(m_curClr);
    SetColor(color | curColor & 0xFFFFFF);
    help::SetWndTextAlpha(this, alpha);

    // Use stack to traverse all child windows recursively
    std::vector<m3d::ui::Wnd*> stack;
    stack.push_back(this);

    while (!stack.empty())
    {
        m3d::ui::Wnd* current = stack.back();
        stack.pop_back();

        // Process all siblings of current window
        m3d::ui::Wnd* child = static_cast<m3d::ui::Wnd*>(current->GetFirstChild());
        while (child)
        {
            // Only process if it's a Wnd (not just any Object)
            if (child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
            {
                // Set alpha for this child window
                unsigned int childColor = GetGfxServer()->GetColor(child->GetColor());
                unsigned int newChildColor = (alpha << 24) | (childColor & 0xFFFFFF);
                child->SetColor(newChildColor);
                help::SetWndTextAlpha(child, alpha);

                // If child has children, add to stack for processing
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }

            // Move to next sibling
            child = static_cast<m3d::ui::Wnd*>(child->GetNextSibling());
        }
    }
}

void TargetInfoWnd::UpdateHealth()
{
    using namespace ai;

    if ((m_gameDataFlags & 1) != 0)
    {
        m_pbHealth->ShowWindow(0);
        m_lblHealth->SetText({});
        if (m_targetObjId != -1)
        {
            auto* targetObj = GetTargetObj();
            if (targetObj)
            {
                if (ai::thePlayer && ai::thePlayer->GetVehicle())
                {
                    float health = 0.0;
                    float maxHealth = 0.0;
                    if (auto* vehicle = RT_DYNCAST(targetObj, Vehicle const))
                    {
                        health = vehicle->Health().value().get();
                        maxHealth = vehicle->Health().maxValue().get();
                    }
                    else if (auto* autoGun = RT_DYNCAST(targetObj, StaticAutoGun const))
                    {
                        health = autoGun->Health().value().get();
                        maxHealth = autoGun->Health().maxValue().get();
                    }
                    else if (auto* drone = RT_DYNCAST(targetObj, Boss04Drone const))
                    {
                        health = drone->GetHealth();
                        maxHealth = drone->GetMaxHealth();
                    }
                    else
                    {
                        return;
                    }

                    m_pbHealth->ShowWindow(true);
                    m_pbHealth->SetMaxValue(maxHealth);
                    m_pbHealth->SetCurValue(health);

                    int const roundHealth = help::RoundHealth(health);
                    m_lblHealth->SetText(CStr(roundHealth));
                }
            }
        }
    }
}

void TargetInfoWnd::UpdateDistance()
{
    // RVA 0x136420
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndDistance->SetText(CStr());
    if (m_targetObjId == -1)
    {
        return;
    }
    auto const* targetObj = GetTargetObj();
    if (!targetObj || !targetObj->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
    {
        return;
    }
    if (!ai::thePlayer)
    {
        return;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }

    auto const* targetPhysic = static_cast<ai::PhysicObj const*>(targetObj);
    CVector const diff = targetPhysic->GetPosition() - vehicle->GetPosition();
    float const dist = diff.length();

    CStr caption;
    if (dist < 1000.0f)
    {
        caption = CStr(static_cast<int>(dist)) + " " + M3D_APP->GetStringByStringId0("m");
    }
    else
    {
        caption = help::ftoa(dist * 0.001f, 1) + " " + M3D_APP->GetStringByStringId0("Km");
    }
    m_wndDistance->SetText(caption);
}

void TargetInfoWnd::UpdateTargetObj()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        if (ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr)
        {
            if (!GetTargetObj())
            {
                SetTargetObj(-1);
            }

            auto const vehicleInfoObjId = vehicle->GetInfoObjId();
            if (auto const* vehicleInfoObj = ai::theObjects->GetEntityByObjId(vehicleInfoObjId);
                vehicleInfoObj && IsObjClassValidForInfo(vehicleInfoObj->GetClass()) && vehicleInfoObj->IsAlive())
            {
                SetTargetObj(vehicleInfoObjId);
            }
            else if (m_targetObjId != -1)
            {
                SetTargetObj(-1);
            }
        }
        else
        {
            auto oldTargetObjId = m_targetObjId;
            m_targetObjId = -1;
            if (oldTargetObjId != -1)
            {
                CheckAndShow();
                if (!m_fadeStartTime)
                {
                    UpdateName();
                    UpdateResistance();
                }
            }
        }
    }
}

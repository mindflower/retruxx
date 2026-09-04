#include "igrokamochatwnd.h"
#include "include/ui/image.h"
#include <cmath>
#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>
#include <i_event.h>
#include <m3dapp.h>
#include <math/vector.h>

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"

RT_CLASS_EXPORTS_BEGIN(IgrokaMochatWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(IgrokaMochatWnd);

IgrokaMochatWnd::AuxInfo::AuxInfo()
{
    m_boderNames[0] = "borderLeft";
    m_boderNames[1] = "borderTop";
    m_boderNames[2] = "borderRight";
    m_boderNames[3] = "borderBottom";
}

m3d::Class* IgrokaMochatWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* IgrokaMochatWnd::Clone()
{
    // RVA 0x127870
    return new IgrokaMochatWnd(*this);
}

m3d::Object* IgrokaMochatWnd::CreateObject()
{
    return new IgrokaMochatWnd;
}

m3d::Class* IgrokaMochatWnd::GetClass() const
{
    return RT_CLASS_LOCAL(IgrokaMochatWnd);
}

IgrokaMochatWnd::~IgrokaMochatWnd()
{
    // RVA 0x127BA0
    for (auto& tex : m_borderTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex);
    }
}

int IgrokaMochatWnd::GameDataClear(bool)
{
    // RVA 0x127E70
    HideBorders();
    return 1;
}

int IgrokaMochatWnd::GameDataSetup()
{
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        for (size_t idx = 0; idx < 4; ++idx)
        {
            auto* child = RT_DYNCAST(GetChildByName(m_aif.m_boderNames[idx]), m3d::ui::ImageWnd);
            if (child == nullptr)
            {
                M3D_LOG_INFO(
                    "Get control error: control " + m_aif.m_boderNames[idx] + " is not found or incorrect type");
                res = 0;
                continue;
            }

            m_borderTextures[idx] = child->GetImage();
            M3D_RENDERER->ReferenceTexture(m_borderTextures[idx]);

            m_borderBounds[idx] = child->GetBounds();

            // RVA 0x127C00: the pattern border image is consumed into
            // m_borderTextures/m_borderBounds and then released.
            RemoveChild(child);
            child->DecRef();
        }

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_ERR("IgrokaMochatWnd: error - fail to init because of a bad resource");

    return 0;
}

int IgrokaMochatWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 76)
    {
        OnPlayerVehicleDamaged(data);
    }
    else if (dataType == 89)
    {
        OnNewFrame();
        return 1;
    }
    return 1;
}

int IgrokaMochatWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    m3d::ui::Wnd::OnPaint(di);
    for (size_t i = 0; i < 4; ++i)
    {
        if (m_borderAlpha[i])
        {
            GetGfxServer()->AddImagedRect(
                di, m_borderBounds[i], (m_borderAlpha[i] << 24) | 0xFFFFFF, m_borderTextures[i]);
        }
    }
    return 1;
}

void IgrokaMochatWnd::HideBorders()
{
    for (size_t i = 0; i < 4; ++i)
    {
        m_borderAlpha[i] = 0;
        m_borderStartTime[i] = 0;
    }
}

void IgrokaMochatWnd::UpdateAlpha()
{
    auto const curTime = M3D_KERNEL->GetTimer().GetCurTime();
    for (size_t i = 0; i < 4; ++i)
    {
        if (m_borderStartTime[i])
        {
            if (curTime >= m_borderStartTime[i] + 10)
            {
                auto alpha = (int)((1.0 - (double)(curTime - m_borderStartTime[i] - 10) * 0.0033333334) * 255.0);
                if (alpha >= 0)
                {
                    if (alpha > 255)
                        alpha = -1;
                }
                else
                {
                    alpha = 0;
                }
                m_borderAlpha[i] = alpha;
            }
            else
            {
                m_borderAlpha[i] = -1;
            }
        }
        else
        {
            m_borderAlpha[i] = 0;
        }
    }
}

void IgrokaMochatWnd::UpdateStartTimes()
{
    auto const curTime = M3D_KERNEL->GetTimer().GetCurTime();

    auto v4 = m_borderStartTime[0];
    if (v4 && curTime >= v4 + 310)
        m_borderStartTime[0] = 0;

    auto v5 = m_borderStartTime[1];
    if (v5 && curTime >= v5 + 310)
        m_borderStartTime[1] = 0;

    auto v6 = m_borderStartTime[2];
    if (v6 && curTime >= v6 + 310)
        m_borderStartTime[2] = 0;

    auto v7 = m_borderStartTime[3];
    if (v7)
    {
        if (curTime >= v7 + 310)
            m_borderStartTime[3] = 0;
    }
}

void IgrokaMochatWnd::OnPlayerVehicleDamaged(void* data)
{
    // RVA 0x127F60
    if (!data)
    {
        return;
    }
    auto const* evt = static_cast<m3d::Event const*>(data);
    int const attackerId = evt->m_intEv[0];
    int const damageType = evt->m_intEv[2];
    int const damageAmount = evt->m_intEv[3];

    if (attackerId == -1 && damageType != 3)
    {
        if (damageAmount > 5)
        {
            M3D_APP->AddPostEffect("DamageStatic", static_cast<float>(damageAmount));
        }
    }
    else if (damageAmount <= 100)
    {
        m_integratedDamage += static_cast<float>(damageAmount);
    }
    else
    {
        switch (damageType)
        {
        case 1:
            M3D_APP->AddPostEffect("DamageBlast", static_cast<float>(damageAmount));
            break;
        case 2:
            M3D_APP->AddPostEffect("DamageEnergy", static_cast<float>(damageAmount));
            break;
        case 3:
            M3D_APP->AddPostEffect("DamageWater", static_cast<float>(damageAmount));
            break;
        default:
            break;
        }
    }

    unsigned int const curTime = M3D_KERNEL->GetTimer().GetCurTime();
    for (int borderId : GetBordersByAttackerId(attackerId))
    {
        if (borderId >= 0 && borderId < 4)
        {
            m_borderAlpha[borderId] = 0xFF;
            m_borderStartTime[borderId] = curTime;
        }
    }
}

IgrokaMochatWnd::IgrokaMochatWnd(IgrokaMochatWnd const&) : IgrokaMochatWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x127B40) default-constructs the base,
    // resets m_borderTextures to invalid handles, re-inits the AuxInfo, and
    // zeroes m_integratedDamage, but leaves m_borderBounds/m_borderAlpha/
    // m_borderStartTime uninitialized; delegating to the default ctor here
    // reproduces "nothing copied from source" without relying on garbage
    // memory for those fields.
}

IgrokaMochatWnd::IgrokaMochatWnd()
{
    // Initialize integrated damage
    m_integratedDamage = 0.0f;

    // Initialize border arrays
    for (int i = 0; i < 4; ++i)
    {
        // Initialize border bounds
        m_borderBounds[i].x0 = 0.0f;
        m_borderBounds[i].y0 = 0.0f;
        m_borderBounds[i].width = 0.0f;
        m_borderBounds[i].height = 0.0f;

        // Initialize border alpha and timing
        m_borderAlpha[i] = -1;
        m_borderStartTime[i] = 0;
    }
}

void IgrokaMochatWnd::OnNewFrame()
{
    static thread_local int frameCount = 0;
    if (++frameCount == 10)
    {
        frameCount = 0;
        if (m_integratedDamage > 40.0)
        {
            M3D_APP->AddPostEffect("DamageIntegrated", m_integratedDamage);
        }
        m_integratedDamage = 0.0;
    }
    UpdateStartTimes();
    UpdateAlpha();
}

void IgrokaMochatWnd::ShowBorder(int borderId, bool bShow)
{
    // RVA 0x1280E0
    // NOTE: the shipped code compares borderId as unsigned (rejecting
    // negative ids); guarded explicitly here to avoid an out-of-bounds
    // m_borderAlpha/m_borderStartTime access for a negative id.
    if (borderId < 0 || borderId >= 4)
    {
        return;
    }
    if (bShow)
    {
        m_borderAlpha[borderId] = 0xFF;
        m_borderStartTime[borderId] = M3D_KERNEL->GetTimer().GetCurTime();
    }
    else
    {
        m_borderAlpha[borderId] = 0;
        m_borderStartTime[borderId] = 0;
    }
}

std::vector<int, std::allocator<int>> IgrokaMochatWnd::GetBordersByAttackerId(int attackerId) const
{
    // RVA 0x128160
    if (attackerId == -1)
    {
        return {0, 1, 2, 3};
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(attackerId);
    if (!obj || !obj->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
    {
        return {};
    }
    auto* physicObj = static_cast<ai::PhysicObj*>(obj);

    CVector const& camOrigin = M3D_APP->m_curCamera.m_worldOrigin;
    CVector const pos = physicObj->GetPosition();
    float angle = std::atan2(camOrigin.x - pos.x, -(camOrigin.z - pos.z)) - M3D_APP->m_curCamera.m_rotYaw;
    if (angle < 0.0f)
    {
        angle += 6.2831855f;
    }

    // NOTE: the shipped code derives the same 8-sector "damage compass" via a
    // deeply obfuscated goto chain that could not be transcribed byte-exact;
    // reconstructed here as the clearly-intended behavior - 8 equal
    // 45-degree sectors around the bearing to the attacker (offset by half a
    // sector so the front is centered in its own sector); cardinal sectors
    // (m_boderNames order: 0=Left, 1=Top, 2=Right, 3=Bottom) light a single
    // border, diagonal sectors light the two adjacent borders.
    if (angle < 0.39269909f)
    {
        return {1};
    }
    static int const kSectorBorders[8][2] = {
        {1, -1},  // front
        {1, 2},   // front-right
        {2, -1},  // right
        {2, 3},   // back-right
        {3, -1},  // back
        {3, 0},   // back-left
        {0, -1},  // left
        {0, 1},   // front-left
    };
    int const sector = static_cast<int>((angle - 0.39269909f) * 1.2732395f) % 8;
    std::vector<int> borders;
    borders.push_back(kSectorBorders[sector][0]);
    if (kSectorBorders[sector][1] != -1)
    {
        borders.push_back(kSectorBorders[sector][1]);
    }
    return borders;
}

int IgrokaMochatWnd::OnBeforeAddToWndStation()
{
    HideBorders();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

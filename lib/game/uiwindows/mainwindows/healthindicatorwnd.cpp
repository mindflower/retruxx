#include "healthindicatorwnd.h"

#include "core/kernel.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"

#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"

RT_CLASS_EXPORTS_BEGIN(HealthIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HealthIndicatorWnd);

HealthIndicatorWnd::AuxInfo::AuxInfo()
{
    // RVA 0x126BB0
    m_barTexNames[TYPE_IN_MAIN_INTERFACE][0] = "HealthGreenMain";
    m_barTexNames[TYPE_IN_MAIN_INTERFACE][1] = "HealthYellowMain";
    m_barTexNames[TYPE_IN_MAIN_INTERFACE][2] = "HealthRedMain";
    m_barTexNames[TYPE_IN_CHARACTERISTIC_WND][0] = "HealthGreenChar";
    m_barTexNames[TYPE_IN_CHARACTERISTIC_WND][1] = "HealthYellowChar";
    m_barTexNames[TYPE_IN_CHARACTERISTIC_WND][2] = "HealthRedChar";
    m_barTexNames[TYPE_IN_FIGNYA_WND][0] = "HealthGreenFignya";
    m_barTexNames[TYPE_IN_FIGNYA_WND][1] = "HealthYellowFignya";
    m_barTexNames[TYPE_IN_FIGNYA_WND][2] = "HealthRedFignya";
    m_strHealthId = "Construction";
}

HealthIndicatorWnd::~HealthIndicatorWnd()
{
    // RVA 0x126F20 - m_strHealth and the ProgressBarWnd base clean up automatically.
    for (auto& tex : m_barTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex);
    }
}

void HealthIndicatorWnd::SetType(Type type)
{
    // RVA 0x127840
    m_type = type;
    if (type < TYPE_NUM_TYPES)
    {
        SetBarTextures(m_aif.m_barTexNames[type][0], m_aif.m_barTexNames[type][1], m_aif.m_barTexNames[type][2]);
    }
}

m3d::Object* HealthIndicatorWnd::Clone()
{
    // RVA 0x126AD0
    return new HealthIndicatorWnd(*this);
}

int HealthIndicatorWnd::CreateFromPattern(m3d::ui::Wnd* p, bool deleteSrc)
{
    // RVA 0x126FA0
    if (!p || !p->IsKindOf(&m3d::ui::ProgressBarWnd::m_classProgressBarWnd))
    {
        M3D_LOG_INFO("HealthIndicatorWnd::CreateFromPattern error - invalid pattern wnd");
        return 0;
    }
    auto* pattern = static_cast<m3d::ui::ProgressBarWnd*>(p);

    m3d::Object* parent = pattern->GetParent();
    if (!parent)
    {
        M3D_LOG_INFO("HealthIndicatorWnd::CreateFromPattern error - invali parent for pattern wnd");
        return 0;
    }

    Create({}, pattern->GetStyle(), pattern->GetBounds(), pattern->GetId());
    SetBackground(pattern->GetBackground());
    SetPane(pattern->GetPaneName());
    SetPaneFlags(pattern->GetPaneFlags());
    SetNumOfSteps(pattern->GetNumOfSteps());
    SetBarTexture(pattern->GetBarTexture());
    SetOrientation(pattern->GetOrientation());
    SetBarColor(pattern->GetBarColor());
    SetTextStyle(pattern->GetTextStyle());
    SetTextureStyle(pattern->GetTextureStyle());
    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(pattern);
        pattern->DecRef();
    }

    m_strHealth = M3D_APP->GetStringByStringId0(m_aif.m_strHealthId);
    UpdateTooltip();
    return 1;
}

m3d::Object* HealthIndicatorWnd::CreateObject()
{
    // RVA 0x126B20
    return new HealthIndicatorWnd;
}

m3d::Class* HealthIndicatorWnd::GetBaseClass()
{
    // RVA 0x126B20
    return RT_CLASS_LOCAL(ProgressBarWnd);
}

void HealthIndicatorWnd::SetVehicleId(int vehicleId)
{
    // RVA 0x1272B0
    m_vehicleId = vehicleId;
    if (ai::Vehicle* vehicle = GetVehicle())
    {
        SetMaxValue(vehicle->GetMaxHealth());
        SetCurValue(vehicle->GetHealth());
    }
    else
    {
        SetCurValue(0.0f);
        SetMaxValue(0.0f);
    }
    if (m_type == TYPE_IN_CHARACTERISTIC_WND)
    {
        UpdateTooltip();
    }
}

m3d::Class* HealthIndicatorWnd::GetClass() const
{
    // RVA 0x126B10
    return RT_CLASS_LOCAL(HealthIndicatorWnd);
}

int HealthIndicatorWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x127210
    if (dataType == 89)
    {
        UpdateOnNewFrame();
    }
    return 1;
}

void HealthIndicatorWnd::UpdateBarTexture()
{
    // RVA 0x1276D0
    if (m_type >= TYPE_NUM_TYPES)
    {
        return;
    }

    float const pct = (GetCurValue() / (GetMaxValue() - GetMinValue())) * 100.0f;
    int const pctInt = static_cast<int>(pct);

    m3d::rend::TexHandle newTex;
    if (pctInt > 0)
    {
        if (pctInt >= 66)
        {
            newTex = m_barTextures[0];
        }
        else if (pctInt >= 33)
        {
            newTex = m_barTextures[1];
        }
        else
        {
            newTex = m_barTextures[2];
        }
    }
    if (GetBarTexture() != newTex)
    {
        SetBarTexture(newTex);
    }
}

HealthIndicatorWnd::HealthIndicatorWnd()
{
    // RVA 0x126E40
    // NOTE: the shipped ctor leaves m_vehicleId uninitialized; defaulted to -1
    // here for safety (GetVehicle() treats -1 as "no vehicle").
    m_vehicleId = -1;
    m_type = TYPE_IN_MAIN_INTERFACE;
}

HealthIndicatorWnd::HealthIndicatorWnd(HealthIndicatorWnd const&) : HealthIndicatorWnd()
{
    // Matches the original: the copy ctor default-constructs the base and
    // resets the bar textures/health string; nothing is copied from the source.
}

ai::Vehicle* HealthIndicatorWnd::GetVehicle() const
{
    // RVA 0x127250
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    return (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle)) ? static_cast<ai::Vehicle*>(obj) : nullptr;
}

void HealthIndicatorWnd::SetBarTextures(CStr const& texGreenName, CStr const& texYellowName, CStr const& texRedName)
{
    // RVA 0x127770
    for (auto& tex : m_barTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex);
    }
    m_barTextures[0] = M3D_APP->m_pInterfaceManager->GetIcoByName(texGreenName, 0);
    m_barTextures[1] = M3D_APP->m_pInterfaceManager->GetIcoByName(texYellowName, 0);
    m_barTextures[2] = M3D_APP->m_pInterfaceManager->GetIcoByName(texRedName, 0);
    for (auto& tex : m_barTextures)
    {
        M3D_RENDERER->ReferenceTexture(tex);
    }
}

int HealthIndicatorWnd::UpdateOnNewFrame()
{
    // RVA 0x127230
    UpdateValue();
    UpdateBarTexture();
    return 1;
}

void HealthIndicatorWnd::UpdateValue()
{
    // RVA 0x127320
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }
    float const oldCurVal = GetCurValue();
    float const oldMaxVal = GetMaxValue();
    SetMaxValue(vehicle->GetMaxHealth());
    SetCurValue(vehicle->GetHealth());
    if (m_type == TYPE_IN_CHARACTERISTIC_WND && (oldCurVal != GetCurValue() || oldMaxVal != GetMaxValue()))
    {
        UpdateTooltip();
    }
}

void HealthIndicatorWnd::UpdateTooltip()
{
    // RVA 0x1273C0
    CStr text =
        m_strHealth + ": " + CStr(static_cast<int>(GetCurValue())) + "/" + CStr(static_cast<int>(GetMaxValue()));
    SetProperty(PROP_WND_TOOLTIP, &text);
}

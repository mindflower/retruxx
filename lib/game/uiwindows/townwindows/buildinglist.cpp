#include "buildinglist.h"

#include <core/kernel.h>
#include <core/log.h>

#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>

#include <ui/button.h>
#include <ui/image.h>
#include <ui/textbox.h>

#include <server/objects/bar.h>
#include <server/objects/building.h>
#include <server/objects/town.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypeinfo.h>

// ===========================================================================
//  BuildingButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(BuildingButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BuildingButton);

BuildingButton::AuxInfo::AuxInfo()
{
    // RVA 0x95B70
    m_wndPatternName = "wndBuildingButton";
    m_wndIcoName = "wndBuildingIco";
    m_lblNameName = "lblBuildingName";
    m_wndPattern = nullptr;
    m_wndPatternIco = nullptr;
    m_lblPatternName = nullptr;
}

BuildingButton::AuxInfo::AuxInfo(BuildingButton::AuxInfo const& o)
{
    m_wndPatternName = o.m_wndPatternName;
    m_wndIcoName = o.m_wndIcoName;
    m_lblNameName = o.m_lblNameName;
    m_wndPattern = o.m_wndPattern;
    m_wndPatternIco = o.m_wndPatternIco;
    m_lblPatternName = o.m_lblPatternName;
}

BuildingButton::BuildingButton()
{
    // RVA 0x95C50
    m_wndBuildingIco = nullptr;
    m_lblBuildingName = nullptr;
    m_buildingId = -1;
    m_idx = -1;
}

BuildingButton::BuildingButton(BuildingButton const&) : BuildingButton()
{
    // Matches the original: the copy ctor default-constructs the base and only
    // resets the vtable; no BuildingButton state is copied. Reached via Clone().
}

BuildingButton::~BuildingButton()
{
    // The icon and label are owned children, released by the base destructor.
}

m3d::Object* BuildingButton::Clone()
{
    // RVA 0x95A00
    return new BuildingButton(*this);
}

m3d::Object* BuildingButton::CreateObject()
{
    // RVA 0x95AB0
    return new BuildingButton;
}

m3d::Class* BuildingButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* BuildingButton::GetClass() const
{
    // RVA 0x95AA0
    return RT_CLASS_LOCAL(BuildingButton);
}

int BuildingButton::GetIdx() const
{
    // RVA 0x96560
    return m_idx;
}

void BuildingButton::SetIdx(int idx)
{
    // RVA 0x96570
    m_idx = idx;
}

ai::Building* BuildingButton::GetBuilding() const
{
    // RVA 0x96610
    if (m_buildingId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_buildingId);
    if (obj && obj->IsKindOf(&ai::Building::m_classBuilding))
    {
        return static_cast<ai::Building*>(obj);
    }
    return nullptr;
}

CStr BuildingButton::GetNameForBuilding(ai::Building const* building) const
{
    // RVA 0x966F0
    if (!building)
    {
        return CStr();
    }
    return ai::theObjects->GetObjectFullName(building->GetName());
}

m3d::rend::TexHandle BuildingButton::GetIcoForBuilding(ai::Building const* building) const
{
    // RVA 0x96670
    if (!building)
    {
        return m3d::rend::TexHandle();
    }
    ai::BuildingPrototypeInfo const* pi = building->GetPrototypeInfo();
    ai::BuildingType const type = pi ? pi->m_buildingType : ai::NUM_BUILDINGTYPES;
    return M3D_APP->m_pInterfaceManager->GetIcoByName(ai::Building::GetBuildingTypeName(type), 1);
}

int BuildingButton::LoadPattern(m3d::ui::Wnd* pattern)
{
    // RVA 0x95CD0
    if (!pattern)
    {
        M3D_LOG_INFO("BuildingButton::CreateFromPattern - error to create - invalid pattern wnd");
        return 0;
    }

    m3d::Object* c = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (!c || !c->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
    {
        M3D_LOG_INFO("BuildingButton::CreateFromPattern - error to create - invalid pattern wnd");
        return 0;
    }
    m_aif.m_wndPattern = static_cast<m3d::ui::ButtonWnd*>(c);
    pattern->RemoveChild(m_aif.m_wndPattern);

    BoundsBase<float> const patB = m_aif.m_wndPattern->GetBounds();

    m3d::Object* ico = pattern->GetChildByName(m_aif.m_wndIcoName);
    if (!ico || !ico->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
    {
        M3D_LOG_INFO(CStr("BuildingButton::LoadPattern error - cannot find child wnd ") + m_aif.m_wndIcoName);
        return 0;
    }
    m_aif.m_wndPatternIco = static_cast<m3d::ui::ImageWnd*>(ico);
    pattern->RemoveChild(m_aif.m_wndPatternIco);
    {
        BoundsBase<float> b = m_aif.m_wndPatternIco->GetBounds();
        b.x0 -= patB.x0;
        b.y0 -= patB.y0;
        m_aif.m_wndPatternIco->SetBounds(b, true);
    }

    m3d::Object* lbl = pattern->GetChildByName(m_aif.m_lblNameName);
    if (!lbl || !lbl->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO(CStr("BuildingButton::LoadPattern error - cannot find child wnd ") + m_aif.m_lblNameName);
        return 0;
    }
    m_aif.m_lblPatternName = static_cast<m3d::ui::Wnd*>(lbl);
    pattern->RemoveChild(m_aif.m_lblPatternName);
    {
        BoundsBase<float> b = m_aif.m_lblPatternName->GetBounds();
        b.x0 -= patB.x0;
        b.y0 -= patB.y0;
        m_aif.m_lblPatternName->SetBounds(b, true);
    }
    return 1;
}

void BuildingButton::ClearPattern()
{
    // RVA 0x95F60
    if (m_aif.m_wndPattern)
    {
        m_aif.m_wndPattern->DecRef();
    }
    m_aif.m_wndPattern = nullptr;
    if (m_aif.m_wndPatternIco)
    {
        m_aif.m_wndPatternIco->DecRef();
    }
    m_aif.m_wndPatternIco = nullptr;
    if (m_aif.m_lblPatternName)
    {
        m_aif.m_lblPatternName->DecRef();
    }
    m_aif.m_lblPatternName = nullptr;
}

int BuildingButton::CreateFromPattern()
{
    // RVA 0x95FB0
    if (Valid())
    {
        return 0;
    }

    if (!help::IsWndValid(m_aif.m_wndPattern))
    {
        M3D_LOG_INFO("BuildingButton::CreateFromPattern error - null pattern");
        return 0;
    }

    if (!m3d::ui::Wnd::Create({}, m_aif.m_wndPattern->GetStyle(), m_aif.m_wndPattern->GetBounds(),
                              m_aif.m_wndPattern->GetId()))
    {
        M3D_LOG_INFO("BuildingButton::CreateFromPattern error - cannot create window");
        return 0;
    }

    if (m_aif.m_wndPattern->IsImaged())
    {
        m3d::ui::ButtonWnd::SetImaged(
            m_aif.m_wndPattern->GetImageRegular(),
            m_aif.m_wndPattern->GetImageDown(),
            m_aif.m_wndPattern->GetImageIn(),
            m3d::rend::TexHandle());
    }
    else
    {
        SetPane(m_aif.m_wndPattern->GetPaneName());
        SetPaneFlags(m_aif.m_wndPattern->GetPaneFlags());
    }

    if (!CreateChildren())
    {
        M3D_LOG_INFO("BuildingButton::CreateFromPattern error - cannot create children");
        return 0;
    }

    m_gameDataFlags |= 1u;
    return 1;
}

int BuildingButton::CreateChildren()
{
    // RVA 0x96220
    if (!help::IsWndValid(m_aif.m_wndPatternIco) || !help::IsWndValid(m_aif.m_lblPatternName))
    {
        return 0;
    }

    if (m_wndBuildingIco || m_lblBuildingName)
    {
        M3D_LOG_INFO("BuildingButton::CreateChildren error - children already exist");
        return 0;
    }

    // --- building icon ---------------------------------------------------
    m_wndBuildingIco = static_cast<m3d::ui::ImageWnd*>(M3D_KERNEL->New("ImageWnd"));
    if (!m_wndBuildingIco)
    {
        return 0;
    }
    if (!m_wndBuildingIco->CreateImageWnd(m_aif.m_wndPatternIco->GetBounds(), m_aif.m_wndPatternIco->GetImage()))
    {
        m_wndBuildingIco->DecRef();
        m_wndBuildingIco = nullptr;
        return 0;
    }
    m_wndBuildingIco->SetStyle(m_aif.m_wndPatternIco->GetStyle());
    m_wndBuildingIco->SetPane(m_aif.m_wndPatternIco->GetPaneName());
    m_wndBuildingIco->SetPaneFlags(m_aif.m_wndPatternIco->GetPaneFlags());
    AddChild(m_wndBuildingIco);

    // --- building name label -------------------------------------------
    m_lblBuildingName = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_lblBuildingName)
    {
        return 0;
    }
    if (!m_lblBuildingName->Create({}, m_aif.m_lblPatternName->GetStyle(), m_aif.m_lblPatternName->GetBounds(),
                                   m_aif.m_lblPatternName->GetId()))
    {
        m_lblBuildingName->DecRef();
        m_lblBuildingName = nullptr;
        return 0;
    }
    m_lblBuildingName->SetDefaultFont(m_aif.m_lblPatternName->GetDefaultFont());
    m_lblBuildingName->SetWrapMode(m_aif.m_lblPatternName->GetWrapMode());
    m_lblBuildingName->SetFormatMode(m_aif.m_lblPatternName->GetFormatMode());
    m_lblBuildingName->SetColor(m_aif.m_lblPatternName->GetColor());
    m_lblBuildingName->SetTextColor(m_aif.m_lblPatternName->GetTextColor());
    m_lblBuildingName->SetPane(m_aif.m_lblPatternName->GetPaneName());
    m_lblBuildingName->SetPaneFlags(m_aif.m_lblPatternName->GetPaneFlags());
    AddChild(m_lblBuildingName);
    return 1;
}

int BuildingButton::SetUpForBuilding(ai::Building const* building)
{
    // RVA 0x96580
    if (Valid() || !building || !CreateFromPattern())
    {
        return 0;
    }

    m_buildingId = building->GetId();
    m_lblBuildingName->SetText(GetNameForBuilding(building));
    m_wndBuildingIco->SetImage(GetIcoForBuilding(building));
    return 1;
}

// ===========================================================================
//  BuildingList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(BuildingList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BuildingList);

BuildingList::AuxInfo::AuxInfo()
{
    // RVA 0x967E0
    m_spaceY = 7.0f;
}

BuildingList::BuildingList()
{
    // RVA 0x967F0
    m_townId = -1;
    m_aif.m_spaceY = 7.0f;
}

BuildingList::BuildingList(BuildingList const&) : BuildingList()
{
    // Matches the original: the copy ctor default-constructs everything; nothing
    // is copied from the source. Reached only via Clone().
}

BuildingList::~BuildingList()
{
    // RVA 0x968A0
    m_townId = -1;
    ClearItems();
    BuildingButton::ClearPattern();
    // The m_buttons storage is released by std::vector's destructor.
}

m3d::Object* BuildingList::Clone()
{
    // RVA 0x95A30
    return new BuildingList(*this);
}

m3d::Object* BuildingList::CreateObject()
{
    // RVA 0x96770
    return new BuildingList;
}

m3d::Class* BuildingList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* BuildingList::GetClass() const
{
    // RVA 0x96760
    return RT_CLASS_LOCAL(BuildingList);
}

ai::Town const* BuildingList::GetTown() const
{
    // RVA 0x96D20
    if (m_townId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_townId);
    if (obj && obj->IsKindOf(&ai::Town::m_classTown))
    {
        return static_cast<ai::Town const*>(obj);
    }
    return nullptr;
}

int BuildingList::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x96900
    if (!patternWnd)
    {
        M3D_LOG_INFO("BuildingList::CreateFromPattern error - null patternWnd");
        return 0;
    }

    if (!m3d::ui::Wnd::Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        M3D_LOG_INFO("BuildingList::CreateFromPattern error - cannot create window");
        return 0;
    }

    auto* parent = RT_DYNCAST(patternWnd->GetParent(), m3d::ui::Wnd);
    if (!parent)
    {
        M3D_LOG_INFO("BuildingList::CreateFromPattern erro - null parent for paternWnd");
        return 0;
    }
    parent->AddChild(this);

    if (!BuildingButton::LoadPattern(parent))
    {
        M3D_LOG_INFO("BuildingList::CreateFromPattern error - cannot load pattern for BuildingButton");
        return 0;
    }

    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }

    m_gameDataFlags |= 1u;
    return 1;
}

int BuildingList::GameDataClear(bool)
{
    // RVA 0x96B20
    m_townId = -1;
    ClearItems();
    return 1;
}

void BuildingList::ClearItems()
{
    // RVA 0x96B40
    for (BuildingButton* btn : m_buttons)
    {
        if (btn)
        {
            btn->DecRef();
        }
    }
    m_buttons.clear();
}

void BuildingList::SetUpForTown(ai::Town* town)
{
    // RVA 0x96BD0
    if (town)
    {
        m_townId = town->GetId();
        if (IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
        {
            CreateItems();
        }
    }
    else
    {
        GameDataClear(false);
    }
}

int BuildingList::OnBeforeAddToWndStation()
{
    // RVA 0x96C10
    CreateItems();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int BuildingList::OnAfterRemoveFromWndStation()
{
    // RVA 0x96C30
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    ClearItems();
    return res;
}

void BuildingList::FullUpdate()
{
    // RVA 0x96C50
    CreateItems();
}

int BuildingList::CreateItems()
{
    // RVA 0x96C60
    ClearItems();
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    ai::Town const* town = GetTown();
    if (!town)
    {
        return 0;
    }

    int res = 1;
    for (ai::Building* building : town->GetAllBuildings())
    {
        auto* btn = static_cast<BuildingButton*>(M3D_KERNEL->New("BuildingButton"));
        if (!btn)
        {
            return 0;
        }
        if (btn->SetUpForBuilding(building))
        {
            AddButton(btn);
        }
        else
        {
            btn->DecRef();
            res = 0;
        }
    }
    return res;
}

int BuildingList::AddButton(BuildingButton* btn)
{
    // RVA 0x96D80
    if (!btn)
    {
        return 0;
    }
    m_buttons.push_back(btn);
    btn->SetIdx(static_cast<int>(m_buttons.size()) - 1);
    PlaceButton(btn);
    AddChild(btn);
    return 1;
}

void BuildingList::PlaceButton(BuildingButton* btn)
{
    // RVA 0x96EC0
    if (!btn)
    {
        return;
    }
    int const idx = btn->GetIdx();
    if (idx < 0 || idx >= static_cast<int>(m_buttons.size()))
    {
        return;
    }

    float x0 = 0.0f;
    float y0 = 0.0f;
    if (idx > 0)
    {
        BuildingButton* prev = m_buttons[idx - 1];
        if (prev)
        {
            BoundsBase<float> const prevB = prev->GetBounds();
            y0 = (m_aif.m_spaceY + prevB.height) + prevB.y0;
            x0 = prevB.x0;
        }
    }

    BoundsBase<float> b = btn->GetBounds();
    b.x0 = x0;
    b.y0 = y0;
    btn->SetBounds(b, true);
}

void BuildingList::RecalcLayot()
{
    // RVA 0x96DE0
    for (BuildingButton* btn : m_buttons)
    {
        PlaceButton(btn);
    }

    if (!m_buttons.empty())
    {
        BuildingButton* last = m_buttons.back();
        if (last)
        {
            BoundsBase<float> const b = last->GetBounds();
            m_bounds.height = b.height + b.y0;
            m_bounds.width = b.width + b.x0;
        }
    }
    else
    {
        m_bounds.width = 0.0f;
        m_bounds.height = 0.0f;
    }
}

int BuildingList::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x96F40 - a thunk straight to the base reflection helper.
    return ReflectChildNotifyToParent(from, id, msg, data);
}

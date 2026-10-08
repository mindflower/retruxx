#include "weapongroupbuttonlist.h"

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <game/uimisc/weapongroup.h>
#include <game/uiwindows/mainwindows/weaponinfolist.h>
#include <m3dapp.h>
#include <server/objects/base/complexphysicobj.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/vehicle.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>

namespace
{
    // Every gun can belong to one of five weapon groups.
    int const NUM_WEAPON_GROUPS = 5;

    // Id shared by all of the group buttons; the button object itself carries
    // which group it stands for.
    unsigned const ID_BTN_WEAPON_GROUP = 1000;

    // GameDataUpdate data types this list reacts to.
    int const DATATYPE_KEY_BINDINGS_CHANGED = 17;
    int const DATATYPE_WEAPON_GROUP_CHANGED = 30;
    int const DATATYPE_VEHICLE_PART_CHANGED = 65;

    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window.
    void CopyWndPropsFromPattern(m3d::ui::Wnd* dst, m3d::ui::Wnd const* pat)
    {
        dst->SetStyle(pat->GetStyle());
        dst->SetText(pat->GetText());
        dst->SetId(pat->GetId());
        dst->SetName(pat->GetName());
        dst->SetBounds(pat->GetBounds(), true);
        dst->SetDefaultFont(pat->GetDefaultFont());
        dst->SetWrapMode(pat->GetWrapMode());
        dst->SetFormatMode(pat->GetFormatMode());
        dst->SetColor(pat->GetColor());
        dst->SetTextColor(pat->GetTextColor());
        dst->SetTextColorDisabled(pat->GetTextColorDisabled());
        dst->SetClientEdges(pat->GetClientEdges());
        dst->SetPane(pat->GetPaneName());
        dst->SetPaneFlags(pat->GetPaneFlags());
        dst->SetScrollPane(pat->GetScrollPaneName());
        dst->SetBackground(pat->GetBackground());

        CStr tooltip;
        pat->GetProperty(PROP_WND_TOOLTIP, &tooltip);
        dst->SetProperty(PROP_WND_TOOLTIP, &tooltip);

        dst->SetOnShowAnimation(pat->GetOnShowAnimation());
        dst->SetOnHideAnimation(pat->GetOnHideAnimation());
    }
}  // namespace

// ===========================================================================
//  WeaponGroupButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(WeaponGroupButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupButton);

WeaponGroupButton::AuxInfo::AuxInfo() :
    m_texIdSelected("WeaponGroupBtnSel_"),
    m_texIdUnselectedOut("WeaponGroupBtnUnselOut"),
    m_texIdUnselectedIn("WeaponGroupBtnUnselIn"),
    m_texIdDisabled("WeaponGroupBtnDisabled")
{
    // RVA 0x481EC0 - the selected texture id is a prefix; the group number is
    // appended to it in GetTexturesByState.
}

WeaponGroupButton::AuxInfo::AuxInfo(WeaponGroupButton::AuxInfo const& rhs) :
    m_texIdSelected(rhs.m_texIdSelected),
    m_texIdUnselectedOut(rhs.m_texIdUnselectedOut),
    m_texIdUnselectedIn(rhs.m_texIdUnselectedIn),
    m_texIdDisabled(rhs.m_texIdDisabled)
{
}

m3d::Class* WeaponGroupButton::GetBaseClass()
{
    return &m3d::ui::ButtonWnd::m_classButtonWnd;
}

m3d::Class* WeaponGroupButton::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponGroupButton);
}

m3d::Object* WeaponGroupButton::CreateObject()
{
    return new WeaponGroupButton;
}

m3d::Object* WeaponGroupButton::Clone()
{
    // RVA 0x481DB0 - a fresh default-constructed button, not a copy of *this.
    return new WeaponGroupButton;
}

WeaponGroupButton::WeaponGroupButton() : m_groupId(-1), m_state(STATE_DISABLED)
{
    // RVA 0x481FD0 - a button starts disabled until the list tells it otherwise.
}

WeaponGroupButton::WeaponGroupButton(WeaponGroupButton const&) :
    // NOTE: the shipped copy ctor (RVA 0x4820D0) runs the ButtonWnd base ctor
    // and the AuxInfo ctor and stops, leaving the group id and state
    // uninitialised. Given the default values here.
    m_groupId(-1),
    m_state(STATE_DISABLED)
{
}

WeaponGroupButton::~WeaponGroupButton() = default;

int WeaponGroupButton::SetupForGroup(int groupId)
{
    // RVA 0x482110. NOTE two quirks, both as shipped: the group id is stored
    // before the range check, so an out-of-range id sticks even though the call
    // reports failure; and the textures are only refreshed when the button
    // happens to be selected, so an unselected button keeps the old artwork
    // until something else calls SetState.
    m_groupId = groupId;
    if (static_cast<unsigned>(groupId) > 4u)
    {
        return 0;
    }
    if (m_state == STATE_SELECTED)
    {
        UpdateTextures();
    }
    return 1;
}

int WeaponGroupButton::GetGroupId() const
{
    // RVA 0x482140
    return m_groupId;
}

void WeaponGroupButton::SetState(WeaponGroupButton::State state)
{
    // RVA 0x482150
    m_state = state;
    UpdateTextures();
}

WeaponGroupButton::State WeaponGroupButton::GetState() const
{
    // RVA 0x482160
    return m_state;
}

void WeaponGroupButton::UpdateTextures()
{
    // RVA 0x482170 - the disabled image is always left invalid, so a disabled
    // button shows its "disabled state" texture in all three live states rather
    // than through the button's own disabled slot.
    m3d::rend::TexHandle texOut;
    m3d::rend::TexHandle texIn;
    m3d::rend::TexHandle texDown;
    GetTexturesByState(m_state, m_groupId, texOut, texIn, texDown);
    SetImaged(texOut, texIn, texDown, m3d::rend::TexHandle());
}

void WeaponGroupButton::GetTexturesByState(
    WeaponGroupButton::State state,
    int groupId,
    m3d::rend::TexHandle& texOut,
    m3d::rend::TexHandle& texIn,
    m3d::rend::TexHandle& texDown) const
{
    // RVA 0x4821E0
    switch (state)
    {
    case STATE_UNSELECTED:
        texOut = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdUnselectedOut, 0);
        texIn = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdUnselectedIn, 0);
        texDown = texIn;
        break;
    case STATE_SELECTED:
    {
        // Groups are numbered from 1 in the artwork.
        m3d::rend::TexHandle const tex =
            M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdSelected + CStr(groupId + 1), 0);
        texOut = tex;
        texIn = tex;
        texDown = tex;
        break;
    }
    case STATE_DISABLED:
    {
        m3d::rend::TexHandle const tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdDisabled, 0);
        texOut = tex;
        texIn = tex;
        texDown = tex;
        break;
    }
    default:
        break;
    }
}

// ===========================================================================
//  WeaponGroupButtonList::Pattern / AuxInfo
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(WeaponGroupButtonList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupButtonList);

WeaponGroupButtonList::AuxInfo::AuxInfo() :
    m_wndPatternName("wndWeaponGroups"),
    m_patternWeaponGroupButtonName("btnWeaponGroup_"),
    m_wndPatternWeaponSlotName("wndWeaponSlot")
{
    // RVA 0x4823E0
}

WeaponGroupButtonList::AuxInfo::AuxInfo(WeaponGroupButtonList::AuxInfo const& rhs) :
    m_wndPatternName(rhs.m_wndPatternName),
    m_patternWeaponGroupButtonName(rhs.m_patternWeaponGroupButtonName),
    m_wndPatternWeaponSlotName(rhs.m_wndPatternWeaponSlotName)
{
}

WeaponGroupButtonList::Pattern::Pattern() : m_wndPattern(nullptr), m_ref(0)
{
    // RVA 0x4824C0 - the button slots are sized up front, one per weapon group.
    m_patternWeaponGroupButtons.assign(NUM_WEAPON_GROUPS, nullptr);
}

WeaponGroupButtonList::Pattern::Pattern(WeaponGroupButtonList::Pattern const& rhs) :
    m_wndPattern(rhs.m_wndPattern),
    m_patternWeaponGroupButtons(rhs.m_patternWeaponGroupButtons),
    m_ref(rhs.m_ref)
{
}

WeaponGroupButtonList::Pattern::~Pattern()
{
    // RVA 0x482520
    Clear();
}

void WeaponGroupButtonList::Pattern::IncRef()
{
    // RVA 0x482560
    ++m_ref;
}

void WeaponGroupButtonList::Pattern::DecRef()
{
    // RVA 0x482570 - the last holder tears the shared pattern down.
    if (--m_ref <= 0)
    {
        Clear();
    }
}

int WeaponGroupButtonList::Pattern::GetRef() const
{
    // RVA 0x482590
    return m_ref;
}

void WeaponGroupButtonList::Pattern::Clear()
{
    // RVA 0x4825A0
    if (m_wndPattern)
    {
        m_wndPattern->DecRef();
    }
    m_wndPattern = nullptr;

    for (int i = 0; i < static_cast<int>(m_patternWeaponGroupButtons.size()); ++i)
    {
        if (m_patternWeaponGroupButtons[i])
        {
            m_patternWeaponGroupButtons[i]->DecRef();
        }
        m_patternWeaponGroupButtons[i] = nullptr;
    }
    m_patternWeaponGroupButtons.clear();
}

// ===========================================================================
//  WeaponGroupButtonList
// ===========================================================================

m3d::Class* WeaponGroupButtonList::GetBaseClass()
{
    return &m3d::ui::Wnd::m_classWnd;
}

m3d::Class* WeaponGroupButtonList::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponGroupButtonList);
}

m3d::Object* WeaponGroupButtonList::CreateObject()
{
    return new WeaponGroupButtonList;
}

m3d::Object* WeaponGroupButtonList::Clone()
{
    // RVA 0x481DF0
    return new WeaponGroupButtonList;
}

WeaponGroupButtonList::WeaponGroupButtonList() : m_vehicleId(-1)
{
    // RVA 0x482620 - one button slot per weapon group, filled in by
    // CreateChildren.
    m_weaponGroupButtons.assign(NUM_WEAPON_GROUPS, nullptr);
}

WeaponGroupButtonList::WeaponGroupButtonList(WeaponGroupButtonList const&) :
    // NOTE: the shipped copy ctor (RVA 0x4826E0) leaves the button vector empty
    // and never initialises m_vehicleId. Nulled here.
    m_vehicleId(-1)
{
}

WeaponGroupButtonList::~WeaponGroupButtonList() = default;

int WeaponGroupButtonList::SetupForGunPart(CStr const& gunPartName, int vehicleId)
{
    // RVA 0x482790
    m_gunPartName = gunPartName;
    m_vehicleId = vehicleId;
    if (vehicleId == -1 || m_gunPartName.empty() || !help::IsVehiclePartNameAGunPartName(m_gunPartName) ||
        !CreateFromPattern())
    {
        return 0;
    }
    UpdateButtonsState();
    UpdateButtonsTooltips();
    return 1;
}

ai::Vehicle const* WeaponGroupButtonList::GetVehicle() const
{
    // RVA 0x483AC0
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehicleId), ai::Vehicle);
}

ai::VehiclePart const* WeaponGroupButtonList::GetGun() const
{
    // RVA 0x483B20
    ai::Vehicle const* vehicle = GetVehicle();
    return vehicle ? vehicle->GetPartByName(m_gunPartName) : nullptr;
}

bool WeaponGroupButtonList::IsSetForGun() const
{
    // RVA 0x482AB0
    return GetGun() != nullptr;
}

int WeaponGroupButtonList::GetCurGroupId() const
{
    // RVA 0x482A30 - whichever button is currently showing as selected.
    if ((m_gameDataFlags & 1) == 0 || !IsSetForGun())
    {
        return -1;
    }
    for (int i = 0; i < static_cast<int>(m_weaponGroupButtons.size()); ++i)
    {
        if (m_weaponGroupButtons[i] && m_weaponGroupButtons[i]->GetState() == WeaponGroupButton::STATE_SELECTED)
        {
            return m_weaponGroupButtons[i]->GetGroupId();
        }
    }
    return -1;
}

void WeaponGroupButtonList::UpdateCurGroupId()
{
    // RVA 0x482980 - re-reads the gun's group from the weapon group manager and,
    // if it moved, repaints every button as selected / unselected.
    if ((m_gameDataFlags & 1) == 0 || !IsSetForGun())
    {
        return;
    }
    int const groupId = M3D_APP->m_pInterfaceManager->GetWeaponGroupManager()->GetWeaponGroupIdForWeapon(m_gunPartName);
    if (groupId == GetCurGroupId())
    {
        return;
    }
    for (int i = 0; i < static_cast<int>(m_weaponGroupButtons.size()); ++i)
    {
        if (m_weaponGroupButtons[i])
        {
            m_weaponGroupButtons[i]->SetState(
                m_weaponGroupButtons[i]->GetGroupId() == groupId ? WeaponGroupButton::STATE_SELECTED
                                                                 : WeaponGroupButton::STATE_UNSELECTED);
        }
    }
}

void WeaponGroupButtonList::UpdateButtonsState()
{
    // RVA 0x482AE0 - a slot with no gun in it greys every button out.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (IsSetForGun())
    {
        UpdateCurGroupId();
        return;
    }
    for (int i = 0; i < static_cast<int>(m_weaponGroupButtons.size()); ++i)
    {
        if (m_weaponGroupButtons[i])
        {
            m_weaponGroupButtons[i]->SetState(WeaponGroupButton::STATE_DISABLED);
        }
    }
}

void WeaponGroupButtonList::UpdateButtonsTooltips()
{
    // RVA 0x483B70 - each button advertises the key bound to its group.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    for (int i = 0; i < static_cast<int>(m_weaponGroupButtons.size()); ++i)
    {
        if (!m_weaponGroupButtons[i])
        {
            continue;
        }
        CStr tooltip = WeaponGroupChoiceDlg::CreateTooltipForWeaponGroup(m_weaponGroupButtons[i]->GetGroupId());
        m_weaponGroupButtons[i]->SetProperty(PROP_WND_TOOLTIP, &tooltip);
    }
}

void WeaponGroupButtonList::AddWeaponToGroup(int groupId)
{
    // RVA 0x482900
    M3D_APP->m_pInterfaceManager->GetWeaponGroupManager()->AddWeaponToWeaponGroup(m_gunPartName, groupId);
}

void WeaponGroupButtonList::OnBtnWeaponGroupClick(m3d::ui::Wnd* wndFrom, int, m3d::AIParam const&)
{
    // RVA 0x4828C0 - clicking a group button moves this gun into that group.
    if (!IsSetForGun() || !wndFrom || !wndFrom->IsKindOf(&WeaponGroupButton::m_classWeaponGroupButton))
    {
        return;
    }
    AddWeaponToGroup(static_cast<WeaponGroupButton*>(wndFrom)->GetGroupId());
}

void WeaponGroupButtonList::OnWeaponGroupChanged()
{
    // RVA 0x482930
    if (!IsChildOf(M3D_APP) || !IsSetForGun())
    {
        return;
    }
    UpdateCurGroupId();
}

void WeaponGroupButtonList::OnVehiclePartChanged(void* data)
{
    // RVA 0x483A70 - only a change to this list's own gun slot matters.
    if (!IsChildOf(M3D_APP) || !data)
    {
        return;
    }
    if (static_cast<int*>(data)[13] != m_vehicleId)
    {
        return;
    }
    if (CStr::my_strcmp(static_cast<char const**>(data)[3], m_gunPartName.c_str()) != 0)
    {
        return;
    }
    UpdateButtonsState();
}

void WeaponGroupButtonList::OnKeyBindingsChanged()
{
    // RVA 0x483B40
    if (!IsChildOf(M3D_APP))
    {
        return;
    }
    UpdateButtonsTooltips();
}

int WeaponGroupButtonList::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x482800. NOTE: unlike most windows in this family there is no
    // m_gameDataFlags guard here; the handlers check for themselves.
    switch (dataType)
    {
    case DATATYPE_KEY_BINDINGS_CHANGED:
        OnKeyBindingsChanged();
        break;
    case DATATYPE_WEAPON_GROUP_CHANGED:
        OnWeaponGroupChanged();
        return 1;
    case DATATYPE_VEHICLE_PART_CHANGED:
        OnVehiclePartChanged(data);
        return 1;
    default:
        break;
    }
    return 1;
}

int WeaponGroupButtonList::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x482870
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id != ID_BTN_WEAPON_GROUP || msg != 1)
    {
        return 0;
    }
    OnBtnWeaponGroupClick(from, static_cast<int>(id), data);
    return 1;
}

// ---- pattern -------------------------------------------------------------

int WeaponGroupButtonList::LoadPattern(m3d::ui::Wnd* pattern)
{
    // RVA 0x482B60 - pulls the group-button row out of the weapon slot pattern
    // and re-bases everything onto it, so a whole row can be cloned at once.
    // Already-loaded patterns are shared, not rebuilt.
    if (m_pattern.GetRef() > 0)
    {
        return 1;
    }
    if (!pattern)
    {
        M3D_LOG_INFO("WeaponGroupButtonList::LoadPattern - error to create - invalid pattern wnd");
        return 0;
    }

    m3d::Object* wndPattern = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (!wndPattern || !wndPattern->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("WeaponGroupButtonList::LoadPattern - error to create - invalid pattern wnd");
        return 0;
    }
    m_pattern.m_wndPattern = static_cast<m3d::ui::Wnd*>(wndPattern);
    pattern->RemoveChild(m_pattern.m_wndPattern);

    int res = 1;
    for (int i = 0; i < static_cast<int>(m_pattern.m_patternWeaponGroupButtons.size()); ++i)
    {
        // The buttons are named from 1 in the XML.
        CStr const name = m_aif.m_patternWeaponGroupButtonName + CStr(i + 1);
        m3d::Object* child = pattern->GetChildByName(name);
        if (!child || !child->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + CStr(i + 1));
            res = 0;
            continue;
        }
        auto* btn = static_cast<m3d::ui::ButtonWnd*>(child);
        m_pattern.m_patternWeaponGroupButtons[i] = btn;
        pattern->RemoveChild(btn);

        BoundsBase<float> b = btn->GetBounds();
        b.x0 -= m_pattern.m_wndPattern->GetBounds().x0;
        b.y0 -= m_pattern.m_wndPattern->GetBounds().y0;
        btn->SetBounds(b, true);
    }

    m3d::Object* weaponSlotPattern = pattern->GetChildByName(m_aif.m_wndPatternWeaponSlotName);
    if (!weaponSlotPattern || !weaponSlotPattern->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("WeaponGroupButtonList::LoadPattern - error to create - invalid weapon slot pattern wnd");
        return 0;
    }
    // The row itself is re-based onto the slot it lives in.
    BoundsBase<float> const slotB = static_cast<m3d::ui::Wnd*>(weaponSlotPattern)->GetBounds();
    BoundsBase<float> rowB = m_pattern.m_wndPattern->GetBounds();
    rowB.x0 -= slotB.x0;
    rowB.y0 -= slotB.y0;
    m_pattern.m_wndPattern->SetBounds(rowB, true);
    return res;
}

int WeaponGroupButtonList::CreateFromPattern()
{
    // RVA 0x482F40
    if (Valid())
    {
        return 0;
    }
    if (!m_pattern.m_wndPattern || !M3D_APP->IsWndAlive(m_pattern.m_wndPattern, -1) ||
        !m_pattern.m_wndPattern->Valid())
    {
        M3D_LOG_INFO("WeaponGroupButtonList::CreateFromPattern error - null pattern");
        return 0;
    }

    if (!Create(CStr(), m_pattern.m_wndPattern->GetStyle(), m_pattern.m_wndPattern->GetBounds(),
                m_pattern.m_wndPattern->GetId()))
    {
        M3D_LOG_INFO("WeaponGroupButtonList::CreateFromPattern error - cannot create window");
        return 0;
    }
    CopyWndPropsFromPattern(this, m_pattern.m_wndPattern);

    if (!CreateChildren())
    {
        M3D_LOG_INFO("WeaponGroupButtonList::CreateFromPattern error - cannot create children");
        return 0;
    }
    m_gameDataFlags |= 1u;
    return 1;
}

int WeaponGroupButtonList::CreateChildren()
{
    // RVA 0x483350 - one WeaponGroupButton per pattern button, each bound to the
    // group its slot index names.
    int res = 1;
    for (int i = 0; i < static_cast<int>(m_weaponGroupButtons.size()); ++i)
    {
        m3d::ui::ButtonWnd* pat = m_pattern.m_patternWeaponGroupButtons[i];
        if (!pat)
        {
            M3D_LOG_INFO("CREATE error - NULL pattern");
            M3D_LOG_INFO("CREATE_BUTTON error creation window");
            res = 0;
            continue;
        }

        auto* btn = RT_DYNCAST(M3D_KERNEL->New("WeaponGroupButton"), WeaponGroupButton);
        m_weaponGroupButtons[i] = btn;
        if (!btn)
        {
            M3D_LOG_INFO("CREATE error - cannot instantiate object");
            M3D_LOG_INFO("CREATE_BUTTON error creation window");
            res = 0;
            continue;
        }
        if (!btn->Create(CStr(), pat->GetStyle(), pat->GetBounds(), pat->GetId()))
        {
            M3D_LOG_INFO("CREATE_BUTTON error creation window");
            res = 0;
            continue;
        }
        CopyWndPropsFromPattern(btn, pat);
        btn->SetupForGroup(i);
        AddChild(btn);
    }
    return res;
}

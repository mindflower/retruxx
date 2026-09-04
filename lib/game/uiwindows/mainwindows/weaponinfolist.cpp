#include "weaponinfolist.h"

#include "core/kernel.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimanager/uidefs.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/weapongroup.h"
#include "ui/image.h"
#include "ui/progressbarwnd.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/guns/compoundgun.h"
#include "server/objects/guns/gun.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

namespace
{
    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window; the shipped WeaponInfoWnd pattern-copy
    // inlines this block once per widget.
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

    // Instantiates one child of `className`, creates it from the pattern's
    // style/bounds/id, links it at the front of the parent's child list and
    // copies the pattern's properties over. `failMsg` is the per-widget-kind
    // message the shipped CREATE_IMAGE / CREATE_WND / CREATE_PROGRESS_BAR macros
    // log after the specific cause.
    m3d::ui::Wnd* CreateChildFromPattern(
        m3d::ui::Wnd* parent,
        char const* className,
        m3d::ui::Wnd const* pat,
        char const* failMsg)
    {
        if (!pat)
        {
            M3D_LOG_INFO("CREATE error - NULL pattern");
        }
        else if (auto* wnd = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New(className)))
        {
            if (wnd->Create(CStr(), pat->GetStyle(), pat->GetBounds(), pat->GetId()))
            {
                parent->AddChild(wnd);
                parent->MoveChildToFirstPosition(wnd);
                CopyWndPropsFromPattern(wnd, pat);
                return wnd;
            }
            wnd->DecRef();
            M3D_LOG_INFO("CREATE error - cannot create window");
        }
        else
        {
            M3D_LOG_INFO("CREATE error - cannot instantiate object");
        }
        M3D_LOG_INFO(failMsg);
        return nullptr;
    }
}  // namespace

RT_CLASS_EXPORTS_BEGIN(WeaponInfoList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponInfoList);

WeaponInfoList::AuxInfo::AuxInfo()
{
    m_smallSpace = 3.0f;
    m_bigSpace = 5.0f;
    m_bottom = 22.0f;
    m_groupInMainUIW = 12.0f;
    m_groupInCharWndW = 12.0f;
    m_groupInFignyaWndW = 20.0f;
    m_groupInCharWndSpace = 10.0f;

    m_groupTexInMainUi = "WeaponGroupInMainUi";
    m_groupTexInCharWnd = "WeaponGroupInCharWnd";
    m_groupTexInFignyaWnd = "WeaponGroupInFignyaWnd";
    m_wndDecorTopName = "wndDecorTop";
    m_wndDecorBottomName = "wndDecorBottom";
}

m3d::Class* WeaponInfoList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* WeaponInfoList::CreateObject()
{
    return new WeaponInfoList;
}

int WeaponInfoList::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x13D090
    if (!patternWnd)
    {
        return 0;
    }
    auto* parent = static_cast<m3d::ui::Wnd*>(patternWnd->GetParent());
    if (!parent)
    {
        return 0;
    }

    if (!Create({}, 0x201u, patternWnd->GetBounds(), 0))
    {
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        patternWnd->DecRef();
    }
    m_type = TYPE_IN_CHARACTERISTIC_WND;
    if (!WeaponInfoWnd::LoadPattern(parent, TYPE_IN_CHARACTERISTIC_WND))
    {
        M3D_LOG_INFO("WeaponInfoList::CreateFromPattern error - cannot load pattern for WeaponInfoWnd");
        return 0;
    }
    InitGroupTextures();
    m_gameDataFlags |= 1u;
    return 1;
}

m3d::Object* WeaponInfoList::Clone()
{
    // RVA 0x137EF0
    return new WeaponInfoList(*this);
}

m3d::Class* WeaponInfoList::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponInfoList);
}

WeaponInfoList::~WeaponInfoList()
{
    // RVA 0x13CD00 - m_aif's CStr members, the m_items/m_groupButtons/
    // m_groupTextures vectors, and the Wnd base all clean up automatically.
    GameDataClear(false);
    WeaponInfoWnd::ClearPattern(m_type);
    ReleaseGroupTextures();
}

void WeaponInfoList::UpdateOnKeyBindingsChanged()
{
    // RVA 0x13D540
    if (!IsChildOf(M3D_APP))
    {
        return;
    }
    for (auto* btn : m_groupButtons)
    {
        SetTooltipForGroupButton(btn);
    }
}

int WeaponInfoList::RecalcLayotInMainInterface()
{
    // RVA 0x13D8A0
    // NOTE: the shipped code computes this same "row per weapon group" grid
    // layout via heavily aliased/reused stack slots that could not be
    // transcribed byte-exact; reconstructed here as the clearly-intended
    // behavior - items in a group are placed side by side (one
    // pattern-width apart) in a row below the decor-top image, rows stacked
    // per group with m_aif.m_bigSpace between them, and the decor-bottom
    // image repositioned to sit right below the last row.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (m_items.empty())
    {
        m_bounds.height = 0.0f;
        return 1;
    }

    BoundsBase<float> const patternBounds = WeaponInfoWnd::m_patterns[TYPE_IN_MAIN_INTERFACE].m_wndPattern->GetBounds();
    BoundsBase<float> const decorTopBounds = m_wndDecorTop->GetBounds();
    float rowTop = decorTopBounds.y0 + decorTopBounds.height;

    std::vector<ref_ptr<WeaponInfoWnd>> groupItems;
    for (int group = 0; group < GetNumOfGroups(); ++group)
    {
        GetItemsByGroup(group, groupItems);
        for (size_t i = 0; i < groupItems.size(); ++i)
        {
            BoundsBase<float> itemBounds = patternBounds;
            itemBounds.x0 = static_cast<float>(i) * patternBounds.width;
            itemBounds.y0 = rowTop;
            groupItems[i]->SetBounds(itemBounds, true);
        }
        if (!groupItems.empty())
        {
            rowTop += patternBounds.height + m_aif.m_bigSpace;
        }
    }

    float const newHeight = rowTop - m_aif.m_bigSpace;
    m_bounds.height = newHeight;
    BoundsBase<float> decorBottomBounds = m_wndDecorBottom->GetBounds();
    decorBottomBounds.y0 = newHeight - decorBottomBounds.height;
    m_wndDecorBottom->SetBounds(decorBottomBounds, true);
    return 1;
}

int WeaponInfoList::GameDataSetup()
{
    using namespace m3d::ui;

    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        if (m_guiId == IW_WND_WEAPON_INFO_LIST)
        {
            m_type = TYPE_IN_MAIN_INTERFACE;

            auto child = GetChildByName(m_aif.m_wndDecorTopName);
            if (child && IS_KIND_OF(child, ImageWnd))
            {
                m_wndDecorTop = RT_DYNCAST(child, ImageWnd);
            }
            else
            {
                M3D_LOG_ERR(
                    "Get control error: control " + m_aif.m_wndDecorTopName + " is not found or incorrect type");
                res = 0;
            }

            child = GetChildByName(m_aif.m_wndDecorBottomName);
            if (child && IS_KIND_OF(child, ImageWnd))
            {
                m_wndDecorBottom = RT_DYNCAST(child, ImageWnd);
            }
            else
            {
                M3D_LOG_ERR(
                    "Get control error: control " + m_aif.m_wndDecorBottomName + " is not found or incorrect type");
                res = 0;
            }
        }
        else if (m_guiId <= IW_WND_WEAPON_INFO_LIST || m_guiId > IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_RIGHT)
        {
            res = 0;
        }
        else
        {
            m_type = TYPE_IN_CHARACTERISTIC_WND;
        }
        InitGroupTextures();
        if (WeaponInfoWnd::LoadPattern(this, m_type))
        {
            if (res)
            {
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_ERR("WeaponInfoList::GameDataSetup error - cannot load pattern for WeaponInfoWnd");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_ERR("WeaponInfoList: error - fail to init because of a bad resource");
    return 0;
}

int WeaponInfoList::OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
{
    // RVA 0x13DE60
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (idFrom < 500000 || idFrom >= 500005 || message != 1)
    {
        return 0;
    }
    auto* item = reinterpret_cast<m3d::Object*>(from->GetInt());
    if (!item || !item->IsKindOf(&WeaponInfoWnd::m_classWeaponInfoWnd))
    {
        return 1;
    }
    auto* wiw = static_cast<WeaponInfoWnd*>(item);
    ai::Obj const* gun = wiw->GetGun();
    if (!gun)
    {
        return 1;
    }
    int const gunObjId = gun->GetId();

    auto weaponGroupChoiceDlg = M3D_APP->m_pInterfaceManager->GetWindow(89);
    if (weaponGroupChoiceDlg)
    {
        BoundsBase<float> const fromScreenBounds = ToScreen(from->GetBounds());
        BoundsBase<float> dlgBounds = weaponGroupChoiceDlg->GetBounds();
        dlgBounds.x0 = fromScreenBounds.x0 + fromScreenBounds.width;
        dlgBounds.y0 = fromScreenBounds.y0 + fromScreenBounds.height;
        weaponGroupChoiceDlg->SetBounds(dlgBounds, false);

        int selectedGroup = -1;
        M3D_APP->m_pInterfaceManager->ShowWindow(89, true, false, true, false, &selectedGroup);
        if (selectedGroup != -1)
        {
            M3D_APP->m_pInterfaceManager->GetWeaponGroupManager()->AddWeaponToWeaponGroup(gunObjId, selectedGroup);
        }
    }
    return 1;
}

WeaponInfoList::WeaponInfoList()
{
    // RVA 0x13CB70
    // NOTE: the shipped ctor never initializes m_vehicleId; defaulted to -1
    // here for safety (it's used as an array index in GetVehicle()).
    m_vehicleId = -1;
    m_wndDecorTop = nullptr;
    m_wndDecorBottom = nullptr;
    m_type = TYPE_NUM_TYPES;
}

WeaponInfoList::WeaponInfoList(WeaponInfoList const&) : WeaponInfoList()
{
    // NOTE: the shipped copy ctor (RVA 0x13CCA0) default-constructs the
    // base, empty vectors, and the AuxInfo, but leaves m_vehicleId/m_type/
    // m_wndDecorTop/m_wndDecorBottom uninitialized; delegating to the
    // default ctor here avoids reading uninitialized pointers while still
    // copying nothing from the source.
}

void WeaponInfoList::GetItemsByGroup(int groupId, std::vector<ref_ptr<WeaponInfoWnd>>& items) const
{
    // RVA 0x13D6B0
    items.clear();
    for (auto const& item : m_items)
    {
        if (item && item->GetGroupId() == groupId)
        {
            items.push_back(item);
        }
    }
}

ai::Vehicle* WeaponInfoList::GetVehicle() const
{
    // RVA 0x13D3F0
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    return (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle)) ? static_cast<ai::Vehicle*>(obj) : nullptr;
}

int WeaponInfoList::GetNumOfGroups() const
{
    // RVA 0x13D670
    return 5;
}

int WeaponInfoList::CreateItems()
{
    // RVA 0x13D5A0
    GameDataClear(false);
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return 0;
    }

    for (CStr const& partName : vehicle->GetAttachedPartNames())
    {
        ai::VehiclePart* part = vehicle->GetPartByName(partName);
        if (!part || !(part->IsKindOf(&ai::Gun::m_classGun) || part->IsKindOf(&ai::CompoundGun::m_classCompoundGun)))
        {
            continue;
        }
        int const groupId = GetGroupByVehiclePartName(partName);
        if (groupId != -1)
        {
            AddItem(part->GetId(), groupId);
        }
    }

    if (m_type == TYPE_IN_MAIN_INTERFACE)
    {
        RecalcLayotInMainInterface();
    }
    else if (m_type == TYPE_IN_CHARACTERISTIC_WND)
    {
        RecalcLayotInCharacteristicWnd();
    }
    return 1;
}

int WeaponInfoList::RecalcLayot()
{
    // RVA 0x13D870
    if (m_type == TYPE_IN_MAIN_INTERFACE)
    {
        return RecalcLayotInMainInterface();
    }
    if (m_type == TYPE_IN_CHARACTERISTIC_WND)
    {
        return RecalcLayotInCharacteristicWnd();
    }
    return 0;
}

int WeaponInfoList::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 17:
        if (IsChildOf(M3D_APP))
        {
            UpdateOnKeyBindingsChanged();
        }
        return 1;

    case 30:
        UpdateOnWeaponGroupChanged();
        return 1;

    case 64:
        UpdateOnPlayerVehicleChanged();
        return 1;

    case 65:
        UpdateOnVehiclePartChanged(data);
        return 1;

    default:
        break;
    }
    return 1;
}

int WeaponInfoList::AddGroupButton(int group, BoundsBase<float> const& btnB, WeaponInfoWnd const* wiw, int idx)
{
    // RVA 0x13DD50
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    auto* btn = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!btn)
    {
        return 0;
    }
    if (!btn->Create(CStr(group + 1), 265728u, btnB, group + 500000))
    {
        btn->DecRef();
        return 0;
    }
    btn->SetBackground(GetGroupTextureForWeaponBtn(wiw, idx));
    btn->SetPaneFlags(0);
    btn->SetDefaultFont(1);
    btn->SetTextColor(m_textColor);
    btn->SetInt(reinterpret_cast<unsigned int>(wiw));
    SetTooltipForGroupButton(btn);
    m_groupButtons.push_back(btn);
    AddChild(btn);
    return 1;
}

bool WeaponInfoList::CanGunShotToTarget(ai::Obj const* gun) const
{
    // RVA 0x13E0A0 (thunk)
    return help::CanGunShotToSeenObj(gun);
}

int WeaponInfoList::UpdateOnVehiclePartChanged(void* data)
{
    // RVA 0x13D4A0
    if (!IsChildOf(M3D_APP) || !data)
    {
        return 0;
    }
    auto const* ev = static_cast<m3d::Event const*>(data);
    if (m_vehicleId != ev->m_intEv[0])
    {
        return 0;
    }
    if (M3D_APP->m_pInterfaceManager->GetWeaponGroupManager()->GetWeaponGroupIdForWeapon(ev->m_strEv) == -1)
    {
        return 0;
    }
    return CreateItems();
}

void WeaponInfoList::UpdateOnWeaponGroupChanged()
{
    // RVA 0x13D510
    if (IsChildOf(M3D_APP))
    {
        CreateItems();
    }
}

m3d::rend::TexHandle WeaponInfoList::GetGroupTextureForWeaponBtn(WeaponInfoWnd const* wiw, int)
{
    // RVA 0x13E070
    if (!wiw || m_groupTextures.empty())
    {
        return {};
    }
    bool const canShoot = help::CanGunShotToSeenObj(wiw->GetGun());
    return canShoot ? m_groupTextures.front().second : m_groupTextures.front().first;
}

int WeaponInfoList::RecalcLayotInCharacteristicWnd()
{
    // RVA 0x13DAD0
    // NOTE: reconstructed for the same reason as RecalcLayotInMainInterface
    // - items in a group stack vertically (one pattern-height apart) with a
    // group-select button to their left, groups stacked with
    // m_aif.m_bigSpace between them.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    BoundsBase<float> const patternBounds =
        WeaponInfoWnd::m_patterns[TYPE_IN_CHARACTERISTIC_WND].m_wndPattern->GetBounds();
    float const x0 = patternBounds.x0;
    float groupTop = patternBounds.y0;
    int groupBtnIdx = 0;
    bool anyItems = false;

    std::vector<ref_ptr<WeaponInfoWnd>> groupItems;
    for (int group = 0; group < GetNumOfGroups(); ++group)
    {
        GetItemsByGroup(group, groupItems);
        for (size_t i = 0; i < groupItems.size(); ++i)
        {
            BoundsBase<float> itemBounds = patternBounds;
            itemBounds.x0 = x0;
            itemBounds.y0 = groupTop + static_cast<float>(i) * patternBounds.height;
            groupItems[i]->SetBounds(itemBounds, true);

            BoundsBase<float> btnBounds;
            btnBounds.x0 = patternBounds.x0 - m_aif.m_groupInCharWndW - m_aif.m_smallSpace;
            btnBounds.y0 = itemBounds.y0;
            btnBounds.width = m_aif.m_groupInCharWndW;
            btnBounds.height = patternBounds.height;
            AddGroupButton(group, btnBounds, groupItems[i].get(), groupBtnIdx++);
        }
        if (!groupItems.empty())
        {
            anyItems = true;
            groupTop += static_cast<float>(groupItems.size()) * patternBounds.height + m_aif.m_bigSpace;
        }
    }

    m_bounds.height = anyItems ? (groupTop - m_aif.m_bigSpace) : 0.0f;
    return 1;
}

int WeaponInfoList::GetGroupByVehiclePartName(CStr const& partName) const
{
    // RVA 0x13D690
    return M3D_APP->m_pInterfaceManager->GetWeaponGroupManager()->GetWeaponGroupIdForWeapon(partName);
}

int WeaponInfoList::OnBeforeAddToWndStation()
{
    CreateItems();
    return Wnd::OnBeforeAddToWndStation();
}

int WeaponInfoList::UpdateOnPlayerVehicleChanged()
{
    // RVA 0x13D450
    ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    m_vehicleId = vehicle ? vehicle->GetId() : -1;
    if (IsChildOf(M3D_APP))
    {
        return CreateItems();
    }
    return 1;
}

int WeaponInfoList::AddItem(int gunId, int groupId)
{
    // RVA 0x13D750
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    auto* item = static_cast<WeaponInfoWnd*>(M3D_KERNEL->New("WeaponInfoWnd"));
    if (!item)
    {
        return 0;
    }
    if (!item->SetupForGun(gunId, groupId, m_type))
    {
        item->DecRef();
        return 0;
    }
    m_items.push_back(item);
    AddChild(item);
    MoveChildToFirstPosition(item);

    int itemGuiId = -1;
    M3D_APP->m_pInterfaceManager->AddWindow(item, itemGuiId, false, false);
    M3D_APP->m_pInterfaceManager->SetEventsForWindow(itemGuiId, {89});
    return 1;
}

void WeaponInfoList::InitGroupTextures()
{
    ReleaseGroupTextures();
    CStr groupTexName;
    if (m_type)
    {
        if (m_type != 1)
        {
            return;
        }
        groupTexName = m_aif.m_groupTexInCharWnd;
    }
    else
    {
        groupTexName = m_aif.m_groupTexInMainUi;
    }

    auto const unselTex = M3D_APP->m_pInterfaceManager->GetIcoByName(groupTexName, 0);
    auto const selTex = M3D_APP->m_pInterfaceManager->GetIcoByName(groupTexName, 1);
    M3D_RENDERER->ReferenceTexture(unselTex);
    M3D_RENDERER->ReferenceTexture(selTex);
    m_groupTextures.push_back({unselTex, selTex});
}

void WeaponInfoList::ReleaseGroupTextures()
{
    for (auto& tex : m_groupTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex.first);
        M3D_RENDERER->ReleaseTexture(tex.second);
    }

    m_groupTextures.clear();
}

void WeaponInfoList::SetTooltipForGroupButton(m3d::ui::Wnd* groupButton)
{
    // RVA 0x13E010
    if (m_type != TYPE_IN_CHARACTERISTIC_WND || !groupButton)
    {
        return;
    }
    auto* item = reinterpret_cast<m3d::Object*>(groupButton->GetInt());
    if (item && item->IsKindOf(&WeaponInfoWnd::m_classWeaponInfoWnd))
    {
        auto* wiw = static_cast<WeaponInfoWnd*>(item);
        CStr tooltip = WeaponGroupChoiceDlg::CreateTooltipForWeaponGroup(wiw->GetGroupId());
        groupButton->SetProperty(PROP_WND_TOOLTIP, &tooltip);
    }
}

int WeaponInfoList::GameDataClear(bool)
{
    // RVA 0x13D290
    for (auto const& item : m_items)
    {
        if (item)
        {
            M3D_APP->m_pInterfaceManager->RemoveWindow(item->GetGuiId());
        }
    }
    m_items.clear();
    for (auto* btn : m_groupButtons)
    {
        if (btn)
        {
            btn->DecRef();
        }
    }
    m_groupButtons.clear();
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(WeaponInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponInfoWnd);

WeaponInfoWnd::AuxInfo WeaponInfoWnd::m_aif;
WeaponInfoWnd::Pattern WeaponInfoWnd::m_patterns[2];
m3d::rend::TexHandle WeaponInfoWnd::m_pbChargeTextureReady;
m3d::rend::TexHandle WeaponInfoWnd::m_pbChargeTextureRecharging;

WeaponInfoWnd::Pattern::Pattern()
{
    // RVA 0x138540
    m_wndPattern = nullptr;
    m_wndPatternIco = nullptr;
    m_wndPatternName = nullptr;
    m_wndPatternCharge = nullptr;
    m_wndPatternAmmo = nullptr;
    m_wndPatternFrame0 = nullptr;
    m_wndPatternFrame1 = nullptr;
    m_wndPatternBg = nullptr;
    m_wndPatternCanShotBg = nullptr;
    m_wndPatternGroup = nullptr;
}

WeaponInfoWnd::AuxInfo::AuxInfo()
{
    // RVA 0x138190
    m_wndPatternName = "WeaponInfoWnd";
    m_wndPatternIcoName = "wndIco";
    m_wndPatternChargeName = "wndCharge";
    m_wndPatternNameName = "wndName";
    m_wndPatternAmmoName = "wndAmmo";
    m_wndPatternFrame0Name = "wndFrame0";
    m_wndPatternFrame1Name = "wndFrame1";
    m_wndPatternBgName = "wndBg";
    m_wndPatternCanShotBgName = "wndCanShotBg";
    m_wndPatternGroupName = "wndGroup";
    m_height = 24.0f;
    m_paneName = "PaneGrayThin";
    m_space = 5.0f;
    m_chargeBarW = 55.0f;
    m_chargeBarTexName = "ChargeBar";
    m_lblReloadsW = 60.0f;
    for (int ft = ai::FT_MACHINE_GUN; ft < ai::FT_NUM_FIRING_TYPES; ++ft)
    {
        m_weaponTypeTexNames[ft] = ai::GunPrototypeInfo::FiringType2Str(static_cast<ai::FiringTypes>(ft));
    }
}

int WeaponInfoWnd::GetGroupId() const
{
    // RVA 0x138890
    return m_groupId;
}

ai::Obj const* WeaponInfoWnd::GetGun() const
{
    // RVA 0x138820
    if (m_gunId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_gunId);
    if (obj && (obj->IsKindOf(&ai::Gun::m_classGun) || obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun)))
    {
        return obj;
    }
    return nullptr;
}

m3d::Object* WeaponInfoWnd::CreateObject()
{
    // RVA 0x138160
    return new WeaponInfoWnd;
}

m3d::Class* WeaponInfoWnd::GetClass() const
{
    // RVA 0x138150
    return RT_CLASS_LOCAL(WeaponInfoWnd);
}

m3d::Class* WeaponInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

WeaponInfoWnd::~WeaponInfoWnd()
{
    // RVA 0x138610 - the Wnd base cleans up automatically.
}

int WeaponInfoWnd::SetupForGun(int gunId, int groupId, WeaponInfoList::Type type)
{
    // RVA 0x138620
    if (Valid() || gunId == -1 || groupId == -1 || type >= WeaponInfoList::TYPE_NUM_TYPES || !CreateFromPattern(type))
    {
        return 0;
    }
    m_type = type;
    m_gunId = gunId;
    m_groupId = groupId;

    ai::Obj const* gun = GetGun();
    if (!gun)
    {
        GameDataClear(false);
        return 0;
    }
    m_wndIco->SetImage(GetIco());

    if (m_type != WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND)
    {
        if (!help::IsGunWithCharging(gun))
        {
            m_wndCharge->SetMaxValue(100.0f);
            m_wndCharge->SetCurValue(m_wndCharge->GetMaxValue());
            m_wndCharge->SetBarTexture(m_pbChargeTextureReady);
        }
        m_wndGroup->SetText(CStr(groupId + 1));
    }
    if (m_type == WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND)
    {
        m_wndName->SetText(ai::thePrototypeManager->GetPrototypeFullName(gun->GetPrototypeId()));
    }
    return 1;
}

m3d::Object* WeaponInfoWnd::Clone()
{
    // RVA 0x137F20
    return new WeaponInfoWnd(*this);
}

WeaponInfoWnd::WeaponInfoWnd()
{
    // RVA 0x138570
    m_gunId = -1;
    m_wndCharge = nullptr;
    m_wndIco = nullptr;
    m_wndName = nullptr;
    m_wndAmmo = nullptr;
    m_wndFrame0 = nullptr;
    m_wndFrame1 = nullptr;
    m_wndBg = nullptr;
    m_wndCanShotBg = nullptr;
    // NOTE: the shipped ctor never initializes m_wndGroup; defaulted to
    // nullptr here for safety (it is dereferenced later, e.g. in
    // SetupForGun).
    m_wndGroup = nullptr;
    m_groupId = -1;
    m_type = WeaponInfoList::TYPE_NUM_TYPES;
}

WeaponInfoWnd::WeaponInfoWnd(WeaponInfoWnd const&) : WeaponInfoWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x138600) default-constructs only the
    // base, leaving every WeaponInfoWnd-specific member (m_gunId and all
    // child window pointers, m_groupId, m_type) uninitialized; delegating to
    // the default ctor here avoids reading uninitialized pointers while
    // still copying nothing from the source.
}

m3d::rend::TexHandle WeaponInfoWnd::GetIco() const
{
    // RVA 0x13C8B0
    ai::Obj const* gun = GetGun();
    if (gun && gun->GetPrototypeInfo())
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName(gun->GetPrototypeInfo()->m_prototypeName, 1);
    }
    return {};
}

int WeaponInfoWnd::GetAmmo() const
{
    // RVA 0x13C8F0
    ai::Obj const* gun = GetGun();
    if (!gun || !help::IsGunWithCharging(gun))
    {
        return -1;
    }
    bool withPoolLimit = false;
    if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
    {
        withPoolLimit = g->IsWithShellsPoolLimit();
    }
    else if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
    {
        withPoolLimit = cg->IsWithShellsPoolLimit();
    }
    else
    {
        return -1;
    }
    if (!withPoolLimit)
    {
        return -1;
    }
    return static_cast<int>(help::GetGunShellsInPool(gun) + help::GetGunShellsInCurrentCharge(gun));
}

void WeaponInfoWnd::UpdateAmmo()
{
    // RVA 0x138A90
    if (m_type != WeaponInfoList::TYPE_IN_MAIN_INTERFACE || (m_gameDataFlags & 1) == 0)
    {
        return;
    }
    int const ammo = GetAmmo();
    m_wndAmmo->SetText(ammo == -1 ? CStr() : CStr(ammo));
}

void WeaponInfoWnd::UpdateCanShotBg()
{
    // RVA 0x138B10
    if (m_type != WeaponInfoList::TYPE_IN_MAIN_INTERFACE || (m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndCanShotBg->ShowWindow(help::CanGunFire(GetGun()));
}

void WeaponInfoWnd::UpdateOnNewFrame()
{
    // RVA 0x1388C0
    if (GetGun())
    {
        UpdateCharge();
        UpdateAmmo();
        if (m_type == WeaponInfoList::TYPE_IN_MAIN_INTERFACE && (m_gameDataFlags & 1) != 0)
        {
            m_wndCanShotBg->ShowWindow(help::CanGunFire(GetGun()));
        }
    }
    else
    {
        GameDataClear(false);
    }
}

int WeaponInfoWnd::CreateFromPattern(WeaponInfoList::Type type)
{
    // RVA 0x139750
    if (static_cast<unsigned int>(type) > WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND)
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateFromPattern error - invalid type");
        return 0;
    }
    if (Valid())
    {
        return 0;
    }
    if (!help::IsWndValid(m_patterns[type].m_wndPattern))
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateFromPattern error - null pattern");
        return 0;
    }

    auto* patternWnd = m_patterns[type].m_wndPattern;
    if (!Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateFromPattern error - cannot create window");
        return 0;
    }
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());

    if (!CreateChildren(type))
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateFromPattern error - cannot create children");
        return 0;
    }
    m_gameDataFlags |= 1u;
    return 1;
}

m3d::rend::TexHandle WeaponInfoWnd::GetBarTexture(bool bReady) const
{
    // RVA 0x13C890
    return bReady ? m_pbChargeTextureReady : m_pbChargeTextureRecharging;
}

int WeaponInfoWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x1388A0
    if (dataType == 89)
    {
        UpdateOnNewFrame();
    }
    return 1;
}

int WeaponInfoWnd::GameDataClear(bool)
{
    // RVA 0x138760
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_gunId = -1;
    if (m_wndCharge)
    {
        m_wndCharge->SetCurValue(0.0f);
    }
    if (m_wndName)
    {
        m_wndName->SetText(CStr());
    }
    if (m_wndAmmo)
    {
        m_wndAmmo->SetText(CStr());
    }
    return 1;
}

ai::DamageType WeaponInfoWnd::GetWeaponTypeForGun(ai::Obj const* gun) const
{
    // RVA 0x138B50
    if (!gun)
    {
        return ai::DAMAGE_NUM_TYPES;
    }
    if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
    {
        return g->GetDamageType();
    }
    if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
    {
        return cg->GetDamageType();
    }
    return ai::DAMAGE_NUM_TYPES;
}

void WeaponInfoWnd::UpdateCharge()
{
    // RVA 0x138930
    if (m_type == WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND || (m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::Obj const* gun = GetGun();
    if (!gun)
    {
        return;
    }

    m3d::rend::TexHandle newTex;
    if (IsGunReady())
    {
        m_wndCharge->SetMaxValue(static_cast<float>(help::GetGunChargeSize(gun)));
        m_wndCharge->SetCurValue(static_cast<float>(help::GetGunShellsInCurrentCharge(gun)));
        newTex = m_pbChargeTextureReady;
    }
    else
    {
        m_wndCharge->SetMaxValue(help::GetGunRechargingTime(gun));
        m_wndCharge->SetCurValue(help::GetGunCurrentRechargingTime(gun));
        newTex = m_pbChargeTextureRecharging;
    }
    if (m_wndCharge->GetBarTexture() != newTex)
    {
        m_wndCharge->SetBarTexture(newTex);
    }
}

void WeaponInfoWnd::ClearPattern(WeaponInfoList::Type type)
{
    // RVA 0x138B90
    if (type > WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND)
    {
        return;
    }
    Pattern& pattern = m_patterns[type];
    if (pattern.m_wndPattern)
    {
        pattern.m_wndPattern->DecRef();
    }
    pattern.m_wndPattern = nullptr;
    if (pattern.m_wndPatternIco)
    {
        pattern.m_wndPatternIco->DecRef();
    }
    pattern.m_wndPatternIco = nullptr;
    if (pattern.m_wndPatternName)
    {
        pattern.m_wndPatternName->DecRef();
    }
    pattern.m_wndPatternName = nullptr;
    if (pattern.m_wndPatternCharge)
    {
        pattern.m_wndPatternCharge->DecRef();
    }
    pattern.m_wndPatternCharge = nullptr;
    if (pattern.m_wndPatternAmmo)
    {
        pattern.m_wndPatternAmmo->DecRef();
    }
    pattern.m_wndPatternAmmo = nullptr;
    if (pattern.m_wndPatternFrame0)
    {
        pattern.m_wndPatternFrame0->DecRef();
    }
    pattern.m_wndPatternFrame0 = nullptr;
    if (pattern.m_wndPatternFrame1)
    {
        pattern.m_wndPatternFrame1->DecRef();
    }
    pattern.m_wndPatternFrame1 = nullptr;
    if (pattern.m_wndPatternBg)
    {
        pattern.m_wndPatternBg->DecRef();
    }
    pattern.m_wndPatternBg = nullptr;
    if (pattern.m_wndPatternCanShotBg)
    {
        pattern.m_wndPatternCanShotBg->DecRef();
    }
    pattern.m_wndPatternCanShotBg = nullptr;
    if (pattern.m_wndPatternGroup)
    {
        pattern.m_wndPatternGroup->DecRef();
    }
    pattern.m_wndPatternGroup = nullptr;

    M3D_RENDERER->ReleaseTexture(m_pbChargeTextureReady);
    M3D_RENDERER->ReleaseTexture(m_pbChargeTextureRecharging);
}

int WeaponInfoWnd::LoadPattern(m3d::ui::Wnd* pattern, WeaponInfoList::Type type)
{
    // RVA 0x138CC0
    using namespace m3d::ui;

    if (type > WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND)
    {
        M3D_LOG_INFO("WeaponInfoWnd::LoadPattern error - invalid type");
        return 0;
    }
    if (!pattern)
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateFromPattern - error to create - invalid pattern wnd");
        return 0;
    }
    if (m_patterns[type].m_wndPattern)
    {
        return 1;
    }

    auto* baseWnd = RT_DYNCAST(pattern->GetChildByName(m_aif.m_wndPatternName), Wnd);
    if (!baseWnd)
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateFromPattern - error to create - invalid pattern wnd");
        return 0;
    }
    m_patterns[type].m_wndPattern = baseWnd;
    pattern->RemoveChild(baseWnd);

    int res = 1;
    BoundsBase<float> const baseBounds = baseWnd->GetBounds();
    // NOTE: the shipped code unrolls this into ~10 near-identical
    // find-validate-detach-reposition blocks (one per named sub-control)
    // that could not be transcribed byte-exact from the decompile;
    // reconstructed here via a small local helper capturing the same steps.
    auto const takeChild = [&](CStr const& name, m3d::Class const& kind) -> Wnd*
    {
        auto* child = RT_DYNCAST(pattern->GetChildByName(name), Wnd);
        if (!child || !child->IsKindOf(&kind))
        {
            M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + name);
            res = 0;
            return nullptr;
        }
        pattern->RemoveChild(child);
        BoundsBase<float> bounds = child->GetBounds();
        bounds.x0 -= baseBounds.x0;
        bounds.y0 -= baseBounds.y0;
        child->SetBounds(bounds, true);
        return child;
    };

    if (type == WeaponInfoList::TYPE_IN_MAIN_INTERFACE)
    {
        m_patterns[type].m_wndPatternIco =
            static_cast<ImageWnd*>(takeChild(m_aif.m_wndPatternIcoName, ImageWnd::m_classImageWnd));
        m_patterns[type].m_wndPatternCharge = static_cast<ProgressBarWnd*>(
            takeChild(m_aif.m_wndPatternChargeName, ProgressBarWnd::m_classProgressBarWnd));
        m_patterns[type].m_wndPatternAmmo = takeChild(m_aif.m_wndPatternAmmoName, Wnd::m_classWnd);
        m_patterns[type].m_wndPatternBg =
            static_cast<ImageWnd*>(takeChild(m_aif.m_wndPatternBgName, ImageWnd::m_classImageWnd));
        m_patterns[type].m_wndPatternCanShotBg =
            static_cast<ImageWnd*>(takeChild(m_aif.m_wndPatternCanShotBgName, ImageWnd::m_classImageWnd));
        m_patterns[type].m_wndPatternGroup = takeChild(m_aif.m_wndPatternGroupName, Wnd::m_classWnd);
    }
    else
    {
        m_patterns[type].m_wndPatternIco =
            static_cast<ImageWnd*>(takeChild(m_aif.m_wndPatternIcoName, ImageWnd::m_classImageWnd));
        m_patterns[type].m_wndPatternName = takeChild(m_aif.m_wndPatternNameName, Wnd::m_classWnd);
        m_patterns[type].m_wndPatternFrame0 = takeChild(m_aif.m_wndPatternFrame0Name, Wnd::m_classWnd);
        m_patterns[type].m_wndPatternFrame1 = takeChild(m_aif.m_wndPatternFrame1Name, Wnd::m_classWnd);
    }

    m_pbChargeTextureReady = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_chargeBarTexName, 0);
    M3D_RENDERER->ReferenceTexture(m_pbChargeTextureReady);
    m_pbChargeTextureRecharging = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_chargeBarTexName, 1);
    M3D_RENDERER->ReferenceTexture(m_pbChargeTextureRecharging);

    return res;
}

void WeaponInfoWnd::UpdateIco()
{
    // RVA 0x138A20
    if (m_type == WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND || (m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!GetGun())
    {
        return;
    }
    // NOTE: matches the shipped build, which calls this and discards the
    // result here.
    IsGunReady();
    m3d::rend::TexHandle const ico = GetIco();
    if (m_wndIco->GetImage() != ico)
    {
        m_wndIco->SetImage(ico);
    }
}

bool WeaponInfoWnd::IsGunReady() const
{
    // RVA 0x13C800
    ai::Obj const* gun = GetGun();
    if (!gun)
    {
        return false;
    }
    bool withCharging;
    if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
    {
        withCharging = g->IsWithCharging();
    }
    else if (auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const))
    {
        withCharging = cg->IsWithCharging();
    }
    else
    {
        return true;
    }
    if (!withCharging)
    {
        return true;
    }
    if (auto const* g = RT_DYNCAST(gun, ai::Gun const))
    {
        return g->GetChargeState() == ai::Gun::csReady;
    }
    auto const* cg = RT_DYNCAST(gun, ai::CompoundGun const);
    return !cg || cg->GetChargeState() == ai::Gun::csReady;
}

int WeaponInfoWnd::CreateChildren(WeaponInfoList::Type type)
{
    // RVA 0x139980
    using namespace m3d::ui;

    if (static_cast<unsigned int>(type) > WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND)
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateChildren error - invalid type");
        return 0;
    }

    Pattern const& pattern = m_patterns[type];
    bool const inCharWnd = type == WeaponInfoList::TYPE_IN_CHARACTERISTIC_WND;
    if (!help::IsWndValid(pattern.m_wndPatternIco) || (inCharWnd && !help::IsWndValid(pattern.m_wndPatternName)) ||
        (!inCharWnd && (!help::IsWndValid(pattern.m_wndPatternCharge) || !help::IsWndValid(pattern.m_wndPatternAmmo))))
    {
        return 0;
    }

    if (m_wndIco || m_wndCharge || m_wndName || m_wndAmmo)
    {
        M3D_LOG_INFO("WeaponInfoWnd::CreateChildren error - children already exist");
        return 0;
    }

    int res = 1;
    auto create = [&](char const* className, Wnd const* pat, char const* failMsg) -> Wnd*
    {
        Wnd* wnd = CreateChildFromPattern(this, className, pat, failMsg);
        if (!wnd)
        {
            res = 0;
        }
        return wnd;
    };

    if (inCharWnd)
    {
        m_wndIco =
            static_cast<ImageWnd*>(create("ImageWnd", pattern.m_wndPatternIco, "CREATE_IMAGE error creation window"));
        if (m_wndIco)
        {
            m_wndIco->SetImage(pattern.m_wndPatternIco->GetImage());
        }
        m_wndName = create("Wnd", pattern.m_wndPatternName, "CREATE_WND error creation window");
        m_wndFrame0 = create("Wnd", pattern.m_wndPatternFrame0, "CREATE_WND error creation window");
        m_wndFrame1 = create("Wnd", pattern.m_wndPatternFrame1, "CREATE_WND error creation window");
        return res;
    }

    m_wndCharge = static_cast<ProgressBarWnd*>(
        create("ProgressBarWnd", pattern.m_wndPatternCharge, "CREATE_PROGRESS_BAR error creation window"));
    if (m_wndCharge)
    {
        ProgressBarWnd const* patCharge = pattern.m_wndPatternCharge;
        m_wndCharge->SetBarColor(patCharge->GetBarColor());
        m_wndCharge->SetBarTexture(patCharge->GetBarTexture());
        m_wndCharge->SetOrientation(patCharge->GetOrientation());
        m_wndCharge->SetNumOfSteps(patCharge->GetNumOfSteps());
        m_wndCharge->SetTextStyle(patCharge->GetTextStyle());
        m_wndCharge->SetTextureStyle(patCharge->GetTextureStyle());
    }

    m_wndBg = static_cast<ImageWnd*>(create("ImageWnd", pattern.m_wndPatternBg, "CREATE_IMAGE error creation window"));
    if (m_wndBg)
    {
        m_wndBg->SetImage(pattern.m_wndPatternBg->GetImage());
    }

    m_wndCanShotBg =
        static_cast<ImageWnd*>(create("ImageWnd", pattern.m_wndPatternCanShotBg, "CREATE_IMAGE error creation window"));
    if (m_wndCanShotBg)
    {
        m_wndCanShotBg->SetImage(pattern.m_wndPatternCanShotBg->GetImage());
    }

    m_wndIco =
        static_cast<ImageWnd*>(create("ImageWnd", pattern.m_wndPatternIco, "CREATE_IMAGE error creation window"));
    if (m_wndIco)
    {
        m_wndIco->SetImage(pattern.m_wndPatternIco->GetImage());
    }

    m_wndAmmo = create("Wnd", pattern.m_wndPatternAmmo, "CREATE_WND error creation window");
    m_wndGroup = create("Wnd", pattern.m_wndPatternGroup, "CREATE_WND error creation window");
    return res;
}

RT_CLASS_EXPORTS_BEGIN(WeaponGroupChoiceDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupChoiceDlg);

WeaponGroupChoiceDlg::AuxInfo::AuxInfo()
{
    m_groupButtonName = "btnGroup";
}

m3d::Object* WeaponGroupChoiceDlg::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr WeaponGroupChoiceDlg::CreateTooltipForWeaponGroup(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupChoiceDlg::CreateObject()
{
    return new WeaponGroupChoiceDlg;
}

m3d::Class* WeaponGroupChoiceDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

WeaponGroupChoiceDlg::~WeaponGroupChoiceDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupChoiceDlg::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponGroupChoiceDlg);
}

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg() = default;

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg(WeaponGroupChoiceDlg const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::GameDataSetup()
{
    // TODO: implement WeaponGroupChoiceDlg::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void WeaponGroupChoiceDlg::SelectButton(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupChoiceDlg::OnKeyBindingsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int WeaponGroupChoiceDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

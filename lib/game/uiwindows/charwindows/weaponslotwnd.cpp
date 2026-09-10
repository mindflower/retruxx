#include "weaponslotwnd.h"
#include "weapongroupbuttonlist.h"

#include <i_event.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/objects/base/prototypemanager.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <ui/wndstation.h>
#include <ui/image.h>
#include <server/resourcemanager.h>
#include <server/objects/vehicle.h>
#include <server/objects/player.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORTS_BEGIN(WeaponSlotWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponSlotWnd);

namespace
{
    // Events the embedded weapon-group row subscribes to: weapon group changed,
    // vehicle part changed, key bindings changed.
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

    // Lifts one pattern child out of the slot pattern and re-bases its origin
    // onto the slot, so the whole row can be cloned as a unit.
    template <class T>
    T* TakePatternChild(m3d::ui::Wnd* pattern, m3d::ui::Wnd* owner, CStr const& name, m3d::Class const* cls, int& res)
    {
        m3d::Object* child = pattern->GetChildByName(name);
        if (!child || !child->IsKindOf(cls))
        {
            M3D_LOG_INFO("GET_CHILD_PATTERN error - cannot find child wnd " + name);
            res = 0;
            return nullptr;
        }
        auto* wnd = static_cast<T*>(child);
        pattern->RemoveChild(wnd);
        BoundsBase<float> b = wnd->GetBounds();
        b.x0 -= owner->GetBounds().x0;
        b.y0 -= owner->GetBounds().y0;
        wnd->SetBounds(b, true);
        return wnd;
    }
}  // namespace

WeaponSlotWnd::AuxInfo::AuxInfo()
{
    m_wndPatternName = "wndWeaponSlot";
    m_wndPatternGunPartIcoName = "wndGunPartIco";
    m_wndPatternGunNameName = "wndGunName";
    m_wndPatternGunIcoName = "wndGunIco";
    m_wndPatternInactiveBgName = "wndInactiveBg";
    m_texIdGunPart = "GunSlot_";
    m_texIdGunPartNone = "GunSlot_None";
    m_strPadding = "---";
    m_strIdGunNone = "NotInstalled";
    m_colorGunNone = static_cast<unsigned int>(-7323584);
}

WeaponSlotWnd::Pattern::Pattern()
{
    // RVA 0x488480. NOTE: the shipped ctor nulls the six pointers but not
    // m_ref; harmless in practice because the only instance is a static.
    m_wndPattern = nullptr;
    m_wndPatternGunPartIco = nullptr;
    m_wndPatternGunName = nullptr;
    m_wndPatternGunIco = nullptr;
    m_wndPatternWeaponGroups = nullptr;
    m_wndPatternInactiveBg = nullptr;
    m_ref = 0;
}

WeaponSlotWnd::Pattern::~Pattern()
{
    Clear();
}

void WeaponSlotWnd::Pattern::Clear()
{
    if (m_wndPattern)
    {
        m_wndPattern->DecRef();
    }
    m_wndPattern = nullptr;
    if (m_wndPatternGunPartIco)
    {
        m_wndPatternGunPartIco->DecRef();
    }
    m_wndPatternGunPartIco = nullptr;
    if (m_wndPatternGunName)
    {
        m_wndPatternGunName->DecRef();
    }
    m_wndPatternGunName = nullptr;
    if (m_wndPatternGunIco)
    {
        m_wndPatternGunIco->DecRef();
    }
    m_wndPatternGunIco = nullptr;
    if (m_wndPatternWeaponGroups)
    {
        m_wndPatternWeaponGroups->DecRef();
    }
    m_wndPatternWeaponGroups = nullptr;
    if (m_wndPatternInactiveBg)
    {
        m_wndPatternInactiveBg->DecRef();
    }
    m_wndPatternInactiveBg = nullptr;
}

void WeaponSlotWnd::Pattern::IncRef()
{
    ++m_ref;
}

void WeaponSlotWnd::Pattern::DecRef()
{
    if (--m_ref <= 0)
    {
        Clear();
    }
}

int WeaponSlotWnd::Pattern::GetRef() const
{
    return m_ref;
}

CStr const& WeaponSlotWnd::GetGunPartName() const
{
    return m_gunPartName;
}

m3d::Class* WeaponSlotWnd::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponSlotWnd);
}

m3d::Object* WeaponSlotWnd::CreateObject()
{
    return new WeaponSlotWnd;
}

m3d::Object* WeaponSlotWnd::Clone()
{
    return new WeaponSlotWnd(*this);
}

int WeaponSlotWnd::SetupForGunPart(CStr const& gunPartName, int vehicleId)
{
    // RVA 0x4886E0. NOTE: an *empty* gun part name is deliberately allowed - it
    // is how WeaponSlotList pads the panel out to its fixed row count. Only a
    // non-empty name that does not resolve to a gun slot is rejected.
    m_gunPartName = gunPartName;
    m_vehicleId = vehicleId;
    if (vehicleId == -1)
    {
        return 0;
    }
    if (!m_gunPartName.empty() && !help::IsVehiclePartNameAGunPartName(gunPartName))
    {
        return 0;
    }
    if (!CreateFromPattern())
    {
        return 0;
    }
    UpdateOnGunPartChanged();
    return 1;
}

WeaponSlotWnd::~WeaponSlotWnd()
{
    if (m_wndWeaponGroups)
    {
        M3D_APP->m_pInterfaceManager->RemoveWindow(m_wndWeaponGroups->GetGuiId());
    }
    // m_wndWeaponGroups (ref_ptr) and m_gunPartName (CStr) release themselves.
}

m3d::Class* WeaponSlotWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int WeaponSlotWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 65)
    {
        OnVehiclePartChanged(data);
    }
    return 1;
}

bool WeaponSlotWnd::IsSetForGunPart() const
{
    return !m_gunPartName.empty() && m_vehicleId != -1;
}

int WeaponSlotWnd::CreateFromPattern()
{
    // RVA 0x4892B0
    if (Valid())
    {
        return 0;
    }
    if (!m_pattern.m_wndPattern || !M3D_APP->IsWndAlive(m_pattern.m_wndPattern, -1) ||
        !m_pattern.m_wndPattern->Valid())
    {
        M3D_LOG_INFO("WeaponSlotWnd::CreateFromPattern error - null pattern");
        return 0;
    }

    if (!Create(CStr(), m_pattern.m_wndPattern->GetStyle(), m_pattern.m_wndPattern->GetBounds(),
                m_pattern.m_wndPattern->GetId()))
    {
        M3D_LOG_INFO("WeaponSlotWnd::CreateFromPattern error - cannot create window");
        return 0;
    }
    CopyWndPropsFromPattern(this, m_pattern.m_wndPattern);

    if (!CreateChildren())
    {
        M3D_LOG_INFO("WeaponSlotWnd::CreateFromPattern error - cannot create children");
        return 0;
    }
    m_gameDataFlags |= 1u;
    return 1;
}

bool WeaponSlotWnd::NeedShowWeaponGroups() const
{
    if (m_gunPartName.empty() || m_vehicleId == -1)
    {
        return false;
    }
    ai::Vehicle const* playerVehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    return GetVehicle() == playerVehicle;
}

void WeaponSlotWnd::UpdateActiveState()
{
    if ((m_gameDataFlags & 1) != 0 && m_wndInactiveBg)
    {
        m_wndInactiveBg->ShowWindow(!IsSetForGunPart());
    }
}

void WeaponSlotWnd::UpdateGunName(ai::VehiclePart const* gun)
{
    // RVA 0x488C20 - three states: a padding slot shows "---", a live but empty
    // slot shows the localized "not installed" caption in its own colour, and a
    // filled slot shows the gun's prototype name.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!IsSetForGunPart())
    {
        m_wndGunName->SetText(m_aif.m_strPadding);
        m_wndGunName->SetTextColor(m_textColor);
        return;
    }
    if (gun)
    {
        m_wndGunName->SetText(ai::thePrototypeManager->GetPrototypeFullName(gun->GetPrototypeId()));
        m_wndGunName->SetTextColor(m_textColor);
    }
    else
    {
        m_wndGunName->SetText(M3D_APP->GetStringByStringId0(m_aif.m_strIdGunNone));
        m_wndGunName->SetTextColor(m_aif.m_colorGunNone);
    }
}

void WeaponSlotWnd::OnVehiclePartChanged(void* data)
{
    if (!IsChildOf(m3d::Application::g_pApp))
    {
        return;
    }
    auto const* evt = static_cast<m3d::Event const*>(data);
    if (evt && evt->m_intEv[0] == m_vehicleId && CStr::my_strcmp(evt->m_strEv.c_str(), m_gunPartName.c_str()) == 0)
    {
        UpdateOnGunChanged();
    }
}

int WeaponSlotWnd::CreateChildren()
{
    // RVA 0x4896C0 - one clone per pattern widget. Each is moved to the front of
    // the child list as it is added, so the four end up drawn in reverse of the
    // order they are created here.
    int res = 1;

    auto clone = [&](m3d::ui::Wnd* pat, char const* className) -> m3d::ui::Wnd*
    {
        if (!pat)
        {
            res = 0;
            return nullptr;
        }
        auto* wnd = RT_DYNCAST(M3D_KERNEL->New(className), m3d::ui::Wnd);
        if (!wnd || !wnd->Create(CStr(), pat->GetStyle(), pat->GetBounds(), pat->GetId()))
        {
            if (wnd)
            {
                wnd->DecRef();
            }
            res = 0;
            return nullptr;
        }
        AddChild(wnd);
        MoveChildToFirstPosition(wnd);
        CopyWndPropsFromPattern(wnd, pat);
        return wnd;
    };

    m_wndGunPartIco = RT_DYNCAST(clone(m_pattern.m_wndPatternGunPartIco, "ImageWnd"), m3d::ui::ImageWnd);
    m_wndGunName = clone(m_pattern.m_wndPatternGunName, "Wnd");
    m_wndGunIco = RT_DYNCAST(clone(m_pattern.m_wndPatternGunIco, "ImageWnd"), m3d::ui::ImageWnd);
    m_wndInactiveBg = clone(m_pattern.m_wndPatternInactiveBg, "Wnd");

    // The weapon-group row only exists on the player's own vehicle.
    if (NeedShowWeaponGroups())
    {
        auto* groups = RT_DYNCAST(M3D_KERNEL->New("WeaponGroupButtonList"), WeaponGroupButtonList);
        m_wndWeaponGroups = groups;
        if (!m_wndWeaponGroups)
        {
            return 0;
        }
        if (!m_wndWeaponGroups->SetupForGunPart(m_gunPartName, m_vehicleId))
        {
            m_wndWeaponGroups = nullptr;
            return 0;
        }

        int guiId = -1;
        if (!M3D_APP->m_pInterfaceManager->AddWindow(m_wndWeaponGroups.get(), guiId, 0, 0))
        {
            m_wndWeaponGroups = nullptr;
            return 0;
        }

        std::vector<int> events;
        events.push_back(DATATYPE_WEAPON_GROUP_CHANGED);
        events.push_back(DATATYPE_VEHICLE_PART_CHANGED);
        events.push_back(DATATYPE_KEY_BINDINGS_CHANGED);
        M3D_APP->m_pInterfaceManager->SetEventsForWindow(guiId, events);

        AddChild(m_wndWeaponGroups.get());
    }
    return res;
}

void WeaponSlotWnd::UpdateGunPartIco(ai::VehiclePart const* gun)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndGunPartIco->SetImage(GetIcoByGunPartName(m_gunPartName, gun != nullptr));
    }
}

void WeaponSlotWnd::UpdateOnGunChanged()
{
    // RVA 0x4887A0 - the same refresh as UpdateOnGunPartChanged minus the active
    // state, which only the slot's own identity can change.
    ai::VehiclePart const* gun = GetGun();
    UpdateGunPartIco(gun);
    UpdateGunName(gun);
    UpdateGunIco(gun);
}

ai::Vehicle const* WeaponSlotWnd::GetVehicle() const
{
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    if (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        return static_cast<ai::Vehicle const*>(obj);
    }
    return nullptr;
}

int WeaponSlotWnd::LoadPattern(m3d::ui::Wnd* pattern)
{
    // RVA 0x488E30 - lifts the four slot widgets out of the host window and
    // re-bases them onto the slot pattern. An already-loaded pattern is shared.
    // NOTE: m_wndPatternWeaponGroups is never filled in here - the group row is
    // cloned from WeaponGroupButtonList's own pattern instead, which
    // WeaponSlotList loads separately.
    if (m_pattern.GetRef() > 0)
    {
        return 1;
    }
    if (!pattern)
    {
        M3D_LOG_INFO("WeaponSlotWnd::LoadPattern - error to create - invalid pattern wnd");
        return 0;
    }

    m3d::Object* wndPattern = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (!wndPattern || !wndPattern->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("WeaponSlotWnd::LoadPattern - error to create - invalid pattern wnd");
        return 0;
    }
    m_pattern.m_wndPattern = static_cast<m3d::ui::Wnd*>(wndPattern);
    pattern->RemoveChild(m_pattern.m_wndPattern);

    int res = 1;
    m_pattern.m_wndPatternGunPartIco = TakePatternChild<m3d::ui::ImageWnd>(
        pattern, m_pattern.m_wndPattern, m_aif.m_wndPatternGunPartIcoName, &m3d::ui::ImageWnd::m_classImageWnd, res);
    m_pattern.m_wndPatternGunName = TakePatternChild<m3d::ui::Wnd>(
        pattern, m_pattern.m_wndPattern, m_aif.m_wndPatternGunNameName, &m3d::ui::Wnd::m_classWnd, res);
    m_pattern.m_wndPatternGunIco = TakePatternChild<m3d::ui::ImageWnd>(
        pattern, m_pattern.m_wndPattern, m_aif.m_wndPatternGunIcoName, &m3d::ui::ImageWnd::m_classImageWnd, res);

    // NOTE: unlike the three above, a missing inactive background aborts the
    // whole load rather than just clearing `res`.
    int bgRes = 1;
    m_pattern.m_wndPatternInactiveBg = TakePatternChild<m3d::ui::Wnd>(
        pattern, m_pattern.m_wndPattern, m_aif.m_wndPatternInactiveBgName, &m3d::ui::Wnd::m_classWnd, bgRes);
    if (!bgRes)
    {
        return 0;
    }
    return res;
}

void WeaponSlotWnd::UpdateOnGunPartChanged()
{
    // RVA 0x488750
    UpdateActiveState();
    ai::VehiclePart const* gun = GetGun();
    UpdateGunPartIco(gun);
    UpdateGunName(gun);
    UpdateGunIco(gun);
}

ai::VehiclePart const* WeaponSlotWnd::GetGun() const
{
    if (m_gunPartName.empty() || m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Vehicle const* vehicle = GetVehicle();
    if (!vehicle)
    {
        return nullptr;
    }
    return vehicle->GetPartByName(m_gunPartName);
}

void WeaponSlotWnd::UpdateGunIco(ai::VehiclePart const* gun)
{
    // RVA 0x488D40 - an empty slot hides the gun icon entirely rather than
    // showing a blank one.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!gun)
    {
        m_wndGunIco->SetImage(m3d::rend::TexHandle{});
        m_wndGunIco->ShowWindow(false);
        return;
    }
    ai::PrototypeInfo const* proto = gun->GetPrototypeInfo();
    m3d::rend::TexHandle const tex =
        proto ? M3D_APP->m_pInterfaceManager->GetIcoByName(proto->m_prototypeName, 1) : m3d::rend::TexHandle{};
    m_wndGunIco->SetImage(tex);
    m_wndGunIco->ShowWindow(true);
}

WeaponSlotWnd::WeaponSlotWnd(WeaponSlotWnd const&) : WeaponSlotWnd()
{
}

WeaponSlotWnd::WeaponSlotWnd()
{
    // RVA 0x488560. NOTE: the shipped ctor writes every pointer but never
    // touches m_vehicleId (offset 0x22c), leaving it uninitialised until
    // SetupForGunPart runs. Set to -1 here so nothing can read garbage.
    m_vehicleId = -1;
    m_wndGunPartIco = nullptr;
    m_wndGunName = nullptr;
    m_wndGunIco = nullptr;
    m_wndInactiveBg = nullptr;
}

m3d::rend::TexHandle WeaponSlotWnd::GetIcoByGunPartName(CStr const& gunPartName, bool bGunInstalled) const
{
    if (gunPartName.empty())
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdGunPartNone, 0);
    }

    CStr const resourceName = ai::theResourceManager->GetResourceNameByVehiclePartName(gunPartName);
    if (resourceName.empty())
    {
        return m3d::rend::TexHandle{};
    }

    ai::Resource* resource = ai::theResourceManager->GetResource(ai::theResourceManager->GetResourceId(resourceName));
    if (!resource)
    {
        return m3d::rend::TexHandle{};
    }

    PointBase<int> const geomSize = resource->GetGeomSize();
    CStr const texName = m_aif.m_texIdGunPart + CStr(geomSize.x) + CStr("x") + CStr(geomSize.y);
    return M3D_APP->m_pInterfaceManager->GetIcoByName(texName, bGunInstalled ? 0 : 1);
}

void WeaponSlotWnd::ClearWeaponGroups()
{
    if (m_wndWeaponGroups)
    {
        M3D_APP->m_pInterfaceManager->RemoveWindow(m_wndWeaponGroups->GetGuiId());
    }
}

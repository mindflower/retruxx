#include "weaponslotlist.h"
#include "weapongroupbuttonlist.h"
#include "weaponslotwnd.h"

#include <algorithm>

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/objects/base/complexphysicobj.h>
#include <server/objects/base/objcontainer.h>
#include <server/resourcemanager.h>
#include <server/objects/vehicle.h>
#include <ui/wndstation.h>

RT_CLASS_EXPORTS_BEGIN(WeaponSlotList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponSlotList);

namespace
{
    // The panel always shows this many rows, padding with empty slots.
    int const NUM_WEAPON_SLOTS = 6;

    // GameDataUpdate data type / the event each slot subscribes to.
    int const DATATYPE_VEHICLE_PART_CHANGED = 65;

    // RVA 0x4856E0 / 0x4857E0 - orders gun slots by weapon class. A part whose
    // resource matches none of these sorts last, and NOTE that because the
    // right-hand position defaults to -1 the comparator also reports "not less"
    // whenever the right-hand part is unclassified; kept as shipped.
    class PartNameLessByResource
    {
    public:
        PartNameLessByResource()
        {
            static char const* const order[] = {"SMALL_GUN", "BIG_GUN", "GIANT_GUN", "SIDE_GUN", "SPECIAL_WEAPON"};
            for (auto const* name : order)
            {
                m_resourceOrder.push_back(ai::theResourceManager->GetResourceId(CStr(name)));
            }
        }

        bool operator()(CStr const& partName1, CStr const& partName2) const
        {
            int const pos1 = FindPos(partName1);
            int const pos2 = FindPos(partName2);
            return pos1 != -1 && pos1 < pos2;
        }

    private:
        int FindPos(CStr const& partName) const
        {
            int const resourceId =
                ai::theResourceManager->GetResourceId(ai::theResourceManager->GetResourceNameByVehiclePartName(partName));
            for (int i = 0; i < static_cast<int>(m_resourceOrder.size()); ++i)
            {
                if (ai::theResourceManager->bResourceIsKindOf(resourceId, m_resourceOrder[i]))
                {
                    return i;
                }
            }
            return -1;
        }

        std::vector<int> m_resourceOrder;
    };

    // RVA 0x485910 / 0x485920 - within one weapon class, a gun hanging off the
    // cabin comes before one that does not. Parts of different classes compare
    // equal so the first pass's ordering survives.
    class PartNameLessByCBParent
    {
    public:
        explicit PartNameLessByCBParent(ai::ComplexPhysicObjPartDescription const* cabinDesc) :
            m_cabinDesc(cabinDesc)
        {
        }

        bool operator()(CStr const& partName1, CStr const& partName2) const
        {
            CStr const resourceName1 = ai::theResourceManager->GetResourceNameByVehiclePartName(partName1);
            CStr const resourceName2 = ai::theResourceManager->GetResourceNameByVehiclePartName(partName2);
            if (CStr::my_strcmp(resourceName1.c_str(), resourceName2.c_str()) != 0 || !m_cabinDesc)
            {
                return false;
            }
            bool const onCabin1 = m_cabinDesc->GetChildByNameDeep(partName1) != nullptr;
            bool const onCabin2 = m_cabinDesc->GetChildByNameDeep(partName2) != nullptr;
            return onCabin1 && !onCabin2;
        }

    private:
        ai::ComplexPhysicObjPartDescription const* m_cabinDesc;
    };
}  // namespace

WeaponSlotList::AuxInfo::AuxInfo()
{
    // RVA 0x484E90 - a negative space overlaps consecutive slots slightly.
    m_space = -2.0f;
}

m3d::Class* WeaponSlotList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* WeaponSlotList::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponSlotList);
}

m3d::Object* WeaponSlotList::CreateObject()
{
    return new WeaponSlotList;
}

m3d::Object* WeaponSlotList::Clone()
{
    // RVA 0x484DA0
    return new WeaponSlotList(*this);
}

WeaponSlotList::WeaponSlotList() : m_vehicleId(-1)
{
    // RVA 0x484EA0
}

WeaponSlotList::WeaponSlotList(WeaponSlotList const&) :
    // NOTE: the shipped copy ctor (RVA 0x484F10) initialises the item vector and
    // the AuxInfo but never touches m_vehicleId. Set here.
    m_vehicleId(-1)
{
}

WeaponSlotList::~WeaponSlotList()
{
    // RVA 0x484F50 - clearing the vehicle first means the CreateItems call below
    // finds nothing to build and so behaves as a plain ClearItems; that is how
    // the shipped destructor is written.
    m_vehicleId = -1;
    if (IsChildOf(M3D_APP))
    {
        CreateItems();
    }
    // This list holds one reference on each shared pattern from GameDataSetup.
    WeaponSlotWnd::m_pattern.DecRef();
    WeaponGroupButtonList::m_pattern.DecRef();
}

ai::Vehicle const* WeaponSlotList::GetVehicle() const
{
    // RVA 0x485240
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehicleId), ai::Vehicle);
}

int WeaponSlotList::SetupForVehicle(int vehicleId)
{
    // RVA 0x4851F0 - note the list is rebuilt either way, so passing an
    // unresolvable id both reports failure and empties the panel.
    m_vehicleId = vehicleId;
    int res = 1;
    if (vehicleId != -1 && !GetVehicle())
    {
        m_vehicleId = -1;
        res = 0;
    }
    if (IsChildOf(M3D_APP))
    {
        CreateItems();
    }
    return res;
}

int WeaponSlotList::GameDataSetup()
{
    // RVA 0x485050 - both shared patterns are loaded out of this window's own
    // children, which are then thrown away.
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        if (!WeaponGroupButtonList::LoadPattern(this))
        {
            // NOTE: the shipped message names the wrong class here.
            M3D_LOG_INFO("WeaponGroupButtonList::GameDataSetup error - cannot load pattern for WeaponGroupButtonList");
            res = 0;
        }
        WeaponGroupButtonList::m_pattern.IncRef();

        if (!WeaponSlotWnd::LoadPattern(this))
        {
            M3D_LOG_INFO("WeaponSlotList::GameDataSetup error - cannot load pattern for WeaponSlotWnd");
            res = 0;
        }
        WeaponSlotWnd::m_pattern.IncRef();

        RemoveAllChildren();
        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("WeaponSlotList: error - fail to init because of a bad resource");
        return 0;
    }
    return 1;
}

int WeaponSlotList::GameDataClear(bool)
{
    // RVA 0x485010
    m_vehicleId = -1;
    if (IsChildOf(M3D_APP))
    {
        CreateItems();
    }
    return 1;
}

int WeaponSlotList::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4851C0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == DATATYPE_VEHICLE_PART_CHANGED)
    {
        OnVehiclePartChanged(data);
    }
    return 1;
}

void WeaponSlotList::OnVehiclePartChanged(void* data)
{
    // RVA 0x485AF0 - a gun coming or going changes which slots exist, so the
    // whole panel is rebuilt.
    if (!IsChildOf(M3D_APP) || !data || static_cast<int*>(data)[13] != m_vehicleId)
    {
        return;
    }
    FullUpdate();
}

int WeaponSlotList::OnBeforeAddToWndStation()
{
    // RVA 0x485B60
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int WeaponSlotList::OnAfterRemoveFromWndStation()
{
    // RVA 0x485B80
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    ClearItems();
    return res;
}

void WeaponSlotList::FullUpdate()
{
    // RVA 0x4852A0
    CreateItems();
}

void WeaponSlotList::CreateItems()
{
    // RVA 0x4852B0 - one slot per attachable gun mount, then padded out with
    // empty slots so the panel is always NUM_WEAPON_SLOTS rows tall.
    ClearItems();
    ai::Vehicle const* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }

    std::vector<CStr> gunPartNames;
    help::GetGunPartNamesThatCanBeAttached(m_vehicleId, gunPartNames);
    SortGunPartNames(gunPartNames, vehicle);

    for (int i = 0; i < static_cast<int>(gunPartNames.size()); ++i)
    {
        AddItem(gunPartNames[i]);
    }
    for (int i = static_cast<int>(gunPartNames.size()); i < NUM_WEAPON_SLOTS; ++i)
    {
        AddItem(CStr());
    }
    RecalcLayot();
}

void WeaponSlotList::ClearItems()
{
    // RVA 0x4853E0
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        if (m_items[i])
        {
            M3D_APP->m_pInterfaceManager->RemoveWindow(m_items[i]->GetGuiId());
        }
    }
    m_items.clear();
}

int WeaponSlotList::AddItem(CStr const& gunPartName)
{
    // RVA 0x4854A0 - each slot is a window in its own right, registered with the
    // interface manager so it receives the vehicle-part event directly.
    auto* item = RT_DYNCAST(M3D_KERNEL->New("WeaponSlotWnd"), WeaponSlotWnd);
    if (!item)
    {
        return 0;
    }
    ref_ptr<WeaponSlotWnd> const slot(item);

    if (!item->SetupForGunPart(gunPartName, m_vehicleId))
    {
        return 0;
    }

    int guiId = -1;
    if (!M3D_APP->m_pInterfaceManager->AddWindow(item, guiId, 0, 0))
    {
        return 0;
    }

    std::vector<int> events;
    events.push_back(DATATYPE_VEHICLE_PART_CHANGED);
    M3D_APP->m_pInterfaceManager->SetEventsForWindow(guiId, events);

    m_items.push_back(slot);
    AddChild(item);
    return 1;
}

void WeaponSlotList::SortGunPartNames(
    std::vector<CStr, std::allocator<CStr>>& gunPartNames, ai::Vehicle const* vehicle) const
{
    // RVA 0x485A30 - two passes. The first orders the slots by weapon class in
    // the fixed order below; the second, which only ever reorders within a run
    // of equal resource, floats the cabin-mounted guns above the rest.
    if (!vehicle)
    {
        return;
    }
    ai::VehiclePrototypeInfo const* proto = vehicle->GetPrototypeInfo();
    if (!proto)
    {
        return;
    }

    std::sort(gunPartNames.begin(), gunPartNames.end(), PartNameLessByResource());

    ai::ComplexPhysicObjPartDescription const* cabinDesc =
        proto->m_partDescription->GetChildByNameDeep(CStr("CABIN"));
    std::sort(gunPartNames.begin(), gunPartNames.end(), PartNameLessByCBParent(cabinDesc));
}

void WeaponSlotList::RecalcLayot()
{
    // RVA 0x4855D0 - the slots are stacked top to bottom, overlapping by
    // m_aif.m_space (which is negative).
    if (m_items.empty() || !m_items[0])
    {
        return;
    }
    BoundsBase<float> const firstB = m_items[0]->GetBounds();
    float curY0 = firstB.y0 + firstB.height + m_aif.m_space;

    for (int i = 1; i < static_cast<int>(m_items.size()); ++i)
    {
        if (!m_items[i])
        {
            continue;
        }
        BoundsBase<float> itemB = m_items[i]->GetBounds();
        itemB.y0 = curY0;
        m_items[i]->SetBounds(itemB, true);
        curY0 += itemB.height + m_aif.m_space;
    }
}

void WeaponSlotList::GetGunPartNames(int vehicleId, std::vector<CStr, std::allocator<CStr>>& gunPartNames) const
{
    // RVA 0x485690
    help::GetGunPartNamesThatCanBeAttached(vehicleId, gunPartNames);
}

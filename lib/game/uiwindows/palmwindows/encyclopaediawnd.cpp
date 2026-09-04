#include "encyclopaediawnd.h"

#include <algorithm>
#include <cctype>

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>

#include <game/m3dgame.h>
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/guihelper.h"
#include <game/uiwindows/commonwindows/contextmodelwnd.h>

#include <ui/comboboxwnd.h>
#include <ui/textbox.h>

#include <core/aiparam.h>
#include <server/affix.h>
#include <server/server.h>
#include <server/utils.h>
#include <server/resourcemanager.h>
#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <server/objects/gadget.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypemanager.h>
#include <server/objects/base/prototypeinfo.h>
#include <server/objects/base/globalproperties.h>

namespace
{
    // RVA 0x6454F8 - filled by the $E59_2 static initializer.
    const CStr kThemeNames[EncyclopaediaWnd::THEME_NUM_THEMES] = {
        "Vehicles", "VehicleParts", "Weapon", "Gadgets", "Affixes", "Goods", "Clans", "QuestItems"};
}  // namespace

// ============================================================================
//  EncyclopaediaWnd
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(EncyclopaediaWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(EncyclopaediaWnd);

EncyclopaediaWnd::AuxInfo::AuxInfo()
{
    // RVA 0xD45C0
    m_subjectListName = "subjectList";
    m_wndDizName = "wndDiz";
    m_wndModelName = "wndModel";
    m_comboThemesName = "comboThemes";
}

EncyclopaediaWnd::EncyclopaediaWnd()
{
    // RVA 0xD46D0
    m_subjectList = nullptr;
    m_wndDiz = nullptr;
    m_comboThemes = nullptr;
    m_wndModel = nullptr;
    m_bInitedOnce = 0;
    m_prevInfoObjId = -1;
    m_prevCapturingObjId = -1;
    m_prevCapturedObjId = -1;
}

EncyclopaediaWnd::EncyclopaediaWnd(EncyclopaediaWnd const& rhs) : m3d::ui::Wnd(rhs)
{
    // RVA 0xD48D0 - fresh, empty containers (the shipped copy ctor copies nothing).
    m_subjectList = nullptr;
    m_wndDiz = nullptr;
    m_comboThemes = nullptr;
    m_wndModel = nullptr;
    m_bInitedOnce = 0;
    m_prevInfoObjId = -1;
    m_prevCapturingObjId = -1;
    m_prevCapturedObjId = -1;
}

EncyclopaediaWnd::~EncyclopaediaWnd()
{
    // RVA 0xD49B0 - the STL containers and m_aif release themselves.
    GameDataClear(false);
}

m3d::Object* EncyclopaediaWnd::Clone()
{
    // RVA 0xD4330
    return new EncyclopaediaWnd(*this);
}

m3d::Object* EncyclopaediaWnd::CreateObject()
{
    // RVA 0xD4590
    return new EncyclopaediaWnd;
}

m3d::Class* EncyclopaediaWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* EncyclopaediaWnd::GetClass() const
{
    // RVA 0xD4580
    return RT_CLASS_LOCAL(EncyclopaediaWnd);
}

int EncyclopaediaWnd::GameDataClear(bool beforeContinouosLevel)
{
    // RVA 0xD4AE0
    if (!beforeContinouosLevel)
    {
        m_prototypesByTheme.clear();
        m_visiblePrototypeIds.clear();
        m_clanBelongsVector.clear();
        m_clanBelongsSet.clear();
    }
    // m_relatedPrototypeIds is a one-time cache and is deliberately kept.
    m_prevInfoObjId = -1;
    m_prevCapturingObjId = -1;
    m_prevCapturedObjId = -1;
    return 1;
}

int EncyclopaediaWnd::GameDataSetup()
{
    // RVA 0xD4BC0
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        // --- theme combo box ---------------------------------------------
        m3d::Object* child = GetChildByName(m_aif.m_comboThemesName);
        if (child && child->IsKindOf(&m3d::ui::ComboBoxWnd::m_classComboBoxWnd))
        {
            m_comboThemes = static_cast<m3d::ui::ComboBoxWnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_comboThemesName + " is not found or incorrect type");
            res = 0;
        }

        // --- description text box --------------------------------------
        child = GetChildByName(m_aif.m_wndDizName);
        if (child && child->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            m_wndDiz = static_cast<m3d::ui::TextBoxWnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndDizName + " is not found or incorrect type");
            res = 0;
        }

        // --- context model panel -------------------------------------
        int modelOk = res;
        child = GetChildByName(m_aif.m_wndModelName);
        if (!child || !child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndModelName + " is not found or incorrect type");
            res = 0;
            modelOk = res;
        }
        else
        {
            m_wndModel = static_cast<ContextModelWnd*>(M3D_KERNEL->New("ContextModelWnd"));
            if (!m_wndModel)
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndModelName
                    + " - cannot find rtti class ContextModelWnd");
                res = 0;
                modelOk = res;
            }
            else if (!m_wndModel->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndModelName + " from pattern class");
                modelOk = 0;
            }
        }

        // --- subject list --------------------------------------------
        child = GetChildByName(m_aif.m_subjectListName);
        if (child && child->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            m_subjectList = static_cast<SubjectList*>(M3D_KERNEL->New("SubjectList"));
            if (m_subjectList)
            {
                if (m_subjectList->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
                {
                    if (modelOk)
                    {
                        MoveChildToFirstPosition(m_subjectList);
                        MoveChildToFirstPosition(m_comboThemes);
                        if (InitThemes() & modelOk)
                        {
                            m_gameDataFlags |= 1u;
                        }
                    }
                }
                else
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_subjectListName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_subjectListName
                    + " - cannot find rtti class SubjectList");
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: control " + m_aif.m_subjectListName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("EncyclopaediaWnd: error - fail to init because of a bad resource");
    return 0;
}

int EncyclopaediaWnd::InitThemes()
{
    // RVA 0xD5400
    if (!m_comboThemes)
    {
        return 0;
    }
    int res = 1;
    for (int i = 0; i < THEME_NUM_THEMES; ++i)
    {
        int const idx = m_comboThemes->AddItem(M3D_APP->GetStringByStringId0(kThemeNames[i]));
        if (idx == -1)
        {
            res = 0;
        }
        else
        {
            m_comboThemes->SetItemData(idx, i);
        }
    }
    if (m_comboThemes->GetCount())
    {
        m_comboThemes->SetCurSel(0);
    }
    return res;
}

int EncyclopaediaWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0xD5540
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    int const* d = static_cast<int const*>(data);
    switch (dataType)
    {
    case 51:  // '3'
        OnBelongMet(data);
        break;
    case 64:  // '@'
        OnPlayerVehicleChanged();
        break;
    case 69:  // 'E'
        OnGadgetChanged(data);
        break;
    case 72:  // 'H'
        if (data)
        {
            AddObj(d[13]);
        }
        break;
    case 73:  // 'I'
        if (data)
        {
            AddPrototype(d[13]);
        }
        break;
    case 76:  // 'L'
        OnPlayerVehicleDamaged(data);
        break;
    case 77:  // 'M'
        OnQuestItemsChanged();
        break;
    case 85:  // 'U'
        OnStartLevel(data);
        break;
    case 90:  // 'Z'
        OnNewFrameForce();
        break;
    default:
        break;
    }
    return 1;
}

int EncyclopaediaWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0xD5680
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id == 400000)
    {
        if (msg == 5)
        {
            OnThemeChanged();
            return 1;
        }
    }
    else if (id == 400001 && msg == 5)
    {
        OnSubjectChanged();
        return 1;
    }
    return 0;
}

void EncyclopaediaWnd::OnThemeChanged()
{
    // RVA 0xD5700 - the shipped build inlines SubjectList::SetUpForTheme(GetCurTheme()).
    if ((m_gameDataFlags & 1) != 0)
    {
        m_subjectList->SetUpForTheme(GetCurTheme());
    }
}

void EncyclopaediaWnd::OnSubjectChanged()
{
    // RVA 0xD5780
    if ((m_gameDataFlags & 1) != 0)
    {
        UpdateModel();
        UpdateDiz();
    }
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetCurTheme() const
{
    // RVA 0xD57A0
    if ((m_gameDataFlags & 1) == 0)
    {
        return THEME_NUM_THEMES;
    }
    int const sel = m_comboThemes->GetCurSel();
    if (sel == -1)
    {
        return THEME_NUM_THEMES;
    }
    int const themeData = m_comboThemes->GetItemData(sel);
    if (static_cast<unsigned int>(themeData) >= THEME_NUM_THEMES)
    {
        return THEME_NUM_THEMES;
    }
    return static_cast<Theme>(themeData);
}

CStr EncyclopaediaWnd::GetCurSubjectName() const
{
    // RVA 0xD57E0
    if ((m_gameDataFlags & 1) == 0)
    {
        return CStr{};
    }
    return m_subjectList->GetCurSubjectName();
}

int EncyclopaediaWnd::UpdateModel()
{
    // RVA 0xD5830
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    // NOTE: the shipped build calls the protected DestroyObjToShow() +
    // ClearCurrentModelWnd() pair; HideImage() is the public equivalent.
    m_wndModel->HideImage();

    int theme = THEME_NUM_THEMES;
    int const sel = m_comboThemes->GetCurSel();
    if (sel != -1)
    {
        int const themeData = m_comboThemes->GetItemData(sel);
        if (themeData >= 0 && themeData < THEME_NUM_THEMES)
        {
            theme = themeData;
        }
    }

    CStr const subject = GetCurSubjectName();
    if (subject.empty())
    {
        return 1;
    }

    int ok = 1;
    switch (theme)
    {
    case THEME_VEHICLES:
    case THEME_CB:
    case THEME_WEAPON:
    case THEME_GADGETS:
    case THEME_WARES:
    case THEME_QUEST_ITEMS:
        ok = m_wndModel->ShowImageByPrototypeId(ai::thePrototypeManager->GetPrototypeId(subject));
        break;
    case THEME_AFFIXES:
        ok = m_wndModel->Show2dImage(GetImageForAffix(subject));
        break;
    case THEME_CLANS:
        ok = m_wndModel->Show2dImage(GetImageForClan(subject));
        break;
    case THEME_NUM_THEMES:
        break;
    default:
        ok = 0;
        break;
    }
    return ok;
}

int EncyclopaediaWnd::UpdateDiz()
{
    // RVA 0xD59A0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    CStr const subject = GetCurSubjectName();
    CStr diz;
    if (!subject.empty())
    {
        diz = M3D_APP->GetStringByStringId0(subject + CStr("_diz"));
    }
    m_wndDiz->SetText(diz);
    return 1;
}

CStr EncyclopaediaWnd::Theme2Str(EncyclopaediaWnd::Theme theme) const
{
    // RVA 0xD5AC0
    if (theme >= THEME_NUM_THEMES)
    {
        return CStr{};
    }
    return kThemeNames[theme];
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::Str2Theme(CStr const& themeName) const
{
    // RVA 0xD5AF0
    for (int i = 0; i < THEME_NUM_THEMES; ++i)
    {
        if (kThemeNames[i] == themeName)
        {
            return static_cast<Theme>(i);
        }
    }
    return THEME_NUM_THEMES;
}

m3d::rend::TexHandle EncyclopaediaWnd::GetImageForAffix(CStr const& affixName) const
{
    // RVA 0xD5B70
    return M3D_APP->m_pInterfaceManager->GetIcoByName(affixName, 0);
}

m3d::rend::TexHandle EncyclopaediaWnd::GetImageForClan(CStr const& clanName) const
{
    // RVA 0xD5B90
    return M3D_APP->m_pInterfaceManager->GetIcoByName(clanName, 1);
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetThemeByResourceId(int resourceId) const
{
    // RVA 0xD5BB0
    ai::ResourceManager* rm = ai::theResourceManager;
    if (rm->bResourceIsKindOf(resourceId, rm->GetResourceId("VEHICLE")))
    {
        return THEME_VEHICLES;
    }
    if (rm->bResourceIsKindOf(resourceId, rm->GetResourceId("CABIN"))
        || rm->bResourceIsKindOf(resourceId, rm->GetResourceId("BASKET")))
    {
        return THEME_CB;
    }
    if (rm->bResourceIsKindOf(resourceId, rm->GetResourceId("GUN")))
    {
        return THEME_WEAPON;
    }
    if (rm->bResourceIsKindOf(resourceId, rm->GetResourceId("GADGET")))
    {
        return THEME_GADGETS;
    }
    if (rm->bResourceIsKindOf(resourceId, rm->GetResourceId("GOODS")))
    {
        return THEME_WARES;
    }
    if (rm->bResourceIsKindOf(resourceId, rm->GetResourceId("QUEST_ITEM")))
    {
        return THEME_QUEST_ITEMS;
    }
    return THEME_NUM_THEMES;
}

EncyclopaediaWnd::Theme EncyclopaediaWnd::GetThemeByPrototypeId(int prototypeId) const
{
    // RVA 0xD6AA0
    ai::PrototypeInfo const* pi = ai::thePrototypeManager->GetPrototypeInfo(prototypeId);
    return pi ? GetThemeByResourceId(pi->m_resourceId) : THEME_NUM_THEMES;
}

std::vector<int> const* EncyclopaediaWnd::GetPrototypesForTheme(EncyclopaediaWnd::Theme theme) const
{
    // RVA 0xD5E10
    auto const it = m_prototypesByTheme.find(theme);
    return it != m_prototypesByTheme.end() ? &it->second : nullptr;
}

int EncyclopaediaWnd::GetRelatedVisiblePrototypeId(int unvisiblePrototypeId) const
{
    // RVA 0xD6E40
    auto const it = m_relatedPrototypeIds.find(unvisiblePrototypeId);
    return it != m_relatedPrototypeIds.end() ? it->second : -1;
}

bool EncyclopaediaWnd::IsPrototypeVisible(int prototypeId) const
{
    // RVA 0xD6A70
    return m_visiblePrototypeIds.find(prototypeId) != m_visiblePrototypeIds.end();
}

bool EncyclopaediaWnd::IsClanVisible(int clanBelong) const
{
    // RVA 0xD7650
    return m_clanBelongsSet.find(clanBelong) != m_clanBelongsSet.end();
}

std::vector<int> const* EncyclopaediaWnd::GetClans() const
{
    // RVA 0xD7680
    return &m_clanBelongsVector;
}

int EncyclopaediaWnd::AddPrototypeByThemeUnsafe(int prototypeId, EncyclopaediaWnd::Theme theme)
{
    // RVA 0xD6AE0
    if (prototypeId == -1 || theme == THEME_NUM_THEMES)
    {
        return 0;
    }
    auto const it = m_prototypesByTheme.find(theme);
    if (it == m_prototypesByTheme.end())
    {
        m_prototypesByTheme[theme] = std::vector<int>{prototypeId};
    }
    else
    {
        it->second.push_back(prototypeId);
    }
    m_visiblePrototypeIds.insert(prototypeId);
    return 1;
}

int EncyclopaediaWnd::AddPrototype(int prototypeId)
{
    // RVA 0xD6950
    if (m_visiblePrototypeIds.find(prototypeId) != m_visiblePrototypeIds.end())
    {
        return 1;
    }

    ai::PrototypeInfo const* pi = ai::thePrototypeManager->GetPrototypeInfo(prototypeId);
    if (!pi)
    {
        return 0;
    }
    Theme const theme = GetThemeByResourceId(pi->m_resourceId);
    if (theme == THEME_NUM_THEMES)
    {
        return 0;
    }

    // The prototype the player actually met might be an internal variant; show
    // the visible sibling instead.
    int visibleId = prototypeId;
    if (!pi->m_bVisibleInEncyclopedia)
    {
        visibleId = GetRelatedVisiblePrototypeId(prototypeId);
    }
    if (visibleId == -1 || IsPrototypeVisible(visibleId))
    {
        return 1;
    }

    int const res = AddPrototypeByThemeUnsafe(visibleId, theme);
    if (IsChildOf(M3D_APP) && theme == GetCurTheme())
    {
        m_subjectList->Update();
    }
    return res;
}

int EncyclopaediaWnd::AddObj(int objId)
{
    // RVA 0xD6C30
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
    if (!obj || !obj->IsKindOf(&ai::Obj::m_classObj))
    {
        return 0;
    }
    if (obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        return AddVehicle(static_cast<ai::Vehicle*>(obj));
    }
    return AddPrototype(obj->GetPrototypeId());
}

int EncyclopaediaWnd::AddVehicle(ai::Vehicle const* vehicle)
{
    // RVA 0xD6CA0
    if (!vehicle)
    {
        return 0;
    }

    int res = AddPrototype(vehicle->GetPrototypeId()) & 1;

    // Every attached part except the chassis.
    auto* mutableVehicle = const_cast<ai::Vehicle*>(vehicle);
    for (CStr const& partName : mutableVehicle->GetAttachedPartNames())
    {
        if (partName == CStr("CHASSIS"))
        {
            continue;
        }
        ai::VehiclePart* part = mutableVehicle->GetPartByName(partName);
        if (part)
        {
            res &= AddPrototype(part->GetPrototypeId());
        }
        else
        {
            res = 0;
        }
    }

    // Every mounted gadget.
    for (auto const& gadgetSlot : mutableVehicle->GetGadgets())
    {
        ai::Gadget* gadget = gadgetSlot.second;
        if (gadget)
        {
            res &= AddPrototype(gadget->GetPrototypeId());
        }
        else
        {
            res = 0;
        }
    }

    AddClan(vehicle->GetBelong());
    return res;
}

int EncyclopaediaWnd::AddClan(int clanBelong)
{
    // RVA 0xD7500
    if (m_clanBelongsSet.find(clanBelong) == m_clanBelongsSet.end())
    {
        if (ai::theGlobProp.m_namedBelongIds.find(clanBelong) == ai::theGlobProp.m_namedBelongIds.end())
        {
            return 0;
        }
        m_clanBelongsVector.push_back(clanBelong);
        m_clanBelongsSet.insert(clanBelong);
        if (IsChildOf(M3D_APP) && GetCurTheme() == THEME_CLANS)
        {
            m_subjectList->Update();
        }
        M3D_APP->EnqueueMessage(65687, 0, 0, 0, 0, {}, {});
    }
    return 1;
}

void EncyclopaediaWnd::OnPrototypeMet(void* data)
{
    // RVA 0xD6910
    if (data)
    {
        AddPrototype(static_cast<int*>(data)[13]);
    }
}

void EncyclopaediaWnd::OnObjMet(void* data)
{
    // RVA 0xD6920
    if (data)
    {
        AddObj(static_cast<int*>(data)[13]);
    }
}

void EncyclopaediaWnd::OnBelongMet(void* data)
{
    // RVA 0xD7850
    if (data)
    {
        AddClan(static_cast<int*>(data)[13]);
    }
}

void EncyclopaediaWnd::OnPlayerVehicleDamaged(void* data)
{
    // RVA 0xD6930
    if (data)
    {
        int const prototypeId = static_cast<int*>(data)[14];
        if (prototypeId != -1)
        {
            AddPrototype(prototypeId);
        }
    }
}

void EncyclopaediaWnd::OnStartLevel(void*)
{
    // RVA 0xD6E00
    if (!m_bInitedOnce)
    {
        CacheRelatedPrototypes();
        InitAlwaysVisiblePrototypes();
        m_bInitedOnce = true;
    }
}

int EncyclopaediaWnd::InitOnce()
{
    // RVA 0xD6E20
    // NOTE: unlike OnStartLevel, the shipped build does not set m_bInitedOnce here.
    if (!m_bInitedOnce)
    {
        CacheRelatedPrototypes();
        InitAlwaysVisiblePrototypes();
    }
    return 1;
}

void EncyclopaediaWnd::OnPlayerVehicleChanged()
{
    // RVA 0xD7290
    if (ai::thePlayer)
    {
        if (ai::Vehicle* vehicle = ai::thePlayer->GetVehicle())
        {
            AddVehicle(vehicle);
        }
    }
}

void EncyclopaediaWnd::OnGadgetChanged(void* data)
{
    // RVA 0xD72C0
    if (!data || !ai::thePlayer)
    {
        return;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return;
    }
    int const* d = static_cast<int const*>(data);
    if (vehicle->GetId() != d[13])
    {
        return;
    }
    auto const& gadgets = vehicle->GetGadgets();
    auto const it = gadgets.find(d[14]);
    if (it != gadgets.end() && it->second)
    {
        AddPrototype(it->second->GetPrototypeId());
    }
}

void EncyclopaediaWnd::OnQuestItemsChanged()
{
    // RVA 0xD7320
    if (!ai::thePlayer)
    {
        return;
    }
    for (CStr const& name : ai::thePlayer->GetQuestItemPrototypeNames())
    {
        int const prototypeId = ai::thePrototypeManager->GetPrototypeId(name);
        if (prototypeId != -1)
        {
            AddPrototype(prototypeId);
        }
    }
}

void EncyclopaediaWnd::OnNewFrameForce()
{
    // RVA 0xD7690
    CheckAndAddTargetInfoVehicle();
    CheckAndAddTargetCapturingVehicle();
    CheckAndAddTargetCapturedVehicle();
}

void EncyclopaediaWnd::CheckAndAddTargetInfoVehicle()
{
    // RVA 0xD76B0
    ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    if (!vehicle)
    {
        m_prevInfoObjId = -1;
        return;
    }
    int const objId = vehicle->GetInfoObjId();
    if (objId == -1 || objId == m_prevInfoObjId)
    {
        m_prevInfoObjId = objId;
        return;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
    if (obj && obj->IsAlive() && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        AddVehicle(static_cast<ai::Vehicle*>(obj));
    }
    m_prevInfoObjId = objId;
}

void EncyclopaediaWnd::CheckAndAddTargetCapturingVehicle()
{
    // RVA 0xD7730
    ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    if (!vehicle)
    {
        m_prevCapturingObjId = -1;
        return;
    }
    int const objId = vehicle->GetToBeLockedObjId();
    if (objId == -1 || objId == m_prevCapturingObjId)
    {
        m_prevCapturingObjId = objId;
        return;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
    if (obj && obj->IsKindOf(&ai::Obj::m_classObj) && obj->IsAlive() && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        AddVehicle(static_cast<ai::Vehicle*>(obj));
    }
    m_prevCapturingObjId = objId;
}

void EncyclopaediaWnd::CheckAndAddTargetCapturedVehicle()
{
    // RVA 0xD77C0
    ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    if (!vehicle)
    {
        m_prevCapturedObjId = -1;
        return;
    }
    int const objId = vehicle->GetLockedObjId();
    if (objId == -1 || objId == m_prevCapturedObjId)
    {
        m_prevCapturedObjId = objId;
        return;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
    if (obj && obj->IsKindOf(&ai::Obj::m_classObj) && obj->IsAlive() && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        AddVehicle(static_cast<ai::Vehicle*>(obj));
    }
    m_prevCapturedObjId = objId;
}

void EncyclopaediaWnd::GetPrototypeIdsKindOfResource(int resourceId, std::vector<int>& prototypeIds) const
{
    // RVA 0xD71A0
    prototypeIds.clear();
    int const count = ai::thePrototypeManager->GetNumOfPrototypes();
    for (int i = 0; i < count; ++i)
    {
        ai::PrototypeInfo const* pi = ai::thePrototypeManager->GetPrototypeInfo(i);
        if (pi && ai::theResourceManager->bResourceIsKindOf(pi->m_resourceId, resourceId))
        {
            prototypeIds.push_back(i);
        }
    }
}

void EncyclopaediaWnd::CacheRelatedPrototypes()
{
    // RVA 0xD6E70 - for every vehicle prototype that is not itself shown in the
    // encyclopaedia, remember the visible sibling that shares its parent.
    m_relatedPrototypeIds.clear();

    std::vector<int> vehiclePrototypeIds;
    GetPrototypeIdsKindOfResource(ai::theResourceManager->GetResourceId("VEHICLE"), vehiclePrototypeIds);

    std::map<CStr, int> visibleByParent;
    for (int pid : vehiclePrototypeIds)
    {
        ai::PrototypeInfo const* pi = ai::thePrototypeManager->GetPrototypeInfo(pid);
        if (!pi)
        {
            continue;
        }
        if (!pi->bIsAbstract() && pi->m_bVisibleInEncyclopedia)
        {
            if (visibleByParent.find(pi->GetParentPrototypeName()) == visibleByParent.end())
            {
                visibleByParent[pi->GetParentPrototypeName()] = pid;
            }
        }
    }

    for (int pid : vehiclePrototypeIds)
    {
        ai::PrototypeInfo const* pi = ai::thePrototypeManager->GetPrototypeInfo(pid);
        if (!pi || pi->m_bVisibleInEncyclopedia)
        {
            continue;
        }
        CStr const& key = pi->bIsAbstract() ? pi->m_prototypeName : pi->GetParentPrototypeName();
        auto const it = visibleByParent.find(key);
        if (it != visibleByParent.end())
        {
            m_relatedPrototypeIds[pid] = it->second;
        }
    }
}

void EncyclopaediaWnd::InitAlwaysVisiblePrototypes()
{
    // RVA 0xD73D0 - "GOODS" prototypes are always in the encyclopaedia.
    std::vector<int> visibleResources;
    visibleResources.push_back(ai::theResourceManager->GetResourceId("GOODS"));

    int const count = ai::thePrototypeManager->GetNumOfPrototypes();
    for (int i = 0; i < count; ++i)
    {
        ai::PrototypeInfo const* pi = ai::thePrototypeManager->GetPrototypeInfo(i);
        if (!pi)
        {
            continue;
        }
        for (int resId : visibleResources)
        {
            if (ai::theResourceManager->bResourceIsKindOf(pi->m_resourceId, resId))
            {
                AddPrototype(i);
                break;
            }
        }
    }
}

void EncyclopaediaWnd::ShowAll()
{
    // RVA 0xD7870
    int const count = ai::thePrototypeManager->GetNumOfPrototypes();
    for (int i = 0; i < count; ++i)
    {
        AddPrototype(i);
    }
    for (int belong : ai::theGlobProp.m_namedBelongIds)
    {
        AddClan(belong);
    }
}

int EncyclopaediaWnd::OnAfterAddToWndStation()
{
    // RVA 0xD7910
    int const r = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return r;
}

int EncyclopaediaWnd::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0xD5E40
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("EncyclopaediaWnd::GameDataSave error - encyclopaedia has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("EncyclopaediaWnd::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr encNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Encyclopaedia");
    guiNode->AddChild(encNode);

    ref_ptr prototypesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Prototypes");
    encNode->AddChild(prototypesNode);

    for (auto const& themeEntry : m_prototypesByTheme)
    {
        ref_ptr themeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Theme");
        prototypesNode->AddChild(themeNode);
        themeNode->SetAttribute("ThemeName", Theme2Str(themeEntry.first).c_str());

        retruxx::vector<CStr> prototypeNames;
        for (int pid : themeEntry.second)
        {
            prototypeNames.push_back(ai::thePrototypeManager->GetPrototypeName(pid));
        }
        themeNode->SetAttribute("PrototypeNames", ai::StringVectorToStr(prototypeNames).c_str());
    }

    ref_ptr clansNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Clans");
    encNode->AddChild(clansNode);
    retruxx::vector<int> clanBelongs(m_clanBelongsVector.begin(), m_clanBelongsVector.end());
    clansNode->SetAttribute("Belongs", ai::IntVectorToStr(clanBelongs).c_str());
    return 1;
}

int EncyclopaediaWnd::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0xD62B0
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("EncyclopaediaWnd::GameDataLoad error - encyclopaedia has been not properly inited");
        return 0;
    }
    GameDataClear(false);
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("EncyclopaediaWnd::GameDataLoad error - invalid params");
        return 0;
    }

    // NOTE: the shipped build walks an intermediate wrapper node ("journal node");
    // GameDataSave adds the Encyclopaedia node straight under guiNode, so this
    // reads it back the same way.
    ref_ptr encNode = xmlFile->CreateNode();
    guiNode->GetFirstChild(encNode, "Encyclopaedia");
    if (encNode->IsEmpty())
    {
        M3D_LOG_INFO("EncyclopaediaWnd::GameDataLoad error - cannot find journal node");
        return 0;
    }

    int res = 1;

    ref_ptr prototypesNode = xmlFile->CreateNode();
    encNode->GetFirstChild(prototypesNode, "Prototypes");
    if (!prototypesNode->IsEmpty())
    {
        ref_ptr themeNode = xmlFile->CreateNode();
        prototypesNode->GetFirstChild(themeNode, "Theme");
        while (!themeNode->IsEmpty())
        {
            CStr themeName;
            m3d::SafeStrAttrib(themeName, themeNode, "ThemeName");
            if (Str2Theme(themeName) == THEME_NUM_THEMES)
            {
                M3D_LOG_INFO("EncyclopaediaWnd::GameDataLoad error - invalid theme " + themeName);
                res = 0;
            }
            else
            {
                CStr strPrototypeNames;
                m3d::SafeStrAttrib(strPrototypeNames, themeNode, "PrototypeNames");
                retruxx::vector<CStr> prototypeNames;
                ai::StrToStringVector(strPrototypeNames, prototypeNames);
                for (CStr const& name : prototypeNames)
                {
                    int const prototypeId = ai::thePrototypeManager->GetPrototypeId(name);
                    if (prototypeId == -1)
                    {
                        M3D_LOG_INFO("EncyclopaediaWnd::GameDataLoad error - invalid prototype " + name);
                        res = 0;
                    }
                    else
                    {
                        res &= AddPrototype(prototypeId);
                    }
                }
            }
            themeNode->GetNextSibling(themeNode, "Theme");
        }
    }

    ref_ptr clansNode = xmlFile->CreateNode();
    encNode->GetFirstChild(clansNode, "Clans");
    if (!clansNode->IsEmpty())
    {
        CStr strClanBelongs;
        m3d::SafeStrAttrib(strClanBelongs, clansNode, "Belongs");
        retruxx::vector<int> belongs;
        ai::StrToIntVector(strClanBelongs, belongs);
        for (int belong : belongs)
        {
            m_clanBelongsVector.push_back(belong);
            m_clanBelongsSet.insert(belong);
        }
    }

    if (!res)
    {
        M3D_LOG_INFO("EncyclopaediaWnd was loaded with errors");
    }
    return res;
}

// ============================================================================
//  SubjectButton
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(SubjectButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SubjectButton);

SubjectButton::SubjectButton()
{
    // RVA 0xD7A10
    m_theme = EncyclopaediaWnd::THEME_NUM_THEMES;
    m_id = -1;
}

SubjectButton::SubjectButton(SubjectButton const& rhs) : CheckButton(rhs)
{
    // NOTE: the shipped copy ctor leaves m_theme / m_id uninitialised.
    m_theme = EncyclopaediaWnd::THEME_NUM_THEMES;
    m_id = -1;
}

SubjectButton::~SubjectButton()
{
    // Base destructor releases the owned children.
}

m3d::Object* SubjectButton::Clone()
{
    // RVA 0xD4360
    return new SubjectButton(*this);
}

m3d::Object* SubjectButton::CreateObject()
{
    // RVA 0xD79D0
    return new SubjectButton;
}

m3d::Class* SubjectButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CheckButton);
}

m3d::Class* SubjectButton::GetClass() const
{
    // RVA 0xD79C0
    return RT_CLASS_LOCAL(SubjectButton);
}

int SubjectButton::SetUp(CStr const& name, PointBase<float> const& origin, float width, CStr const& fullName)
{
    // RVA 0xD7C80
    return CheckButton::SetUp(name, origin, width, fullName);
}

int SubjectButton::SetUp(
    CStr const& name, PointBase<float> const& origin, float width, EncyclopaediaWnd::Theme theme, int id)
{
    // RVA 0xD7C10
    m_id = id;
    m_theme = theme;
    return SetUp(name, origin, width, CStr{}) ? 1 : 0;
}

CStr SubjectButton::CalcFullName() const
{
    // RVA 0xD7AA0 - resolve the localized display name for the subject.
    CStr fullName;
    switch (m_theme)
    {
    case EncyclopaediaWnd::THEME_VEHICLES:
    case EncyclopaediaWnd::THEME_CB:
    case EncyclopaediaWnd::THEME_WEAPON:
    case EncyclopaediaWnd::THEME_GADGETS:
    case EncyclopaediaWnd::THEME_WARES:
    case EncyclopaediaWnd::THEME_QUEST_ITEMS:
        fullName = ai::thePrototypeManager->GetPrototypeFullName(m_name);
        break;
    case EncyclopaediaWnd::THEME_AFFIXES:
        if (ai::Affix* affix = ai::pServer->GetAffixManager()->GetAffixById(m_id))
        {
            fullName = affix->GetLocalizedName(0);
        }
        break;
    case EncyclopaediaWnd::THEME_CLANS:
        fullName = M3D_APP->GetStringByStringId0(m_name);
        break;
    default:
        break;
    }

    // NOTE: the shipped build uppercases the first character with a locale-aware
    // LCMapStringA; a plain toupper is used here.
    if (!fullName.empty())
    {
        char* buf = const_cast<char*>(fullName.c_str());
        buf[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(buf[0])));
    }
    return fullName;
}

// ============================================================================
//  SubjectList
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(SubjectList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SubjectList);

SubjectList::SubjectList()
{
    // RVA 0xD7CE0 - NOTE: the shipped ctor leaves m_theme uninitialised.
    m_theme = EncyclopaediaWnd::THEME_NUM_THEMES;
}

SubjectList::SubjectList(SubjectList const& rhs) : CheckList(rhs)
{
    // RVA 0xD7D90 - fresh, empty list.
    m_theme = EncyclopaediaWnd::THEME_NUM_THEMES;
}

SubjectList::~SubjectList()
{
    // Items are owned children, released by the base destructor.
}

m3d::Object* SubjectList::Clone()
{
    // RVA 0xD4390
    return new SubjectList(*this);
}

m3d::Object* SubjectList::CreateObject()
{
    // RVA 0xD7CB0
    return new SubjectList;
}

m3d::Class* SubjectList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CheckList);
}

m3d::Class* SubjectList::GetClass() const
{
    // RVA 0xD7CA0
    return RT_CLASS_LOCAL(SubjectList);
}

int SubjectList::AddButtonByName(CStr const& name, CStr const&)
{
    // RVA 0xD7E30
    return AddButtonByNameAndId(name, -1);
}

int SubjectList::AddButtonByNameAndId(CStr const& name, int id)
{
    // RVA 0xD7E40
    if (name.empty())
    {
        return -1;
    }
    auto* btn = static_cast<SubjectButton*>(M3D_KERNEL->New("SubjectButton"));
    if (!btn)
    {
        return -1;
    }

    // NOTE: the shipped build feeds the client bounds through oddly - the button
    // origin becomes (clientBounds.y0, clientBounds.width) and its "width" is
    // clientBounds.height. RenderItem re-positions every row anyway.
    BoundsBase<float> const clientB = GetClientBounds();
    PointBase<float> const origin{clientB.y0, clientB.width};
    if (!btn->SetUp(name, origin, clientB.height, m_theme, id))
    {
        delete btn;
        return -1;
    }
    AddChild(btn);
    return AddItem(btn);
}

int SubjectList::SetUpForTheme(EncyclopaediaWnd::Theme theme)
{
    // RVA 0xD7F00
    if (m_theme == theme)
    {
        return 1;
    }
    m_theme = theme;
    if (!IsChildOf(M3D_APP))
    {
        return 1;
    }
    return Update();
}

int SubjectList::Update()
{
    // RVA 0xD7F40
    int res = 1;
    RemoveAllItems();
    switch (m_theme)
    {
    case EncyclopaediaWnd::THEME_VEHICLES:
    case EncyclopaediaWnd::THEME_CB:
    case EncyclopaediaWnd::THEME_WEAPON:
    case EncyclopaediaWnd::THEME_GADGETS:
    case EncyclopaediaWnd::THEME_WARES:
    case EncyclopaediaWnd::THEME_QUEST_ITEMS:
        res = CreateItemsForPrototypes(m_theme);
        break;
    case EncyclopaediaWnd::THEME_AFFIXES:
        res = CreateItemsForAffixes();
        break;
    case EncyclopaediaWnd::THEME_CLANS:
        res = CreateItemsForClans();
        break;
    case EncyclopaediaWnd::THEME_NUM_THEMES:
        break;
    default:
        res = 0;
        break;
    }
    SetCurSel(m_items.empty() ? -1 : 0);
    return res;
}

int SubjectList::CreateItemsForPrototypes(EncyclopaediaWnd::Theme theme)
{
    // RVA 0xD7FF0
    ref_ptr<m3d::ui::Wnd> encWnd = M3D_APP->m_pInterfaceManager->GetWindow(97);
    if (!encWnd)
    {
        return 0;
    }
    auto* enc = RT_DYNCAST(encWnd.get(), EncyclopaediaWnd);
    if (!enc)
    {
        return 0;
    }

    int res = 1;
    if (std::vector<int> const* ids = enc->GetPrototypesForTheme(theme))
    {
        for (int prototypeId : *ids)
        {
            ai::PrototypeInfo const* pi = ai::thePrototypeManager->GetPrototypeInfo(prototypeId);
            if (pi)
            {
                res &= AddButtonByName(pi->m_prototypeName, CStr{});
            }
        }
    }
    return res;
}

int SubjectList::CreateItemsForAffixes()
{
    // RVA 0xD8170
    int res = 1;

    retruxx::vector<CStr> affixNames;
    int const numAffixes = ai::pServer->GetAffixManager()->GetNumAffixes();
    for (int i = 0; i < numAffixes; ++i)
    {
        ai::Affix* affix = ai::pServer->GetAffixManager()->GetAffixById(i);
        if (affix)
        {
            bool seen = false;
            for (CStr const& known : affixNames)
            {
                if (known == affix->GetName())
                {
                    seen = true;
                    break;
                }
            }
            if (!seen)
            {
                affixNames.push_back(affix->GetName());
            }
        }
    }

    for (size_t i = 0; i < affixNames.size(); ++i)
    {
        res &= AddButtonByNameAndId(affixNames[i], static_cast<int>(i));
    }
    return res;
}

int SubjectList::CreateItemsForClans()
{
    // RVA 0xD8280
    ref_ptr<m3d::ui::Wnd> encWnd = M3D_APP->m_pInterfaceManager->GetWindow(97);
    if (!encWnd)
    {
        return 0;
    }
    auto* enc = RT_DYNCAST(encWnd.get(), EncyclopaediaWnd);
    if (!enc)
    {
        return 0;
    }

    int res = 1;
    retruxx::vector<CStr> clanNames;
    for (int belong : *enc->GetClans())
    {
        clanNames.push_back(help::GetClanNameByBelong(belong));
    }
    for (CStr const& clanName : clanNames)
    {
        res &= AddButtonByName(clanName, CStr{});
    }
    return res;
}

CStr SubjectList::GetCurSubjectName() const
{
    // RVA 0xD8480
    if (m_curSel == -1)
    {
        return CStr{};
    }
    CheckButton* item = m_items[m_curSel].m_item;
    if (!item)
    {
        return CStr{};
    }
    return item->GetName();
}

int SubjectList::OnBeforeAddToWndStation()
{
    // RVA 0xD84C0
    Update();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

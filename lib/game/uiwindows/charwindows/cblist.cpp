#include "cblist.h"
#include "repositoryitemwnd.h"

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/gameuimanager.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/geomrepository.h>
#include <server/geomrepositoryitem.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypeinfo.h>
#include <server/objects/basket.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <server/objects/town.h>
#include <server/objects/workshop.h>
#include <server/resourcemanager.h>
#include <server/server.h>
#include <ui/button.h>
#include <ui/image.h>

RT_CLASS_EXPORTS_BEGIN(CBButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBButton);

RT_CLASS_EXPORTS_BEGIN(CBList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBList);

CBButton::AuxInfo CBButton::m_aif;
ref_ptr<m3d::ui::Wnd> CBButton::m_wndPattern[2];

namespace
{
    // Ids the XML gives the shared widgets; the list and the buttons talk to each
    // other through these rather than by pointer.
    unsigned const ID_BTN_NEXT = 300600;
    unsigned const ID_BTN_PREV = 300601;
    unsigned const ID_CB_BUTTON = 300602;
    unsigned const ID_BTN_BUY = 300650;

    // CallParentNotify message the buy button raises on its owning list.
    unsigned const MSG_BUY_CB = 21;

    // Interface-manager window ids of the two XML button patterns.
    int const WND_ID_CABIN_BUTTON_PATTERN = 131;

    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window; the shipped CBButton pattern-copy
    // inlines this block once per widget kind.
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
//  CBButton::AuxInfo
// ===========================================================================

CBButton::AuxInfo::AuxInfo()
{
    // RVA 0x43BDD0
    m_wndNameName = "wndName";
    m_btnBuyName = "btnBuy";
    m_wndPictureName = "wndPicture";
    m_wndDurabilityName = "wndDurabilityVal";
    m_wndResistPiercingName = "wndResistPiercingVal";
    m_wndResistBlastName = "wndResistBlastVal";
    m_wndResistEnergyName = "wndResistEnergyVal";
    m_wndWeightName = "wndWeightVal";
    m_wndPriceName = "wndPrice";
    m_selPaneName = "PaneCBButtonSel";
}

CBButton::AuxInfo::AuxInfo(CBButton::AuxInfo const& rhs) :
    m_wndNameName(rhs.m_wndNameName),
    m_btnBuyName(rhs.m_btnBuyName),
    m_wndPictureName(rhs.m_wndPictureName),
    m_wndDurabilityName(rhs.m_wndDurabilityName),
    m_wndResistPiercingName(rhs.m_wndResistPiercingName),
    m_wndResistBlastName(rhs.m_wndResistBlastName),
    m_wndResistEnergyName(rhs.m_wndResistEnergyName),
    m_wndWeightName(rhs.m_wndWeightName),
    m_wndPriceName(rhs.m_wndPriceName),
    m_selPaneName(rhs.m_selPaneName)
{
}

// ===========================================================================
//  CBButton
// ===========================================================================

CBButton::CBButton() :
    m_cbId(-1),
    m_wndName(nullptr),
    m_btnBuy(nullptr),
    m_wndDurability(nullptr),
    m_wndResistPiercing(nullptr),
    m_wndResistBlast(nullptr),
    m_wndResistEnergy(nullptr),
    // NOTE: the shipped constructor (RVA 0x43C050) writes every other pointer but
    // skips m_wndWeight, leaving it uninitialised until CreateChildren() assigns
    // it. Initialised here so nothing can read garbage on a failed setup.
    m_wndWeight(nullptr),
    m_wndPrice(nullptr),
    m_type(NUM_TYPES),
    m_bSelected(false)
{
}

CBButton::CBButton(CBButton const&) : CBButton()
{
    // NOTE: the shipped copy ctor (RVA 0x43C130) builds a default Wnd and nulls
    // only m_wndPicture, leaving every other member uninitialised, and copies
    // nothing from rhs. Delegating avoids reading that garbage while copying
    // just as little.
}

CBButton::~CBButton() = default;

m3d::Object* CBButton::Clone()
{
    // RVA 0x43BC20
    return new CBButton(*this);
}

m3d::Object* CBButton::CreateObject()
{
    return new CBButton;
}

m3d::Class* CBButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* CBButton::GetClass() const
{
    return RT_CLASS_LOCAL(CBButton);
}

int CBButton::GetCBId() const
{
    // RVA 0x43DB20
    return m_cbId;
}

bool CBButton::IsSelected() const
{
    // Declared in the PDB; fully inlined in the shipped build.
    return m_bSelected;
}

int CBButton::GetPrice() const
{
    // RVA 0x43DEB0 - what the current town's workshop charges for this object.
    ai::Town const* town = M3D_APP->m_pInterfaceManager->GetCurrentTown();
    if (!town)
    {
        return 0;
    }

    int const price = help::GetBuyPriceByObjId(m_cbId, town->GetId());
    return price < 0 ? 0 : price;
}

void CBButton::Select(bool bSelect)
{
    // RVA 0x43DEF0 - selection is purely a pane swap; deselecting restores the
    // pane the XML pattern came with.
    m_bSelected = bSelect;
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (bSelect)
    {
        SetPane(m_aif.m_selPaneName);
    }
    else
    {
        SetPane(m_wndPattern[m_type]->GetPaneName());
    }
}

int CBButton::SetupForCB(int cbId)
{
    // RVA 0x43DAB0
    if (Valid())
    {
        return 0;
    }

    ai::Obj* cb = ai::theObjects->GetEntityByObjId(cbId);
    if (!cb || !cb->IsKindOf(&ai::Obj::m_classObj) || !CreateFromPattern())
    {
        return 0;
    }

    m_cbId = cbId;
    return FullUpdate();
}

int CBButton::LoadPattern(ref_ptr<m3d::ui::Wnd> pattern, Type type)
{
    // RVA 0x43C180 - the first list of a kind to come up wins; later ones just
    // reuse the pattern it stored.
    if (!pattern || type == NUM_TYPES)
    {
        M3D_LOG_INFO("CBButton::LoadPattern - error to create - invalid params");
        return 0;
    }

    if (m_wndPattern[type])
    {
        return 1;
    }
    m_wndPattern[type] = pattern;
    return 1;
}

void CBButton::ClearPattern(Type type)
{
    // RVA 0x43C270
    if (!m_wndPattern[type])
    {
        return;
    }

    if (m_wndPattern[type]->GetRefCount() == 1)
    {
        m_wndPattern[type] = nullptr;
        return;
    }

    // NOTE: when the pattern is still referenced elsewhere the shipped code only
    // drops this reference without clearing the slot, so m_wndPattern[type] keeps
    // pointing at a window it no longer holds a reference to; preserved as-is.
    m_wndPattern[type]->DecRef();
}

int CBButton::CreateFromPattern()
{
    // RVA 0x43C2F0 - builds this button as a copy of the shared XML pattern.
    if (Valid())
    {
        return 0;
    }
    if (m_type == NUM_TYPES)
    {
        return 0;
    }

    m3d::ui::Wnd* pattern = m_wndPattern[m_type].get();
    if (!pattern || !M3D_APP->IsWndAlive(pattern, -1) || !pattern->Valid())
    {
        M3D_LOG_INFO("CBButton::CreateFromPattern error - null pattern");
        return 0;
    }

    if (!Create(CStr(), pattern->GetStyle(), pattern->GetBounds(), pattern->GetId()))
    {
        M3D_LOG_INFO("CBButton::CreateFromPattern error - cannot create window");
        return 0;
    }

    CopyWndPropsFromPattern(this, pattern);

    if (!CreateChildren())
    {
        M3D_LOG_INFO("CBButton::CreateFromPattern error - cannot create children");
        return 0;
    }

    m_gameDataFlags |= 1u;
    return 1;
}

int CBButton::CreateChildren()
{
    // RVA 0x43C850 - clones every child of the pattern, then picks the widgets
    // this button drives out of the copies by name.
    if (m_type == NUM_TYPES)
    {
        return 0;
    }

    m3d::ui::Wnd* pattern = m_wndPattern[m_type].get();
    for (m3d::Object* child = pattern->GetFirstChild(); child; child = child->GetNextSibling())
    {
        if (!child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            continue;
        }
        auto* patChild = static_cast<m3d::ui::Wnd*>(child);

        auto* copy = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New(patChild->GetClassNameA()));
        if (!copy ||
            !copy->Create(patChild->GetText(), patChild->GetStyle(), patChild->GetBounds(), patChild->GetId()))
        {
            continue;
        }

        CopyWndPropsFromPattern(copy, patChild);

        if (copy->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            auto* patBtn = static_cast<m3d::ui::ButtonWnd*>(patChild);
            auto* btn = static_cast<m3d::ui::ButtonWnd*>(copy);
            if (patBtn->IsImaged())
            {
                btn->SetImaged(patBtn->GetImageRegular(), patBtn->GetImageDown(), patBtn->GetImageIn(), {});
            }
            else
            {
                btn->SetRegular();
            }
        }
        else if (copy->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
        {
            static_cast<m3d::ui::ImageWnd*>(copy)->SetImage(static_cast<m3d::ui::ImageWnd*>(patChild)->GetImage());
        }

        AddChild(copy);
    }

    int res = 1;
    auto const find = [&](CStr const& name, m3d::ui::Wnd*& out)
    {
        m3d::Object* child = GetChildByName(name);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            out = static_cast<m3d::ui::Wnd*>(child);
            return;
        }
        M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
        res = 0;
    };

    find(m_aif.m_wndNameName, m_wndName);

    m3d::Object* btnBuy = GetChildByName(m_aif.m_btnBuyName);
    if (btnBuy && btnBuy->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
    {
        m_btnBuy = static_cast<m3d::ui::ButtonWnd*>(btnBuy);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_btnBuyName + " is not found or incorrect type");
        res = 0;
    }

    find(m_aif.m_wndDurabilityName, m_wndDurability);
    find(m_aif.m_wndResistPiercingName, m_wndResistPiercing);
    find(m_aif.m_wndResistBlastName, m_wndResistBlast);
    find(m_aif.m_wndResistEnergyName, m_wndResistEnergy);
    find(m_aif.m_wndWeightName, m_wndWeight);
    find(m_aif.m_wndPriceName, m_wndPrice);

    // The picture is not used as it comes: it is rebuilt as a RepositoryItemWnd
    // from the pattern child, and a failure here aborts outright rather than just
    // clearing res.
    m3d::Object* picture = GetChildByName(m_aif.m_wndPictureName);
    if (!picture || !picture->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO(
            "Make control error: control " + m_aif.m_wndPictureName + " is not found or incorrect type");
        return 0;
    }

    m_wndPicture = static_cast<RepositoryItemWnd*>(M3D_KERNEL->New("RepositoryItemWnd"));
    if (!m_wndPicture->CreateFromPattern(static_cast<m3d::ui::Wnd*>(picture), true))
    {
        M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndPictureName + " from pattern class");
        return 0;
    }

    return res;
}

int CBButton::FullUpdate()
{
    // RVA 0x43DBE0 - fills the row from the part the button stands for.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_cbId);
    if (!obj || !obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
    {
        return 0;
    }
    auto* part = static_cast<ai::VehiclePart*>(obj);

    m_wndName->SetText(part->GetFullDescriptionWithAffixes());
    m_wndDurability->SetText(CStr(static_cast<int>(part->Durability().maxValue().get())));
    m_wndResistPiercing->SetText(
        CStr(static_cast<int>(part->GetDurabilityCoeffForDamageType(ai::DAMAGE_PIERCING))));
    m_wndResistBlast->SetText(CStr(static_cast<int>(part->GetDurabilityCoeffForDamageType(ai::DAMAGE_BLAST))));
    m_wndResistEnergy->SetText(CStr(static_cast<int>(part->GetDurabilityCoeffForDamageType(ai::DAMAGE_ENERGY))));
    m_wndWeight->SetText(CStr(static_cast<int>(part->GetMass())));
    m_wndPrice->SetText(CStr(GetPrice()));

    ai::GeomRepository* repository = part->GetParentRepository();
    if (!repository)
    {
        return 0;
    }

    m_wndPicture->SetUpForRepositoryItem(repository->GetItem(repository->GetSlotByObjId(m_cbId)));
    return 1;
}

int CBButton::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x43DB30 - the buy button is answered by telling the owning list which
    // object was bought.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (idFrom != ID_BTN_BUY || message != 1)
    {
        return 0;
    }

    CallParentNotify(MSG_BUY_CB, m3d::AIParam(m_cbId), false);
    return 1;
}

// ===========================================================================
//  CBList::AuxInfo
// ===========================================================================

CBList::AuxInfo::AuxInfo()
{
    // RVA 0x43DFA0
    m_btnNextName = "btnNext";
    m_btnPrevName = "btnPrev";
    m_wndListBgName = "wndList";
    m_space = -2.0f;
    m_edge = 4.0f;
}

CBList::AuxInfo::AuxInfo(CBList::AuxInfo const& rhs) :
    m_btnNextName(rhs.m_btnNextName),
    m_btnPrevName(rhs.m_btnPrevName),
    m_wndListBgName(rhs.m_wndListBgName),
    m_space(rhs.m_space),
    m_edge(rhs.m_edge)
{
}

// ===========================================================================
//  CBList
// ===========================================================================

CBList::CBList() :
    m_btnNext(nullptr),
    m_btnPrev(nullptr),
    m_wndListBg(nullptr),
    m_listBounds{},
    m_firstItemId(-1),
    m_lastItemId(-1),
    m_workshopId(-1),
    m_type(CBButton::NUM_TYPES),
    m_selItemId(-1)
{
}

CBList::CBList(CBList const&) : CBList()
{
}

CBList::~CBList()
{
    // RVA 0x43E1F0
    ClearItems();
    CBButton::ClearPattern(m_type);
}

m3d::Class* CBList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* CBList::GetRtClass() const
{
    // RVA 0x43DF90
    return RT_CLASS_LOCAL(CBList);
}

int CBList::GameDataSetup()
{
    // RVA 0x43E310 - collects the scroll buttons and the list background, then
    // borrows the XML button pattern long enough to hand it to CBButton.
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        m3d::Object* btnNext = GetChildByName(m_aif.m_btnNextName);
        if (btnNext && btnNext->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnNext = static_cast<m3d::ui::ButtonWnd*>(btnNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnNextName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnPrev = GetChildByName(m_aif.m_btnPrevName);
        if (btnPrev && btnPrev->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnPrev = static_cast<m3d::ui::ButtonWnd*>(btnPrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnPrevName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndListBg = GetChildByName(m_aif.m_wndListBgName);
        if (wndListBg && wndListBg->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndListBg = static_cast<m3d::ui::Wnd*>(wndListBg);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndListBgName + " is not found or incorrect type");
            res = 0;
        }

        if (m_wndListBg)
        {
            float const edge = 0.0f - m_aif.m_edge;
            m_listBounds = m_wndListBg->GetBounds();
            m_listBounds.x0 -= edge;
            m_listBounds.y0 -= edge;
            m_listBounds.width += edge * 2.0f;
            m_listBounds.height += edge * 2.0f;
        }

        int const patternWndId = WND_ID_CABIN_BUTTON_PATTERN + (m_type != CBButton::TYPE_CABIN ? 1 : 0);
        if (M3D_APP->m_pInterfaceManager->CreateAndAddWindow(patternWndId))
        {
            int const ok = CBButton::LoadPattern(M3D_APP->m_pInterfaceManager->GetWindow(patternWndId), m_type) & res;
            M3D_APP->m_pInterfaceManager->RemoveWindow(patternWndId);
            if (ok)
            {
                m_gameDataFlags |= 1u;
            }
        }
    }

    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("CBList: error - fail to init because of a bad resource");
        return 0;
    }
    return 1;
}

int CBList::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x43E7E0 - repository change notifications carry the repository at
    // offset 52 of the payload.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    if (dataType == 58 && data && IsChildOf(M3D_APP))
    {
        if (GetWorkshopRepository() == reinterpret_cast<ai::GeomRepository*>(static_cast<int*>(data)[13]))
        {
            OnRepositoryChanged();
        }
    }
    return 1;
}

int CBList::GameDataClear(bool)
{
    // RVA 0x43E250
    ClearItems();
    return 1;
}

void CBList::ClearItems()
{
    // RVA 0x43E260
    for (auto*& item : m_items)
    {
        delete item;
        item = nullptr;
    }
    m_items.clear();

    m_selItemId = -1;
    m_firstItemId = -1;
    m_lastItemId = -1;
    UpdateNextPrevButtonState();
}

int CBList::CreateItems()
{
    // RVA 0x43F040 - one button per compatible part on sale in the workshop.
    ClearItems();
    if (m_workshopId == -1)
    {
        return 0;
    }

    ai::Vehicle const* vehicle = help::GetPlayerVehicle();
    std::vector<int> cbIds;
    help::GetCompatibleVehiclePartsFromWorkshop(
        m_workshopId, GetCBResourceId(), vehicle ? vehicle->GetId() : -1, cbIds);

    int res = 1;
    for (int i = 0; i < static_cast<int>(cbIds.size()); ++i)
    {
        res &= AddItem(cbIds[i]);
    }
    return res;
}

int CBList::AddItem(int cbId)
{
    // RVA 0x43E850
    if ((m_gameDataFlags & 1) == 0 || cbId == -1)
    {
        return 0;
    }

    CBButton* btn = CreateItem();
    if (!btn)
    {
        return 0;
    }
    if (!btn->SetupForCB(cbId))
    {
        delete btn;
        return 0;
    }

    m_items.push_back(btn);
    if (m_firstItemId == -1)
    {
        m_firstItemId = 0;
    }
    RecalcLayot();
    return 1;
}

void CBList::RecalcLayot()
{
    // RVA 0x43E8D0 - stacks the visible slice of buttons down the list, hides the
    // rest and shrinks the background to whatever ended up on screen.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    int const count = static_cast<int>(m_items.size());
    if (m_firstItemId < 0 || m_firstItemId >= count)
    {
        m_wndListBg->SetBounds(BoundsBase<float>{}, true);
        return;
    }

    float currentTop = m_listBounds.y0;
    m_lastItemId = m_firstItemId;
    for (int i = m_firstItemId; i < static_cast<int>(m_items.size()); ++i)
    {
        CBButton* item = m_items[i];
        if (!item)
        {
            continue;
        }

        BoundsBase<float> const b = item->GetBounds();
        if (b.height + currentTop > m_listBounds.y0 + m_listBounds.height)
        {
            break;
        }
        item->m_bounds.x0 = b.x0;
        item->m_bounds.y0 = currentTop;
        currentTop += b.height + m_aif.m_space;
        m_lastItemId = i;
    }

    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        CBButton* item = m_items[i];
        if (!item)
        {
            continue;
        }
        if (i < m_firstItemId || i > m_lastItemId)
        {
            if (IsDirectChild(item))
            {
                RemoveChild(item);
            }
        }
        else if (!IsDirectChild(item))
        {
            AddChild(item);
            MoveChildToFirstPosition(item);
        }
    }

    if (m_firstItemId >= 0 && m_firstItemId < count && m_lastItemId >= 0 && m_lastItemId < count &&
        m_items[m_firstItemId] && m_items[m_lastItemId])
    {
        BoundsBase<float> const firstB = m_items[m_firstItemId]->GetBounds();
        BoundsBase<float> const lastB = m_items[m_lastItemId]->GetBounds();

        BoundsBase<float> bgB;
        bgB.x0 = firstB.x0 - m_aif.m_edge;
        bgB.y0 = firstB.y0 - m_aif.m_edge;
        bgB.width = (firstB.x0 + firstB.width + m_aif.m_edge) - bgB.x0;
        bgB.height = (lastB.y0 + lastB.height + m_aif.m_edge) - bgB.y0;
        m_wndListBg->SetBounds(bgB, true);
    }
    else
    {
        m_wndListBg->SetBounds(BoundsBase<float>{}, true);
    }
    UpdateNextPrevButtonState();
}

bool CBList::CanScrollNext()
{
    // RVA 0x43EC40
    return m_lastItemId < static_cast<int>(m_items.size()) - 1 && m_lastItemId != -1;
}

bool CBList::CanScrollPrev()
{
    // RVA 0x43EC80
    return m_firstItemId > 0;
}

void CBList::ScrollNext()
{
    // RVA 0x43EC90
    if (m_items.empty())
    {
        m_firstItemId = -1;
    }
    else if (m_firstItemId < static_cast<int>(m_items.size()) - 1)
    {
        ++m_firstItemId;
    }
    RecalcLayot();
}

void CBList::ScrollPrev()
{
    // RVA 0x43ED00
    if (m_items.empty())
    {
        m_firstItemId = -1;
    }
    else if (m_firstItemId > 0)
    {
        --m_firstItemId;
    }
    RecalcLayot();
}

void CBList::UpdateNextPrevButtonState()
{
    // RVA 0x43ED50
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_btnPrev->EnableWindow(CanScrollPrev());
    m_btnNext->EnableWindow(CanScrollNext());
}

int CBList::OnMouseWheel(int ticks, PointBase<float> const& at)
{
    // RVA 0x43F150
    if (ticks <= 0)
    {
        if (CanScrollNext())
        {
            ScrollNext();
        }
    }
    else if (CanScrollPrev())
    {
        ScrollPrev();
    }
    return m3d::ui::Wnd::OnMouseWheel(ticks, at);
}

int CBList::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x43EDC0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }

    if (idFrom == ID_BTN_NEXT)
    {
        if (message != 1)
        {
            return 0;
        }
        ScrollNext();
        return 1;
    }

    if (idFrom == ID_BTN_PREV)
    {
        if (message != 1)
        {
            return 0;
        }
        ScrollPrev();
        return 1;
    }

    if (idFrom != ID_CB_BUTTON)
    {
        return 0;
    }

    if (message == MSG_BUY_CB)
    {
        OnBuyCB(data.GetAsID());
        return 1;
    }
    if (message != 1)
    {
        return 0;
    }
    if (from->IsKindOf(&CBButton::m_classCBButton))
    {
        SelectItem(static_cast<CBButton*>(from));
    }
    return 1;
}

void CBList::OnBuyCB(int cbId)
{
    // RVA 0x43EE90
    M3D_APP->EnqueueMessage(65671, -1, cbId, m_workshopId, 0, CStr(), m3d::AIParam());
    PostTriggerEventOnBuyCb();
}

void CBList::SelectItem(CBButton* btn)
{
    // RVA 0x43F270 - selecting a row also tells the rest of the character screen
    // which object is now under the cursor.
    int const oldSelItemId = m_selItemId;

    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        CBButton* item = m_items[i];
        if (!item)
        {
            continue;
        }
        if (item == btn)
        {
            item->Select(true);
            m_selItemId = i;
        }
        else
        {
            item->Select(false);
        }
    }

    if (m_selItemId != -1 && m_selItemId != oldSelItemId)
    {
        M3D_APP->m_pInterfaceManager->LaunchEvent(
            107, GUI_EVENT_CUSTOM, reinterpret_cast<void*>(btn ? btn->m_cbId : -1));
    }
}

int CBList::SetupForWorkshop(int workshopId)
{
    // RVA 0x43EF40
    ClearItems();
    m_workshopId = workshopId;

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(workshopId);
    if (!obj || !obj->IsKindOf(&ai::Workshop::m_classWorkshop))
    {
        return 0;
    }

    return IsChildOf(M3D_APP) ? CreateItems() : 1;
}

int CBList::OnBeforeAddToWndStation()
{
    // RVA 0x43EFC0
    HackedRestore();
    CreateItems();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int CBList::OnAfterAddToWndStation()
{
    // RVA 0x43EFE0
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    AddInfoToEncyclopaedia();
    return res;
}

int CBList::OnAfterRemoveFromWndStation()
{
    // RVA 0x43F000 - clears the "object under cursor" the list published.
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    M3D_APP->m_pInterfaceManager->LaunchEvent(107, GUI_EVENT_CUSTOM, reinterpret_cast<void*>(-1));
    return res;
}

void CBList::FullUpdate()
{
    // RVA 0x43F030
    HackedRestore();
    CreateItems();
}

void CBList::OnRepositoryChanged()
{
    // RVA 0x43F130
    HackedRestore();
    CreateItems();
}

ai::Workshop* CBList::GetWorkshop() const
{
    // RVA 0x43F210
    if (m_workshopId == -1)
    {
        return nullptr;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_workshopId);
    if (obj && obj->IsKindOf(&ai::Workshop::m_classWorkshop))
    {
        return static_cast<ai::Workshop*>(obj);
    }
    return nullptr;
}

ai::GeomRepository* CBList::GetWorkshopRepository() const
{
    // RVA 0x43F1F0
    ai::Workshop* workshop = GetWorkshop();
    return workshop ? workshop->GetRepositoryByType(ai::WORKSHOP_CABINS_AND_BASKETS) : nullptr;
}

void CBList::HackedRestore()
{
    // RVA 0x43F340 - the "hacked" workshop flow disables the whole subtree; this
    // walks it and switches everything back on before the list is rebuilt.
    EnableWindow(true);

    std::vector<m3d::Object*> stack;
    stack.push_back(this);
    while (!stack.empty())
    {
        m3d::Object* top = stack.back();
        stack.pop_back();

        for (m3d::Object* child = top->GetFirstChild(); child; child = child->GetNextSibling())
        {
            static_cast<m3d::ui::Wnd*>(child)->EnableWindow(true);
            if (child->GetFirstChild())
            {
                stack.push_back(child);
            }
        }
    }
}

void CBList::AddInfoToEncyclopaedia()
{
    // RVA 0x43F440 - seeing a part on sale is enough to unlock its encyclopaedia
    // entry.
    for (auto* item : m_items)
    {
        if (!item)
        {
            continue;
        }
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(item->m_cbId);
        if (!obj || !obj->IsKindOf(&ai::Obj::m_classObj))
        {
            continue;
        }
        M3D_APP->EnqueueMessage(66558, obj->GetPrototypeId(), 0, 0, 0, CStr(), m3d::AIParam());
    }
}

#include "warewnd.h"

#include "repositorywnd.h"
#include "videownd.h"

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimanager/gameuimanager.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimanager/uidefs.h>
#include <game/uimisc/guihelper.h>
#include <m3dapp.h>
#include <server/geomrepository.h>
#include <server/izvratrepository.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypemanager.h>
#include <server/objects/player.h>
#include <server/objects/town.h>
#include <server/objects/ware.h>
#include <server/objects/workshop.h>
#include <server/resourcemanager.h>
#include <game/profile.h>
#include <core/timer.h>
#include <server/objects/article.h>
#include <ui/image.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>

namespace
{
    // Ids the XML gives the controls in this family.
    unsigned const ID_WARE_ITEM = 300250;
    unsigned const ID_BTN_MODE = 300261;

    // WareItem notify messages beyond the plain click.
    unsigned const MSG_ITEM_SELECTED = 7;
    unsigned const MSG_ITEM_DESELECTED = 8;

    // GameDataUpdate data types WareWnd reacts to.
    int const DATATYPE_CUR_PROFILE_CHANGED = 0x28;
    int const DATATYPE_CUR_PROFILE_PARAM_CHANGED = 0x2A;
    int const DATATYPE_NEW_FRAME = 0x59;
    int const DATATYPE_DD_DISTANT_MOUSE_IN = 0x5E;
    int const DATATYPE_DD_DISTANT_MOUSE_OUT = 0x5F;
    int const DATATYPE_DD_ITEM_QUICK_DROP = 0x63;

    // Raised so the tutorial / trade layer sees a quick drop.
    int const EVENT_ITEM_DROPPED_ON_TAB = 97;

    // A workshop never lists more than this many wares.
    unsigned const MAX_WARE_ITEMS = 10;

    // Price sentinels help::GetBuyPriceByPrototypeId / GetSellPriceByPrototypeId
    // hand back when a town refuses to trade in a ware at all.
    int const PRICE_NOT_BUYED_BY_TOWN = -2;
    int const PRICE_NOT_SELLED_BY_TOWN = -3;

    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window - the shipped CreateFromPattern methods
    // in this file each inline this block.
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

    // Re-bases a pattern child's origin onto the pattern button and re-parents it
    // there, which is what lets a whole WareItem be cloned as one subtree.
    void ReparentPatternChild(m3d::ui::Wnd* pattern, m3d::ui::Wnd* owner, m3d::ui::Wnd* child)
    {
        if (!child)
        {
            return;
        }
        BoundsBase<float> b = child->GetBounds();
        b.x0 -= owner->GetBounds().x0;
        b.y0 -= owner->GetBounds().y0;
        child->SetBounds(b, true);
        pattern->RemoveChild(child);
        owner->AddChild(child);
    }
}  // namespace

// ===========================================================================
//  WareItem
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(WareItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WareItem);

WareItem::AuxInfo::AuxInfo() :
    m_wndIcoName("wndWareIco"),
    m_wndNameName("wndWareName"),
    m_wndBuyPriceName("wndWareBuyPrice"),
    m_wndSellPriceName("wndWareSellPrice"),
    m_wndAmountName("wndWareAmount"),
    m_wndCannotBuyIcoName("wndCannotBuyIco"),
    m_wndCannotSellIcoName("wndCannotSellIco"),
    m_wndPatternName("wndItemPattern"),
    m_cannotSellStrId("NotSelledByTown"),
    m_cannotBuyStrId("NotBuyedByTown"),
    m_cannotSellColor(0x77333333u),
    m_markByPrototypeColor(0x55007700u)
{
    // RVA 0x47C2C0
}

WareItem::AuxInfo::AuxInfo(WareItem::AuxInfo const& rhs) :
    m_wndIcoName(rhs.m_wndIcoName),
    m_wndNameName(rhs.m_wndNameName),
    m_wndBuyPriceName(rhs.m_wndBuyPriceName),
    m_wndSellPriceName(rhs.m_wndSellPriceName),
    m_wndAmountName(rhs.m_wndAmountName),
    m_wndCannotBuyIcoName(rhs.m_wndCannotBuyIcoName),
    m_wndCannotSellIcoName(rhs.m_wndCannotSellIcoName),
    m_wndPatternName(rhs.m_wndPatternName),
    m_cannotSellStrId(rhs.m_cannotSellStrId),
    m_cannotBuyStrId(rhs.m_cannotBuyStrId),
    m_cannotSellColor(rhs.m_cannotSellColor),
    m_markByPrototypeColor(rhs.m_markByPrototypeColor)
{
}

WareItem::Pattern::Pattern() :
    m_wndPattern(nullptr),
    m_wndPatternIco(nullptr),
    m_wndPatternName(nullptr),
    m_wndPatternBuyPrice(nullptr),
    m_wndPatternSellPrice(nullptr),
    m_wndPatternAmount(nullptr),
    m_wndPatternCannotBuyIco(nullptr),
    m_wndPatternCannotSellIco(nullptr)
{
    // RVA 0x47C550
}

m3d::Class* WareItem::GetBaseClass()
{
    return &m3d::ui::ButtonWnd::m_classButtonWnd;
}

m3d::Class* WareItem::GetClass() const
{
    return RT_CLASS_LOCAL(WareItem);
}

m3d::Object* WareItem::CreateObject()
{
    return new WareItem;
}

m3d::Object* WareItem::Clone()
{
    // RVA 0x47A250 - a fresh default-constructed item, not a copy of *this.
    return new WareItem;
}

WareItem::WareItem() :
    m_warePrototypeId(-1),
    m_workshopId(-1),
    m_idx(-1),
    m_wndIco(nullptr),
    m_wndName(nullptr),
    m_wndBuyPrice(nullptr),
    m_wndSellPrice(nullptr),
    m_wndAmount(nullptr),
    m_wndCannotBuyIco(nullptr),
    m_wndCannotSellIco(nullptr),
    m_bMarkedByPrototype(false),
    m_buyPrice(-1),
    m_sellPrice(-1),
    m_amount(0),
    m_bActive(true)
{
    // RVA 0x47C570
}

WareItem::WareItem(WareItem const&) :
    // NOTE: the shipped copy ctor (RVA 0x47C610) runs the ButtonWnd base ctor and
    // stops, leaving every field uninitialised rather than copying them.
    // Initialised here so nothing can read garbage.
    m_warePrototypeId(-1),
    m_workshopId(-1),
    m_idx(-1),
    m_wndIco(nullptr),
    m_wndName(nullptr),
    m_wndBuyPrice(nullptr),
    m_wndSellPrice(nullptr),
    m_wndAmount(nullptr),
    m_wndCannotBuyIco(nullptr),
    m_wndCannotSellIco(nullptr),
    m_bMarkedByPrototype(false),
    m_buyPrice(-1),
    m_sellPrice(-1),
    m_amount(0),
    m_bActive(true)
{
}

WareItem::~WareItem() = default;

int WareItem::GetWarePrototypeId() const
{
    // RVA 0x47C680
    return m_warePrototypeId;
}

bool WareItem::IsMarkedByPrototype() const
{
    // RVA 0x47F400
    return m_bMarkedByPrototype;
}

void WareItem::MarkByPrototype(bool bMark)
{
    m_bMarkedByPrototype = bMark;
}

int WareItem::SetUp(int warePrototypeId, int workshopId, int idx)
{
    // RVA 0x47C630
    if (Valid() || warePrototypeId == -1 || workshopId == -1 || !CreateFromPattern())
    {
        return 0;
    }
    m_warePrototypeId = warePrototypeId;
    m_workshopId = workshopId;
    m_idx = idx;
    FullUpdate();
    return 1;
}

ai::Workshop* WareItem::GetWorkshop() const
{
    // RVA 0x47CFC0
    if (m_workshopId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_workshopId), ai::Workshop);
}

int WareItem::GetTownId() const
{
    // RVA 0x47D020
    ai::Workshop* workshop = GetWorkshop();
    return workshop ? workshop->GetParentId() : -1;
}

m3d::rend::TexHandle WareItem::GetIcoForWare(int warePrototypeId) const
{
    // RVA 0x47CF50 - wares are iconified by prototype name, at the large size.
    return M3D_APP->m_pInterfaceManager->GetIcoByName(ai::thePrototypeManager->GetPrototypeName(warePrototypeId), 1);
}

CStr WareItem::GetFullNameForWare(int warePrototypeId) const
{
    // RVA 0x47CFB0
    return ai::thePrototypeManager->GetPrototypeFullName(warePrototypeId);
}

void WareItem::FullUpdate()
{
    // RVA 0x47C9B0
    UpdateName();
    UpdateIco();
    UpdateSellPrice(true);
    UpdateBuyPrice(true);
    UpdateAmount(true);
}

void WareItem::UpdateName()
{
    // RVA 0x47CA00
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndName->SetText(ai::thePrototypeManager->GetPrototypeFullName(m_warePrototypeId));
}

void WareItem::UpdateIco()
{
    // RVA 0x47CA50
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndIco->SetImage(GetIcoForWare(m_warePrototypeId));
}

void WareItem::UpdateSellPrice(bool bForce)
{
    // RVA 0x47CA80 - the "sell price" column is what the town sells the ware
    // for, so it is filled from help::GetBuyPriceByPrototypeId (the price from
    // the player's side). The buy/sell naming is from opposite perspectives
    // throughout this class; kept as shipped.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    int const oldPrice = m_sellPrice;
    m_sellPrice = help::GetBuyPriceByPrototypeId(m_warePrototypeId, GetTownId());
    if (oldPrice == m_sellPrice && !bForce)
    {
        return;
    }

    if (m_sellPrice == PRICE_NOT_SELLED_BY_TOWN)
    {
        m_wndSellPrice->SetText(CStr());
        m_wndCannotSellIco->ShowWindow(true);
    }
    else
    {
        m_wndSellPrice->SetText(m_sellPrice >= 0 ? CStr(m_sellPrice) : CStr("-"));
        m_wndCannotSellIco->ShowWindow(false);
    }
    UpdateTooltip();
    UpdateActiveState();
}

void WareItem::UpdateBuyPrice(bool bForce)
{
    // RVA 0x47CBC0 - the mirror of UpdateSellPrice: the "buy price" column is
    // what the town pays, so it comes from help::GetSellPriceByPrototypeId.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    int const oldPrice = m_buyPrice;
    m_buyPrice = help::GetSellPriceByPrototypeId(m_warePrototypeId, GetTownId());
    if (oldPrice == m_buyPrice && !bForce)
    {
        return;
    }

    if (m_buyPrice == PRICE_NOT_BUYED_BY_TOWN)
    {
        m_wndBuyPrice->SetText(CStr());
        m_wndCannotBuyIco->ShowWindow(true);
    }
    else
    {
        m_wndBuyPrice->SetText(m_buyPrice >= 0 ? CStr(m_buyPrice) : CStr("-"));
        m_wndCannotBuyIco->ShowWindow(false);
    }
    UpdateTooltip();
}

void WareItem::UpdateAmount(bool bForce)
{
    // RVA 0x47CCE0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::Workshop* workshop = GetWorkshop();
    int const oldAmount = m_amount;
    m_amount = workshop ? static_cast<int>(workshop->GetArticleAmount(m_warePrototypeId)) : 0;
    if (oldAmount == m_amount && !bForce)
    {
        return;
    }
    m_wndAmount->SetText(m_amount > 0 ? CStr(m_amount) : CStr("-"));
    UpdateActiveState();
}

void WareItem::UpdateTooltip()
{
    // RVA 0x47CDF0 - the two refusals stack, separated by a space when both
    // apply.
    CStr tooltip;
    if (m_buyPrice == PRICE_NOT_BUYED_BY_TOWN)
    {
        tooltip = M3D_APP->GetStringByStringId0(m_aif.m_cannotBuyStrId);
    }
    if (m_sellPrice == PRICE_NOT_SELLED_BY_TOWN)
    {
        CStr const separator = tooltip.empty() ? CStr() : CStr(" ");
        tooltip += separator + M3D_APP->GetStringByStringId0(m_aif.m_cannotSellStrId);
    }
    SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void WareItem::UpdateActiveState()
{
    // RVA 0x47F410 - a row is only clickable while the town sells the ware and
    // actually has some in stock.
    SetActive(m_sellPrice >= 0 && m_amount);
}

void WareItem::SetActive(bool bActive)
{
    // RVA 0x47F440
    m_bActive = bActive;
    m3d::rend::TexHandle imgOut;
    m3d::rend::TexHandle imgOver;
    m3d::rend::TexHandle imgDown;
    m3d::rend::TexHandle imgDisabled;
    GetBtnImages(bActive, imgOut, imgOver, imgDown, imgDisabled);
    SetImaged(imgOut, imgDown, imgOver, imgDisabled);
}

void WareItem::GetBtnImages(
    bool bActive,
    m3d::rend::TexHandle& imgOut,
    m3d::rend::TexHandle& imgOver,
    m3d::rend::TexHandle& imgDown,
    m3d::rend::TexHandle& imgDisabled) const
{
    // RVA 0x47F4C0 - an inactive row wears the disabled art in all four states.
    imgOut = {};
    imgOver = {};
    imgDown = {};
    imgDisabled = {};
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (bActive)
    {
        imgOut = m_pattern.m_wndPattern->GetImageRegular();
        imgOver = m_pattern.m_wndPattern->GetImageIn();
        imgDown = m_pattern.m_wndPattern->GetImageDown();
        imgDisabled = m_pattern.m_wndPattern->GetImageDisabled();
    }
    else
    {
        imgDisabled = m_pattern.m_wndPattern->GetImageDisabled();
        imgDown = imgDisabled;
        imgOver = imgDisabled;
        imgOut = imgDisabled;
    }
}

void WareItem::OnNewFrame()
{
    // RVA 0x47C690 - the same three updates as FullUpdate, but unforced so only
    // genuine changes redraw.
    UpdateSellPrice(false);
    UpdateBuyPrice(false);
    UpdateAmount(false);
}

void WareItem::OnPaintOverChildren(m3d::ui::DrawInfo const& di)
{
    // RVA 0x47F390 - the highlight a distant drag paints over the matching row.
    if (!m_bMarkedByPrototype)
    {
        return;
    }
    BoundsBase<float> const rect(0.0f, 0.0f, m_bounds.width, m_bounds.height);
    GetGfxServer()->AddFlatAxialQuad(di, rect, m_aif.m_markByPrototypeColor);
}

int WareItem::LoadPattern(m3d::ui::Wnd* pattern)
{
    // RVA 0x47E960 - pulls the seven pattern widgets out of the list window and
    // hangs them off the pattern button, so a whole row can be cloned at once.
    if (!pattern)
    {
        return 0;
    }

    m3d::Object* wndPattern = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (!wndPattern || !wndPattern->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndPatternName + " is not found or incorrect type");
        return 0;
    }
    m_pattern.m_wndPattern = static_cast<m3d::ui::ButtonWnd*>(wndPattern);
    pattern->RemoveChild(m_pattern.m_wndPattern);

    int res = 1;

    auto take = [&](CStr const& name, m3d::Class const* cls) -> m3d::ui::Wnd*
    {
        m3d::Object* child = pattern->GetChildByName(name);
        if (child && child->IsKindOf(cls))
        {
            return static_cast<m3d::ui::Wnd*>(child);
        }
        M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
        res = 0;
        return nullptr;
    };

    m_pattern.m_wndPatternName = take(m_aif.m_wndNameName, &m3d::ui::Wnd::m_classWnd);
    ReparentPatternChild(pattern, m_pattern.m_wndPattern, m_pattern.m_wndPatternName);

    m_pattern.m_wndPatternIco =
        static_cast<m3d::ui::ImageWnd*>(take(m_aif.m_wndIcoName, &m3d::ui::ImageWnd::m_classImageWnd));
    ReparentPatternChild(pattern, m_pattern.m_wndPattern, m_pattern.m_wndPatternIco);

    m_pattern.m_wndPatternBuyPrice = take(m_aif.m_wndBuyPriceName, &m3d::ui::Wnd::m_classWnd);
    ReparentPatternChild(pattern, m_pattern.m_wndPattern, m_pattern.m_wndPatternBuyPrice);

    m_pattern.m_wndPatternSellPrice = take(m_aif.m_wndSellPriceName, &m3d::ui::Wnd::m_classWnd);
    ReparentPatternChild(pattern, m_pattern.m_wndPattern, m_pattern.m_wndPatternSellPrice);

    m_pattern.m_wndPatternAmount = take(m_aif.m_wndAmountName, &m3d::ui::Wnd::m_classWnd);
    ReparentPatternChild(pattern, m_pattern.m_wndPattern, m_pattern.m_wndPatternAmount);

    m_pattern.m_wndPatternCannotBuyIco =
        static_cast<m3d::ui::ImageWnd*>(take(m_aif.m_wndCannotBuyIcoName, &m3d::ui::ImageWnd::m_classImageWnd));
    ReparentPatternChild(pattern, m_pattern.m_wndPattern, m_pattern.m_wndPatternCannotBuyIco);

    m_pattern.m_wndPatternCannotSellIco =
        static_cast<m3d::ui::ImageWnd*>(take(m_aif.m_wndCannotSellIcoName, &m3d::ui::ImageWnd::m_classImageWnd));
    ReparentPatternChild(pattern, m_pattern.m_wndPattern, m_pattern.m_wndPatternCannotSellIco);

    return res;
}

void WareItem::ClearPattern()
{
    // RVA 0x47F370
    m_pattern = Pattern();
}

int WareItem::CreateFromPattern()
{
    // RVA 0x47D040
    if (Valid())
    {
        return 0;
    }
    if (!m_pattern.m_wndPattern || !M3D_APP->IsWndAlive(m_pattern.m_wndPattern, -1) ||
        !m_pattern.m_wndPattern->Valid())
    {
        M3D_LOG_INFO("WareItem::CreateFromPattern error - null pattern");
        return 0;
    }

    if (!Create(CStr(), m_pattern.m_wndPattern->GetStyle(), m_pattern.m_wndPattern->GetBounds(),
                m_pattern.m_wndPattern->GetId()))
    {
        M3D_LOG_INFO("WareItem::CreateFromPattern error - cannot create window");
        return 0;
    }

    CopyWndPropsFromPattern(this, m_pattern.m_wndPattern);
    if (m_pattern.m_wndPattern->IsImaged())
    {
        SetImaged(
            m_pattern.m_wndPattern->GetImageRegular(),
            m_pattern.m_wndPattern->GetImageDown(),
            m_pattern.m_wndPattern->GetImageIn(),
            m_pattern.m_wndPattern->GetImageDisabled());
    }
    return CreateChildren();
}

int WareItem::CreateChildren()
{
    // RVA 0x47D4A0 - one child per pattern widget, each cloned and adopted.
    int res = 1;

    auto clone = [&](m3d::ui::Wnd* pat) -> m3d::ui::Wnd*
    {
        if (!pat)
        {
            res = 0;
            return nullptr;
        }
        auto* wnd = RT_DYNCAST(M3D_KERNEL->New(pat->GetClass()->m_className), m3d::ui::Wnd);
        if (!wnd)
        {
            res = 0;
            return nullptr;
        }
        if (!wnd->Create(CStr(), pat->GetStyle(), pat->GetBounds(), pat->GetId()))
        {
            res = 0;
            return nullptr;
        }
        CopyWndPropsFromPattern(wnd, pat);
        AddChild(wnd);
        return wnd;
    };

    m_wndName = clone(m_pattern.m_wndPatternName);
    m_wndIco = RT_DYNCAST(clone(m_pattern.m_wndPatternIco), m3d::ui::ImageWnd);
    m_wndBuyPrice = clone(m_pattern.m_wndPatternBuyPrice);
    m_wndSellPrice = clone(m_pattern.m_wndPatternSellPrice);
    m_wndAmount = clone(m_pattern.m_wndPatternAmount);
    m_wndCannotBuyIco = RT_DYNCAST(clone(m_pattern.m_wndPatternCannotBuyIco), m3d::ui::ImageWnd);
    m_wndCannotSellIco = RT_DYNCAST(clone(m_pattern.m_wndPatternCannotSellIco), m3d::ui::ImageWnd);

    if (res)
    {
        m_gameDataFlags |= 1u;
    }
    return res;
}

// ===========================================================================
//  WareList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(WareList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WareList);

WareList::AuxInfo::AuxInfo() : m_wndItemPatternName("wndItem"), m_space(2.0f), m_twinklePeriod(300), m_numTwinkles(2)
{
    // RVA 0x47F5C0
}

WareList::AuxInfo::AuxInfo(WareList::AuxInfo const& rhs) :
    m_wndItemPatternName(rhs.m_wndItemPatternName),
    m_space(rhs.m_space),
    m_twinklePeriod(rhs.m_twinklePeriod),
    m_numTwinkles(rhs.m_numTwinkles)
{
}

WareList::TwinkleInfo::TwinkleInfo(int itemPrototypeId) : m_itemPrototypeId(itemPrototypeId), m_startTime(0)
{
    // RVA 0x47A280
}

void WareList::TwinkleInfo::Invalidate()
{
    m_itemPrototypeId = -1;
    m_startTime = 0;
}

bool WareList::TwinkleInfo::IsValid() const
{
    // RVA 0x47A2A0
    return m_itemPrototypeId != -1;
}

m3d::Class* WareList::GetBaseClass()
{
    return &m3d::ui::Wnd::m_classWnd;
}

m3d::Class* WareList::GetClass() const
{
    return RT_CLASS_LOCAL(WareList);
}

m3d::Object* WareList::CreateObject()
{
    return new WareList;
}

m3d::Object* WareList::Clone()
{
    // RVA 0x47A2B0
    return new WareList;
}

WareList::WareList() : m_workshopId(-1), m_bMouseClickReact(true), m_curMode(MODE_ONLY_SELLED)
{
    // RVA 0x47F630 - a fresh list starts filtered to what the town actually
    // sells.
}

WareList::WareList(WareList const&) :
    // NOTE: the shipped copy ctor (RVA 0x47F6F0) initialises only the two
    // vectors and the AuxInfo, leaving the workshop id, the click flag and the
    // mode uninitialised. Given sane values here.
    m_workshopId(-1),
    m_bMouseClickReact(true),
    m_curMode(MODE_ONLY_SELLED)
{
}

WareList::~WareList()
{
    // RVA 0x47F740
    ClearItems();
}

ai::Workshop* WareList::GetWorkshop() const
{
    // RVA 0x47FEC0
    if (m_workshopId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_workshopId), ai::Workshop);
}

WareList::Mode WareList::GetCurMode() const
{
    // RVA 0x4809C0
    return m_curMode;
}

void WareList::SetCurMode(WareList::Mode mode)
{
    // RVA 0x4809D0
    Mode const oldMode = m_curMode;
    m_curMode = mode;
    if (oldMode != mode && IsChildOf(M3D_APP))
    {
        FullUpdate();
    }
}

void WareList::ReactOnMouseClick(bool bReact)
{
    // RVA 0x4807E0
    m_bMouseClickReact = bReact;
}

void WareList::ClearItems()
{
    // RVA 0x47F7F0
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        if (m_items[i])
        {
            m_items[i]->DecRef();
        }
        m_items[i] = nullptr;
    }
    m_items.clear();
}

int WareList::CreateFromPattern(m3d::ui::Wnd* pattern, bool)
{
    // RVA 0x47F880. NOTE: every log message in this method says
    // "WareWnd::CreateFromPattern" - a copy-paste in the shipped source that is
    // reproduced verbatim here.
    if (!pattern)
    {
        M3D_LOG_INFO("WareList::CreateFromPattern - invalid params");
        return 0;
    }

    if (!Create(CStr(), pattern->GetStyle(), pattern->GetBounds(), pattern->GetId()))
    {
        M3D_LOG_INFO("WareWnd::CreateFromPattern error - cannot create window");
        return 0;
    }
    CopyWndPropsFromPattern(this, pattern);

    auto* parent = RT_DYNCAST(pattern->GetParent(), m3d::ui::Wnd);
    if (!parent)
    {
        M3D_LOG_INFO("WareWnd::CreateFromPattern error - invalid parent for pattern");
        return 0;
    }
    if (!WareItem::LoadPattern(parent))
    {
        M3D_LOG_INFO("WareWnd::CreateFromPattern error - cannot load pattern for WareItem");
        return 0;
    }
    m_gameDataFlags |= 1u;
    return 1;
}

int WareList::AddItem(int warePrototypeId, int idx)
{
    // RVA 0x47FE40
    if (m_workshopId == -1)
    {
        return 0;
    }
    auto* item = RT_DYNCAST(M3D_KERNEL->New("WareItem"), WareItem);
    if (!item)
    {
        return 0;
    }
    if (!item->SetUp(warePrototypeId, m_workshopId, idx))
    {
        item->DecRef();
        return 0;
    }
    m_items.push_back(item);
    AddChild(item);
    return 1;
}

int WareList::CreateItems()
{
    // RVA 0x47FCF0 - every GOODS prototype, capped at ten rows, and in
    // MODE_ONLY_SELLED narrowed to what the town both sells and has in stock.
    ClearItems();
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0;
    }

    std::vector<int> warePrototypeIds;
    ai::thePrototypeManager->GetPrototypeIdsByResourceId(
        ai::theResourceManager->GetResourceId(CStr("GOODS")), warePrototypeIds);
    if (warePrototypeIds.size() > MAX_WARE_ITEMS)
    {
        warePrototypeIds.resize(MAX_WARE_ITEMS, 0);
    }

    int res = 1;
    for (int i = 0; i < static_cast<int>(warePrototypeIds.size()); ++i)
    {
        if (m_curMode == MODE_ONLY_SELLED)
        {
            ai::Article* article = workshop->GetArticle(warePrototypeIds[i]);
            if (!article || !article->IsSellable() || !article->getAmount())
            {
                continue;
            }
        }
        res &= AddItem(warePrototypeIds[i], i);
    }
    RecalcLayot();
    return res;
}

void WareList::RecalcLayot()
{
    // RVA 0x4803B0 - rows are stacked top to bottom, and the list grows to fit
    // the last one.
    float top = 0.0f;
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        if (!m_items[i])
        {
            continue;
        }
        BoundsBase<float> itemB = m_items[i]->GetBounds();
        itemB.y0 = top;
        m_items[i]->SetBounds(itemB, true);
        top += itemB.height + m_aif.m_space;
    }
    if (!m_items.empty() && m_items.back())
    {
        BoundsBase<float> const lastB = m_items.back()->GetBounds();
        BoundsBase<float> listB = GetBounds();
        listB.height = lastB.y0 + lastB.height;
        SetBounds(listB, true);
    }
}

WareItem* WareList::GetItemByWarePrototypeId(int warePrototypeId) const
{
    // RVA 0x480960
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        if (m_items[i] && m_items[i]->GetWarePrototypeId() == warePrototypeId)
        {
            return m_items[i];
        }
    }
    return nullptr;
}

void WareList::MarkItemByPrototypeId(int warePrototypeId)
{
    // RVA 0x4807F0 - passing -1 clears every mark.
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        if (m_items[i])
        {
            m_items[i]->MarkByPrototype(m_items[i]->GetWarePrototypeId() == warePrototypeId);
        }
    }
}

void WareList::SetItemToTwinkle(int warePrototypeId)
{
    // RVA 0x480840
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_twinkleItems.push_back(TwinkleInfo(warePrototypeId));
}

void WareList::TwinkleItem(WareList::TwinkleInfo& twinkleInfo)
{
    // RVA 0x480870 - the row is detached and re-attached on alternate half
    // periods, which reads as a blink, and is left attached when the run ends.
    if ((m_gameDataFlags & 1) == 0 || !twinkleInfo.IsValid())
    {
        return;
    }
    WareItem* item = GetItemByWarePrototypeId(twinkleInfo.m_itemPrototypeId);
    if (!item)
    {
        return;
    }

    unsigned int const now = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
    if (!twinkleInfo.m_startTime)
    {
        twinkleInfo.m_startTime = now;
    }

    unsigned int const runFor = m_aif.m_twinklePeriod * m_aif.m_numTwinkles;
    if (now < twinkleInfo.m_startTime + runFor)
    {
        int const halfPeriod = static_cast<int>(static_cast<double>(m_aif.m_twinklePeriod) * 0.5);
        bool const hidden = ((now - twinkleInfo.m_startTime) / halfPeriod) % 2 == 0;
        if (hidden)
        {
            if (IsDirectChild(item))
            {
                RemoveChild(item);
            }
        }
        else if (!IsDirectChild(item))
        {
            AddChild(item);
        }
        return;
    }

    if (!IsDirectChild(item))
    {
        AddChild(item);
    }
    twinkleInfo.Invalidate();
}

int WareList::SetupForWorkshop(int workshopId)
{
    // RVA 0x4804C0
    m_workshopId = workshopId;
    m_twinkleItems.clear();
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    return IsChildOf(M3D_APP) ? CreateItems() : 1;
}

void WareList::FullUpdate()
{
    // RVA 0x480A20
    CreateItems();
    for (int i = 0; i < static_cast<int>(m_twinkleItems.size()); ++i)
    {
        if (m_twinkleItems[i].IsValid())
        {
            EnsureVisibleWare(m_twinkleItems[i].m_itemPrototypeId);
        }
    }
}

int WareList::EnsureVisibleWare(int warePrototypeId)
{
    // RVA 0x480710 - a ware that is in stock but not sold by the town can only
    // be shown by leaving the filtered mode, which is done through the profile
    // so the change sticks.
    if (GetItemByWarePrototypeId(warePrototypeId))
    {
        return 1;
    }
    if (m_curMode == MODE_FULL)
    {
        return 0;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0;
    }
    ai::Article* article = workshop->GetArticle(warePrototypeId);
    if (!article || !article->getAmount())
    {
        return 0;
    }
    if (article->IsSellable())
    {
        FullUpdate();
        return 1;
    }

    Profile* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return 0;
    }
    // m_curMode is known to be MODE_ONLY_SELLED here, so this always writes
    // MODE_FULL - the shipped code spells it as a comparison anyway.
    profile->SetParam(PP_WARELIST_MODE, m3d::AIParam(m_curMode == MODE_FULL));
    return 1;
}

void WareList::OnSelectItem(WareItem* item)
{
    // RVA 0x480390
    if (item)
    {
        ShowVideo(item);
    }
    else
    {
        HideVideo();
    }
}

void WareList::ShowVideo(WareItem* item)
{
    // RVA 0x480530 - the preview pane spins the selected ware's model.
    if (!item)
    {
        HideVideo();
        return;
    }
    ref_ptr<m3d::ui::Wnd> videoWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_VIDEO);
    if (videoWnd && videoWnd->IsKindOf(&VideoWnd::m_classVideoWnd))
    {
        static_cast<VideoWnd*>(videoWnd.get())->ShowModelByPrototypeId(item->GetWarePrototypeId(), this);
    }
}

void WareList::HideVideo()
{
    // RVA 0x4805F0
    ref_ptr<m3d::ui::Wnd> videoWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_VIDEO);
    if (videoWnd && videoWnd->IsKindOf(&VideoWnd::m_classVideoWnd))
    {
        static_cast<VideoWnd*>(videoWnd.get())->HideModel(this);
    }
}

int WareList::BuyWare(int wareObjId)
{
    // RVA 0x480690 - the workshop takes the ware off the player.
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop || !workshop->BuyArticle(wareObjId))
    {
        return 0;
    }
    if (!IsChildOf(M3D_APP))
    {
        return 1;
    }
    auto* ware = RT_DYNCAST(ai::theObjects->GetEntityByObjId(wareObjId), ai::Ware);
    return ware ? EnsureVisibleWare(ware->GetPrototypeId()) : 0;
}

int WareList::SellWare(int warePrototypeId)
{
    // RVA 0x47FFF0 - the player buys one unit off the town: price checks, a new
    // object, then space in the player's repository.
    if (warePrototypeId == -1)
    {
        return 0;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0;
    }

    int const price = help::GetBuyPriceByPrototypeId(warePrototypeId, workshop->GetParentId());
    if (price == PRICE_NOT_SELLED_BY_TOWN)
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("WareNotSelledByTown")), 1u, false);
        return 0;
    }
    if (price < 0)
    {
        return 0;
    }
    if (!workshop->GetArticleAmount(warePrototypeId))
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NoWare")), 1u, false);
        return 0;
    }
    if (price > ai::thePlayer->GetMoney())
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NotEnoughGold")), 1u, false);
        return 0;
    }

    int const newObjId = ai::theObjects->CreateNewObject(warePrototypeId, "", -1, -1);
    auto* ware = RT_DYNCAST(ai::theObjects->GetEntityByObjId(newObjId), ai::Ware);
    if (!ware)
    {
        return 0;
    }

    ai::GeomRepositoryItem const item(newObjId);
    ai::IzvratRepository* repository = ai::thePlayer->GetRepository();
    if (!repository || !repository->CanAddThing(item))
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NotEnoughSpace")), 1u, false);
        ware->Remove();
        return 0;
    }

    int res = repository->AddThing(item, false);
    ai::thePlayer->AddMoney(-price);

    ref_ptr<m3d::ui::Wnd> playerRepositoryWnd = M3D_APP->m_pInterfaceManager->GetWindow(IW_WND_PLAYERVEHICLE_INVENTORY);
    if (playerRepositoryWnd && playerRepositoryWnd->IsKindOf(&RepositoryWnd::m_classRepositoryWnd))
    {
        static_cast<RepositoryWnd*>(playerRepositoryWnd.get())->SetItemToTwinkle(newObjId);
    }

    if (!workshop->SellArticle(warePrototypeId, 1u))
    {
        res = 0;
    }
    if (IsChildOf(M3D_APP))
    {
        ai::Article* article = workshop->GetArticle(warePrototypeId);
        if (!article || !article->getAmount())
        {
            FullUpdate();
        }
    }

    bool bLooped = false;
    GetGfxServer()->PlayControlSound(CStr("SOUND_DROP_ITEM"), &bLooped);
    return res;
}

void WareList::OnNewFrame()
{
    // RVA 0x47FC20
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        if (m_items[i])
        {
            m_items[i]->OnNewFrame();
        }
    }
    for (int i = 0; i < static_cast<int>(m_twinkleItems.size()); ++i)
    {
        TwinkleItem(m_twinkleItems[i]);
    }
}

int WareList::OnBeforeAddToWndStation()
{
    // RVA 0x480A10
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int WareList::OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
{
    // RVA 0x47FF20
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id != ID_WARE_ITEM || !from || !from->IsKindOf(&WareItem::m_classWareItem))
    {
        return 0;
    }

    auto* item = static_cast<WareItem*>(from);
    switch (msg)
    {
    case 1:
        // A click that ends a drag is swallowed, and only re-arms the list.
        if (m_bMouseClickReact)
        {
            SellWare(item->GetWarePrototypeId());
        }
        else
        {
            m_bMouseClickReact = true;
        }
        return 1;
    case MSG_ITEM_SELECTED:
        OnSelectItem(item);
        return 1;
    case MSG_ITEM_DESELECTED:
        HideVideo();
        return 1;
    default:
        return 0;
    }
}

// ===========================================================================
//  WareWnd
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(WareWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WareWnd);

WareWnd::AuxInfo::AuxInfo() :
    m_wndWareListName("wndWareList"),
    m_wndSellPricesIcoName("wndSellPricesIco"),
    m_wndBuyPricesIcoName("wndBuyPricesIco"),
    m_wndAmountsIcoName("wndAmountsIco"),
    m_dragPaneName("PaneGreenThick"),
    m_btnModeName("btnMode"),
    m_wndEmbossBtnModeName("emboss_btnMode"),
    m_toAdvancePaneName("PanePlusArrow1"),
    m_toSimplePaneName("PaneMinusArrow1"),
    m_strIdShowAll("ShowAllWares"),
    m_strIdShowSelled("ShowTownSelledWares")
{
    // RVA 0x47A380
}

WareWnd::AuxInfo::AuxInfo(WareWnd::AuxInfo const& rhs) :
    m_wndWareListName(rhs.m_wndWareListName),
    m_wndSellPricesIcoName(rhs.m_wndSellPricesIcoName),
    m_wndBuyPricesIcoName(rhs.m_wndBuyPricesIcoName),
    m_wndAmountsIcoName(rhs.m_wndAmountsIcoName),
    m_dragPaneName(rhs.m_dragPaneName),
    m_btnModeName(rhs.m_btnModeName),
    m_wndEmbossBtnModeName(rhs.m_wndEmbossBtnModeName),
    m_toAdvancePaneName(rhs.m_toAdvancePaneName),
    m_toSimplePaneName(rhs.m_toSimplePaneName),
    m_strIdShowAll(rhs.m_strIdShowAll),
    m_strIdShowSelled(rhs.m_strIdShowSelled)
{
}

m3d::Class* WareWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(DragDropItemsWnd);
}

m3d::Class* WareWnd::GetClass() const
{
    return RT_CLASS_LOCAL(WareWnd);
}

m3d::Object* WareWnd::CreateObject()
{
    return new WareWnd;
}

m3d::Object* WareWnd::Clone()
{
    // RVA 0x47A2E0
    return new WareWnd;
}

WareWnd::WareWnd() : m_workshopId(-1), m_bDragging(false), m_btnMode(nullptr)
{
    // RVA 0x47A640
}

WareWnd::WareWnd(WareWnd const&) :
    // NOTE: the shipped copy ctor (RVA 0x47A860) stops after the base ctor and
    // the AuxInfo, leaving the workshop id, the drag flag and the mode button
    // uninitialised.
    m_workshopId(-1),
    m_bDragging(false),
    m_btnMode(nullptr)
{
}

WareWnd::~WareWnd() = default;

ai::Workshop* WareWnd::GetWorkshop() const
{
    // RVA 0x47B9B0
    if (m_workshopId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_workshopId), ai::Workshop);
}

BoundsBase<float> WareWnd::GeomToWndBounds(BoundsBase<int> const&)
{
    // RVA 0x47A8E0 - the ware window has no cell grid, so any geometry maps onto
    // the whole client area.
    return BoundsBase<float>(0.0f, 0.0f, m_bounds.width, m_bounds.height);
}

ai::GeomRepositoryItem WareWnd::GetItemFromOrigin(PointBase<float> const&)
{
    // RVA 0x47A910 - nothing can be picked up out of a ware window.
    return {};
}

int WareWnd::GiveUpItem(ai::GeomRepositoryItem const&, m3d::ui::Wnd*)
{
    // RVA 0x47AAA0
    return 0;
}

void WareWnd::HideDragSrc()
{
    // RVA 0x47AAB0 - deliberately empty; there is no source slot to hide.
}

void WareWnd::ShowDragSrc()
{
    // RVA 0x47AAC0 - deliberately empty.
}

void WareWnd::OnUpdateWhileDrag(PointBase<float> const&)
{
    // RVA 0x47AA60
    m_bDragging = true;
}

void WareWnd::OnUpdateWhileNoDrag(PointBase<float> const&)
{
    // RVA 0x47AA70
    m_bDragging = false;
}

void WareWnd::OnDragOut()
{
    // RVA 0x47AA80
    m_bDragging = false;
}

int WareWnd::OnDragRemove()
{
    // RVA 0x47AA90
    m_bDragging = false;
    return 1;
}

bool WareWnd::IsItemCompatibleByResourceType(ai::GeomRepositoryItem const& item) const
{
    // RVA 0x47B950 - only GOODS may be sold here.
    return ai::theResourceManager->bResourceIsKindOf(
        item.GetResourceId(), ai::theResourceManager->GetResourceId(CStr("GOODS")));
}

int WareWnd::CanAddDragItem(bool)
{
    // RVA 0x47AA10
    if (!m_dragSlot)
    {
        return 0;
    }
    return m_dragSlot->m_item.IsValid() && GetWorkshop() && IsItemCompatibleByResourceType(m_dragSlot->m_item);
}

bool WareWnd::CanAddDragItemDistant(ai::GeomRepositoryItem const& item) const
{
    // RVA 0x47BD30
    return item.IsValid() && GetWorkshop() && IsItemCompatibleByResourceType(item);
}

bool WareWnd::CanWindowQuickDropToMe(m3d::ui::Wnd const* srcWnd, ai::GeomRepositoryItem const& item) const
{
    // RVA 0x47B910 - a quick drop is only accepted from the player's own
    // repository window (RepositoryWnd type 0).
    if (!srcWnd || srcWnd == this || !srcWnd->IsKindOf(&RepositoryWnd::m_classRepositoryWnd))
    {
        return false;
    }
    if (static_cast<RepositoryWnd const*>(srcWnd)->m_type != 0)
    {
        return false;
    }
    return item.IsValid() && IsItemCompatibleByResourceType(item);
}

int WareWnd::AddItem(ai::GeomRepositoryItem const& item)
{
    // RVA 0x47A930 - dropping a ware here sells it to the workshop, and the row
    // it lands in blinks.
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return 0;
    }
    int const res = m_wndWareList->BuyWare(item.GetObjId());
    if (res)
    {
        m_wndWareList->SetItemToTwinkle(item.GetPrototypeId());
        PlayDropSound();
    }
    return res;
}

int WareWnd::BuyWare(int wareObjId)
{
    // RVA 0x47BA10
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return 0;
    }
    return m_wndWareList->BuyWare(wareObjId);
}

int WareWnd::SetupForWorkshop(int workshopId)
{
    // RVA 0x47B5C0
    bool const wasReady = (m_gameDataFlags & 1) != 0;
    m_workshopId = workshopId;
    if (!wasReady || !m_wndWareList)
    {
        return 0;
    }
    return m_wndWareList->SetupForWorkshop(m_workshopId);
}

int WareWnd::UpdateOnItemQuickDrop(ItemAcceptInfo const& info)
{
    // RVA 0x47B800 - answers a shift-click made in another window.
    if (!info.m_eventSrcWnd || info.m_eventSrcWnd == this)
    {
        return 0;
    }
    if (!CanWindowQuickDropToMe(info.m_eventSrcWnd, info.m_item))
    {
        return 0;
    }
    ai::Workshop* workshop = GetWorkshop();
    if (!workshop)
    {
        return 0;
    }

    int const price = help::GetSellPriceByObjId(info.m_item.GetObjId(), workshop->GetParentId());
    if (price == PRICE_NOT_BUYED_BY_TOWN)
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("WareNotBuyedByTown")), 1u, false);
        return 0;
    }
    if (price < 0)
    {
        return 0;
    }

    ItemAcceptInfo acceptInfo(this, info.m_eventSrcWnd, info.m_item);
    M3D_APP->m_pInterfaceManager->LaunchEvent(EVENT_ITEM_DROPPED_ON_TAB, GUI_EVENT_CUSTOM, &acceptInfo);
    return 1;
}

void WareWnd::OnDistantMouseIn(ai::GeomRepositoryItem const& item)
{
    // RVA 0x47BC80 - dragging a ware anywhere on screen highlights its row here.
    if ((m_gameDataFlags & 1) == 0 || !item.IsValid() || !GetWorkshop() ||
        !IsItemCompatibleByResourceType(item) || !m_wndWareList)
    {
        return;
    }
    m_wndWareList->MarkItemByPrototypeId(item.GetPrototypeId());
}

void WareWnd::OnDistantMouseOut()
{
    // RVA 0x47BCF0
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return;
    }
    m_wndWareList->MarkItemByPrototypeId(-1);
}

void WareWnd::OnNewFrame()
{
    // RVA 0x47B7C0
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return;
    }
    m_wndWareList->OnNewFrame();
}

WareList::Mode WareWnd::GetCurMode() const
{
    // RVA 0x47BF00
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return WareList::MODE_ONLY_SELLED;
    }
    return m_wndWareList->GetCurMode();
}

void WareWnd::SetCurMode(WareList::Mode mode)
{
    // RVA 0x47BE10
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return;
    }
    WareList::Mode const oldMode = m_wndWareList->GetCurMode();
    m_wndWareList->SetCurMode(mode);
    if (oldMode != GetCurMode())
    {
        UpdateModeButtonState();
        UpdateModeTooltip();
    }
}

void WareWnd::OnChangeCurMode()
{
    // RVA 0x47BEF0
    UpdateModeButtonState();
    UpdateModeTooltip();
}

void WareWnd::OnModeBtnClicked()
{
    // RVA 0x47BD60 - the mode lives in the profile, so the button only writes
    // there and the change comes back as a profile-param notification.
    Profile* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return;
    }
    WareList::Mode const mode = GetCurMode();
    profile->SetParam(PP_WARELIST_MODE, m3d::AIParam(mode == WareList::MODE_FULL));
}

void WareWnd::UpdateModeButtonState()
{
    // RVA 0x47C070
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return;
    }
    m_btnMode->SetPane(
        GetCurMode() == WareList::MODE_ONLY_SELLED ? m_aif.m_toAdvancePaneName : m_aif.m_toSimplePaneName);
}

void WareWnd::UpdateModeTooltip()
{
    // RVA 0x47C0C0
    if ((m_gameDataFlags & 1) == 0 || !m_wndWareList)
    {
        return;
    }
    CStr tooltip = M3D_APP->GetStringByStringId0(
        GetCurMode() == WareList::MODE_ONLY_SELLED ? m_aif.m_strIdShowAll : m_aif.m_strIdShowSelled);
    m_btnMode->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void WareWnd::OnCurProfileChanged()
{
    // RVA 0x47BF40
    Profile* profile = M3D_APP->GetProfileManager()->GetCurProfile();
    if (!profile)
    {
        return;
    }
    m3d::AIParam param;
    if (profile->GetParam(PP_WARELIST_MODE, param))
    {
        SetCurMode(static_cast<WareList::Mode>(param.GetAsID()));
    }
}

void WareWnd::OnCurProfileParamChanged(void* data)
{
    // RVA 0x47BFD0 - only the ware-list mode param is of interest.
    if (!data || static_cast<int*>(data)[13] != PP_WARELIST_MODE)
    {
        return;
    }
    OnCurProfileChanged();
}

void WareWnd::OnPaintOverChildren(m3d::ui::DrawInfo const& di)
{
    // RVA 0x47BA50 - a drag hovering over the window outlines it.
    if (!m_bDragging)
    {
        return;
    }
    BoundsBase<float> const rect(0.0f, 0.0f, m_bounds.width, m_bounds.height);
    GetGfxServer()->AddFlatAxialPane0(di, rect, m_curClr, 2, m_aif.m_dragPaneName, m3d::ui::PANE_FLAG_BG_OUT);
}

int WareWnd::OnMouseButton0(unsigned int state, PointBase<float> const& at)
{
    // RVA 0x47BC00 - a click that finishes a drag must not also count as a
    // purchase, so the list is told to swallow the next one.
    if (m_dragSlot && m_dragSlot->IsChildOf(M3D_APP) && m_wndWareList)
    {
        m_wndWareList->ReactOnMouseClick(false);
    }
    return DragDropItemsWnd::OnMouseButton0(state, at);
}

int WareWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
{
    // RVA 0x47BAC0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (DragDropItemsWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }

    if (msg == MSG_ITEM_DESELECTED)
    {
        // A child reported the mouse left it; if the pointer is outside this
        // window too, the whole window counts as exited.
        PointBase<float> mousePt(
            static_cast<float>(M3D_APP->GetMouseX()), static_cast<float>(M3D_APP->GetMouseY()));
        M3D_RENDERER->AbsToRel(mousePt.x, mousePt.y);
        PointBase<float> const origin = ToScreen(PointBase<float>(0.0f, 0.0f));
        PointBase<float> const local(mousePt.x - origin.x, mousePt.y - origin.y);
        if (!IsPtInBounds(ToParent(local)))
        {
            OnMouseOut();
        }
        return 1;
    }

    if (id == ID_BTN_MODE && msg == 1)
    {
        OnModeBtnClicked();
        return 1;
    }
    return 0;
}

int WareWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x47B610
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    switch (dataType)
    {
    case DATATYPE_CUR_PROFILE_CHANGED:
        OnCurProfileChanged();
        return 1;
    case DATATYPE_CUR_PROFILE_PARAM_CHANGED:
        OnCurProfileParamChanged(data);
        return 1;
    case DATATYPE_NEW_FRAME:
        OnNewFrame();
        return 1;
    case DATATYPE_DD_DISTANT_MOUSE_IN:
        if (data && IsChildOf(M3D_APP))
        {
            auto const& info = *static_cast<ItemAcceptInfo*>(data);
            if (info.m_eventSrcWnd != this)
            {
                OnDistantMouseIn(info.m_item);
            }
        }
        return 1;
    case DATATYPE_DD_DISTANT_MOUSE_OUT:
        if (IsChildOf(M3D_APP))
        {
            OnDistantMouseOut();
        }
        return 1;
    case DATATYPE_DD_ITEM_QUICK_DROP:
        if (IsChildOf(M3D_APP))
        {
            UpdateOnItemQuickDrop(*static_cast<ItemAcceptInfo*>(data));
        }
        return 1;
    default:
        return DragDropItemsWnd::GameDataUpdate(data, dataType);
    }
}

int WareWnd::CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc)
{
    // RVA 0x47AAD0 - clones this window off the XML pattern, then builds the
    // ware list, the three column icons and the mode button out of it.
    if (!pattern)
    {
        M3D_LOG_INFO("WareWnd::CreateFromPattern - invalid params");
        return 0;
    }

    if (!Create(CStr(), pattern->GetStyle(), pattern->GetBounds(), pattern->GetId()))
    {
        M3D_LOG_INFO("WareWnd::CreateFromPattern error - cannot create window");
        return 0;
    }
    CopyWndPropsFromPattern(this, pattern);

    int res = 1;

    m3d::Object* wareListPattern = pattern->GetChildByName(m_aif.m_wndWareListName);
    if (wareListPattern && wareListPattern->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        auto* wareList = RT_DYNCAST(M3D_KERNEL->New("WareList"), WareList);
        if (wareList && wareList->CreateFromPattern(static_cast<m3d::ui::Wnd*>(wareListPattern), deleteSrc))
        {
            m_wndWareList = wareList;
            AddChild(wareList);
        }
        else
        {
            M3D_LOG_INFO("WareWnd::CreateFromPattern error - cannot create ware list");
            res = 0;
        }
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_wndWareListName + " is not found or incorrect type");
        res = 0;
    }

    m3d::Object* btnMode = pattern->GetChildByName(m_aif.m_btnModeName);
    if (btnMode && btnMode->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
    {
        m_btnMode = static_cast<m3d::ui::ButtonWnd*>(btnMode);
        pattern->RemoveChild(m_btnMode);
        AddChild(m_btnMode);
    }
    else
    {
        M3D_LOG_INFO("Get control error: control " + m_aif.m_btnModeName + " is not found or incorrect type");
        res = 0;
    }

    if (res)
    {
        m_gameDataFlags |= 1u;
        OnCurProfileChanged();
        OnChangeCurMode();
    }
    return res;
}

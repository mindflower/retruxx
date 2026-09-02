#include "advancedlist.h"
#include "buyserviceslider.h"

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <i_event.h>
#include <game/m3dgame.h>

#include <ui/button.h>
#include <ui/frame.h>
#include <ui/image.h>
#include <ui/slider.h>
#include <ui/ui_srv.h>
#include <ui/wndstation.h>

#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <server/objects/workshop.h>
#include <server/objects/ware.h>
#include <server/objects/base/objcontainer.h>

// ===========================================================================
//  AdvancedButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(AdvancedButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(AdvancedButton);

namespace
{
    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window. The shipped
    // AdvancedButton::CreateFromPattern / CreateChildren inline this block once
    // per widget.
    void CopyWndPropsFromPattern(m3d::ui::Wnd* dst, const m3d::ui::Wnd* pat)
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

    // Creates one child window of the given rtti class, positioned/styled from
    // the pattern, links it at the front of parent's child list, and copies the
    // pattern's properties over. Returns nullptr (and logs) on any failure.
    m3d::ui::Wnd* CreateChildFromPattern(m3d::ui::Wnd* parent, const char* className,
                                        const m3d::ui::Wnd* pat, const char* ctx)
    {
        if (!pat)
        {
            M3D_LOG_INFO(CStr("AdvancedButton::CreateChildren: ") + ctx + " - NULL pattern");
            return nullptr;
        }
        auto* w = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New(className));
        if (!w)
        {
            M3D_LOG_INFO(CStr("AdvancedButton::CreateChildren: ") + ctx + " - cannot instantiate " + className);
            return nullptr;
        }
        if (!w->Create(CStr(), pat->GetStyle(), pat->GetBounds(), pat->GetId()))
        {
            w->DecRef();
            M3D_LOG_INFO(CStr("AdvancedButton::CreateChildren: ") + ctx + " - cannot create window");
            return nullptr;
        }
        parent->AddChild(w);
        parent->MoveChildToFirstPosition(w);
        CopyWndPropsFromPattern(w, pat);
        return w;
    }

    void CopyButtonImages(m3d::ui::ButtonWnd* dst, const m3d::ui::ButtonWnd* pat)
    {
        if (pat->IsImaged())
        {
            dst->SetImaged(pat->GetImageRegular(), pat->GetImageDown(), pat->GetImageIn(), m3d::rend::TexHandle{});
        }
        else
        {
            dst->SetRegular();
        }
    }
}  // namespace

AdvancedButton::AuxInfo::AuxInfo()
{
    m_wndPatternName = "wndPattern";
    m_btnPatternActiveZoneName = "btnActiveZone";
    m_wndPatternIcoName = "wndIco";
    m_wndPatternNameName = "wndName";
    m_wndPatternPriceName = "wndPrice";
    m_btnPatternAdvanceName = "btnAdvance";
    m_wndPatternIdioticEmbossBtnAdvanceName = "emboss_btnAdvance";
    m_wndPatternSliderName = "wndSlider";
    m_btnPatternLeftName = "btnLeft";
    m_btnPatternRightName = "btnRight";
    m_wndPatternUnitsName = "wndUnits";
    m_wndPatternFrameName = "wndFrame";
    m_toAdvancePaneName = "PanePlusArrow1";
    m_toSimplePaneName = "PaneMinusArrow1";
    m_strIdTooltipToSimple = "ToSimpleMode";
    m_strIdTooltipToAdvanced = "ToAdvancedMode";
    m_strIdTooltipBuy = "Buy";
    m_strIdTooltipBuyNotNeed = "BuyNotNeed";
    m_strIdTooltipBuyUnavailable = "BuyNotAvailable";
    m_strIdTooltipBuyPartial = "BuyPartial";
    m_strIdTooltipBuyFull = "BuyFull";
    m_strIdTooltipOneUnitPrice = "OneUnitPrice";
    m_strIdTooltipNumUnitsToBuy = "NumUnitsToBuy";
    m_strIdTooltipSummaryPrice = "SummaryPrice";
    m_strIdState = "State";

    m_normalColor = 0xFF404040;
    m_warnColor = 0xFF904040;
    m_colorYellow = 0xFFFED11B;
    m_colorGreen = 0xFF23961B;
    m_colorRed = 0xFF904040;
    // m_colorNormal is left as-is - the shipped ctor never assigns it.
}

AdvancedButton::AuxInfo::AuxInfo(AdvancedButton::AuxInfo const&) : AuxInfo()
{
}

// --- AdvancedButton::Pattern --------------------------------------------------

AdvancedButton::Pattern::Pattern()
{
    m_wndPattern = nullptr;
    m_btnPatternActiveZone = nullptr;
    m_wndPatternIco = nullptr;
    m_wndPatternName = nullptr;
    m_wndPatternPrice = nullptr;
    m_btnPatternAdvance = nullptr;
    m_wndPatternIdioticEmbossBtnAdvance = nullptr;
    m_wndPatternSlider = nullptr;
    m_btnPatternLeft = nullptr;
    m_btnPatternRight = nullptr;
    m_wndPatternUnits = nullptr;
    m_wndPatternFrame = nullptr;
    m_ref = 0;
}

AdvancedButton::Pattern::~Pattern()
{
    Clear();
}

void AdvancedButton::Pattern::Clear()
{
    m3d::ui::Wnd* widgets[] = {
        m_wndPattern,       m_btnPatternActiveZone, m_wndPatternIco,   m_wndPatternName,
        m_wndPatternPrice,  m_btnPatternAdvance,    m_wndPatternIdioticEmbossBtnAdvance,
        m_wndPatternSlider, m_btnPatternLeft,       m_btnPatternRight, m_wndPatternUnits,
        m_wndPatternFrame,
    };
    for (auto* w : widgets)
    {
        if (w)
        {
            w->DecRef();
        }
    }
    m_wndPattern = nullptr;
    m_btnPatternActiveZone = nullptr;
    m_wndPatternIco = nullptr;
    m_wndPatternName = nullptr;
    m_wndPatternPrice = nullptr;
    m_btnPatternAdvance = nullptr;
    m_wndPatternIdioticEmbossBtnAdvance = nullptr;
    m_wndPatternSlider = nullptr;
    m_btnPatternLeft = nullptr;
    m_btnPatternRight = nullptr;
    m_wndPatternUnits = nullptr;
    m_wndPatternFrame = nullptr;
}

void AdvancedButton::Pattern::IncRef()
{
    ++m_ref;
}

void AdvancedButton::Pattern::DecRef()
{
    if (--m_ref <= 0)
    {
        Clear();
    }
}

int AdvancedButton::Pattern::GetRef() const
{
    return m_ref;
}

// --- AdvancedButton lifetime ------------------------------------------------

AdvancedButton::AdvancedButton()
{
    m_btnActiveZone = nullptr;
    m_wndIco = nullptr;
    m_wndName = nullptr;
    m_wndPrice = nullptr;
    m_btnAdvance = nullptr;
    m_wndIdioticEmbossBtnAdvance = nullptr;
    m_wndSlider = nullptr;
    m_btnLeft = nullptr;
    m_btnRight = nullptr;
    m_wndUnits = nullptr;
    m_wndFrame = nullptr;
    m_mode = MODE_SIMPLE;
    m_objId = -1;
    m_workshopId = -1;
    m_unitsToBuy = 0;
    m_maxUnitsToBuy = 0;
    m_maxPossibleUnitsToBuy = 0;
    m_oneUnitPrice = 0.0f;
    m_summaryPrice = 0;
    m_prevUnitsToBuy = 0;
    m_prevMaxUnitsToBuy = 0;
    m_prevMaxPossibleUnitsToBuy = 0;
    m_prevOneUnitPrice = 0.0f;
    m_prevSummaryPrice = 0;
}

AdvancedButton::AdvancedButton(AdvancedButton const&) : AdvancedButton()
{
}

AdvancedButton::~AdvancedButton() = default;

m3d::Class* AdvancedButton::GetBaseClass()
{
    return &m3d::ui::Wnd::m_classWnd;
}

m3d::Class* AdvancedButton::GetRtClass() const
{
    return RT_CLASS_LOCAL(AdvancedButton);
}

// --- simple accessors -----------------------------------------------------

int AdvancedButton::GetObjId() const
{
    return m_objId;
}

int AdvancedButton::GetUnitsToBuy() const
{
    return (m_gameDataFlags & 1) != 0 ? m_wndSlider->GetNotch() : 0;
}

int AdvancedButton::GetPrice() const
{
    return GetUnitsPrice(GetUnitsToBuy());
}

int AdvancedButton::GetUnitsPrice(int units) const
{
    return ai::GetIntRepairPrice(GetPriceForOneUnit() * static_cast<float>(units));
}

int AdvancedButton::GetMaxPossibleUnitsToBuy() const
{
    const int maxUnits = GetMaxUnitsToBuy();
    const int money = ai::thePlayer ? ai::thePlayer->GetMoney() : 0;
    if (GetUnitsPrice(maxUnits) <= money)
    {
        return maxUnits;
    }
    const float oneUnitPrice = GetPriceForOneUnit();
    if (oneUnitPrice <= 0.001f)
    {
        return maxUnits;
    }
    return static_cast<int>(static_cast<float>(money) / oneUnitPrice);
}

ai::Workshop* AdvancedButton::GetWorkshop() const
{
    auto* obj = ai::theObjects->GetEntityByObjId(m_workshopId);
    if (obj && obj->IsKindOf(&ai::Workshop::m_classWorkshop))
    {
        return static_cast<ai::Workshop*>(obj);
    }
    return nullptr;
}

unsigned int AdvancedButton::EnumColor2Color(help::Color enumColor) const
{
    switch (enumColor)
    {
    case help::COLOR_RED:
        return m_aif.m_colorRed;
    case help::COLOR_GREEN:
        return m_aif.m_colorGreen;
    case help::COLOR_YELLOW:
        return m_aif.m_colorYellow;
    default:
        return m_aif.m_normalColor;
    }
}

CStr const& AdvancedButton::GetStrIdTooltipBuy() const
{
    return m_aif.m_strIdTooltipBuy;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyNotNeed() const
{
    return m_aif.m_strIdTooltipBuyNotNeed;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyUnavailable() const
{
    return m_aif.m_strIdTooltipBuyUnavailable;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyPartial() const
{
    return m_aif.m_strIdTooltipBuyPartial;
}

CStr const& AdvancedButton::GetStrIdTooltipBuyFull() const
{
    return m_aif.m_strIdTooltipBuyFull;
}

// --- setup / mode -------------------------------------------------------------

void AdvancedButton::SetMode(AdvancedButton::Mode mode)
{
    const Mode prev = m_mode;
    m_mode = mode;
    if (mode != prev)
    {
        OnChangeMode();
    }
}

int AdvancedButton::SetupForObj(int objId, int workshopId)
{
    if (Valid())
    {
        return 0;
    }
    if (objId == -1 || workshopId == -1 || !CreateFromPattern())
    {
        return 0;
    }
    m_objId = objId;
    m_workshopId = workshopId;
    UpdateServiceName();
    UpdateServiceIco();
    FullUpdate();
    OnChangeMode();
    return 1;
}

int AdvancedButton::SetupForWorkshop(int workshopId)
{
    if (!Valid() || m_objId == -1 || workshopId == -1)
    {
        return 0;
    }
    m_workshopId = workshopId;
    if (IsChildOf(M3D_APP))
    {
        FullUpdate();
    }
    return 1;
}

// --- pattern loading / child creation ---------------------------------------

namespace
{
    // Pulls one named child out of the source pattern dialog, unlinks it, and
    // rebases its origin to the wndPattern anchor. Returns the widget or
    // nullptr (logged) when missing / wrong type.
    m3d::ui::Wnd* GrabPatternChild(m3d::ui::Wnd* src, CStr const& name, const m3d::Class* kind,
                                   const m3d::ui::Wnd* anchor)
    {
        m3d::Object* c = src->GetChildByName(name);
        if (!c || !c->IsKindOf(kind))
        {
            M3D_LOG_INFO(CStr("GET_CHILD_PATTERN error - cannot find child wnd ") + name);
            return nullptr;
        }
        auto* w = static_cast<m3d::ui::Wnd*>(c);
        src->RemoveChild(w);
        BoundsBase<float> b = w->GetBounds();
        const BoundsBase<float> base = anchor->GetBounds();
        b.x0 -= base.x0;
        b.y0 -= base.y0;
        w->SetBounds(b, true);
        return w;
    }
}  // namespace

int AdvancedButton::LoadPattern(m3d::ui::Wnd* pattern)
{
    if (m_pattern.m_ref > 0)
    {
        return 1;
    }
    if (!pattern)
    {
        M3D_LOG_INFO(CStr("AdvancedButton::LoadPattern - error to create - invalid pattern wnd"));
        return 0;
    }

    m3d::Object* root = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (!root || !root->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO(CStr("AdvancedButton::LoadPattern - error to create - invalid pattern wnd"));
        return 0;
    }
    m_pattern.m_wndPattern = static_cast<m3d::ui::Wnd*>(root);
    pattern->RemoveChild(m_pattern.m_wndPattern);

    int ok = 1;
    const m3d::ui::Wnd* anchor = m_pattern.m_wndPattern;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_btnPatternActiveZoneName, &m3d::ui::ButtonWnd::m_classButtonWnd, anchor))
        m_pattern.m_btnPatternActiveZone = static_cast<m3d::ui::ButtonWnd*>(w);
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_wndPatternIcoName, &m3d::ui::ImageWnd::m_classImageWnd, anchor))
        m_pattern.m_wndPatternIco = static_cast<m3d::ui::ImageWnd*>(w);
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_wndPatternNameName, &m3d::ui::Wnd::m_classWnd, anchor))
        m_pattern.m_wndPatternName = w;
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_wndPatternPriceName, &m3d::ui::Wnd::m_classWnd, anchor))
        m_pattern.m_wndPatternPrice = w;
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_btnPatternAdvanceName, &m3d::ui::ButtonWnd::m_classButtonWnd, anchor))
        m_pattern.m_btnPatternAdvance = static_cast<m3d::ui::ButtonWnd*>(w);
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_wndPatternIdioticEmbossBtnAdvanceName, &m3d::ui::Wnd::m_classWnd, anchor))
        m_pattern.m_wndPatternIdioticEmbossBtnAdvance = w;
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_wndPatternSliderName, &m3d::ui::SliderWnd::m_classSliderWnd, anchor))
        m_pattern.m_wndPatternSlider = w;
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_btnPatternLeftName, &m3d::ui::ButtonWnd::m_classButtonWnd, anchor))
        m_pattern.m_btnPatternLeft = static_cast<m3d::ui::ButtonWnd*>(w);
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_btnPatternRightName, &m3d::ui::ButtonWnd::m_classButtonWnd, anchor))
        m_pattern.m_btnPatternRight = static_cast<m3d::ui::ButtonWnd*>(w);
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_wndPatternUnitsName, &m3d::ui::Wnd::m_classWnd, anchor))
        m_pattern.m_wndPatternUnits = w;
    else
        ok = 0;

    if (auto* w = GrabPatternChild(pattern, m_aif.m_wndPatternFrameName, &m3d::ui::Wnd::m_classWnd, anchor))
        m_pattern.m_wndPatternFrame = w;
    else
        return 0;

    return ok;
}

int AdvancedButton::CreateFromPattern()
{
    if (Valid())
    {
        return 0;
    }
    if (!help::IsWndValid(m_pattern.m_wndPattern))
    {
        M3D_LOG_INFO(CStr("AdvancedButton::CreateFromPattern error - null pattern"));
        return 0;
    }

    const m3d::ui::Wnd* p = m_pattern.m_wndPattern;
    if (!Create(CStr(), p->GetStyle(), p->GetBounds(), p->GetId()))
    {
        M3D_LOG_INFO(CStr("AdvancedButton::CreateFromPattern error - cannot create window"));
        return 0;
    }
    CopyWndPropsFromPattern(this, p);

    if (!CreateChildren())
    {
        M3D_LOG_INFO(CStr("AdvancedButton::CreateFromPattern error - cannot create children"));
        return 0;
    }

    m_gameDataFlags |= 1u;
    UpdateControlsOnNewFrame(true);
    return 1;
}

int AdvancedButton::CreateChildren()
{
    if (!help::IsWndValid(m_pattern.m_btnPatternActiveZone) || !help::IsWndValid(m_pattern.m_wndPatternIco) ||
        !help::IsWndValid(m_pattern.m_wndPatternName) || !help::IsWndValid(m_pattern.m_wndPatternPrice) ||
        !help::IsWndValid(m_pattern.m_btnPatternAdvance) ||
        !help::IsWndValid(m_pattern.m_wndPatternIdioticEmbossBtnAdvance) ||
        !help::IsWndValid(m_pattern.m_wndPatternSlider) || !help::IsWndValid(m_pattern.m_btnPatternLeft) ||
        !help::IsWndValid(m_pattern.m_btnPatternRight) || !help::IsWndValid(m_pattern.m_wndPatternUnits) ||
        !help::IsWndValid(m_pattern.m_wndPatternFrame))
    {
        return 0;
    }

    int res = 1;

    if (auto* w = CreateChildFromPattern(this, "Wnd", m_pattern.m_wndPatternFrame, "CREATE_WND (frame)"))
        m_wndFrame = w;
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "ButtonWnd", m_pattern.m_btnPatternActiveZone, "CREATE_BUTTON"))
    {
        m_btnActiveZone = static_cast<m3d::ui::ButtonWnd*>(w);
        CopyButtonImages(m_btnActiveZone, m_pattern.m_btnPatternActiveZone);
    }
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "ImageWnd", m_pattern.m_wndPatternIco, "CREATE_IMAGE"))
    {
        m_wndIco = static_cast<m3d::ui::ImageWnd*>(w);
        m_wndIco->SetImage(m_pattern.m_wndPatternIco->GetImage());
    }
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "Wnd", m_pattern.m_wndPatternName, "CREATE_WND (name)"))
        m_wndName = w;
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "Wnd", m_pattern.m_wndPatternPrice, "CREATE_WND (price)"))
        m_wndPrice = w;
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "Wnd", m_pattern.m_wndPatternIdioticEmbossBtnAdvance, "CREATE_WND (emboss)"))
        m_wndIdioticEmbossBtnAdvance = w;
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "ButtonWnd", m_pattern.m_btnPatternAdvance, "CREATE_BUTTON (advance)"))
    {
        m_btnAdvance = static_cast<m3d::ui::ButtonWnd*>(w);
        CopyButtonImages(m_btnAdvance, m_pattern.m_btnPatternAdvance);
    }
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "BuyServiceSlider", m_pattern.m_wndPatternSlider, "CREATE_SLIDER"))
        m_wndSlider = static_cast<BuyServiceSlider*>(w);
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "ButtonWnd", m_pattern.m_btnPatternLeft, "CREATE_BUTTON (left)"))
    {
        m_btnLeft = static_cast<m3d::ui::ButtonWnd*>(w);
        CopyButtonImages(m_btnLeft, m_pattern.m_btnPatternLeft);
    }
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "ButtonWnd", m_pattern.m_btnPatternRight, "CREATE_BUTTON (right)"))
    {
        m_btnRight = static_cast<m3d::ui::ButtonWnd*>(w);
        CopyButtonImages(m_btnRight, m_pattern.m_btnPatternRight);
    }
    else
        res = 0;

    if (auto* w = CreateChildFromPattern(this, "Wnd", m_pattern.m_wndPatternUnits, "CREATE_WND (units)"))
        m_wndUnits = w;
    else
        res = 0;

    return res;
}

// --- notifications ----------------------------------------------------------

int AdvancedButton::OnBeforeAddToWndStation()
{
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int AdvancedButton::OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
{
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (msg != 1)
    {
        return 0;
    }
    switch (id)
    {
    case 0x495D4u:  // btnAdvance
        OnBtnAdvanceClick();
        return 1;
    case 0x495D5u:  // btnActiveZone (buy)
        OnBuy();
        return 1;
    case 0x495D7u:  // btnLeft
        OnLeft();
        return 1;
    case 0x495D8u:  // btnRight
        OnRight();
        return 1;
    default:
        return 0;
    }
}

void AdvancedButton::OnBtnAdvanceClick()
{
    m3d::AIParam data(static_cast<int>(m_mode == MODE_SIMPLE));
    CallParentNotify(0x17u, data, true);
}

void AdvancedButton::OnBuy()
{
    if (GetUnitsToBuy() != 0)
    {
        m3d::AIParam data;
        CallParentNotify(0x16u, data, false);
    }
}

void AdvancedButton::OnChangeMode()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndSlider->SetNotch(m_wndSlider->GetMaxPossible());
    m_wndFrame->ShowWindow(m_mode != MODE_SIMPLE);
    RecalcLayot();
    UpdateAdvanceButtonState();
}

void AdvancedButton::OnLeft()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndSlider->SetNotch(m_wndSlider->GetNotch() - 1);
    }
}

void AdvancedButton::OnRight()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndSlider->SetNotch(m_wndSlider->GetNotch() + 1);
    }
}

// --- layout / value updates ------------------------------------------------

void AdvancedButton::RecalcLayot()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    const m3d::ui::Wnd* advancedControls[] = {m_btnLeft, m_btnRight, m_wndSlider, m_wndUnits, m_wndFrame};

    float maxBottom = 0.0f;
    for (m3d::Object* c = GetFirstChild(); c; c = c->GetNextSibling())
    {
        if (!c->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            continue;
        }
        auto* w = static_cast<m3d::ui::Wnd*>(c);
        const BoundsBase<float> b = w->GetBounds();
        const float bottom = b.y0 + b.height;
        if (bottom <= maxBottom)
        {
            continue;
        }
        if (m_mode == MODE_ADVANCED)
        {
            maxBottom = bottom;
        }
        else if (m_mode == MODE_SIMPLE)
        {
            bool isAdvancedControl = false;
            for (const m3d::ui::Wnd* ac : advancedControls)
            {
                if (ac == w)
                {
                    isAdvancedControl = true;
                    break;
                }
            }
            if (!isAdvancedControl)
            {
                maxBottom = bottom;
            }
        }
    }

    float frameW = GetClientEdges()[m3d::ui::BOTTOM];
    if ((m_style & m3d::ui::WS_NOFRAME) == 0 && (m_paneFlags & 2) != 0)
    {
        if (m3d::ui::Pane* pane = m_gfx->GetPane(GetPaneName()))
        {
            if (m3d::ui::Frame* fr = pane->m_frame[0])
            {
                frameW += static_cast<float>(fr->m_barUsedWidth);
            }
        }
    }

    m_bounds.height = frameW + maxBottom;

    m3d::AIParam data;
    CallParentNotify(0x15u, data, false);
}

void AdvancedButton::FullUpdate()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        UpdateValues();
        UpdateControlsOnNewFrame(false);
        UpdatePrevValues();
    }
}

void AdvancedButton::UpdateValues()
{
    m_unitsToBuy = GetUnitsToBuy();
    m_maxUnitsToBuy = GetMaxUnitsToBuy();
    m_maxPossibleUnitsToBuy = GetMaxPossibleUnitsToBuy();
    m_oneUnitPrice = GetPriceForOneUnit();
    m_summaryPrice = GetUnitsPrice(m_unitsToBuy);
}

void AdvancedButton::UpdatePrevValues()
{
    m_prevUnitsToBuy = m_unitsToBuy;
    m_prevMaxUnitsToBuy = m_maxUnitsToBuy;
    m_prevMaxPossibleUnitsToBuy = m_maxPossibleUnitsToBuy;
    m_prevOneUnitPrice = m_oneUnitPrice;
    m_prevSummaryPrice = m_summaryPrice;
}

void AdvancedButton::UpdateControlsOnNewFrame(bool bForce)
{
    UpdateSlider(bForce);
    UpdateUnitsControls(bForce);
    UpdatePriceControls(bForce);
    UpdateTooltip(bForce);
}

void AdvancedButton::UpdateSlider(bool)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndSlider->SetMinMax(0, m_maxUnitsToBuy);
    m_wndSlider->SetMaxPossible(m_maxPossibleUnitsToBuy);
    if (m_mode == MODE_SIMPLE)
    {
        m_wndSlider->SetNotch(m_wndSlider->GetMaxPossible());
    }
    UpdateLeftRightButtonsState();
}

void AdvancedButton::UpdateLeftRightButtonsState()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    const int notch = m_wndSlider->GetNotch();
    m_btnLeft->EnableWindow(notch > m_wndSlider->GetMin());
    m_btnRight->EnableWindow(notch < m_wndSlider->GetMaxPossible());
}

void AdvancedButton::UpdateAdvanceButtonState()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_btnAdvance->SetPane(m_mode != MODE_SIMPLE ? m_aif.m_toSimplePaneName : m_aif.m_toAdvancePaneName);

    const CStr& tooltipId = m_mode != MODE_SIMPLE ? m_aif.m_strIdTooltipToSimple : m_aif.m_strIdTooltipToAdvanced;
    CStr tooltip = M3D_APP->GetStringByStringId0(tooltipId);
    m_btnAdvance->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

void AdvancedButton::UpdatePriceControls(bool bForce)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (m_summaryPrice != m_prevSummaryPrice || bForce)
    {
        m_wndPrice->SetText(CStr(m_summaryPrice));
    }

    int colorSel;
    if (m_maxUnitsToBuy != 0)
    {
        const int pct = static_cast<int>(static_cast<float>(m_unitsToBuy) / static_cast<float>(m_maxUnitsToBuy) * 100.0f);
        if (pct >= 50)
        {
            colorSel = (pct < 100) ? 2 : 1;
        }
        else
        {
            colorSel = 0;
        }
    }
    else
    {
        colorSel = 3;
    }

    unsigned int color;
    switch (colorSel)
    {
    case 0:
        color = m_aif.m_colorRed;
        break;
    case 1:
        color = m_aif.m_colorGreen;
        break;
    case 2:
        color = m_aif.m_colorYellow;
        break;
    default:
        color = m_aif.m_normalColor;
        break;
    }

    if (color != m_wndPrice->GetTextColor() || bForce)
    {
        m_wndPrice->SetTextColor(color);
    }
}

void AdvancedButton::UpdateUnitsControls(bool bForce)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_unitsToBuy != m_prevUnitsToBuy || m_maxUnitsToBuy != m_prevMaxUnitsToBuy || bForce)
    {
        m_wndUnits->SetText(CStr(m_unitsToBuy) + CStr("/") + CStr(m_maxUnitsToBuy));
    }
}

void AdvancedButton::UpdateServiceName()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndName->SetText(GetServiceName());
    }
}

void AdvancedButton::UpdateServiceIco()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndIco->SetImage(GetServiceIco());
    }
}

void AdvancedButton::UpdateTooltip(bool bForce)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!bForce && m_unitsToBuy == m_prevUnitsToBuy && m_maxUnitsToBuy == m_prevMaxUnitsToBuy &&
        m_maxPossibleUnitsToBuy == m_prevMaxPossibleUnitsToBuy && m_oneUnitPrice == m_prevOneUnitPrice &&
        m_summaryPrice == m_prevSummaryPrice)
    {
        return;
    }

    // Pick the "buy state" phrase id (empty => omit that line).
    CStr buyStateId;
    if (m_maxUnitsToBuy == 0)
    {
        buyStateId = GetStrIdTooltipBuyNotNeed();
    }
    else if (m_maxPossibleUnitsToBuy == 0)
    {
        buyStateId = GetStrIdTooltipBuyUnavailable();
    }
    else if (m_unitsToBuy != 0 && m_unitsToBuy < m_maxUnitsToBuy)
    {
        buyStateId = GetStrIdTooltipBuyPartial();
    }
    else if (m_unitsToBuy == m_maxUnitsToBuy)
    {
        buyStateId = GetStrIdTooltipBuyFull();
    }

    CStr tooltip = M3D_APP->GetStringByStringId0(GetStrIdTooltipBuy());
    if (!buyStateId.empty())
    {
        tooltip += CStr("|") + M3D_APP->GetStringByStringId0(m_aif.m_strIdState) + CStr(": ") +
                   M3D_APP->GetStringByStringId0(buyStateId);
    }
    if (m_mode == MODE_ADVANCED)
    {
        tooltip += CStr("||") + M3D_APP->GetStringByStringId0(m_aif.m_strIdTooltipOneUnitPrice) + CStr(": ") +
                   help::ftoa(m_oneUnitPrice, -1);
        tooltip += CStr("|") + M3D_APP->GetStringByStringId0(m_aif.m_strIdTooltipNumUnitsToBuy) + CStr(": ") +
                   CStr(m_unitsToBuy);
        tooltip += CStr("|") + M3D_APP->GetStringByStringId0(m_aif.m_strIdTooltipSummaryPrice) + CStr(": ") +
                   CStr(m_summaryPrice);
    }

    m_btnActiveZone->SetProperty(PROP_WND_TOOLTIP, &tooltip);
}

// ===========================================================================
//  AdvancedList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(AdvancedList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(AdvancedList);

AdvancedList::AuxInfo::AuxInfo()
{
    m_space = 6.0f;
}

AdvancedList::AdvancedList()
{
    m_switchB.x0 = 0.0f;
    m_switchB.y0 = 0.0f;
    m_switchB.width = 0.0f;
    m_switchB.height = 0.0f;
    m_workshopId = -1;
}

AdvancedList::AdvancedList(AdvancedList const&) : AdvancedList()
{
}

AdvancedList::~AdvancedList()
{
    ClearItems();
    AdvancedButton::m_pattern.DecRef();
}

m3d::Class* AdvancedList::GetBaseClass()
{
    return &m3d::ui::Wnd::m_classWnd;
}

m3d::Class* AdvancedList::GetRtClass() const
{
    return RT_CLASS_LOCAL(AdvancedList);
}

ai::Workshop* AdvancedList::GetWorkshop() const
{
    auto* obj = ai::theObjects->GetEntityByObjId(m_workshopId);
    if (obj && obj->IsKindOf(&ai::Workshop::m_classWorkshop))
    {
        return static_cast<ai::Workshop*>(obj);
    }
    return nullptr;
}

AdvancedButton* AdvancedList::NewItem() const
{
    return static_cast<AdvancedButton*>(M3D_KERNEL->New("AdvancedButton"));
}

void AdvancedList::SetSwitchWndBounds(BoundsBase<float> const& b)
{
    m_switchB = b;
    if (IsChildOf(M3D_APP))
    {
        RecalcLayot();
    }
}

int AdvancedList::SetupForWorkshop(int workshopId)
{
    m_workshopId = workshopId;
    if (workshopId != -1 && !GetWorkshop())
    {
        return 0;
    }
    int res = 1;
    for (auto* item : m_items)
    {
        if (item)
        {
            res &= item->SetupForWorkshop(m_workshopId);
        }
    }
    return res;
}

// --- item management ------------------------------------------------------

void AdvancedList::ClearItems()
{
    for (auto* item : m_items)
    {
        if (item)
        {
            item->DecRef();
        }
    }
    m_items.clear();
}

int AdvancedList::CreateItems()
{
    ClearItems();
    if (!GetWorkshop())
    {
        return 0;
    }

    int res = 1;
    const std::vector<int> objIds = GetObjIds();
    for (int objId : objIds)
    {
        res &= AddItem(objId);
    }
    RecalcLayot();
    return res;
}

int AdvancedList::AddItem(int objId)
{
    if (objId == -1)
    {
        return 0;
    }
    AdvancedButton* btn = NewItem();
    if (!btn)
    {
        return 0;
    }
    if (!btn->SetupForObj(objId, m_workshopId))
    {
        btn->DecRef();
        return 0;
    }
    m_items.push_back(btn);
    AddChild(btn);
    return 1;
}

void AdvancedList::SetAdvancedModeForButton(AdvancedButton* btn)
{
    for (auto* item : m_items)
    {
        if (!item)
        {
            continue;
        }
        const AdvancedButton::Mode prev = item->m_mode;
        item->m_mode = (item == btn) ? AdvancedButton::MODE_ADVANCED : AdvancedButton::MODE_SIMPLE;
        if (item->m_mode != prev)
        {
            item->OnChangeMode();
        }
    }
}

void AdvancedList::OnAdvancedModeChanged(m3d::ui::Wnd* srcWnd, m3d::AIParam const& data)
{
    if (!srcWnd || !srcWnd->IsKindOf(&AdvancedButton::m_classAdvancedButton))
    {
        return;
    }
    const int id = data.GetAsID();
    if (id >= 0 && id <= 1)
    {
        SetAdvancedModeForButton(id == 1 ? static_cast<AdvancedButton*>(srcWnd) : nullptr);
    }
}

// --- game data ----------------------------------------------------------------

int AdvancedList::GameDataSetup()
{
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        if (!AdvancedButton::LoadPattern(this))
        {
            M3D_LOG_INFO(CStr("AdvancedList::GameDataSetup error - cannot load pattern for AdvancedButton"));
            res = 0;
        }
        AdvancedButton::m_pattern.IncRef();
        RemoveAllChildren();
        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO(CStr("AdvancedList: error - fail to init because of a bad resource"));
    return 0;
}

int AdvancedList::GameDataClear(bool)
{
    ClearItems();
    return 1;
}

int AdvancedList::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 64:
        OnPlayerVehicleChanged();
        break;
    case 65:
        OnVehiclePartChanged(data);
        return 1;
    case 89:
        OnNewFrame();
        return 1;
    default:
        break;
    }
    return 1;
}

int AdvancedList::OnBeforeAddToWndStation()
{
    if ((m_gameDataFlags & 4) != 0)
    {
        CreateItems();
        m_gameDataFlags &= ~4u;
    }
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int AdvancedList::OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id != 300400)
    {
        return 0;
    }
    if (msg == 21)
    {
        RecalcLayot();
        return 1;
    }
    if (msg == 22)
    {
        if (from && from->IsKindOf(&AdvancedButton::m_classAdvancedButton))
        {
            BuyService(static_cast<AdvancedButton*>(from));
            return 1;
        }
        return 1;
    }
    if (msg == 23)
    {
        OnAdvancedModeChanged(from, data);
    }
    return 0;
}

void AdvancedList::OnPlayerVehicleChanged()
{
    if (IsChildOf(M3D_APP))
    {
        CreateItems();
    }
    else
    {
        m_gameDataFlags |= 4u;
    }
}

void AdvancedList::OnVehiclePartChanged(void* data)
{
    if (!data)
    {
        return;
    }
    auto* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
    const int playerVehicleId = vehicle ? vehicle->GetId() : -1;
    if (static_cast<const m3d::Event*>(data)->m_intEv[0] != playerVehicleId)
    {
        return;
    }
    if (IsChildOf(M3D_APP))
    {
        CreateItems();
    }
    else
    {
        m_gameDataFlags |= 4u;
    }
}

void AdvancedList::OnNewFrame()
{
    for (auto* item : m_items)
    {
        if (item)
        {
            item->FullUpdate();
        }
    }
}

void AdvancedList::RecalcLayot()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    float frameW = 0.0f;
    if ((m_style & m3d::ui::WS_NOFRAME) == 0 && (m_paneFlags & 2) != 0)
    {
        if (m3d::ui::Pane* pane = m_gfx->GetPane(GetPaneName()))
        {
            if (m3d::ui::Frame* fr = pane->m_frame[0])
            {
                frameW = static_cast<float>(fr->m_barUsedWidth);
            }
        }
    }

    float y = GetClientEdges()[m3d::ui::TOP] + frameW;
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        AdvancedButton* item = m_items[i];
        if (!item)
        {
            continue;
        }
        BoundsBase<float> b = item->GetBounds();
        b.y0 = y;
        item->SetBounds(b, true);
        y += b.height;
        if (i != m_items.size() - 1)
        {
            y += m_aif.m_space;
        }
    }

    float totalH = 0.0f;
    if (!m_items.empty() && m_items.back())
    {
        const BoundsBase<float> lb = m_items.back()->GetBounds();
        totalH = lb.y0 + lb.height + frameW + GetClientEdges()[m3d::ui::BOTTOM];
    }

    float newY0 = (m_switchB.y0 + m_switchB.height) - totalH;
    if (newY0 + totalH > 768.0f)
    {
        newY0 = 768.0f - totalH;
    }
    m_bounds.height = totalH;
    m_bounds.y0 = newY0;
}

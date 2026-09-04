#include "checklist.h"

#include <cmath>

#include <core/kernel.h>
#include <core/log.h>

#include <game/m3dgame.h>
#include "game/uimanager/truxxuimanager.h"

#include <ui/image.h>
#include <ui/textbox.h>
#include <ui/ui_srv.h>

// ============================================================================
//  CheckList
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(CheckList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CheckList);

CheckList::CheckList()
{
    // NOTE: the shipped ctor also seeds a couple of ListBoxWnd base fields (a -1
    // selection index and a 2.0f spacing) that the retruxx template ctor sets.
}

CheckList::CheckList(CheckList const& rhs) : ListBoxWnd<CheckButton*>(rhs)
{
    // RVA 0xD1590: fresh, empty list.
}

CheckList::~CheckList()
{
    // Items are owned children, released by the base destructor.
}

m3d::Object* CheckList::Clone()
{
    // RVA 0xD1590
    return new CheckList(*this);
}

m3d::Object* CheckList::CreateObject()
{
    // RVA 0xD1E20
    return new CheckList;
}

m3d::Class* CheckList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* CheckList::GetClass() const
{
    // RVA 0xD1E10
    return RT_CLASS_LOCAL(CheckList);
}

int CheckList::CreateFromPattern(m3d::ui::Wnd* pattern, bool deleteSrc)
{
    // RVA 0xD1FA0
    if (!pattern || !pattern->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
    {
        M3D_LOG_INFO("CheckList::CreateFromPattern error - null patternWnd");
        return 0;
    }

    if (!Create(pattern->GetBounds(), pattern->GetStyle(), pattern->GetId()))
    {
        M3D_LOG_INFO("CheckList::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetStyle(pattern->GetStyle());
    SetText(pattern->GetText());
    SetId(pattern->GetId());
    SetName(pattern->GetName());
    SetBounds(pattern->GetBounds(), true);
    SetDefaultFont(pattern->GetDefaultFont());
    SetWrapMode(pattern->GetWrapMode());
    SetFormatMode(pattern->GetFormatMode());
    SetColor(pattern->GetColor());
    SetTextColor(pattern->GetTextColor());
    SetTextColorDisabled(pattern->GetTextColorDisabled());
    SetClientEdges(pattern->GetClientEdges());
    SetPane(pattern->GetPaneName());
    SetPaneFlags(pattern->GetPaneFlags());
    SetScrollPane(pattern->GetScrollPaneName());
    SetBackground(pattern->GetBackground());

    CStr tooltip;
    pattern->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(pattern->GetOnShowAnimation());
    SetOnHideAnimation(pattern->GetOnHideAnimation());

    SetDrawFlags(static_cast<m3d::ui::TextBoxWnd*>(pattern)->GetDrawFlags());

    m3d::Object* parent = pattern->GetParent();
    if (!parent || !parent->IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        M3D_LOG_INFO("CheckList::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(pattern);
        delete pattern;
    }
    m_gameDataFlags |= 1u;
    return 1;
}

int CheckList::AddButtonByName(CStr const& name, CStr const& fullName)
{
    // RVA 0xD2710
    if (name.empty())
    {
        return -1;
    }
    auto* btn = static_cast<CheckButton*>(M3D_KERNEL->New("CheckButton"));
    if (!btn)
    {
        return -1;
    }

    // NOTE: the shipped build feeds the client bounds through oddly - the button
    // origin becomes (clientBounds.y0, clientBounds.width) and its "width" is
    // clientBounds.height. The list re-positions every row in RenderItem, so the
    // initial placement is irrelevant.
    BoundsBase<float> const clientB = GetClientBounds();
    PointBase<float> const origin{clientB.y0, clientB.width};
    if (!btn->SetUp(name, origin, clientB.height, fullName))
    {
        delete btn;
        return -1;
    }
    AddChild(btn);
    return AddItem(btn);
}

int CheckList::SelectButtonByName(CStr const& name)
{
    // RVA 0xD27D0
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        CheckButton* btn = m_items[i].m_item;
        if (btn && btn->GetName() == name)
        {
            SetCurSel(static_cast<int>(i));
            return 1;
        }
    }
    return 0;
}

void CheckList::SetCurSel(int i)
{
    // RVA 0xD2630
    m3d::ui::ListBoxWnd<CheckButton*>::SetCurSel(i);
    if (m_curSel < 0 || m_curSel >= static_cast<int>(m_items.size()))
    {
        SelectButton(nullptr);
    }
    else
    {
        SelectButton(m_items[i].m_item);
    }
}

void CheckList::SelectButton(CheckButton* btn)
{
    // RVA 0xD26A0
    for (auto const& item : m_items)
    {
        if (item.m_item)
        {
            item.m_item->Select(item.m_item == btn);
        }
    }
}

int CheckList::CompareItem(int, int)
{
    // RVA 0xD2620
    return 0;
}

int CheckList::DeleteItem(int itemIdx)
{
    // RVA 0xD25E0
    if (CheckButton* item = m_items[itemIdx].m_item)
    {
        item->DecRef();
    }
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

int CheckList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    // RVA 0xD2340
    BoundsBase<float> const& vb = m_items[itemIdx].m_item->GetVirtualBounds();
    bounds.x0 = 0.0f;
    bounds.y0 = 0.0f;
    bounds.width = vb.width;
    bounds.height = vb.height;
    return 1;
}

int CheckList::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    // RVA 0xD23B0
    CheckButton* item = m_items[itemIdx].m_item;
    BoundsBase<float> const& vb = item->GetVirtualBounds();

    float const rowY = (di.m_clientRect.y0 - di.m_originalRect.y0) + org.y;

    BoundsBase<float> virtualB;
    virtualB.x0 = vb.x0;
    virtualB.y0 = rowY;
    virtualB.width = vb.width;
    virtualB.height = vb.height;

    PointBase<float> const screenPt = ToScreen(PointBase<float>{virtualB.x0, rowY});
    BoundsBase<float> b;
    b.x0 = screenPt.x;
    b.y0 = screenPt.y;
    b.width = virtualB.width;
    b.height = virtualB.height;

    float const clientRight = di.m_clientRect.x0 + di.m_clientRect.width;
    float const clientBottom = di.m_clientRect.y0 + di.m_clientRect.height;
    if (b.x0 > clientRight || di.m_clientRect.x0 > b.x0 + b.width || b.y0 > clientBottom ||
        di.m_clientRect.y0 > b.y0 + b.height)
    {
        b.x0 = 0.0f;
        b.y0 = 0.0f;
        b.width = 0.0f;
        b.height = 0.0f;
    }
    else
    {
        float const left = (b.x0 <= di.m_clientRect.x0) ? di.m_clientRect.x0 : b.x0;
        float const right = (clientRight > b.x0 + b.width) ? b.x0 + b.width : clientRight;
        float const top = (b.y0 <= di.m_clientRect.y0) ? di.m_clientRect.y0 : b.y0;
        float const bottom = (clientBottom <= b.y0 + b.height) ? clientBottom : b.y0 + b.height;
        b.x0 = left;
        b.y0 = top;
        b.width = right - left;
        b.height = bottom - top;
    }

    b = ToWindow(b);
    item->SetBounds(b, virtualB);
    return 1;
}

// ============================================================================
//  CheckButton
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(CheckButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CheckButton);

CheckButton::AuxInfo::AuxInfo()
{
    // RVA 0xD1680
    m_icoTexName = "RecordIco";
    m_icoSz = PointBase<float>{16.0f, 16.0f};
    m_spaceX = 5.0f;
    m_spaceY = 3.0f;
    m_textColorSel = 0xFF404040u;
    m_textColorUnsel = 0xFF7F7F7Fu;
}

CheckButton::CheckButton()
{
    m_ico = nullptr;
    m_lbl = nullptr;
    m_virtualBounds = BoundsBase<float>{0.0f, 0.0f, 0.0f, 0.0f};
}

CheckButton::CheckButton(CheckButton const& rhs) : ButtonWnd(rhs)
{
    m_ico = nullptr;
    m_lbl = nullptr;
    m_virtualBounds = BoundsBase<float>{0.0f, 0.0f, 0.0f, 0.0f};
}

CheckButton::~CheckButton()
{
    // The icon and label are owned children, released by the base destructor.
}

m3d::Object* CheckButton::Clone()
{
    // RVA 0xD1530
    return new CheckButton(*this);
}

m3d::Object* CheckButton::CreateObject()
{
    // RVA 0xD1650
    return new CheckButton;
}

m3d::Class* CheckButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* CheckButton::GetClass() const
{
    // RVA 0xD1640
    return RT_CLASS_LOCAL(CheckButton);
}

CStr const& CheckButton::GetName() const
{
    // RVA 0xD1C60
    return m_name;
}

CStr const& CheckButton::GetFullName() const
{
    // RVA 0xD1CA0
    return m_fullName;
}

BoundsBase<float> const& CheckButton::GetVirtualBounds() const
{
    // RVA 0xD1CB0
    return m_virtualBounds;
}

CStr CheckButton::CalcFullName() const
{
    // RVA 0xD1C70
    return M3D_APP->GetStringByStringId0(m_name);
}

void CheckButton::Select(bool bSelect)
{
    // RVA 0xD1C00
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m3d::rend::TexHandle const tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_icoTexName, bSelect ? 0 : 1);
    m_ico->SetImage(tex);
    m_lbl->SetTextColor(bSelect ? m_aif.m_textColorSel : m_aif.m_textColorUnsel);
}

void CheckButton::SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin)
{
    // RVA 0xD1CC0 - a lightweight override that skips the base layout pass.
    m_bounds = rect;
    if (bUpdateBaseOrigin)
    {
        m_baseOrigin.x = m_bounds.x0;
        m_baseOrigin.y = m_bounds.y0;
    }
}

void CheckButton::SetBounds(BoundsBase<float> const& rect, BoundsBase<float> const& virtualRect)
{
    // RVA 0xD1D10
    float const oldVirtualY0 = m_virtualBounds.y0;
    BoundsBase<float> const oldB = GetBounds();

    m_virtualBounds = virtualRect;
    SetBounds(rect, true);

    float const shift = (m_virtualBounds.y0 - m_bounds.y0) - (oldVirtualY0 - oldB.y0);
    if (std::fabs(shift) > 0.001f)
    {
        for (m3d::Object* child = GetFirstChild(); child; child = child->GetNextSibling())
        {
            if (auto* w = RT_DYNCAST(child, m3d::ui::Wnd))
            {
                BoundsBase<float> childB = w->GetBounds();
                childB.y0 += shift;
                w->SetBounds(childB, true);
            }
        }
    }
}

int CheckButton::SetUp(CStr const& name, PointBase<float> const& origin, float width, CStr const& fullName)
{
    // RVA 0xD1880
    if (Valid() || (m_gameDataFlags & 1) != 0 || name.empty())
    {
        return 0;
    }

    BoundsBase<float> const rc{origin.x, origin.y, 0.0f, 0.0f};
    if (!m3d::ui::ButtonWnd::Create({}, 0x40260u, rc, 0))
    {
        return 0;
    }

    m_name = name;
    m_fullName = fullName.empty() ? CalcFullName() : fullName;

    // --- record icon --------------------------------------------------------
    BoundsBase<float> const icoB{0.0f, m_aif.m_spaceY, m_aif.m_icoSz.x, m_aif.m_icoSz.y};
    m_ico = static_cast<m3d::ui::ImageWnd*>(M3D_KERNEL->New("ImageWnd"));
    if (!m_ico->CreateImageWnd(icoB, m3d::rend::TexHandle()))
    {
        delete m_ico;
        m_ico = nullptr;
        return 0;
    }
    AddChild(m_ico);

    // --- text label -------------------------------------------------------
    float const lblMaxWidth = width - m_aif.m_spaceX - m_aif.m_icoSz.x;
    PointBase<float> const nameSz = GetGfxServer()->MeasureText(m_fullName, 0, m3d::TW_WORD_WRAP, lblMaxWidth);

    BoundsBase<float> lblB;
    lblB.x0 = icoB.width + icoB.x0 + m_aif.m_spaceX;
    lblB.y0 = m_aif.m_spaceY;
    lblB.width = lblMaxWidth;
    lblB.height = nameSz.y;

    m_lbl = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_lbl->Create(m_fullName, 832u, lblB, 0))
    {
        delete m_lbl;
        m_lbl = nullptr;
        return 0;
    }
    m_lbl->SetDefaultFont(0);
    m_lbl->SetWrapMode(m3d::TW_WORD_WRAP);
    AddChild(m_lbl);

    float const contentH = (lblB.height <= icoB.height) ? icoB.height : lblB.height;
    BoundsBase<float> const selfB{origin.x, origin.y, width, m_aif.m_spaceY * 2.0f + contentH};
    SetBounds(selfB, true);

    m_gameDataFlags |= 1u;
    m_virtualBounds = m_bounds;
    return 1;
}

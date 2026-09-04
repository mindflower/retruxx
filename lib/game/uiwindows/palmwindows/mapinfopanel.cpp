#include "mapinfopanel.h"

#include <core/kernel.h>
#include <core/log.h>

#include <game/m3dgame.h>
#include "game/uimisc/guihelper.h"
#include "game/uimisc/levelinfo.h"

#include <ui/button.h>
#include <ui/image.h>
#include <ui/linewnd.h>
#include <ui/textbox.h>

#include "mapselllist.h"

RT_CLASS_EXPORTS_BEGIN(MapInfoPanel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapInfoPanel);

MapInfoPanel::AuxInfo::AuxInfo()
{
    // RVA 0xED330
    m_lblObjectNameName = "lblObjectName";
    m_lblSellName = "wndSell";
    m_lblBuyName = "wndBuy";
    m_lnHorz0Name = "lnHorz0";
    m_lnHorz1Name = "lnHorz1";
    m_lnVert0Name = "lnVert0";
    m_lnVert1Name = "lnVert1";
    m_txtObjectDizName = "txtObjectDiz";
    m_wndSellListName = "wndSellList";
    m_btnModeTradeName = "btnModeTrade";
    m_btnModeClansName = "btnModeClans";
    m_wndEmbossBtnModeTradeName = "emboss_btnModeTrade";
    m_wndEmbossBtnModeClansName = "emboss_btnModeClans";
    m_wndFrameName = "wndInfoFrame";
    m_modeBtnPaneNameUnselected = "PaneBtnGray1";
    m_modeBtnPaneNameSelected = "PaneBtnGray1Selected";
    m_strIdRuined = "Ruined";
}

// --- lifecycle ---------------------------------------------------------------------------------

MapInfoPanel::MapInfoPanel()
{
    m_lblObjectName = nullptr;
    m_txtObjectDiz = nullptr;
    m_wndSellList = nullptr;
    m_wndSell = nullptr;
    m_wndBuy = nullptr;
    m_lnHorz0 = nullptr;
    m_lnHorz1 = nullptr;
    m_lnVert0 = nullptr;
    m_lnVert1 = nullptr;
    m_btnModeTrade = nullptr;
    m_btnModeClans = nullptr;
    m_wndEmbossBtnModeTrade = nullptr;
    m_wndEmbossBtnModeClans = nullptr;
    m_wndFrame = nullptr;
    m_objectInfo = nullptr;
    m_mode = LocalMapWnd::MODE_TRADE;
    m_fromSaveList = false;
}

MapInfoPanel::MapInfoPanel(MapInfoPanel const& rhs) : Wnd(rhs)
{
    // RVA 0xED2A0: fresh window + AuxInfo; every child pointer null.
    m_lblObjectName = nullptr;
    m_txtObjectDiz = nullptr;
    m_wndSellList = nullptr;
    m_wndSell = nullptr;
    m_wndBuy = nullptr;
    m_lnHorz0 = nullptr;
    m_lnHorz1 = nullptr;
    m_lnVert0 = nullptr;
    m_lnVert1 = nullptr;
    m_btnModeTrade = nullptr;
    m_btnModeClans = nullptr;
    m_wndEmbossBtnModeTrade = nullptr;
    m_wndEmbossBtnModeClans = nullptr;
    m_wndFrame = nullptr;
    m_objectInfo = nullptr;
    m_mode = LocalMapWnd::MODE_TRADE;
    m_fromSaveList = false;
}

MapInfoPanel::~MapInfoPanel()
{
    // Child windows are released by the base destructor.
}

m3d::Object* MapInfoPanel::Clone()
{
    // RVA 0xED2A0
    return new MapInfoPanel(*this);
}

m3d::Object* MapInfoPanel::CreateObject()
{
    // RVA 0xED300
    return new MapInfoPanel;
}

m3d::Class* MapInfoPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* MapInfoPanel::GetClass() const
{
    // RVA 0xED2F0
    return RT_CLASS_LOCAL(MapInfoPanel);
}

// --- accessors --------------------------------------------------------------------------------

ObjectInfo* MapInfoPanel::GetObjectInfo() const
{
    // RVA 0xEFA20
    return m_objectInfo;
}

LocalMapWnd::Mode MapInfoPanel::GetCurrentMode() const
{
    // RVA 0xEFA30
    return m_mode;
}

int MapInfoPanel::SavePrices() const
{
    // RVA 0xEFA00
    if (!m_objectInfo)
    {
        return 0;
    }
    m_objectInfo->SetSavedPricesPersistant(true);
    return 1;
}

CStr MapInfoPanel::GetObjFullName() const
{
    // RVA 0xEFBA0
    if (!m_objectInfo)
    {
        return {};
    }
    CStr fullName = m_objectInfo->GetFullName();
    if (m_objectInfo->IsRuined())
    {
        // NOTE: the shipped build also splices the panel's own name into the
        // parenthesised suffix; retruxx keeps just the localized "ruined" marker.
        fullName += " (" + M3D_APP->GetStringByStringId0(m_aif.m_strIdRuined) + ")";
    }
    return fullName;
}

// --- setup / mode switching ------------------------------------------------------------------

int MapInfoPanel::SetUpForObject(ObjectInfo* objectInfo, bool fromSaveList)
{
    // RVA 0xEF150
    m_objectInfo = objectInfo;
    m_fromSaveList = fromSaveList;
    if (objectInfo && objectInfo->GetLevelName() != help::GetCurrentLevelName())
    {
        m_fromSaveList = true;
    }

    m3d::AIParam data;
    CallParentNotify(0x1A, data, false);
    return Setup();
}

int MapInfoPanel::SetUpForMode(LocalMapWnd::Mode mode)
{
    // RVA 0xEF250
    m_mode = mode;
    return Setup();
}

void MapInfoPanel::ValidateMode()
{
    // RVA 0xEFB50
    if (m_objectInfo && m_mode == LocalMapWnd::MODE_TRADE && !LocalMapWnd::IsObjectInfoValidForPrices(m_objectInfo))
    {
        m_mode = LocalMapWnd::MODE_CLANS;
    }
}

int MapInfoPanel::Setup()
{
    // RVA 0xEF260
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!m_objectInfo)
    {
        return Clear();
    }

    if (m_mode == LocalMapWnd::MODE_TRADE && !LocalMapWnd::IsObjectInfoValidForPrices(m_objectInfo))
    {
        m_mode = LocalMapWnd::MODE_CLANS;
    }

    int res = (m_mode == LocalMapWnd::MODE_CLANS) ? (SetupForClansMode() & 1) : (SetupForTradeMode() & 1);
    UpdateModeButtonsState();
    return res;
}

int MapInfoPanel::SetupForTradeMode()
{
    // RVA 0xEF570
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!m_objectInfo)
    {
        Clear();
        return 0;
    }

    if (m_objectInfo->IsTown() && !m_objectInfo->IsRuined())
    {
        ShowClansModeControls(false);
        m_wndSellList->SetUpForObject(m_objectInfo);
        ShowTradeModeControls(true);
        m_lblObjectName->SetText(GetObjFullName());
        return 1;
    }

    Clear();
    return 0;
}

int MapInfoPanel::SetupForClansMode()
{
    // RVA 0xEF3B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!m_objectInfo)
    {
        Clear();
        return 0;
    }

    ShowTradeModeControls(false);
    ShowClansModeControls(true);
    m_lblObjectName->SetText(GetObjFullName());

    // NOTE: the shipped build assembles the description string id as
    // <fullName>_<name>_diz.
    CStr const dizId = m_objectInfo->GetFullName() + "_" + m_objectInfo->GetName() + "_diz";
    m_txtObjectDiz->SetText(M3D_APP->GetStringByStringId0(dizId));
    return 1;
}

int MapInfoPanel::ShowTradeModeControls(bool show)
{
    // RVA 0xEF640
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    // NOTE: the shipped build toggles exactly these controls (lnHorz0 stays put).
    m3d::ui::Wnd* controls[] = {m_wndSellList, m_wndSell, m_wndBuy, m_lnHorz1, m_lnVert0, m_lnVert1, m_lblObjectName};
    for (auto* c : controls)
    {
        if (!c)
        {
            continue;
        }
        if (show)
        {
            if (!IsDirectChild(c))
            {
                AddChild(c);
                MoveChildToFirstPosition(c);
            }
        }
        else if (IsDirectChild(c))
        {
            RemoveChild(c);
        }
    }
    return 1;
}

int MapInfoPanel::ShowClansModeControls(bool show)
{
    // RVA 0xEF890
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m3d::ui::Wnd* controls[] = {m_lblObjectName, m_txtObjectDiz};
    for (auto* c : controls)
    {
        if (!c)
        {
            continue;
        }
        if (show)
        {
            if (!IsDirectChild(c))
            {
                AddChild(c);
                MoveChildToFirstPosition(c);
            }
        }
        else if (IsDirectChild(c))
        {
            RemoveChild(c);
        }
    }
    return 1;
}

void MapInfoPanel::UpdateModeButtonsState()
{
    // RVA 0xEFA40
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (!m_objectInfo)
    {
        m_btnModeTrade->ShowWindow(false);
        m_btnModeClans->ShowWindow(false);
        m_wndEmbossBtnModeTrade->ShowWindow(false);
        m_wndEmbossBtnModeClans->ShowWindow(false);
        return;
    }

    m_btnModeClans->ShowWindow(true);
    m_wndEmbossBtnModeClans->ShowWindow(true);
    m_btnModeClans->SetPane(
        (m_mode == LocalMapWnd::MODE_CLANS) ? m_aif.m_modeBtnPaneNameSelected : m_aif.m_modeBtnPaneNameUnselected);

    if (!LocalMapWnd::IsObjectInfoValidForPrices(m_objectInfo))
    {
        m_btnModeTrade->ShowWindow(false);
        m_wndEmbossBtnModeTrade->ShowWindow(false);
    }
    else
    {
        m_btnModeTrade->ShowWindow(true);
        m_wndEmbossBtnModeTrade->ShowWindow(true);
        m_btnModeTrade->SetPane(
            (m_mode == LocalMapWnd::MODE_TRADE) ? m_aif.m_modeBtnPaneNameSelected : m_aif.m_modeBtnPaneNameUnselected);
    }
}

int MapInfoPanel::Clear()
{
    // RVA 0xEF2E0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    ShowTradeModeControls(false);
    if (m_txtObjectDiz && IsDirectChild(m_txtObjectDiz))
    {
        RemoveChild(m_txtObjectDiz);
    }
    if (m_lblObjectName && IsDirectChild(m_lblObjectName))
    {
        RemoveChild(m_lblObjectName);
    }
    if (m_lblObjectName)
    {
        m_lblObjectName->SetText({});
    }
    // NOTE: the shipped build clears the sell list through an inlined helper;
    // retruxx re-uses SetUpForObject(nullptr).
    if (m_wndSellList)
    {
        m_wndSellList->SetUpForObject(nullptr);
    }
    UpdateModeButtonsState();
    return 1;
}

// --- notifications ----------------------------------------------------------------------------

int MapInfoPanel::GameDataClear(bool)
{
    // RVA 0xEF950
    SetUpForObject(nullptr, m_fromSaveList);
    return 1;
}

int MapInfoPanel::OnBeforeAddToWndStation()
{
    // RVA 0xEFB80
    Setup();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int MapInfoPanel::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0xEF970
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (idFrom == 401 && message == 1)
    {
        m_mode = LocalMapWnd::MODE_TRADE;
        Setup();
        return 1;
    }
    if (idFrom == 402 && message == 1)
    {
        m_mode = LocalMapWnd::MODE_CLANS;
        Setup();
        return 1;
    }
    return 0;
}

// --- one control grabbed from the pattern's parent ------------------------------------------------

namespace
{
    // Shared body of the inlined "GetControlFromPattern" blocks in
    // MapInfoPanel::CreateFromPattern (RVA 0xEDBA0). Moves a named child from the
    // pattern's parent into `owner`, keeping it at the same screen position.
    template<typename T>
    bool GrabControlFromPattern(
        m3d::ui::Wnd* owner,
        m3d::Object* oldParent,
        CStr const& childName,
        char const* rttiClass,
        T*& out)
    {
        out = nullptr;
        m3d::Class const* cls = M3D_KERNEL->FindClass(rttiClass);
        if (!cls)
        {
            M3D_LOG_INFO(
                CStr("GetControlFromPattern error - invalid class for pattern wnd; control name ") + childName);
            return false;
        }
        m3d::Object* child = oldParent->GetChildByName(childName);
        if (!child || !child->IsKindOf(cls))
        {
            M3D_LOG_INFO(CStr("GetControlFromPattern error - no child or invalid class; control name - ") + childName);
            return false;
        }

        auto* wndChild = static_cast<m3d::ui::Wnd*>(child);
        oldParent->RemoveChild(child);
        owner->AddChild(child);

        BoundsBase<float> cb = wndChild->GetBounds();
        PointBase<float> const screenPt =
            static_cast<m3d::ui::Wnd*>(oldParent)->ToScreen(PointBase<float>{cb.x0, cb.y0});
        PointBase<float> const winPt = owner->ToWindow(screenPt);
        cb.x0 = winPt.x;
        cb.y0 = winPt.y;
        wndChild->SetBounds(cb, false);

        out = static_cast<T*>(wndChild);
        return true;
    }
}  // namespace

int MapInfoPanel::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    // RVA 0xEDBA0
    if (!patternWnd)
    {
        return 0;
    }

    if (!m3d::ui::Wnd::Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        return 0;
    }

    // MapInfoPanel copies only a subset of the pattern's appearance.
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetBackground(patternWnd->GetBackground());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetTextColor());

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent || !parent->IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        return 0;
    }
    parent->AddChild(this);

    bool ok = true;
    ok &= GrabControlFromPattern(this, parent, m_aif.m_lblObjectNameName, "Wnd", m_lblObjectName);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_lblSellName, "ImageWnd", m_wndSell);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_lblBuyName, "ImageWnd", m_wndBuy);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_lnHorz0Name, "LineWnd", m_lnHorz0);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_lnHorz1Name, "LineWnd", m_lnHorz1);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_lnVert0Name, "LineWnd", m_lnVert0);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_lnVert1Name, "LineWnd", m_lnVert1);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_txtObjectDizName, "TextBoxWnd", m_txtObjectDiz);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_btnModeTradeName, "ButtonWnd", m_btnModeTrade);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_btnModeClansName, "ButtonWnd", m_btnModeClans);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_wndEmbossBtnModeTradeName, "Wnd", m_wndEmbossBtnModeTrade);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_wndEmbossBtnModeClansName, "Wnd", m_wndEmbossBtnModeClans);
    ok &= GrabControlFromPattern(this, parent, m_aif.m_wndFrameName, "Wnd", m_wndFrame);

    // The sell list is created from its own pattern child rather than moved.
    if (m3d::Object* sellChild = parent->GetChildByName(m_aif.m_wndSellListName))
    {
        if (sellChild->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndSellList = static_cast<MapSellList*>(M3D_KERNEL->New("MapSellList"));
            if (m_wndSellList)
            {
                if (m_wndSellList->CreateFromPattern(static_cast<m3d::ui::Wnd*>(sellChild), true))
                {
                    parent->RemoveChild(m_wndSellList);
                    AddChild(m_wndSellList);
                }
                else
                {
                    delete m_wndSellList;
                    m_wndSellList = nullptr;
                }
            }
        }
    }
    if (!m_wndSellList)
    {
        M3D_LOG_INFO(CStr("Cannot create control ") + m_aif.m_wndSellListName);
        ok = false;
    }

    if (deleteSrc)
    {
        delete const_cast<m3d::ui::Wnd*>(patternWnd);
    }

    if (!ok)
    {
        M3D_LOG_INFO("MapInfoPanel: error - fail to init because of a bad resource");
        return 0;
    }

    m_gameDataFlags |= 1u;
    SetUpForObject(m_objectInfo, false);
    return 1;
}

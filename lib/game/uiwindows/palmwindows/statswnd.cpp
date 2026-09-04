#include "statswnd.h"

#include <core/kernel.h>
#include <core/log.h>

#include <game/m3dgame.h>
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/guihelper.h"
#include <game/uiwindows/commonwindows/itemmodelwnd.h>

#include <skelmodel.h>
#include <core/aiparam.h>

#include <ui/textbox.h>
#include <ui/ui_srv.h>

#include "server/objects/player.h"
#include "server/objects/base/prototypemanager.h"
#include "server/statistic/statisticmanager.h"
#include "server/statistic/intstatistic.h"
#include "server/statistic/floatstatistic.h"
#include "server/statistic/timestatistic.h"
#include "server/statistic/favoritestringstatistic.h"
#include "server/statistic/intintratiostatistic.h"

// ============================================================================
//  StatsButton
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(StatsButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsButton);

StatsButton::AuxInfo StatsButton::m_aif;

StatsButton::AuxInfo::AuxInfo()
{
    // RVA 0x107580
    m_textColor = 0xFF404040u;
    m_nameWRel = 0.75f;
    m_spaceX = 10.0f;
    m_spaceY = 5.0f;
}

StatsButton::StatsButton()
{
    m_lblName = nullptr;
    m_lblValue = nullptr;
    m_virtualBounds = BoundsBase<float>{0.0f, 0.0f, 0.0f, 0.0f};
}

StatsButton::StatsButton(StatsButton const& rhs) : ButtonWnd(rhs)
{
    // NOTE: the shipped copy ctor only re-inits m_statsName and leaves the two
    // label pointers and m_virtualBounds uninitialised; set them to safe values.
    m_lblName = nullptr;
    m_lblValue = nullptr;
    m_virtualBounds = BoundsBase<float>{0.0f, 0.0f, 0.0f, 0.0f};
}

StatsButton::~StatsButton()
{
    // m_lblName / m_lblValue are owned children, released by the base destructor.
}

m3d::Object* StatsButton::Clone()
{
    // RVA 0x1063F0
    return new StatsButton(*this);
}

m3d::Object* StatsButton::CreateObject()
{
    // RVA 0x107550
    return new StatsButton;
}

m3d::Class* StatsButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* StatsButton::GetClass() const
{
    // RVA 0x107540
    return RT_CLASS_LOCAL(StatsButton);
}

CStr const& StatsButton::GetStatsName() const
{
    // RVA 0x107B10
    return m_statsName;
}

BoundsBase<float> const& StatsButton::GetVirtualBounds() const
{
    // RVA 0x107BF0
    return m_virtualBounds;
}

CStr StatsButton::GetStatsFullName() const
{
    // RVA 0x107B20
    CStr id = CStr("stat_");
    id += m_statsName;
    return M3D_APP->GetStringByStringId0(id);
}

CStr StatsButton::GetHackedDefaultValueForStats(CStr const& statsName) const
{
    // RVA 0x108150 - per-statistic placeholder shown before the first real value.
    if (statsName == ai::STATISTIC_REAL_TIME)
    {
        return CStr("00 : 00");
    }
    if (statsName == ai::STATISTIC_PATH_ELAPSED)
    {
        return help::ftoa(0.0f, -1);
    }
    if (statsName == ai::STATISTIC_FAVORITE_GUN)
    {
        return CStr("-");
    }
    if (statsName == ai::STATISTIC_WARES || statsName == ai::STATISTIC_GAME_TIME ||
        statsName == ai::STATISTIC_PEOPLES_KILLED || statsName == ai::STATISTIC_GDP ||
        statsName == ai::STATISTIC_DEATH_COUNTER || statsName == ai::STATISTIC_VEHICLE_KILLED ||
        statsName == ai::STATISTIC_SHELLS_EMITTED || statsName == ai::STATISTIC_HIT_RATIO)
    {
        return CStr(0);
    }
    return CStr{};
}

int StatsButton::SetUpForStats(CStr const& statsName, PointBase<float> const& origin, float width)
{
    // RVA 0x107700
    if (Valid() || (m_gameDataFlags & 1) != 0 || statsName.empty())
    {
        return 0;
    }

    BoundsBase<float> const rc{origin.x, origin.y, 0.0f, 0.0f};
    if (!m3d::ui::ButtonWnd::Create({}, 0x40260u, rc, 0))
    {
        return 0;
    }

    m_statsName = statsName;
    CStr const fullName = GetStatsFullName();

    float const nameW = m_aif.m_nameWRel * width;
    PointBase<float> const textSz = GetGfxServer()->MeasureText(fullName, 0, m3d::TW_WORD_WRAP, nameW);

    // --- name label -------------------------------------------------------
    BoundsBase<float> nameB;
    nameB.x0 = 0.0f;
    nameB.y0 = m_aif.m_spaceY;
    nameB.width = nameW;
    nameB.height = textSz.y;

    m_lblName = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_lblName->Create(fullName, 832u, nameB, 0))
    {
        delete m_lblName;
        m_lblName = nullptr;
        return 0;
    }
    m_lblName->SetDefaultFont(0);
    m_lblName->SetWrapMode(m3d::TW_NOWRAP);
    m_lblName->SetFormatMode(m3d::TF_LEFT);
    m_lblName->SetTextColor(m_aif.m_textColor);
    AddChild(m_lblName);

    // --- value label ----------------------------------------------------
    BoundsBase<float> valB;
    valB.x0 = nameB.width + nameB.x0 + m_aif.m_spaceX;
    valB.y0 = m_aif.m_spaceY;
    // (width - nameW - spaceX + nameB.width + nameB.x0 + spaceX) - valB.x0, which
    // with nameB.width == nameW and nameB.x0 == 0 reduces to the following.
    valB.width = width - nameW - m_aif.m_spaceX;
    valB.height = textSz.y;

    m_lblValue = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_lblValue->Create({}, 832u, valB, 0))
    {
        delete m_lblValue;
        m_lblValue = nullptr;
        return 0;
    }
    m_lblValue->SetDefaultFont(0);
    m_lblValue->SetWrapMode(m3d::TW_NOWRAP);
    m_lblValue->SetFormatMode(m3d::TF_LEFT);
    m_lblValue->SetTextColor(m_aif.m_textColor);
    m_lblValue->SetText(GetHackedDefaultValueForStats(m_statsName));
    AddChild(m_lblValue);

    // NOTE: the shipped build assembles the self-bounds on the stack and the
    // Hex-Rays output leaves the height field reading uninitialised stack (a
    // decompiler artifact of the __userpurge frame). retruxx uses the twin
    // CheckButton layout: spaceY padding above and below a single text row.
    BoundsBase<float> const selfB{origin.x, origin.y, width, m_aif.m_spaceY * 2.0f + textSz.y};
    SetBounds(selfB, true);

    m_gameDataFlags |= 1u;
    m_virtualBounds = m_bounds;
    return 1;
}

void StatsButton::SetBounds(BoundsBase<float> const& rect, bool bUpdateBaseOrigin)
{
    // RVA 0x107C00 - lightweight override that skips the base layout pass.
    m_bounds = rect;
    if (bUpdateBaseOrigin)
    {
        m_baseOrigin.x = m_bounds.x0;
        m_baseOrigin.y = m_bounds.y0;
    }
}

void StatsButton::SetBounds(BoundsBase<float> const& rect, BoundsBase<float> const& virtualRect)
{
    // RVA 0x107C50
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

int StatsButton::UpdateValue()
{
    // RVA 0x107D40
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    // NOTE: the shipped build also computes GetHackedDefaultValueForStats() here
    // and throws the result away; reproduced only for the branches below.
    int res = 1;
    ai::Statistic const* stat = ai::theStatisticManager->GetStatistic(m_statsName);
    CStr strVal;
    if (stat)
    {
        m3d::AIParam const val = stat->GetValueAsAIParam();

        if (stat->IsKindOf(&ai::IntStatistic::m_classIntStatistic))
        {
            strVal = CStr(val.GetAsID());
        }
        else if (stat->IsKindOf(&ai::FloatStatistic::m_classFloatStatistic))
        {
            float fVal = val.GetAsFloat();
            if (m_statsName == ai::STATISTIC_PATH_ELAPSED)
            {
                fVal = fVal * 0.001f;
            }
            strVal = help::ftoa(fVal, -1);
        }
        else if (stat->IsKindOf(&ai::TimeStatistic::m_classTimeStatistic))
        {
            retruxx::vector<int> const idList = val.GetAsIdList();
            CStr const statName(stat->GetName());
            if (idList.size() == 4 && statName == ai::STATISTIC_REAL_TIME)
            {
                strVal = CStr(idList[1]) + CStr(" : ") + CStr(idList[2]);
            }
            else if (idList.size() == 4 && statName == ai::STATISTIC_GAME_TIME)
            {
                strVal = CStr(idList[0]);
            }
            else
            {
                res = 0;
            }
        }
        else if (stat->IsKindOf(&ai::FavoriteStringStatistic::m_classFavoriteStringStatistic))
        {
            CStr const sVal = val.GetAsStr();
            if (sVal.empty())
            {
                strVal = CStr("-");
            }
            else
            {
                strVal = ai::thePrototypeManager->GetPrototypeFullName(sVal);
            }
        }
        else if (stat->IsKindOf(&ai::IntIntRatioStatistic::m_classIntIntRatioStatistic))
        {
            int ratio = static_cast<int>(val.GetAsFloat());
            if (ratio < 0)
            {
                ratio = 0;
            }
            else if (ratio > 100)
            {
                ratio = 100;
            }
            strVal = CStr(ratio);
        }
        else
        {
            res = 0;
        }
    }

    m_lblValue->SetText(strVal);
    return res;
}

void StatsButton::ClearValue()
{
    // RVA 0x108100
    if (m_lblValue)
    {
        m_lblValue->SetText(GetHackedDefaultValueForStats(m_statsName));
    }
}

// ============================================================================
//  StatsList
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(StatsList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsList);

StatsList::StatsList()
{
    // The ListBoxWnd<StatsButton*> template ctor seeds the base fields the
    // shipped ctor writes explicitly (curSel = -1, client edge = 2.0f).
}

StatsList::StatsList(StatsList const& rhs) : ListBoxWnd<StatsButton*>(rhs)
{
    // RVA 0x108370 - fresh, empty list (the shipped copy ctor copies nothing).
}

StatsList::~StatsList()
{
    // Items are owned children, released by the base destructor.
}

m3d::Object* StatsList::Clone()
{
    // RVA 0x106440
    return new StatsList(*this);
}

m3d::Object* StatsList::CreateObject()
{
    // RVA 0x108290
    return new StatsList;
}

m3d::Class* StatsList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* StatsList::GetClass() const
{
    // RVA 0x108280
    return RT_CLASS_LOCAL(StatsList);
}

int StatsList::CreateFromPattern(m3d::ui::Wnd* pw, bool deleteSrc)
{
    // RVA 0x108410
    if (!pw || !pw->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
    {
        M3D_LOG_INFO("StatsList::CreateFromPattern error - null patternWnd");
        return 0;
    }

    if (!Create(pw->GetBounds(), pw->GetStyle(), pw->GetId()))
    {
        M3D_LOG_INFO("StatsList::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetStyle(pw->GetStyle());
    SetText(pw->GetText());
    SetId(pw->GetId());
    SetName(pw->GetName());
    SetBounds(pw->GetBounds(), true);
    SetDefaultFont(pw->GetDefaultFont());
    SetWrapMode(pw->GetWrapMode());
    SetFormatMode(pw->GetFormatMode());
    SetColor(pw->GetColor());
    SetTextColor(pw->GetTextColor());
    SetTextColorDisabled(pw->GetTextColorDisabled());
    SetClientEdges(pw->GetClientEdges());
    SetPane(pw->GetPaneName());
    SetPaneFlags(pw->GetPaneFlags());
    SetScrollPane(pw->GetScrollPaneName());
    SetBackground(pw->GetBackground());

    CStr tooltip;
    pw->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(pw->GetOnShowAnimation());
    SetOnHideAnimation(pw->GetOnHideAnimation());

    SetDrawFlags(static_cast<m3d::ui::TextBoxWnd*>(pw)->GetDrawFlags());

    m3d::Object* parent = pw->GetParent();
    if (!parent || !parent->IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        M3D_LOG_INFO("StatsList::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(pw);
        delete pw;
    }

    if (!InitStats())
    {
        M3D_LOG_INFO("StatsList::CreateFromPattern error - fail to init stats");
        return 0;
    }
    m_gameDataFlags |= 1u;
    return 1;
}

int StatsList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    // RVA 0x108800
    BoundsBase<float> const& vb = m_items[itemIdx].m_item->GetVirtualBounds();
    bounds.x0 = 0.0f;
    bounds.y0 = 0.0f;
    bounds.width = vb.width;
    bounds.height = vb.height;
    return 1;
}

int StatsList::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    // RVA 0x108870
    StatsButton* item = m_items[itemIdx].m_item;
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

int StatsList::DeleteItem(int itemIdx)
{
    // RVA 0x108AA0
    if (StatsButton* item = m_items[itemIdx].m_item)
    {
        item->DecRef();
    }
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

int StatsList::CompareItem(int, int)
{
    // RVA 0x108AE0
    return 0;
}

int StatsList::GetButtonIdByStatsName(CStr const& statsName) const
{
    // RVA 0x108AF0
    if (statsName.empty())
    {
        return -1;
    }
    for (size_t i = 0; i < m_items.size(); ++i)
    {
        StatsButton* btn = m_items[i].m_item;
        if (btn && btn->GetStatsName() == statsName)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int StatsList::AddButtonByStatsName(CStr const& statsName)
{
    // RVA 0x108B90
    if (statsName.empty())
    {
        return -1;
    }
    auto* btn = static_cast<StatsButton*>(M3D_KERNEL->New("StatsButton"));
    if (!btn)
    {
        return -1;
    }

    // NOTE: the shipped build feeds the client bounds through oddly - the button
    // origin becomes (clientBounds.y0, clientBounds.width) and its "width" is
    // clientBounds.height. RenderItem re-positions every row anyway.
    BoundsBase<float> const clientB = GetClientBounds();
    PointBase<float> const origin{clientB.y0, clientB.width};
    if (!btn->SetUpForStats(statsName, origin, clientB.height))
    {
        delete btn;
        return -1;
    }
    AddChild(btn);
    return AddItem(btn);
}

void StatsList::UpdateStats()
{
    // RVA 0x108C40
    retruxx::vector<ai::Statistic const*> allStats;
    ai::theStatisticManager->GetAllStatistics(allStats);

    for (size_t i = 0; i < allStats.size(); ++i)
    {
        ai::Statistic const* stat = allStats[i];
        if (!stat)
        {
            continue;
        }
        CStr const statsName(stat->GetName());
        int id = GetButtonIdByStatsName(statsName);
        if (id == -1 && stat->GetGlobalFlag())
        {
            id = AddButtonByStatsName(statsName);
        }
        if (id != -1)
        {
            if (StatsButton* btn = m_items[id].m_item)
            {
                btn->UpdateValue();
            }
        }
    }
}

void StatsList::ClearStats()
{
    // RVA 0x108D20
    for (auto const& item : m_items)
    {
        if (item.m_item)
        {
            item.m_item->ClearValue();
        }
    }
}

int StatsList::InitStats()
{
    // RVA 0x108DD0
    if (!Valid())
    {
        return 0;
    }
    if (!m_items.empty())
    {
        return 0;
    }

    // Every button is added (side effects) regardless of earlier failures, then
    // the results are AND-ed together, matching the shipped evaluation order.
    bool ok = AddButtonByStatsName(ai::STATISTIC_REAL_TIME) != -1;
    ok = (AddButtonByStatsName(ai::STATISTIC_GAME_TIME) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_DEATH_COUNTER) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_VEHICLE_KILLED) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_PEOPLES_KILLED) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_FAVORITE_GUN) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_SHELLS_EMITTED) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_HIT_RATIO) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_PATH_ELAPSED) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_GDP) != -1) && ok;
    ok = (AddButtonByStatsName(ai::STATISTIC_WARES) != -1) && ok;
    return ok ? 1 : 0;
}

// ============================================================================
//  StatsWnd
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(StatsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(StatsWnd);

StatsWnd::AuxInfo::AuxInfo()
{
    // RVA 0x106570
    m_statsListName = "statsList";
    m_wndPlayerDizName = "wndPlayerDiz";
    m_wndPlayerPortraitName = "wndPlayerPortrait";
}

StatsWnd::StatsWnd()
{
    // RVA 0x106650
    m_statsList = nullptr;
    m_wndPlayerDiz = nullptr;
}

StatsWnd::StatsWnd(StatsWnd const& rhs) : ScreenWnd(rhs)
{
    // RVA 0x106790 - fresh window (the shipped copy ctor copies nothing).
    m_statsList = nullptr;
    m_wndPlayerDiz = nullptr;
}

StatsWnd::~StatsWnd()
{
    // RVA 0x1067D0 - m_aif and the m_wndPlayerPortrait ref_ptr release themselves;
    // m_statsList / m_wndPlayerDiz are owned children released by the base.
}

m3d::Object* StatsWnd::Clone()
{
    // RVA 0x1064B0
    return new StatsWnd(*this);
}

m3d::Object* StatsWnd::CreateObject()
{
    // RVA 0x106540
    return new StatsWnd;
}

m3d::Class* StatsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Class* StatsWnd::GetClass() const
{
    // RVA 0x106530
    return RT_CLASS_LOCAL(StatsWnd);
}

int StatsWnd::GameDataSetup()
{
    // RVA 0x106810
    int res = 1;

    if ((m_gameDataFlags & 2) == 0)
    {
        // --- stats list ---------------------------------------------------
        m3d::Object* listChild = GetChildByName(m_aif.m_statsListName);
        if (!listChild || !listChild->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            M3D_LOG_INFO(
                CStr("Make control error: control ") + m_aif.m_statsListName + CStr(" is not found or incorrect type"));
            res = 0;
        }
        else
        {
            m_statsList = static_cast<StatsList*>(M3D_KERNEL->New("StatsList"));
            if (!m_statsList)
            {
                M3D_LOG_INFO(
                    CStr("Make control error: cannot create ") + m_aif.m_statsListName +
                    CStr(" - cannot find rtti class ") + CStr("StatsList"));
                res = 0;
            }
            else if (!m_statsList->CreateFromPattern(static_cast<m3d::ui::Wnd*>(listChild), true))
            {
                M3D_LOG_INFO(
                    CStr("Make control error: cannot create ") + m_aif.m_statsListName + CStr(" from pattern class"));
                res = 0;
            }
        }

        // --- player description text ------------------------------------
        m3d::Object* dizChild = GetChildByName(m_aif.m_wndPlayerDizName);
        if (dizChild && dizChild->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            m_wndPlayerDiz = static_cast<m3d::ui::TextBoxWnd*>(dizChild);
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Get control error: control ") + m_aif.m_wndPlayerDizName +
                CStr(" is not found or incorrect type"));
            res = 0;
        }

        // --- player portrait -------------------------------------------
        m3d::Object* portraitChild = GetChildByName(m_aif.m_wndPlayerPortraitName);
        if (!portraitChild || !portraitChild->IsKindOf(&m3d::ui::ModelWnd::m_classModelWnd))
        {
            M3D_LOG_INFO(
                CStr("Make control error: control ") + m_aif.m_wndPlayerPortraitName +
                CStr(" is not found or incorrect type"));
            res = 0;
        }
        else
        {
            m_wndPlayerPortrait = static_cast<ItemModelWnd*>(M3D_KERNEL->New("ItemModelWnd"));
            if (!m_wndPlayerPortrait)
            {
                M3D_LOG_INFO(
                    CStr("Make control error: cannot create ") + m_aif.m_wndPlayerPortraitName +
                    CStr(" - cannot find rtti class ") + CStr("ItemModelWnd"));
                res = 0;
            }
            else if (!m_wndPlayerPortrait->CreateFromPattern(static_cast<m3d::ui::Wnd*>(portraitChild), true))
            {
                M3D_LOG_INFO(
                    CStr("Make control error: cannot create ") + m_aif.m_wndPlayerPortraitName +
                    CStr(" from pattern class"));
                res = 0;
            }
        }

        if (m_wndPlayerPortrait)
        {
            // NOTE: the shipped build writes these ItemModelWnd flags directly;
            // the equivalent public setters are used here.
            m_wndPlayerPortrait->AllowRotate(false);
            m_wndPlayerPortrait->AllowRotateByHandX(false);
            m_wndPlayerPortrait->AllowRotateByHandY(false);
            m_wndPlayerPortrait->SetAutosized(false);

            int guiId = -1;
            M3D_APP->m_pInterfaceManager->AddWindow(m_wndPlayerPortrait.get(), guiId, true, false);
        }

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("StatsWnd: error - fail to init because of a bad resource");
    return 0;
}

int StatsWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x107030
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 85)
    {
        UpdatePlayerPortrait();
        UpdatePlayerDiz();
    }
    else if (dataType == 89)
    {
        if (m_statsList)
        {
            m_statsList->UpdateStats();
        }
        UpdatePlayerPortraitAnmation();
        return 1;
    }
    return 1;
}

int StatsWnd::GameDataClear(bool)
{
    // RVA 0x107080
    if ((m_gameDataFlags & 1) != 0)
    {
        if (m_statsList)
        {
            m_statsList->ClearStats();
        }
        m_wndPlayerDiz->SetText(CStr{});
    }
    return 1;
}

void StatsWnd::OnNewFrame()
{
    // RVA 0x1070E0
    if ((m_gameDataFlags & 1) != 0 && m_statsList)
    {
        m_statsList->UpdateStats();
    }
    UpdatePlayerPortraitAnmation();
}

void StatsWnd::UpdateStats()
{
    // RVA 0x107100
    if ((m_gameDataFlags & 1) != 0 && m_statsList)
    {
        m_statsList->UpdateStats();
    }
}

void StatsWnd::OnStartLevel()
{
    // RVA 0x107120
    UpdatePlayerPortrait();
    UpdatePlayerDiz();
}

void StatsWnd::UpdatePlayerPortrait()
{
    // RVA 0x107130
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (ai::thePlayer)
    {
        unsigned int const cfgNumber = ai::thePlayer->GetCfgNumber();
        unsigned int const skinNumber = ai::thePlayer->GetSkinNumber();
        m_wndPlayerPortrait->SetModelByName(ai::thePlayer->GetModelName(), skinNumber, cfgNumber);

        if (m_wndPlayerPortrait->GetModel() && m_wndPlayerPortrait->Animation())
        {
            ActionType const stand = help::GetRandomStandAnimation(m_wndPlayerPortrait->GetModel());
            if (!m_wndPlayerPortrait->Animation()->SetAnimation(stand))
            {
                m_wndPlayerPortrait->Animation()->SetAnimation(AT_STAND1);
            }
        }
    }
    else
    {
        m_wndPlayerPortrait->SetModelByName(CStr{}, 0, 0);
    }
}

void StatsWnd::UpdatePlayerPortraitAnmation()
{
    // RVA 0x107290
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (m_wndPlayerPortrait->GetModel() && m_wndPlayerPortrait->Animation())
    {
        // TODO(RVA 0x107290): calls help::RandomizeCurAnimationOnFinish (RVA
        // 0x1561E0) - a guihelper animation routine not yet ported to retruxx.
        // It swaps the portrait's current animation for a fresh random stand
        // pose once the running clip is within two frames of its end.
        // help::RandomizeCurAnimationOnFinish(m_wndPlayerPortrait->GetModel(),
        //                                     m_wndPlayerPortrait->Animation(), help::_AT_STAND);
    }
}

void StatsWnd::UpdatePlayerDiz()
{
    // RVA 0x107340
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // TODO(RVA 0x107340): the "with player" branch needs ai::Obj::GetText() (a
    // virtual absent from retruxx). The shipped code takes the player's text
    // record, appends "_diz" to its id, resolves the result through
    // GetStringByStringId0 and shows it. The no-player / no-text branches (and
    // this fallback) just clear the description.
    m_wndPlayerDiz->SetText(CStr{});
}

int StatsWnd::OnAfterAddToWndStation()
{
    // RVA 0x107490
    int const r = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, CStr{}, m3d::AIParam{});
    return r;
}

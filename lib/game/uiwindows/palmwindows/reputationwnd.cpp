#include "reputationwnd.h"

#include "encyclopaediawnd.h"

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <m3dapp.h>
#include <server/objects/player.h>
#include <server/server.h>
#include <ui/image.h>

// ---------------------------------------------------------------------------
//  ReputationButton
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(ReputationButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ReputationButton);

ReputationButton::AuxInfo ReputationButton::m_aif;

ReputationButton::AuxInfo::AuxInfo()
{
    // RVA 0x500C80
    m_wndPatternName = "wndReputationButton";
    m_wndClanIcoName = "wndClanIco";
    m_lblClanNameName = "lblClanName";
    m_wndToleranceNameName = "wndToleranceName";
    m_strIdPeaceAvailable = "PeaceAvailable";
    m_peaceAvailableColor = 0xFF00FF00;
    m_wndPattern = nullptr;
    m_wndPatternClanIco = nullptr;
    m_lblPatternClanName = nullptr;
    m_wndPatternToleranceName = nullptr;
    m_friendToleranceColor = 0xFF237A9B;
    m_enemyToleranceColor = 0xFF9C2FE1;
    m_strIdToleranceFriend1 = "FriendlyRelation";
    m_strIdToleranceEnemy1 = "EnemyRelation";
    m_strIdToleranceFriend2 = "Peace";
    m_strIdToleranceEnemy2 = "War";
}

ReputationButton::AuxInfo::AuxInfo(ReputationButton::AuxInfo const& rhs) :
    m_wndPatternName(rhs.m_wndPatternName),
    m_wndClanIcoName(rhs.m_wndClanIcoName),
    m_lblClanNameName(rhs.m_lblClanNameName),
    m_wndToleranceNameName(rhs.m_wndToleranceNameName),
    m_strIdPeaceAvailable(rhs.m_strIdPeaceAvailable),
    m_peaceAvailableColor(rhs.m_peaceAvailableColor),
    m_wndPattern(rhs.m_wndPattern),
    m_wndPatternClanIco(rhs.m_wndPatternClanIco),
    m_lblPatternClanName(rhs.m_lblPatternClanName),
    m_wndPatternToleranceName(rhs.m_wndPatternToleranceName),
    m_friendToleranceColor(rhs.m_friendToleranceColor),
    m_enemyToleranceColor(rhs.m_enemyToleranceColor),
    m_strIdToleranceFriend1(rhs.m_strIdToleranceFriend1),
    m_strIdToleranceEnemy1(rhs.m_strIdToleranceEnemy1),
    m_strIdToleranceFriend2(rhs.m_strIdToleranceFriend2),
    m_strIdToleranceEnemy2(rhs.m_strIdToleranceEnemy2)
{
}

ReputationButton::ReputationButton() :
    m_clanBelong(-1),
    m_lblClanName(nullptr),
    m_wndClanIco(nullptr),
    m_wndToleranceName(nullptr)
{
}

ReputationButton::ReputationButton(ReputationButton const&) : ReputationButton()
{
}

ReputationButton::~ReputationButton() = default;

m3d::Object* ReputationButton::Clone()
{
    return new ReputationButton(*this);
}

m3d::Object* ReputationButton::CreateObject()
{
    return new ReputationButton;
}

m3d::Class* ReputationButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* ReputationButton::GetClass() const
{
    return RT_CLASS_LOCAL(ReputationButton);
}

int ReputationButton::GetClanBelong() const
{
    return m_clanBelong;
}

namespace
{
    // Shifts a pattern child's origin so it is expressed relative to the pattern
    // row rather than to the screen, which is what LoadPattern caches.
    void RebaseOntoPattern(m3d::ui::Wnd* child, m3d::ui::Wnd const* pattern)
    {
        BoundsBase<float> b = child->GetBounds();
        auto const patternB = pattern->GetBounds();
        b.x0 -= patternB.x0;
        b.y0 -= patternB.y0;
        child->SetBounds(b, true);
    }
}  // namespace

int ReputationButton::LoadPattern(m3d::ui::Wnd* pattern)
{
    // RVA 0x500F60 - caches the pattern row and its three children, rebasing each
    // child's origin onto the row so a clone can be positioned as a unit.
    if (!pattern)
    {
        return 0;
    }

    m3d::Object* patternWnd = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (!patternWnd || !patternWnd->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("ReputationButton::LoadPattern error - cannot find " + m_aif.m_wndPatternName);
        return 0;
    }
    m_aif.m_wndPattern = static_cast<m3d::ui::Wnd*>(patternWnd);

    m3d::Object* clanIco = pattern->GetChildByName(m_aif.m_wndClanIcoName);
    if (!clanIco || !clanIco->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
    {
        M3D_LOG_INFO("ReputationButton::LoadPattern error - cannot find " + m_aif.m_wndClanIcoName);
        return 0;
    }
    m_aif.m_wndPatternClanIco = static_cast<m3d::ui::ImageWnd*>(clanIco);
    RebaseOntoPattern(m_aif.m_wndPatternClanIco, m_aif.m_wndPattern);

    m3d::Object* clanName = pattern->GetChildByName(m_aif.m_lblClanNameName);
    if (!clanName || !clanName->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("ReputationButton::LoadPattern error - cannot find " + m_aif.m_lblClanNameName);
        return 0;
    }
    m_aif.m_lblPatternClanName = static_cast<m3d::ui::Wnd*>(clanName);
    RebaseOntoPattern(m_aif.m_lblPatternClanName, m_aif.m_wndPattern);

    m3d::Object* toleranceName = pattern->GetChildByName(m_aif.m_wndToleranceNameName);
    if (!toleranceName || !toleranceName->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("ReputationButton::LoadPattern error - cannot find " + m_aif.m_wndToleranceNameName);
        return 0;
    }
    m_aif.m_wndPatternToleranceName = static_cast<m3d::ui::Wnd*>(toleranceName);
    RebaseOntoPattern(m_aif.m_wndPatternToleranceName, m_aif.m_wndPattern);

    BoundsBase<float> patternB = m_aif.m_wndPattern->GetBounds();
    patternB.x0 = 0.0f;
    patternB.y0 = 0.0f;
    m_aif.m_wndPattern->SetBounds(patternB, true);
    return 1;
}

void ReputationButton::ClearPattern()
{
    // RVA 0x5012B0
    delete m_aif.m_wndPattern;
    m_aif.m_wndPattern = nullptr;
    delete m_aif.m_wndPatternClanIco;
    m_aif.m_wndPatternClanIco = nullptr;
    delete m_aif.m_lblPatternClanName;
    m_aif.m_lblPatternClanName = nullptr;
    delete m_aif.m_wndPatternToleranceName;
    m_aif.m_wndPatternToleranceName = nullptr;
}

int ReputationButton::CreateFromPattern()
{
    // RVA 0x501310
    if (Valid())
    {
        return 0;
    }
    if (!m_aif.m_wndPattern || !M3D_APP->IsWndAlive(m_aif.m_wndPattern, -1) || !m_aif.m_wndPattern->Valid())
    {
        M3D_LOG_INFO("ReputationButton::CreateFromPattern error - null pattern");
        return 0;
    }

    if (m3d::ui::Wnd::Create(
            {}, m_aif.m_wndPattern->GetStyle(), m_aif.m_wndPattern->GetBounds(), m_aif.m_wndPattern->GetId()) == 0)
    {
        M3D_LOG_INFO("ReputationButton::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetPane(m_aif.m_wndPattern->GetPaneName());
    SetPaneFlags(m_aif.m_wndPattern->GetPaneFlags());
    return CreateChildren();
}

int ReputationButton::CreateChildren()
{
    // RVA 0x501540 - clones the three pattern children onto this row.
    if (m_wndClanIco || m_lblClanName || m_wndToleranceName)
    {
        M3D_LOG_INFO("NpcButton::CreateChildren error - children already exist");
        return 0;
    }
    if (!m_aif.m_wndPatternClanIco || !m_aif.m_lblPatternClanName || !m_aif.m_wndPatternToleranceName)
    {
        return 0;
    }

    m_wndClanIco = static_cast<m3d::ui::ImageWnd*>(m3d::g_Kernel->New("ImageWnd"));
    if (!m_wndClanIco ||
        m_wndClanIco->Create(
            {},
            m_aif.m_wndPatternClanIco->GetStyle(),
            m_aif.m_wndPatternClanIco->GetBounds(),
            m_aif.m_wndPatternClanIco->GetId()) == 0)
    {
        return 0;
    }
    m_wndClanIco->SetName(m_aif.m_wndPatternClanIco->GetName());
    m_wndClanIco->SetPane(m_aif.m_wndPatternClanIco->GetPaneName());
    m_wndClanIco->SetPaneFlags(m_aif.m_wndPatternClanIco->GetPaneFlags());
    AddChild(m_wndClanIco);

    m_lblClanName = static_cast<m3d::ui::Wnd*>(m3d::g_Kernel->New("Wnd"));
    if (!m_lblClanName ||
        m_lblClanName->Create(
            {},
            m_aif.m_lblPatternClanName->GetStyle(),
            m_aif.m_lblPatternClanName->GetBounds(),
            m_aif.m_lblPatternClanName->GetId()) == 0)
    {
        return 0;
    }
    m_lblClanName->SetName(m_aif.m_lblPatternClanName->GetName());
    m_lblClanName->SetPane(m_aif.m_lblPatternClanName->GetPaneName());
    m_lblClanName->SetPaneFlags(m_aif.m_lblPatternClanName->GetPaneFlags());
    m_lblClanName->SetDefaultFont(m_aif.m_lblPatternClanName->GetDefaultFont());
    m_lblClanName->SetFormatMode(m_aif.m_lblPatternClanName->GetFormatMode());
    AddChild(m_lblClanName);

    m_wndToleranceName = static_cast<m3d::ui::Wnd*>(m3d::g_Kernel->New("Wnd"));
    if (!m_wndToleranceName ||
        m_wndToleranceName->Create(
            {},
            m_aif.m_wndPatternToleranceName->GetStyle(),
            m_aif.m_wndPatternToleranceName->GetBounds(),
            m_aif.m_wndPatternToleranceName->GetId()) == 0)
    {
        return 0;
    }
    m_wndToleranceName->SetName(m_aif.m_wndPatternToleranceName->GetName());
    m_wndToleranceName->SetPane(m_aif.m_wndPatternToleranceName->GetPaneName());
    m_wndToleranceName->SetPaneFlags(m_aif.m_wndPatternToleranceName->GetPaneFlags());
    m_wndToleranceName->SetDefaultFont(m_aif.m_wndPatternToleranceName->GetDefaultFont());
    m_wndToleranceName->SetFormatMode(m_aif.m_wndPatternToleranceName->GetFormatMode());
    AddChild(m_wndToleranceName);
    return 1;
}

int ReputationButton::SetUpForClan(int clanBelong)
{
    // RVA 0x501F10
    if (Valid() || clanBelong == -1 || !CreateFromPattern())
    {
        return 0;
    }
    m_clanBelong = clanBelong;

    CStr const clanName = help::GetClanNameByBelong(clanBelong);
    m_lblClanName->SetText(M3D_APP->GetStringByStringId0(clanName));
    m_wndClanIco->SetImage(M3D_APP->m_pInterfaceManager->GetIcoByName(clanName, 0));
    UpdateTolerance();
    return 1;
}

CStr ReputationButton::BWTolerance2StrId1(help::BlackWightTolerance bwTolerance)
{
    // RVA 0x501FF0
    switch (bwTolerance)
    {
    case help::BW_TOLERANCE_ENEMY:
        return m_aif.m_strIdToleranceEnemy1;
    case help::BW_TOLERANCE_FRIEND:
        return m_aif.m_strIdToleranceFriend1;
    default:
        return {};
    }
}

CStr ReputationButton::BWTolerance2StrId2(help::BlackWightTolerance bwTolerance) const
{
    // RVA 0x502030
    switch (bwTolerance)
    {
    case help::BW_TOLERANCE_ENEMY:
        return m_aif.m_strIdToleranceEnemy2;
    case help::BW_TOLERANCE_FRIEND:
        return m_aif.m_strIdToleranceFriend2;
    default:
        return {};
    }
}

unsigned ReputationButton::BWTolerance2Color(help::BlackWightTolerance bwTolerance) const
{
    // RVA 0x5021E0
    return bwTolerance ? m_aif.m_friendToleranceColor : m_aif.m_enemyToleranceColor;
}

void ReputationButton::UpdateTolerance()
{
    // RVA 0x502070 - anything that is not plainly hostile reads as friendly; a
    // tolerance the scale does not cover blanks the indicator out entirely.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!ai::thePlayer)
    {
        UpdateToleranceIndicator(help::BW_TOLERANCE_NUM_BW_TOLERANCES);
        UpdateTooltip(help::BW_TOLERANCE_NUM_BW_TOLERANCES);
        return;
    }

    ai::eTolerance const tolerance = ai::pServer->CheckTolerance(ai::thePlayer->GetBelong(), m_clanBelong);
    help::BlackWightTolerance bwTolerance = help::BW_TOLERANCE_FRIEND;
    if (tolerance == ai::RS_ENEMY)
    {
        bwTolerance = help::BW_TOLERANCE_ENEMY;
    }
    else if (tolerance <= ai::RS_ENEMY || tolerance > ai::RS_OWN)
    {
        UpdateToleranceIndicator(help::BW_TOLERANCE_NUM_BW_TOLERANCES);
        UpdateTooltip(help::BW_TOLERANCE_NUM_BW_TOLERANCES);
        return;
    }
    UpdateToleranceIndicator(bwTolerance);
    UpdateTooltip(bwTolerance);
}

void ReputationButton::UpdateToleranceIndicator(help::BlackWightTolerance bwTolerance)
{
    // RVA 0x5020E0 - the clan icon and the relation caption both take the
    // relation's colour, and the caption its short "Peace"/"War" wording.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    unsigned const clr =
        (bwTolerance == help::BW_TOLERANCE_ENEMY) ? m_aif.m_enemyToleranceColor : m_aif.m_friendToleranceColor;
    m_wndClanIco->SetColor(clr);
    m_wndToleranceName->SetColor(clr);
    m_wndToleranceName->SetText(M3D_APP->GetStringByStringId0(BWTolerance2StrId2(bwTolerance)));
}

void ReputationButton::UpdateTooltip(help::BlackWightTolerance bwTolerance)
{
    // RVA 0x502200 - the tooltip spells the relation out, and when the player is
    // at war it appends a coloured note if peace can still be bought.
    CStr tip;
    if (bwTolerance != help::BW_TOLERANCE_NUM_BW_TOLERANCES)
    {
        tip = M3D_APP->GetStringByStringId0(BWTolerance2StrId1(bwTolerance));
        if (bwTolerance == help::BW_TOLERANCE_ENEMY && help::IsPeaceWithEnemyAvailable(m_clanBelong))
        {
            tip += CStr("|") + help::Color2Str(m_aif.m_peaceAvailableColor) +
                   M3D_APP->GetStringByStringId0(m_aif.m_strIdPeaceAvailable);
        }
    }
    SetProperty(PROP_WND_TOOLTIP, const_cast<char*>(tip.c_str()));
}

// ---------------------------------------------------------------------------
//  AuxRPanel
// ---------------------------------------------------------------------------

AuxRPanel::AuxRPanel()
{
    m_btns[0] = nullptr;
    m_btns[1] = nullptr;
}

AuxRPanel::~AuxRPanel() = default;

AuxRPanel::Slot AuxRPanel::GetEmptySlotId() const
{
    // RVA 0x502500
    for (int i = 0; i < NUM_SLOTS; ++i)
    {
        if (!m_btns[i])
        {
            return static_cast<Slot>(i);
        }
    }
    return NUM_SLOTS;
}

bool AuxRPanel::HasEmptySlot() const
{
    // RVA 0x5024E0
    return GetEmptySlotId() != NUM_SLOTS;
}

ReputationButton* AuxRPanel::GetButtonBySlot(AuxRPanel::Slot slotId) const
{
    // RVA 0x502520
    return (slotId == NUM_SLOTS) ? nullptr : m_btns[slotId];
}

ReputationButton* AuxRPanel::AddButton(int clanBelong, m3d::ui::Wnd* parentWnd)
{
    // RVA 0x502450
    if (clanBelong == -1 || !parentWnd)
    {
        return nullptr;
    }
    Slot const slot = GetEmptySlotId();
    if (slot == NUM_SLOTS)
    {
        return nullptr;
    }

    auto* btn = static_cast<ReputationButton*>(m3d::g_Kernel->New("ReputationButton"));
    if (!btn)
    {
        return nullptr;
    }
    if (!btn->SetUpForClan(clanBelong))
    {
        delete btn;
        return nullptr;
    }
    parentWnd->AddChild(btn);
    m_btns[slot] = btn;
    return btn;
}

// ---------------------------------------------------------------------------
//  ReputationList
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(ReputationList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ReputationList);

ReputationList::AuxInfo::AuxInfo()
{
    // RVA 0x5025C0
    m_spaceX = 2.0f;
    m_spaceY = 2.0f;
}

ReputationList::ReputationList() = default;

ReputationList::ReputationList(ReputationList const&) : ReputationList()
{
}

ReputationList::~ReputationList() = default;

m3d::Object* ReputationList::Clone()
{
    return new ReputationList(*this);
}

m3d::Object* ReputationList::CreateObject()
{
    return new ReputationList;
}

m3d::Class* ReputationList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* ReputationList::GetClass() const
{
    return RT_CLASS_LOCAL(ReputationList);
}

int ReputationList::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x5026E0 - the list takes the pattern's place, and the pattern's own
    // children become the row template every button is cloned from.
    if (!patternWnd)
    {
        M3D_LOG_INFO("ReputationList::CreateFromPattern error - null patternWnd");
        return 0;
    }

    if (m3d::ui::Wnd::Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()) == 0)
    {
        M3D_LOG_INFO("ReputationList::CreateFromPattern error - cannot create window");
        return 0;
    }
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent || !parent->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("ReputationList::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }

    if (!ReputationButton::LoadPattern(patternWnd))
    {
        return 0;
    }
    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }
    return 1;
}

ReputationButton* ReputationList::GetButtonByClanBelong(int clanBelong) const
{
    // RVA 0x502BF0
    for (m3d::Object* child = GetFirstChild(); child; child = child->GetNextSibling())
    {
        if (child->IsKindOf(&ReputationButton::m_classReputationButton) &&
            static_cast<ReputationButton*>(child)->GetClanBelong() == clanBelong)
        {
            return static_cast<ReputationButton*>(child);
        }
    }
    return nullptr;
}

int ReputationList::GetPanelIdFitToPlaceButton()
{
    // RVA 0x502C30 - first panel with a free slot, or a brand new panel appended
    // to the end.
    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i] && m_items[i]->HasEmptySlot())
        {
            return static_cast<int>(i);
        }
    }

    auto* newPanel = new AuxRPanel;
    if (!newPanel)
    {
        return -1;
    }
    m_items.push_back(newPanel);
    return static_cast<int>(m_items.size()) - 1;
}

ReputationButton* ReputationList::AddButtonByClanBelong(int clanBelong)
{
    // RVA 0x502980
    if (clanBelong == -1)
    {
        return nullptr;
    }
    if (ReputationButton* existing = GetButtonByClanBelong(clanBelong))
    {
        return existing;
    }

    int const panelId = GetPanelIdFitToPlaceButton();
    if (panelId < 0 || panelId >= static_cast<int>(m_items.size()) || !m_items[panelId])
    {
        return nullptr;
    }
    ReputationButton* btn = m_items[panelId]->AddButton(clanBelong, this);
    PlaceButton(btn);
    return btn;
}

PointBase<int> ReputationList::GetButtonPanelCoord(ReputationButton const* btn) const
{
    // RVA 0x502DD0 - x is the slot within the panel, y is the panel itself.
    PointBase<int> coord;
    coord.x = -1;
    coord.y = -1;
    if (!btn)
    {
        return coord;
    }
    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        AuxRPanel* panel = m_items[i];
        if (!panel)
        {
            continue;
        }
        for (int slot = 0; slot < AuxRPanel::NUM_SLOTS; ++slot)
        {
            if (panel->GetButtonBySlot(static_cast<AuxRPanel::Slot>(slot)) == btn)
            {
                coord.x = slot;
                coord.y = static_cast<int>(i);
                return coord;
            }
        }
    }
    return coord;
}

void ReputationList::PlaceButton(ReputationButton* btn)
{
    // RVA 0x502D40 - the grid position falls straight out of the panel/slot the
    // button landed in.
    if (!btn)
    {
        return;
    }
    PointBase<int> const coord = GetButtonPanelCoord(btn);
    if (coord.x == -1 && coord.y == -1)
    {
        return;
    }
    BoundsBase<float> b = btn->GetBounds();
    b.x0 = (m_aif.m_spaceX + b.width) * static_cast<float>(coord.x);
    b.y0 = (m_aif.m_spaceY + b.height) * static_cast<float>(coord.y);
    btn->SetBounds(b, true);
}

void ReputationList::UpdateClans()
{
    // RVA 0x502A00 - the encyclopaedia owns the list of clans the player has met.
    ClearClans();

    ref_ptr wndEncyclopaedia = M3D_APP->m_pInterfaceManager->GetWindow(97);
    auto* enc = RT_DYNCAST(wndEncyclopaedia.get(), EncyclopaediaWnd);
    if (!enc)
    {
        return;
    }
    if (std::vector<int> const* clans = enc->GetClans())
    {
        for (unsigned i = 0; i < clans->size(); ++i)
        {
            AddButtonByClanBelong((*clans)[i]);
        }
    }
}

void ReputationList::ClearClans()
{
    // RVA 0x502AE0 - the panels own their buttons.
    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        AuxRPanel* panel = m_items[i];
        if (!panel)
        {
            continue;
        }
        for (int slot = 0; slot < AuxRPanel::NUM_SLOTS; ++slot)
        {
            delete panel->GetButtonBySlot(static_cast<AuxRPanel::Slot>(slot));
        }
        delete panel;
        m_items[i] = nullptr;
    }
    m_items.clear();
}

int ReputationList::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0x502BD0
    if (!beforeContinuousLevel)
    {
        ClearClans();
    }
    return 1;
}

int ReputationList::OnBeforeAddToWndStation()
{
    // RVA 0x502D20
    UpdateClans();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

// ---------------------------------------------------------------------------
//  ReputationWnd
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(ReputationWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ReputationWnd);

ReputationWnd::AuxInfo::AuxInfo()
{
    // RVA 0x5004F0
    m_reputationListName = "reputationList";
}

ReputationWnd::AuxInfo::AuxInfo(ReputationWnd::AuxInfo const& rhs) : m_reputationListName(rhs.m_reputationListName)
{
}

ReputationWnd::ReputationWnd() : m_reputationList(nullptr)
{
}

ReputationWnd::ReputationWnd(ReputationWnd const&) : ReputationWnd()
{
}

ReputationWnd::~ReputationWnd() = default;

m3d::Object* ReputationWnd::Clone()
{
    return new ReputationWnd(*this);
}

m3d::Object* ReputationWnd::CreateObject()
{
    return new ReputationWnd;
}

m3d::Class* ReputationWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Class* ReputationWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ReputationWnd);
}

int ReputationWnd::GameDataSetup()
{
    // RVA 0x500690
    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* child = GetChildByName(m_aif.m_reputationListName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_reputationList = static_cast<ReputationList*>(m3d::g_Kernel->New("ReputationList"));
            if (m_reputationList)
            {
                if (m_reputationList->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
                {
                    m_gameDataFlags |= 1u;
                }
                else
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_reputationListName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_reputationListName);
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Get control error: control " + m_aif.m_reputationListName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("ReputationWnd: error - fail to init because of a bad resource");
    return 0;
}

int ReputationWnd::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0x5009C0
    if ((m_gameDataFlags & 1) != 0)
    {
        m_reputationList->GameDataClear(beforeContinuousLevel);
    }
    return 1;
}

int ReputationWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x5009F0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 52)
    {
        OnKnownClansChanged();
    }
    return 1;
}

void ReputationWnd::OnKnownClansChanged()
{
    // RVA 0x500A20
    if ((m_gameDataFlags & 1) != 0 && m_reputationList->IsChildOf(M3D_APP))
    {
        m_reputationList->UpdateClans();
    }
}

int ReputationWnd::OnAfterAddToWndStation()
{
    // RVA 0x500A60
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

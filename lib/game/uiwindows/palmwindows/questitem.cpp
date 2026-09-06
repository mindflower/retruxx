#include "questitem.h"

#include "navpointbutton.h"

#include <client.h>
#include <core/kernel.h>
#include <game/m3dgame.h>
#include <game/uimisc/questinfo.h>
#include <level.h>
#include <m3dapp.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/dynamicquest.h>
#include <server/quest.h>
#include <ui/ui_srv.h>
#include <world.h>

namespace
{
    CStr CurrentLevelName()
    {
        if (m3d::pClient)
        {
            if (auto* level = m3d::pClient->GetWorld().m_level)
            {
                return level->GetLevelName();
            }
        }
        return {};
    }
}  // namespace

// ---------------------------------------------------------------------------
//  QuestItem
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(QuestItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItem);

QuestItem::AuxInfo QuestItem::m_aif;

QuestItem::AuxInfo::AuxInfo()
{
    // RVA 0x4F8A50
    m_xSpace = 8.0f;
    m_ySpace0 = 5.0f;
    m_ySpace1 = 0.0f;
    m_navPointBtnSz.x = 32.0f;
    m_navPointBtnSz.y = 32.0f;
}

QuestItem::AuxInfo::AuxInfo(QuestItem::AuxInfo const& rhs) :
    m_xSpace(rhs.m_xSpace),
    m_ySpace0(rhs.m_ySpace0),
    m_ySpace1(rhs.m_ySpace1),
    m_navPointBtnSz(rhs.m_navPointBtnSz)
{
}

QuestItem::QuestItem() :
    m_questId(-1),
    m_questType(help::QUESTTYPE_NUM_QUEST_TYPES),
    m_btnQuestStatus(nullptr),
    m_wndQuestLabel(nullptr),
    m_btnNavPoint(nullptr)
{
}

QuestItem::QuestItem(QuestItem const&) : QuestItem()
{
}

QuestItem::~QuestItem() = default;

m3d::Object* QuestItem::Clone()
{
    return new QuestItem(*this);
}

m3d::Object* QuestItem::CreateObject()
{
    return new QuestItem;
}

m3d::Class* QuestItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* QuestItem::GetClass() const
{
    return RT_CLASS_LOCAL(QuestItem);
}

help::QuestType QuestItem::GetQuestType() const
{
    return m_questType;
}

int QuestItem::GetQuestId() const
{
    return m_questId;
}

int QuestItem::CreateQuestItem(PointBase<float> origin, float maxWidth, help::QuestType questType, int questId, int id)
{
    // RVA 0x4F8B20 - one journal row: an optional nav-point button, a status
    // icon and the wrapped quest text, sized to whatever they add up to.
    if (questId == -1 || questType == help::QUESTTYPE_NUM_QUEST_TYPES)
    {
        return 0;
    }

    BoundsBase<float> b;
    b.x0 = origin.x;
    b.y0 = origin.y;
    b.width = 0.0f;
    b.height = 0.0f;
    if (m3d::ui::Wnd::Create({}, 0x40260u, b, id) == 0)
    {
        return 0;
    }

    m_questType = questType;
    m_questId = questId;

    if (NeedNavPointBtnBeAdded(questType, questId))
    {
        m_btnNavPoint = static_cast<NavPointButton*>(m3d::g_Kernel->New("NavPointButton"));
        if (!m_btnNavPoint)
        {
            return 0;
        }
        b.x0 = 0.0f;
        b.y0 = m_aif.m_ySpace0;
        b.width = m_aif.m_navPointBtnSz.x;
        b.height = m_aif.m_navPointBtnSz.y;
        if (m_btnNavPoint->Create({}, 262688u, b, 7u) == 0 ||
            !m_btnNavPoint->SetupForQuest(m_questType, m_questId))
        {
            delete m_btnNavPoint;
            m_btnNavPoint = nullptr;
            return 0;
        }
        AddChild(m_btnNavPoint);
    }

    m_btnQuestStatus = static_cast<QuestStatusBtn*>(m3d::g_Kernel->New("QuestStatusBtn"));
    if (!m_btnQuestStatus)
    {
        return 0;
    }
    PointBase<float> statusOrigin;
    statusOrigin.x = 0.0f;
    statusOrigin.y = m_aif.m_ySpace1;
    if (!m_btnQuestStatus->CreateQuestStatusBtn(statusOrigin, 5u))
    {
        delete m_btnQuestStatus;
        m_btnQuestStatus = nullptr;
        return 0;
    }
    AddChild(m_btnQuestStatus);

    m_wndQuestLabel = static_cast<QuestLabel*>(m3d::g_Kernel->New("QuestLabel"));
    if (!m_wndQuestLabel)
    {
        return 0;
    }
    // The text starts just right of the status icon and gets whatever width is left.
    auto const statusB = m_btnQuestStatus->GetBounds();
    PointBase<float> labelOrigin;
    labelOrigin.x = statusB.x0 + statusB.width + m_aif.m_xSpace;
    labelOrigin.y = m_aif.m_ySpace0;
    if (m_wndQuestLabel->CreateQuestLabel(
            GetQuestDizById(m_questType, m_questId), labelOrigin, maxWidth - labelOrigin.x, 6) == 0)
    {
        delete m_wndQuestLabel;
        m_wndQuestLabel = nullptr;
        return 0;
    }
    AddChild(m_wndQuestLabel);

    // The row is exactly as tall as its tallest child, plus the top margin.
    float maxBottom = 0.0f;
    for (m3d::Object* child = GetFirstChild(); child; child = child->GetNextSibling())
    {
        if (child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            auto const childB = static_cast<m3d::ui::Wnd*>(child)->GetBounds();
            if (childB.y0 + childB.height > maxBottom)
            {
                maxBottom = childB.y0 + childB.height;
            }
        }
    }
    m_gameDataFlags |= 1u;
    m_bounds.width = maxWidth;
    m_bounds.height = maxBottom + m_aif.m_ySpace0;

    FullUpdate();
    return 1;
}

int QuestItem::FullUpdate()
{
    // RVA 0x4F9190
    int const res = SetupNavPointBtn(m_questType, m_questId) & 1;
    UpdateQuestStatusBtn();
    UpdateQuestLabelWnd();
    return res;
}

void QuestItem::UpdateQuestStatusBtn()
{
    // RVA 0x4F91E0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_btnQuestStatus->SetQuestStatus(help::GetQuestUnifyStatusByQuestId(m_questType, m_questId));
    m_btnQuestStatus->UpdateIcon();
    // Without its own nav-point button the status icon doubles as the marker.
    m_btnQuestStatus->ShowWindow(m_btnNavPoint == nullptr);
    UpdateOnQuestNavPointChangedChildIncluded();
}

void QuestItem::UpdateQuestLabelWnd()
{
    // RVA 0x4F9230
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndQuestLabel->SetQuestStatus(help::GetQuestUnifyStatusByQuestId(m_questType, m_questId));
    m_wndQuestLabel->UpdateTextColor();
}

void QuestItem::SetSelected(bool isSelected)
{
    // RVA 0x4F9260
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_btnQuestStatus->SetSelected(isSelected);
    m_btnQuestStatus->UpdateIcon();
    m_wndQuestLabel->SetSelected(isSelected);
    m_wndQuestLabel->UpdateTextColor();
}

int QuestItem::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x4F9290
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }
    if (idFrom != 7 || message != 1)
    {
        return 0;
    }
    OnBtnNavPointClick(from, idFrom, data);
    return 1;
}

bool QuestItem::NeedNavPointBtnBeAdded(help::QuestType questType, int questId) const
{
    // RVA 0x4F92F0 - only sub-quests of a static quest get their own button; the
    // parent row shows the marker on its status icon instead.
    if (!help::CanNavPointBeAddedOnQuest(questType, questId) || questType != help::QUESTTYPE_STATIC || questId == -1)
    {
        return false;
    }
    ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
    return quest && quest->GetParentId() != -1;
}

void QuestItem::OnBtnNavPointClick(m3d::ui::Wnd* wndFrom, unsigned, m3d::AIParam const&)
{
    // RVA 0x4F9330 - the button toggles: no point yet means add one, otherwise
    // drop the one already there.
    auto* btn = RT_DYNCAST(wndFrom, NavPointButton);
    if (!btn)
    {
        return;
    }
    if (btn->GetNavPointId() == -1)
    {
        AddNavPoint(btn->GetNavPointType());
    }
    else
    {
        RemoveNavPoint(btn->GetNavPointId());
    }
}

int QuestItem::AddNavPoint(NavPoint::NavPointType npType)
{
    // RVA 0x4F9380
    if (static_cast<unsigned>(npType) >= NavPoint::NAVPOINT_TYPE_USER_LOCATION)
    {
        return 0;
    }
    NavPoint::ObjectType objectType = NavPoint::OBJECT_TYPE_NUM_OBJECT_TYPES;
    if (m_questType == help::QUESTTYPE_STATIC)
    {
        objectType = NavPoint::OBJECT_TYPE_STATIC_QUEST;
    }
    else if (m_questType == help::QUESTTYPE_DYNAMIC)
    {
        objectType = NavPoint::OBJECT_TYPE_DYNAMIC_QUEST;
    }

    NavPointManager* npm = M3D_APP->m_pInterfaceManager->GetNavPointManager();
    return npm->AddNavPointObjectDependend(CurrentLevelName(), npType, objectType, m_questId, true) != -1;
}

int QuestItem::RemoveNavPoint(int npId)
{
    // RVA 0x4F9440
    return M3D_APP->m_pInterfaceManager->GetNavPointManager()->RemoveNavPointById(npId);
}

int QuestItem::SetupNavPointBtn(help::QuestType questType, int questId)
{
    // RVA 0x4F8EC0 - rebuilt from scratch every update, because whether the row
    // deserves a button at all depends on the quest's current state.
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    delete m_btnNavPoint;
    m_btnNavPoint = nullptr;

    if (questType == help::QUESTTYPE_NUM_QUEST_TYPES || questId == -1)
    {
        return 0;
    }
    if (!NeedNavPointBtnBeAdded(questType, questId))
    {
        return 1;
    }

    m_btnNavPoint = static_cast<NavPointButton*>(m3d::g_Kernel->New("NavPointButton"));
    if (!m_btnNavPoint)
    {
        return 0;
    }
    BoundsBase<float> btnNavPointB;
    btnNavPointB.x0 = 0.0f;
    btnNavPointB.y0 = m_aif.m_ySpace0;
    btnNavPointB.width = m_aif.m_navPointBtnSz.x;
    btnNavPointB.height = m_aif.m_navPointBtnSz.y;
    if (m_btnNavPoint->Create({}, 262688u, btnNavPointB, 7u) == 0 ||
        !m_btnNavPoint->SetupForQuest(m_questType, m_questId))
    {
        delete m_btnNavPoint;
        m_btnNavPoint = nullptr;
        return 0;
    }
    AddChild(m_btnNavPoint);
    return 1;
}

CStr QuestItem::GetQuestDizById(help::QuestType questType, int questId) const
{
    // RVA 0x4F9030 - the one-line summary shown in the journal.
    if (questType == help::QUESTTYPE_STATIC)
    {
        ai::Quest* quest = ai::theQuestManager->GetQuestById(questId);
        if (quest)
        {
            QuestInfo const* qi =
                M3D_APP->m_pInterfaceManager->GetQuestInfoManager()->GetQuestInfoByQuestInfoName(quest->GetName());
            if (qi)
            {
                return qi->GetBriefDiz();
            }
        }
    }
    else if (questType == help::QUESTTYPE_DYNAMIC)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(questId);
        if (obj && obj->IsKindOf(&ai::DynamicQuest::m_classDynamicQuest))
        {
            QuestInfo const* qi =
                M3D_APP->m_pInterfaceManager->GetQuestInfoManager()->GetQuestInfoForDynamicQuest(questId);
            return qi ? qi->GetBriefDiz() : CStr();
        }
    }
    return {};
}

void QuestItem::UpdateOnQuestNavPointChangedChildIncluded()
{
    // RVA 0x4F9460 - the status icon carries the nav-point marker. For a static
    // quest the marker also appears when any of its *sub*-quests is marked, so a
    // collapsed parent row still shows that something under it is flagged.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    m_btnQuestStatus->SetNavPointType(NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES);
    m_btnQuestStatus->UpdateNpTexture();

    CStr const curLevelName = CurrentLevelName();
    NavPointManager* npm = M3D_APP->m_pInterfaceManager->GetNavPointManager();
    NavPoint const* np = nullptr;

    if (m_questType == help::QUESTTYPE_DYNAMIC)
    {
        np = npm->GetNavPointByObjectId(curLevelName, m_questId, NavPoint::OBJECT_TYPE_DYNAMIC_QUEST);
    }
    else if (m_questType == help::QUESTTYPE_STATIC)
    {
        ai::Quest* quest = ai::theQuestManager->GetQuestById(m_questId);
        if (!quest)
        {
            return;
        }
        np = npm->GetNavPointByObjectId(curLevelName, m_questId, NavPoint::OBJECT_TYPE_STATIC_QUEST);
        if (!np)
        {
            auto const& subQuestIds = quest->GetSubQuestIds();
            for (unsigned i = 0; i < subQuestIds.size(); ++i)
            {
                np = M3D_APP->m_pInterfaceManager->GetNavPointManager()->GetNavPointByObjectId(
                    help::GetCurrentLevelName(), subQuestIds[i], NavPoint::OBJECT_TYPE_STATIC_QUEST);
                if (np)
                {
                    break;
                }
            }
        }
    }
    else
    {
        return;
    }

    if (np)
    {
        m_btnQuestStatus->SetNavPointType(np->GetNavPointType());
    }
}

// ---------------------------------------------------------------------------
//  QuestStatusBtn
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(QuestStatusBtn)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestStatusBtn);

QuestStatusBtn::AuxInfo QuestStatusBtn::m_aif;

QuestStatusBtn::AuxInfo::AuxInfo()
{
    // RVA 0x4F9690 - indexed by help::UnifyQuestStatus.
    m_bgTextures[0] = "QuestNoncomplete";
    m_bgTextures[1] = "QuestComplete";
    m_bgTextures[2] = "QuestFail";
    m_size.x = 32.0f;
    m_size.y = 32.0f;
    m_npTexSz.x = 16.0f;
    m_npTexSz.y = 16.0f;
    m_strNpButton = "QuestLogNavPointBtn_";
    m_npTexColor = 0x80FFFFFF;
}

QuestStatusBtn::AuxInfo::AuxInfo(QuestStatusBtn::AuxInfo const& rhs) :
    m_size(rhs.m_size),
    m_npTexSz(rhs.m_npTexSz),
    m_strNpButton(rhs.m_strNpButton),
    m_npTexColor(rhs.m_npTexColor)
{
    for (int i = 0; i < 3; ++i)
    {
        m_bgTextures[i] = rhs.m_bgTextures[i];
    }
}

QuestStatusBtn::QuestStatusBtn() :
    m_isSelected(false),
    m_questStatus(help::QUESTSTATUS_NONCOMPLETE),
    m_npType(NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
{
}

QuestStatusBtn::QuestStatusBtn(QuestStatusBtn const&) : QuestStatusBtn()
{
}

QuestStatusBtn::~QuestStatusBtn()
{
    M3D_RENDERER->ReleaseTexture(m_npTex);
}

m3d::Object* QuestStatusBtn::Clone()
{
    return new QuestStatusBtn(*this);
}

m3d::Object* QuestStatusBtn::CreateObject()
{
    return new QuestStatusBtn;
}

m3d::Class* QuestStatusBtn::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* QuestStatusBtn::GetClass() const
{
    return RT_CLASS_LOCAL(QuestStatusBtn);
}

void QuestStatusBtn::SetSelected(bool isSelected)
{
    m_isSelected = isSelected;
}

bool QuestStatusBtn::IsSelected() const
{
    return m_isSelected;
}

void QuestStatusBtn::SetQuestStatus(help::UnifyQuestStatus questStatus)
{
    m_questStatus = questStatus;
}

help::UnifyQuestStatus QuestStatusBtn::GetQuestStatus() const
{
    return m_questStatus;
}

int QuestStatusBtn::CreateQuestStatusBtn(PointBase<float> const& origin, unsigned id)
{
    // RVA 0x4F98E0
    BoundsBase<float> rc;
    rc.x0 = origin.x;
    rc.y0 = origin.y;
    rc.width = m_aif.m_size.x;
    rc.height = m_aif.m_size.y;
    return ButtonWnd::Create({}, 0x220u, rc, id);
}

void QuestStatusBtn::UpdateIcon()
{
    // RVA 0x4F99A0 - the icon is picked by quest status, and the selected state
    // chooses which variant of it the interface manager hands back.
    m3d::rend::TexHandle const tex =
        M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_bgTextures[m_questStatus], m_isSelected);
    SetImaged(tex, m3d::rend::TexHandle{}, m3d::rend::TexHandle{}, m3d::rend::TexHandle{});
}

void QuestStatusBtn::SetNavPointType(NavPoint::NavPointType npType)
{
    // RVA 0x4F9AD0. NOTE: the marker only changes while the quest is still in
    // progress, or when it is being cleared outright.
    if (m_questStatus == help::QUESTSTATUS_NONCOMPLETE || npType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        m_npType = npType;
        UpdateNpTexture();
    }
}

void QuestStatusBtn::UpdateNpTexture()
{
    // RVA 0x4F9AF0
    M3D_RENDERER->ReleaseTexture(m_npTex);
    m_npTex.SetInvalid();
    if (m_npType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return;
    }
    CStr const npTexId = m_aif.m_strNpButton + NavPointButton::NavPointType2Str(m_npType);
    m_npTex = M3D_APP->m_pInterfaceManager->GetIcoByName(npTexId, 0);
    M3D_RENDERER->ReferenceTexture(m_npTex);
}

void QuestStatusBtn::OnNcPaint(m3d::ui::DrawInfo const& di, unsigned clr)
{
    // RVA 0x4F9A00 - the nav-point marker sits centred on the status icon.
    ButtonWnd::OnNcPaint(di, clr);
    if (m_npType == NavPoint::NAVPOINT_TYPE_NUM_NAVPOINT_TYPES)
    {
        return;
    }
    BoundsBase<float> npB;
    npB.x0 = (m_bounds.width - m_aif.m_npTexSz.x) * 0.5f;
    npB.y0 = (m_bounds.height - m_aif.m_npTexSz.y) * 0.5f;
    npB.width = ((m_bounds.width + m_aif.m_npTexSz.x) * 0.5f) - npB.x0;
    npB.height = ((m_bounds.height + m_aif.m_npTexSz.y) * 0.5f) - npB.y0;
    GetGfxServer()->AddImagedRect(di, npB, m_aif.m_npTexColor, m_npTex);
}

// ---------------------------------------------------------------------------
//  QuestLabel
// ---------------------------------------------------------------------------

RT_CLASS_EXPORTS_BEGIN(QuestLabel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestLabel);

QuestLabel::AuxInfo QuestLabel::m_aif;

QuestLabel::AuxInfo::AuxInfo()
{
    // RVA 0x4F9C40
    m_selColor = 0xFF400000;
    m_unselColor = 0xFF7F7F7F;
    m_selFailColor = 0xFFD34331;
    m_unselFailColor = 0xFF752663;
    m_crossTexName = "QuestCrossLine";
    m_crossLineH = 2.0f;
}

QuestLabel::AuxInfo::AuxInfo(QuestLabel::AuxInfo const& rhs) :
    m_selColor(rhs.m_selColor),
    m_unselColor(rhs.m_unselColor),
    m_selFailColor(rhs.m_selFailColor),
    m_unselFailColor(rhs.m_unselFailColor),
    m_crossTexName(rhs.m_crossTexName),
    m_crossLineH(rhs.m_crossLineH)
{
}

QuestLabel::QuestLabel() : m_isSelected(false), m_questStatus(help::QUESTSTATUS_NONCOMPLETE)
{
}

QuestLabel::QuestLabel(QuestLabel const&) : QuestLabel()
{
}

QuestLabel::~QuestLabel() = default;

m3d::Object* QuestLabel::Clone()
{
    return new QuestLabel(*this);
}

m3d::Object* QuestLabel::CreateObject()
{
    return new QuestLabel;
}

m3d::Class* QuestLabel::GetBaseClass()
{
    return RT_CLASS_LOCAL(TextBoxWnd);
}

m3d::Class* QuestLabel::GetClass() const
{
    return RT_CLASS_LOCAL(QuestLabel);
}

void QuestLabel::SetSelected(bool isSelected)
{
    m_isSelected = isSelected;
}

bool QuestLabel::IsSelected() const
{
    return m_isSelected;
}

void QuestLabel::SetQuestStatus(help::UnifyQuestStatus questStatus)
{
    m_questStatus = questStatus;
}

help::UnifyQuestStatus QuestLabel::GetQuestStatus() const
{
    return m_questStatus;
}

int QuestLabel::CreateQuestLabel(CStr const& caption, PointBase<float> const& origin, float maxWidth, unsigned id)
{
    // RVA 0x4F9D60 - sized to exactly the height the wrapped text needs, and
    // stripped of its scrollbar since the journal scrolls the list instead.
    auto const textSz = GetGfxServer()->MeasureText(caption, 0, m3d::TW_WORD_WRAP, maxWidth);

    BoundsBase<float> rect;
    rect.x0 = origin.x;
    rect.y0 = origin.y;
    rect.width = maxWidth;
    rect.height = textSz.y;
    int const result = TextBoxWnd::Create(rect, 0xA60u, id);
    if (!result)
    {
        return result;
    }

    SetText(caption);
    SetDefaultFont(0);
    SetFormatMode(m3d::TF_LEFT);
    delete m_scrollVWnd;
    m_scrollVWnd = nullptr;
    RecalcLayout();
    return 1;
}

void QuestLabel::UpdateTextColor()
{
    // RVA 0x4F9E50 - a failed quest gets its own pair of colours; anything the
    // journal cannot classify is left plain white.
    switch (m_questStatus)
    {
    case help::QUESTSTATUS_NONCOMPLETE:
    case help::QUESTSTATUS_COMPLETE:
        SetTextColor(m_isSelected ? m_aif.m_selColor : m_aif.m_unselColor);
        break;
    case help::QUESTSTATUS_FAILED:
        SetTextColor(m_isSelected ? m_aif.m_selFailColor : m_aif.m_unselFailColor);
        break;
    default:
        SetTextColor(0xFFFFFFFF);
        break;
    }
}

int QuestLabel::OnPaint(m3d::ui::DrawInfo const& clipToIt)
{
    // RVA 0x4F9EB0 - a finished quest gets struck through, one line at a time.
    TextBoxWnd::OnPaint(clipToIt);
    if (m_questStatus == help::QUESTSTATUS_NONCOMPLETE)
    {
        return 1;
    }

    m3d::rend::TexHandle const crossTex =
        M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_crossTexName, m_questStatus == help::QUESTSTATUS_COMPLETE);
    for (unsigned i = 0; i < m_items.size(); ++i)
    {
        auto const itemB = GetItemBounds(static_cast<int>(i));
        BoundsBase<float> crossLineB;
        crossLineB.x0 = itemB.x0;
        crossLineB.y0 = itemB.y0 + (itemB.height - m_aif.m_crossLineH) * 0.5f;
        crossLineB.width = itemB.width;
        crossLineB.height = m_aif.m_crossLineH;
        GetGfxServer()->AddImagedRect(clipToIt, crossLineB, 0, crossTex);
    }
    return 1;
}

BoundsBase<float> QuestLabel::GetLastStringBounds() const
{
    // RVA 0x4F9FC0
    int const last = static_cast<int>(m_items.size()) - 1;
    if (last == -1)
    {
        return {0.0f, 0.0f, 0.0f, 0.0f};
    }
    return GetItemBounds(last);
}

#include "saveselllist.h"

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>

#include <game/m3dgame.h>
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/levelinfo.h"

#include <ui/frame.h>
#include <ui/ui_srv.h>

// ============================================================================
//  SaveSellButton
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(SaveSellButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveSellButton);

SaveSellButton::AuxInfo SaveSellButton::m_aif;

SaveSellButton::AuxInfo::AuxInfo()
{
    // RVA 0x1054B0
    m_paneName = "PaneBtnGray1";
    m_selectPaneName = "PaneBtnGray1Selected";
    m_space = 3.0f;
    m_textColor = 0xFF404040u;
}

SaveSellButton::SaveSellButton()
{
    m_objectInfo = nullptr;
    m_lblObjectName = nullptr;
    m_bSelected = false;
}

SaveSellButton::SaveSellButton(SaveSellButton const& rhs) : ButtonWnd(rhs)
{
    m_objectInfo = nullptr;
    m_lblObjectName = nullptr;
    m_bSelected = false;
}

SaveSellButton::~SaveSellButton()
{
    // m_lblObjectName is an owned child window, released by the base destructor.
}

m3d::Object* SaveSellButton::Clone()
{
    // RVA 0x103640
    return new SaveSellButton(*this);
}

m3d::Object* SaveSellButton::CreateObject()
{
    // RVA 0x105460
    return new SaveSellButton;
}

m3d::Class* SaveSellButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Class* SaveSellButton::GetClass() const
{
    // RVA 0x105450
    return RT_CLASS_LOCAL(SaveSellButton);
}

ObjectInfo* SaveSellButton::GetObjectInfo() const
{
    // RVA 0x105820
    return m_objectInfo;
}

bool SaveSellButton::IsSelected() const
{
    // RVA 0x105830
    return m_bSelected;
}

void SaveSellButton::Select(bool bSelect)
{
    // RVA 0x105840
    m_bSelected = bSelect;
    SetPane(bSelect ? m_aif.m_selectPaneName : m_aif.m_paneName);
}

int SaveSellButton::SetUp(ObjectInfo* objectInfo, float maxHeight, int id)
{
    // RVA 0x1055F0
    M3D_ASSERT(!Valid());
    m_objectInfo = objectInfo;
    if (!objectInfo)
    {
        return 0;
    }

    BoundsBase<float> rc{0.0f, 0.0f, 0.0f, maxHeight};
    if (!m3d::ui::ButtonWnd::Create({}, 0x40200u, rc, id))
    {
        return 0;
    }
    SetPane(m_aif.m_paneName);

    m_lblObjectName = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New("Wnd"));
    if (!m_lblObjectName)
    {
        return 0;
    }

    CStr const& fullName = objectInfo->GetFullName();
    float const textWidth = GetGfxServer()->MeasureText(fullName, 1, m3d::TW_NOWRAP, 1024.0f).x;

    float barWidth = 0.0f;
    if (m3d::ui::Pane* pane = GetGfxServer()->GetPane(GetPaneName()))
    {
        if (m3d::ui::Frame* frame = pane->m_frame[0])
        {
            barWidth = static_cast<float>(frame->m_barUsedWidth);
        }
    }

    BoundsBase<float> lblB;
    lblB.x0 = m_aif.m_space + barWidth;
    lblB.y0 = barWidth;
    lblB.width = textWidth;
    lblB.height = GetBounds().height - barWidth * 2.0f;
    if (!m_lblObjectName->Create(fullName, 3904u, lblB, 0))
    {
        delete m_lblObjectName;
        m_lblObjectName = nullptr;
        return 0;
    }
    AddChild(m_lblObjectName);
    m_lblObjectName->SetDefaultFont(1);
    m_lblObjectName->SetWrapMode(m3d::TW_NOWRAP);
    m_lblObjectName->SetTextColor(m_aif.m_textColor);

    // NOTE: the shipped build writes m_bounds.width in place; retruxx routes it
    // through SetBounds so the base origin stays consistent.
    BoundsBase<float> b = GetBounds();
    b.width = lblB.width + lblB.x0 + (m_aif.m_space + barWidth);
    SetBounds(b, false);
    return 1;
}

// ============================================================================
//  SaveSellList
// ============================================================================

RT_CLASS_EXPORTS_BEGIN(SaveSellList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveSellList);

SaveSellList::AuxInfo::AuxInfo()
{
    // RVA 0x103720
    m_btnNextName = "btnNext";
    m_btnPrevName = "btnPrev";
    m_wndEmbossBtnNextName = "emboss_btnNext";
    m_wndEmbossBtnPrevName = "emboss_btnPrev";
    m_space = 5.0f;
}

SaveSellList::SaveSellList()
{
    m_btnNext = nullptr;
    m_btnPrev = nullptr;
    m_firstSaveBtnId = -1;
    m_lastSaveBtnId = -1;
    m_selBtnId = -1;
}

SaveSellList::SaveSellList(SaveSellList const& rhs) : Wnd(rhs)
{
    // RVA 0x103670: fresh window + AuxInfo + empty button list.
    m_btnNext = nullptr;
    m_btnPrev = nullptr;
    m_firstSaveBtnId = -1;
    m_lastSaveBtnId = -1;
    m_selBtnId = -1;
}

SaveSellList::~SaveSellList()
{
    // The save buttons are owned children, released with GameDataClear.
    GameDataClear(false);
}

m3d::Object* SaveSellList::Clone()
{
    // RVA 0x103670
    return new SaveSellList(*this);
}

m3d::Object* SaveSellList::CreateObject()
{
    // RVA 0x1036F0
    return new SaveSellList;
}

m3d::Class* SaveSellList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* SaveSellList::GetClass() const
{
    // RVA 0x1036E0
    return RT_CLASS_LOCAL(SaveSellList);
}

// --- one control grabbed from the pattern's parent -------------------------------------------------

namespace
{
    // Shared body of the four inlined "GetControlFromPattern" blocks in
    // SaveSellList::CreateFromPattern (RVA 0x103AC0). Moves a named child from the
    // pattern's parent into `owner`, keeping it at the same screen position.
    template <typename T>
    bool GrabControlFromPattern(
        m3d::ui::Wnd* owner, m3d::Object* oldParent, CStr const& childName, char const* rttiClass, T*& out)
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
            M3D_LOG_INFO(
                CStr("GetControlFromPattern error - no child or invalid class; control name - ") + childName);
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

int SaveSellList::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    // RVA 0x103AC0
    if (!patternWnd)
    {
        return 0;
    }

    if (!m3d::ui::Wnd::Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()))
    {
        return 0;
    }

    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());
    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetTextColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    m3d::Object* parent = patternWnd->GetParent();
    if (!parent || !parent->IsKindOf(RT_CLASS_LOCAL(Wnd)))
    {
        return 0;
    }
    parent->AddChild(this);

    m3d::ui::Wnd* embossNext = nullptr;
    m3d::ui::Wnd* embossPrev = nullptr;
    bool const ok1 = GrabControlFromPattern(this, parent, m_aif.m_btnNextName, "ButtonWnd", m_btnNext);
    bool const ok2 = GrabControlFromPattern(this, parent, m_aif.m_btnPrevName, "ButtonWnd", m_btnPrev);
    bool const ok3 = GrabControlFromPattern(this, parent, m_aif.m_wndEmbossBtnNextName, "Wnd", embossNext);
    bool const ok4 = GrabControlFromPattern(this, parent, m_aif.m_wndEmbossBtnPrevName, "Wnd", embossPrev);
    bool const res = ok1 && ok2 && ok3 && ok4;

    if (res)
    {
        BoundsBase<float> const btnNextB = m_btnNext->GetBounds();
        BoundsBase<float> const btnPrevB = m_btnPrev->GetBounds();
        m_listBounds.x0 = btnNextB.width + m_aif.m_space + btnNextB.x0;
        m_listBounds.y0 = 0.0f;
        m_listBounds.width = (btnPrevB.x0 - m_aif.m_space) - m_listBounds.x0;
        m_listBounds.height = GetBounds().height;
    }

    if (deleteSrc)
    {
        delete const_cast<m3d::ui::Wnd*>(patternWnd);
    }

    if (!res)
    {
        M3D_LOG_INFO("SaveSellList: error - fail to init because of a bad resource");
        return 0;
    }

    m_gameDataFlags |= 1u;
    UpdateNextPrevButtonState();
    return 1;
}

// --- button table ---------------------------------------------------------------------------------

int SaveSellList::AddSaveButton(ObjectInfo* objectInfo)
{
    // RVA 0x104430
    if ((m_gameDataFlags & 1) == 0 || !objectInfo)
    {
        return 0;
    }
    for (auto* btn : m_saveButtons)
    {
        if (btn && btn->GetObjectInfo() == objectInfo)
        {
            return 0;
        }
    }

    auto* btn = static_cast<SaveSellButton*>(M3D_KERNEL->New("SaveSellButton"));
    if (!btn)
    {
        return 0;
    }
    if (!btn->SetUp(objectInfo, GetBounds().height, 404))
    {
        delete btn;
        return 0;
    }
    m_saveButtons.push_back(btn);
    if (m_firstSaveBtnId == -1)
    {
        m_firstSaveBtnId = 0;
    }
    RecalcLayot();
    ShowAtEnd();
    return 1;
}

int SaveSellList::RemoveSaveButton(ObjectInfo* objectInfo)
{
    // RVA 0x104520
    if ((m_gameDataFlags & 1) == 0 || !objectInfo)
    {
        return 0;
    }
    for (auto it = m_saveButtons.begin(); it != m_saveButtons.end(); ++it)
    {
        if (*it && (*it)->GetObjectInfo() == objectInfo)
        {
            delete *it;
            m_saveButtons.erase(it);
            if (m_saveButtons.empty())
            {
                m_firstSaveBtnId = -1;
            }
            RecalcLayot();
            ShowAtEnd();
            return 1;
        }
    }
    return 0;
}

int SaveSellList::GetSaveButtonIdByObjectInfo(ObjectInfo* objectInfo) const
{
    // RVA 0x1051C0
    if (!objectInfo)
    {
        return -1;
    }
    for (size_t i = 0; i < m_saveButtons.size(); ++i)
    {
        if (m_saveButtons[i] && m_saveButtons[i]->GetObjectInfo() == objectInfo)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

ObjectInfo* SaveSellList::GetObjectInfoBySaveButtonId(int idx) const
{
    // RVA 0x105210
    if (idx < 0 || idx >= static_cast<int>(m_saveButtons.size()) || !m_saveButtons[idx])
    {
        return nullptr;
    }
    return m_saveButtons[idx]->GetObjectInfo();
}

void SaveSellList::GetSavedObjectInfos(std::vector<ObjectInfo*, std::allocator<ObjectInfo*>>& objectInfos) const
{
    // RVA 0x105350
    objectInfos.clear();
    for (auto* btn : m_saveButtons)
    {
        if (btn)
        {
            objectInfos.push_back(btn->GetObjectInfo());
        }
    }
}

// --- selection -----------------------------------------------------------------------------------

int SaveSellList::GetCurSel() const
{
    // RVA 0x105140
    return m_selBtnId;
}

void SaveSellList::SetCurSel(int selIdx)
{
    // RVA 0x105150
    m_selBtnId = selIdx;
    if (selIdx < 0 || selIdx >= static_cast<int>(m_saveButtons.size()))
    {
        if (selIdx != -1)
        {
            m_selBtnId = -1;
        }
    }
    else
    {
        EnsureShowButton(selIdx);
    }
    SelectButton(m_selBtnId);
}

void SaveSellList::SelectButton(int idx)
{
    // RVA 0x1052F0 (the shipped build inlines SaveSellButton::Select here)
    for (size_t i = 0; i < m_saveButtons.size(); ++i)
    {
        if (auto* btn = m_saveButtons[i])
        {
            btn->Select(static_cast<int>(i) == idx);
        }
    }
}

// --- scrolling ---------------------------------------------------------------------------------

bool SaveSellList::CanScrollNext()
{
    // RVA 0x104820
    int const count = static_cast<int>(m_saveButtons.size());
    return m_lastSaveBtnId < count - 1 && m_lastSaveBtnId != -1;
}

bool SaveSellList::CanScrollPrev()
{
    // RVA 0x104860
    return m_firstSaveBtnId > 0;
}

void SaveSellList::ScrollNext()
{
    // RVA 0x104870
    int const count = static_cast<int>(m_saveButtons.size());
    if (count == 0)
    {
        m_lastSaveBtnId = -1;
    }
    else if (m_lastSaveBtnId < count - 1)
    {
        ++m_lastSaveBtnId;
    }
    RecalcLayot();
}

void SaveSellList::ScrollPrev()
{
    // RVA 0x1048E0
    if (m_saveButtons.empty())
    {
        m_lastSaveBtnId = -1;
    }
    else if (m_firstSaveBtnId > 0)
    {
        --m_firstSaveBtnId;
    }
    RecalcLayot();
}

void SaveSellList::ShowAtBegin()
{
    // RVA 0x1049A0
    while (m_firstSaveBtnId > 0)
    {
        if (!m_saveButtons.empty())
        {
            --m_firstSaveBtnId;
        }
        else
        {
            m_firstSaveBtnId = -1;
        }
        RecalcLayot();
    }
}

void SaveSellList::ShowAtEnd()
{
    // RVA 0x1049F0
    int const count = static_cast<int>(m_saveButtons.size());
    while (m_lastSaveBtnId < count - 1 && m_lastSaveBtnId != -1)
    {
        ScrollNext();
    }
}

void SaveSellList::EnsureShowButton(int idx)
{
    // RVA 0x105250
    if (idx < 0 || idx >= static_cast<int>(m_saveButtons.size()))
    {
        return;
    }
    if (m_firstSaveBtnId == -1 || m_lastSaveBtnId == -1)
    {
        return;
    }
    if (idx <= m_lastSaveBtnId)
    {
        while (idx < m_firstSaveBtnId && m_firstSaveBtnId > 0)
        {
            ScrollPrev();
        }
    }
    else
    {
        int const count = static_cast<int>(m_saveButtons.size());
        while (m_lastSaveBtnId < count - 1 && m_lastSaveBtnId != -1 && idx != m_lastSaveBtnId)
        {
            ScrollNext();
        }
    }
}

void SaveSellList::RecalcLayot()
{
    // RVA 0x104690
    int const start = m_firstSaveBtnId;
    if (start < 0 || start >= static_cast<int>(m_saveButtons.size()))
    {
        return;
    }

    float currentRight = m_listBounds.x0 + m_listBounds.width;
    m_lastSaveBtnId = start;
    for (int i = start; i < static_cast<int>(m_saveButtons.size()); ++i)
    {
        SaveSellButton* btn = m_saveButtons[i];
        if (!btn)
        {
            continue;
        }
        BoundsBase<float> btnB = btn->GetBounds();
        float const x = currentRight - btnB.width;
        if (m_listBounds.x0 > x)
        {
            break;
        }
        btnB.x0 = x;
        btn->SetBounds(btnB, false);
        currentRight = x - m_aif.m_space;
        m_lastSaveBtnId = i;
    }

    for (int i = 0; i < static_cast<int>(m_saveButtons.size()); ++i)
    {
        SaveSellButton* btn = m_saveButtons[i];
        if (!btn)
        {
            continue;
        }
        bool const visible = i >= m_firstSaveBtnId && i <= m_lastSaveBtnId;
        if (visible && !IsDirectChild(btn))
        {
            AddChild(btn);
        }
        else if (!visible && IsDirectChild(btn))
        {
            RemoveChild(btn);
        }
    }
    UpdateNextPrevButtonState();
}

void SaveSellList::UpdateNextPrevButtonState()
{
    // RVA 0x104930
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_btnPrev->EnableWindow(m_firstSaveBtnId > 0);

    int const count = static_cast<int>(m_saveButtons.size());
    bool const canNext = m_lastSaveBtnId < count - 1 && m_lastSaveBtnId != -1;
    m_btnNext->EnableWindow(canNext);
}

// --- notifications / lifecycle -------------------------------------------------------------------

int SaveSellList::OnWndNotify(m3d::ui::Wnd* from, unsigned idFrom, unsigned message, m3d::AIParam const& data)
{
    // RVA 0x1045E0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, idFrom, message, data);
    }

    if (idFrom == 404)
    {
        if (message != 1)
        {
            return 0;
        }
        ReflectChildNotifyToParent(from, 0x194u, 0x15u, data);
        return 1;
    }
    if (idFrom == 500 && message == 1)
    {
        ScrollNext();
        return 1;
    }
    if (idFrom == 501 && message == 1)
    {
        ScrollPrev();
        return 1;
    }
    return 0;
}

int SaveSellList::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0x103A20
    if (!beforeContinuousLevel)
    {
        for (auto* btn : m_saveButtons)
        {
            delete btn;
        }
        m_saveButtons.clear();
    }
    return 1;
}

int SaveSellList::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x104A30
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("SaveSellList::GameDataSave error - window has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("SaveSellList::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr ssListNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "SaveSellList");
    guiNode->AddChild(ssListNode);

    int res = 1;
    for (auto* btn : m_saveButtons)
    {
        ObjectInfo* oi = btn ? btn->GetObjectInfo() : nullptr;
        if (!oi)
        {
            res = 0;
            continue;
        }
        ref_ptr btnNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "SaveButton");
        ssListNode->AddChild(btnNode);
        btnNode->SetAttribute("LevelName", oi->GetLevelName().c_str());
        btnNode->SetAttribute("ObjName", oi->GetName().c_str());
    }
    if (!res)
    {
        M3D_LOG_INFO("SaveSellList::GameDataSave error - saved with errors");
    }
    return res;
}

int SaveSellList::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    // RVA 0x104CD0
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("SaveSellList::GameDataLoad error - window has been not properly inited");
        return 0;
    }
    GameDataClear(false);
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("SaveSellList::GameDataLoad error - invalid params");
        return 0;
    }

    ref_ptr ssListNode = xmlFile->CreateNode();
    guiNode->GetFirstChild(ssListNode, "SaveSellList");
    if (ssListNode->IsEmpty())
    {
        M3D_LOG_INFO("SaveSellList::GameDataLoad error - cannot find node");
        return 0;
    }

    int res = 1;
    ref_ptr btnNode = xmlFile->CreateNode();
    ssListNode->GetFirstChild(btnNode, "SaveButton");
    while (!btnNode->IsEmpty())
    {
        CStr levelName;
        CStr objName;
        m3d::SafeStrAttrib(levelName, btnNode, "LevelName");
        m3d::SafeStrAttrib(objName, btnNode, "ObjName");

        ObjectInfo* oi = M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetObjectInfo(objName, levelName);
        if (oi)
        {
            if (!AddSaveButton(oi))
            {
                M3D_LOG_INFO("SaveSellList::GameDataLoad error - cannot create SaveButton");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO(
                "SaveSellList::GameDataLoad error - ObjectInfo for objName " + objName + " and levelName " +
                levelName + " not found");
            res = 0;
        }
        btnNode->GetNextSibling(btnNode, "SaveButton");
    }
    if (!res)
    {
        M3D_LOG_INFO("SaveSellList::GameDataLoad - loaded with errors");
    }
    return res;
}

#include "bindkeyswnd.h"
#include <core/log.h>
#include <impulses/i_impulses.h>
#include <ui/listbox.h>

#include "m3dapp.h"

RT_CLASS_EXPORTS_BEGIN(BindKeysWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BindKeysWnd);

BindKeysWnd::KeySetButton::AuxInfo::AuxInfo()
{
    // RVA 0x4A5D40
    m_filler = "---";
    m_normColor = 0xFF404040u;
    m_twinkleColor = 0xF0FF0000u;
    m_twinkleTime = 300;
    m_selColor = 0xFFFFCC00u;
}

int BindKeysWnd::KeySetButton::OnMouseButton0(unsigned state, PointBase<float> const& at)
{
    // TODO(RVA 0x4A8010): first press enters input-capture mode (captures focus,
    // CallParentNotify(0x16)); a press while already capturing records the left
    // mouse button (256) into m_newKeySet. Blocked on the input-mode machinery.
    RETRUXX_NOT_IMPLEMENTED;
    return ButtonWnd::OnMouseButton0(state, at);
}

int BindKeysWnd::KeySetButton::OnMouseButton1(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4A80F0
    if (state)
    {
        m_newKeySet.clear();
        m_newKeySet.push_back(257);
    }
    return Wnd::OnMouseButton1(state, at);
}

int BindKeysWnd::KeySetButton::OnMouseButton2(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4A8160
    if (state)
    {
        m_newKeySet.clear();
        m_newKeySet.push_back(258);
    }
    return Wnd::OnMouseButton2(state, at);
}

int BindKeysWnd::KeySetButton::OnKey(unsigned short, unsigned char, unsigned)
{
    // TODO(RVA 0x4A7D60): while capturing, translate the scan code into the
    // m_newKeySet entry (handles modifier chords). Deep switch over key codes.
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnAnotherInputOn()
{
    // TODO(RVA 0x4A82A0): another KeySetButton started capturing - leave input
    // mode and forward the cancel to the base. Reads members the input-mode
    // machinery owns.
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::DrawWndText(m3d::ui::DrawInfo const&)
{
    // TODO(RVA 0x4A8520): while capturing, twinkle the text colour between
    // m_aif.m_normColor / m_twinkleColor on m_twinkleTime, then Wnd::DrawWndText.
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnRebind(std::vector<int> const&)
{
    // TODO(RVA-less): commits a new key set into m_keySet and refreshes the label.
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::KeySetButton::~KeySetButton()
{
    // RVA 0x4A7C80: no explicit body - m_keySet / m_newKeySet / m_aif destruct
    // via their own dtors, ~ButtonWnd chains.
}

void BindKeysWnd::KeySetButton::SetBindText()
{
    // TODO(RVA 0x4A8300): formats m_keySet into a human-readable "Ctrl+A / ..."
    // string and SetText()s it. ~900 lines of key-name lookup.
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnAnotherInputOff()
{
    // RVA 0x4A82F0
    m_isAnotherInInputMode = false;
}

int BindKeysWnd::KeySetButton::SetUp(Impulse, std::vector<int>&)
{
    // TODO(RVA-less): records the impulse id + current key set and calls SetBindText.
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnNcPaint(m3d::ui::DrawInfo const&, unsigned)
{
    // TODO(RVA 0x4A84A0): draws the highlight quad (m_aif.m_selColor) when hovered
    // or capturing. Needs GfxServer::AddFlatAxialQuad.
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::KeySetButton::OnMouseWheel(int, PointBase<float> const&)
{
    // RVA 0x4A81D0: while capturing, swallow the wheel; otherwise the binary
    // forwards it to the parent's OnMouseWheel (protected - not reachable from
    // here). Swallowing is close enough for the key-bindings list.
    return 1;
}

BindKeysWnd::KeySetButton::KeySetButton(BoundsBase<float>&)
{
    // TODO(RVA 0x4A7B80): ButtonWnd::Create with style 0x40E00 / id 0xAAE64, sets
    // m_impId = -1, m_lastTwinkleTime = now, m_style |= WS_NODRAW. Blocked on the
    // partial state the input-mode machinery reads.
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysItem::SetUp(Impulse, std::vector<std::vector<int>>, m3d::Object*, int)
{
    // TODO(RVA 0x4A77B0): builds the row - impulse label + up to two KeySetButtons
    // laid out by m_aif percentages, registered with the UI manager.
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::BindKeysItem::~BindKeysItem()
{
    // RVA 0x4A7700: detach + delete the row's child widgets.
    for (auto*& btn : m_btns)
    {
        if (btn)
        {
            if (auto* parent = btn->GetParent())
            {
                parent->RemoveChild(btn);
            }
            delete btn;
            btn = nullptr;
        }
    }
    if (m_bg)
    {
        if (auto* parent = m_bg->GetParent())
        {
            parent->RemoveChild(m_bg);
        }
        delete m_bg;
        m_bg = nullptr;
    }
    if (m_impLabel)
    {
        if (auto* parent = m_impLabel->GetParent())
        {
            parent->RemoveChild(m_impLabel);
        }
        delete m_impLabel;
        m_impLabel = nullptr;
    }
}

BindKeysWnd::BindKeysItem::BindKeysItem(float width)
{
    // RVA 0x4A76A0
    m_btns[0] = nullptr;
    m_btns[1] = nullptr;
    m_bg = nullptr;
    m_impLabel = nullptr;
    m_impId = IM_ERROR;
    m_aif.m_itemW = width;
}

int BindKeysWnd::BindKeysList::CreateItems()
{
    // TODO(RVA-less): one BindKeysItem per editable impulse (IsImpulseForEdit),
    // each SetUp with the profile's current key sets, then AddItem.
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    // RVA 0x4A7460: {0, 0, item->m_aif.m_itemW, item->m_aif.m_itemH}. m_aif is
    // private to BindKeysItem (no accessor); left as a TODO until one is added.
    (void)itemIdx;
    (void)bounds;
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::Rebind(Impulse)
{
    // TODO(RVA-less): re-reads the profile bindings into every row's KeySetButtons.
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    // TODO(RVA 0x4A74D0): positions the row's bg / label / buttons at the given
    // origin and lets the child widgets paint.
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::CreateFromPattern(m3d::ui::StringsListBoxWnd* patternList, bool deleteSrc)
{
    if (!patternList || !patternList->GetParent())
    {
        return 0;
    }
    Create(patternList->GetBounds(), 0x240, patternList->GetId());
    SetStyle(patternList->GetStyle());
    SetText(patternList->GetText());
    SetId(patternList->GetId());
    SetName(patternList->GetName());
    SetBounds(patternList->GetBounds(), true);
    SetDefaultFont(patternList->GetDefaultFont());
    SetWrapMode(patternList->GetWrapMode());
    SetFormatMode(patternList->GetFormatMode());
    SetColor(patternList->GetColor());
    SetTextColor(patternList->GetTextColor());
    SetTextColorDisabled(patternList->GetTextColorDisabled());
    SetClientEdges(patternList->GetClientEdges());
    SetPane(patternList->GetPaneName());
    SetPaneFlags(patternList->GetPaneFlags());
    SetScrollPane(patternList->GetScrollPaneName());
    SetBackground(patternList->GetBackground());
    CStr prop;
    patternList->GetProperty(0x4000, &prop);
    SetProperty(0x4000, &prop);
    SetOnShowAnimation(patternList->GetOnShowAnimation());
    SetOnHideAnimation(patternList->GetOnHideAnimation());
    m_drawFlags = 5;
    patternList->GetParent()->AddChild(this);
    patternList->GetParent()->MoveChildToFirstPosition(this);
    if (deleteSrc)
    {
        //TODO: check dtor
        patternList->GetParent()->RemoveChild(patternList);
        patternList->DecRef();
    }
    return 1;
}

int BindKeysWnd::BindKeysList::BindUnbind(int, Impulse, std::vector<int> const&)
{
    // TODO(RVA-less): applies/clears a key set for an impulse, running the
    // "already bound / reserved" warning dialogs (m_aif.m_strBind* string ids).
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::DeleteItem(int itemIdx)
{
    // RVA 0x4A7610
    delete m_items[itemIdx].m_item;
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

int BindKeysWnd::BindKeysList::ProcessBinding(KeySetButton*)
{
    // TODO(RVA 0x4A6EA0): validates a KeySetButton's pending key set against the
    // other impulses and either applies it or shows a conflict dialog. ~200 lines.
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::BindKeysList::BindKeysList()
{
    m_drawFlags = 0;
    m_curSel = -1;
    m_clientEdges[2] = 2.0;
}

bool BindKeysWnd::BindKeysList::IsImpulseForEdit(Impulse)
{
    // TODO(RVA 0x4A7440): membership test against the file-local l_impulsesForEdit
    // table (29 entries). Permissive default until the table is recovered.
    return true;
}

int BindKeysWnd::BindKeysList::CompareItem(int, int)
{
    // RVA 0x4A7690
    return 0;
}

int BindKeysWnd::BindKeysList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    // TODO(RVA 0x4A6DB0): routes id 700004 child notifications - ProcessBinding on
    // click/msg 1-3/21, and OnAnotherInputOn/Off broadcast on msg 22/23.
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::OnBeforeAddToWndStation()
{
    if (m_items.empty() || !(m_items.size() / 36))
        CreateItems();
    GetStation()->CaptureFocus(this);
    return Wnd::OnBeforeAddToWndStation();
}

BindKeysWnd::BindKeysList::~BindKeysList()
{
    // RVA 0x4A6680
    RemoveAllItems();
}

BindKeysWnd::BindKeysItem::AuxInfo::AuxInfo()
{
    // RVA 0x4A5CC0
    m_impWPerc = 48.8f;
    m_itemW = 0.0f;
    m_itemH = 32.0f;
    m_lblColor = 0xFF404040u;
    m_texIdBg = "BindKeysLineBg";
}

BindKeysWnd::BindKeysList::AuxInfo::AuxInfo()
{
}

BindKeysWnd::AuxInfo::AuxInfo()
{
}

m3d::Object* BindKeysWnd::CreateObject()
{
    return new BindKeysWnd;
}

m3d::Class* BindKeysWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BindKeysWnd);
}

m3d::Class* BindKeysWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* BindKeysWnd::Clone()
{
    // RVA 0x4A5BB0: allocates, runs the plain Wnd ctor + m_aif, copies nothing.
    return new BindKeysWnd(*this);
}

BindKeysWnd::~BindKeysWnd()
{
    // RVA 0x4A5F60: no owned resources (m_aif CStr + ~Wnd chain).
}

int BindKeysWnd::ApplyChanges(bool bForce)
{
    // RVA 0x4A63A0
    if (!m_bChanged)
    {
        return 1;
    }
    if (!bForce)
    {
        auto const ret = M3D_APP->RunMsgBoxDlg(
            CStr(), M3D_APP->GetStringByStringId0("BindingsChanged"), 3u, false);
        if (ret == m3d::ui::MBX_RET_NO)
        {
            CancelBindings();
            return 1;
        }
        if (ret == m3d::ui::MBX_RET_CANCEL)
        {
            return 0;
        }
    }
    M3D_APP->m_pImpulses->SaveToProfile();
    m_bChanged = false;
    return 1;
}

int BindKeysWnd::OnWndNotify(m3d::ui::Wnd*, unsigned id, unsigned msg, m3d::AIParam const&)
{
    // RVA 0x4A6310
    if (msg != 1)
    {
        return 0;
    }
    switch (id)
    {
    case 0xAAE60u:  // "load from profile"
        M3D_APP->m_pImpulses->LoadFromProfile();
        m_bChanged = false;
        break;
    case 0xAAE61u:  // "save to profile"
        M3D_APP->m_pImpulses->SaveToProfile();
        m_bChanged = false;
        return 1;
    case 0xAAE62u:  // "load defaults"
        M3D_APP->m_pImpulses->LoadFromDefaults();
        m_bChanged = true;
        return 1;
    }
    return 1;
}

int BindKeysWnd::OnBeforeAddToWndStation()
{
    this->m_bChanged = false;
    if (m_isInited && this->m_isDirty)
        m_lstBindings->Rebind(IM_ERROR);
    return Wnd::OnBeforeAddToWndStation();
}

void BindKeysWnd::LoadDefaultBindings()
{
    // RVA 0x4A6500
    M3D_APP->m_pImpulses->LoadFromDefaults();
    m_bChanged = true;
}

int BindKeysWnd::GameDataSetup()
{
    using namespace m3d::ui;
    if (m_isInited)
    {
        return 1;
    }
    auto child = GetChildByName(m_aif.m_lstBindingsName);
    if (!child || !child->IsKindOf(RT_CLASS_LOCAL(StringsListBoxWnd)))
    {
        M3D_LOG_INFO("Make control error: control " + m_aif.m_lstBindingsName + " is not found or incorrect type");
        M3D_LOG_INFO("BindKeysWnd error: fail to init because of a bad resource");
        return 0;
    }
    auto stringBox = dynamic_cast<StringsListBoxWnd*>(child);
    m_lstBindings = new BindKeysList;
    if (!m_lstBindings)
    {
        M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_lstBindingsName + " - cannot instantiate variable");
        M3D_LOG_INFO("BindKeysWnd error: fail to init because of a bad resource");
        return 0;
    }
    if (m_lstBindings->CreateFromPattern(stringBox, true))
    {
        m_isInited = true;
        return 1;
    }
    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_lstBindingsName + " from pattern class");
    M3D_LOG_INFO("BindKeysWnd error: fail to init because of a bad resource");
    return 0;
}

BindKeysWnd::BindKeysWnd(BindKeysWnd const&)
{
    // RVA 0x4A5FA0: default-constructs the Wnd base + m_aif and copies nothing.
}

BindKeysWnd::BindKeysWnd()
{
}

int BindKeysWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4A6520
    if (dataType != 17)
    {
        return 1;
    }
    if (!IsChildOf(M3D_APP))
    {
        m_isDirty = true;
        return 1;
    }
    if (data)
    {
        m_lstBindings->Rebind(IM_ERROR);
    }
    return 1;
}

void BindKeysWnd::ApplyBindings()
{
    // RVA 0x4A64E0
    M3D_APP->m_pImpulses->SaveToProfile();
    m_bChanged = false;
}

void BindKeysWnd::CancelBindings()
{
    // RVA 0x4A64C0
    M3D_APP->m_pImpulses->LoadFromProfile();
    m_bChanged = false;
}

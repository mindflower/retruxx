#include "bindkeyswnd.h"
#include <core/log.h>
#include <ui/listbox.h>

#include "m3dapp.h"

RT_CLASS_EXPORTS_BEGIN(BindKeysWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BindKeysWnd);

BindKeysWnd::KeySetButton::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::KeySetButton::OnMouseButton0(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::KeySetButton::OnMouseButton1(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::KeySetButton::OnMouseButton2(unsigned, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::KeySetButton::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnAnotherInputOn()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::DrawWndText(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnRebind(std::vector<int> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::KeySetButton::~KeySetButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::SetBindText()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnAnotherInputOff()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::KeySetButton::SetUp(Impulse, std::vector<int>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::KeySetButton::OnNcPaint(m3d::ui::DrawInfo const&, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::KeySetButton::OnMouseWheel(int, PointBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::KeySetButton::KeySetButton(BoundsBase<float>&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysItem::SetUp(Impulse, std::vector<std::vector<int>>, m3d::Object*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::BindKeysItem::~BindKeysItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::BindKeysItem::BindKeysItem(float)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::Rebind(Impulse)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
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
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::ProcessBinding(KeySetButton*)
{
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
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::BindKeysList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
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
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::BindKeysItem::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::~BindKeysWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::ApplyChanges(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int BindKeysWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

BindKeysWnd::BindKeysWnd()
{
}

int BindKeysWnd::GameDataUpdate(void* data, int dataType)
{
    if (dataType != 17)
        return 1;
    if (!IsChildOf(M3D_APP))
    {
        m_isDirty = 1;
        return 1;
    }
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::ApplyBindings()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void BindKeysWnd::CancelBindings()
{
    RETRUXX_NOT_IMPLEMENTED;
}

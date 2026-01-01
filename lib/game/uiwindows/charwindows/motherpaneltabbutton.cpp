#include "motherpaneltabbutton.h"
#include <core/log.h>
#include <game/m3dgame.h>

RT_CLASS_EXPORTS_BEGIN(MotherPanelTabButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MotherPanelTabButton);

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetUnselTex() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::PerModeInfo::PerModeInfo(m3d::rend::TexHandle, m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetSelTex() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::PerModeInfo::~PerModeInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MotherPanelTabButton::GetClass() const
{
    return RT_CLASS_LOCAL(MotherPanelTabButton);
}

MotherPanel::Tab MotherPanelTabButton::GetTabId() const
{
    return m_tabId;
}

void MotherPanelTabButton::Select(bool bSelect)
{
    m_bSelected = bSelect;
    UpdateTex();
}

bool MotherPanelTabButton::IsSelected() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MotherPanelTabButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::Mode MotherPanelTabButton::GetMode() const
{
    return m_mode;
}

m3d::Class* MotherPanelTabButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

MotherPanelTabButton::~MotherPanelTabButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MotherPanelTabButton::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // TODO: check this
    if (!patternWnd || !patternWnd->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
    {
        M3D_LOG_INFO("OptionTabButton::CreateFromPattern error - null patternWnd or class does not match");
        return 0;
    }

    auto buttonWnd = dynamic_cast<ButtonWnd*>(patternWnd);

    auto const bounds = patternWnd->GetBounds();
    auto res = Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    if (res == 0)
    {
        M3D_LOG_INFO("MotherPanelTabButton::CreateFromPattern error - cannot create window");
        return 0;
    }
    SetStyle(buttonWnd->GetStyle());
    SetText(buttonWnd->GetText());
    SetId(buttonWnd->GetId());
    SetName(buttonWnd->GetName());
    SetBounds(buttonWnd->GetBounds(), true);
    SetDefaultFont(buttonWnd->GetDefaultFont());
    SetWrapMode(buttonWnd->GetWrapMode());
    SetFormatMode(buttonWnd->GetFormatMode());
    SetColor(buttonWnd->GetColor());
    SetTextColor(buttonWnd->GetTextColor());
    SetTextColorDisabled(buttonWnd->GetTextColorDisabled());
    SetClientEdges(buttonWnd->GetClientEdges());
    SetPane(buttonWnd->GetPaneName());
    SetPaneFlags(buttonWnd->GetPaneFlags());
    SetScrollPane(buttonWnd->GetScrollPaneName());
    SetBackground(buttonWnd->GetBackground());

    CStr tooltip;
    buttonWnd->GetProperty(0x4000, &tooltip);
    SetProperty(0x4000, &tooltip);

    SetOnShowAnimation(buttonWnd->GetOnShowAnimation());
    SetOnHideAnimation(buttonWnd->GetOnHideAnimation());

    if (buttonWnd->IsImaged())
    {
        SetImaged(
            buttonWnd->GetImageRegular(),
            buttonWnd->GetImageDown(),
            buttonWnd->GetImageIn(),
            buttonWnd->GetImageDisabled());
    }
    else
    {
        SetRegular();
    }

    auto parent = patternWnd->GetParent();
    if (parent)
    {
        parent->AddChild(this);
        parent->MoveChildToFirstPosition(this);
        if (deleteSrc)
        {
            parent->RemoveChild(patternWnd);
            delete patternWnd;
        }
        return 1;
    }

    M3D_LOG_INFO("MotherPanelTabButton::CreateFromPattern error - null parent for paternWnd");
    return 0;
}

int MotherPanelTabButton::SetupForTab(MotherPanel::Tab tabId)
{
    if (tabId == MotherPanel::TAB_NUM_TABS)
    {
        return 0;
    }
    m_tabId = tabId;
    InitInfo();
    UpdateTex();
    UpdateTooltip();
    return 1;
}

void MotherPanelTabButton::SetMode(Mode mode)
{
    if (m_mode != MODE_NUM_MODES && HasMode(mode) && CanApplyMode(mode))
    {
        m_mode = mode;
        UpdateTex();
        UpdateTooltip();
    }
}

m3d::Object* MotherPanelTabButton::CreateObject()
{
    return new MotherPanelTabButton;
}

void MotherPanelTabButton::UpdateTex()
{
    if (m_mode != MODE_NUM_MODES)
    {
        if (m_info[m_mode])
        {
            if (m_bSelected)
                SetImaged(m_info[m_mode]->GetSelTex(), {}, {}, {});
            else
                SetImaged(m_info[m_mode]->GetUnselTex(), {}, {}, {});
        }
    }
}

void MotherPanelTabButton::ClearInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr MotherPanelTabButton::Mode2Str(Mode)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::MotherPanelTabButton(MotherPanelTabButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MotherPanelTabButton::MotherPanelTabButton()
{
    m_bSelected = 0;
    m_mode = MODE_IN_FIELD;
    m_tabId = MotherPanel::TAB_NUM_TABS;
    m_info[0] = 0;
    m_info[1] = 0;
}

void MotherPanelTabButton::UpdateTooltip()
{
    // TODO: implement MotherPanelTabButton::UpdateTooltip
    // RETRUXX_NOT_IMPLEMENTED;
}

bool MotherPanelTabButton::CanApplyMode(Mode mode) const
{
    if (mode == MODE_IN_FIELD)
        return M3D_APP->m_pInterfaceManager->GetCurrentTown() == nullptr;
    if (mode == MODE_IN_TOWN)
        return GetBuilding(MODE_IN_TOWN) != nullptr;
    return false;
}

bool MotherPanelTabButton::HasMode(Mode mode) const
{
    bool result = false;
    switch (m_tabId)
    {
    case MotherPanel::TAB_QUESTLOG:
    case MotherPanel::TAB_MAP:
    case MotherPanel::TAB_JOURNAL:
        result = mode == MODE_IN_FIELD;
        break;

    case MotherPanel::TAB_INVENTORY_VS_SHOP:
    case MotherPanel::TAB_CHARACTERISTIC_VS_WORKSHOP:
        result = true;
        break;

    case MotherPanel::TAB_BAR:
    case MotherPanel::TAB_ADDITIONAL_BUILDING:
        result = mode == MODE_IN_TOWN;
        break;

    default:
        result = false;
        break;
    }
    return result;
}

void MotherPanelTabButton::InitInfo()
{
    // TODO: implement MotherPanelTabButton::InitInfo
    // RETRUXX_NOT_IMPLEMENTED;
}

ai::Building const* MotherPanelTabButton::GetBuilding(Mode) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

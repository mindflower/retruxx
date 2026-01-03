#include "motherpaneltabbutton.h"
#include <core/log.h>
#include <game/m3dgame.h>

RT_CLASS_EXPORTS_BEGIN(MotherPanelTabButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MotherPanelTabButton);

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetUnselTex() const
{
    return m_selTex;
}

MotherPanelTabButton::PerModeInfo::PerModeInfo(m3d::rend::TexHandle selTex, m3d::rend::TexHandle unselTex)
{
    m_selTex = selTex;
    m_unselTex = unselTex;
    M3D_RENDERER->ReferenceTexture(m_selTex);
    M3D_RENDERER->ReferenceTexture(m_unselTex);
}

m3d::rend::TexHandle MotherPanelTabButton::PerModeInfo::GetSelTex() const
{
    return m_unselTex;
}

MotherPanelTabButton::PerModeInfo::~PerModeInfo()
{
    M3D_RENDERER->ReleaseTexture(m_selTex);
    M3D_RENDERER->ReleaseTexture(m_unselTex);
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
    for (auto& info : m_info)
    {
        if (info)
        {
            M3D_RENDERER->ReleaseTexture(info->GetSelTex());
            M3D_RENDERER->ReleaseTexture(info->GetUnselTex());
            delete info;
            info = nullptr;
        }
    }
}

CStr MotherPanelTabButton::Mode2Str(Mode mode)
{
    static retruxx::map<Mode, CStr> const converter = {
        {MODE_IN_FIELD, "InField"},
        {MODE_IN_TOWN, "InTown"},
    };

    auto const it = converter.find(mode);
    if (it != converter.end())
    {
        return it->second;
    }
    return "";

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
    // TODO: generated code MotherPanelTabButton::InitInfo
    // Clear existing info
    ClearInfo();

    // Check if tab ID is valid
    if (m_tabId == MotherPanel::TAB_NUM_TABS)
    {
        return;
    }

    // Initialize info for each mode
    for (int mode = MODE_IN_FIELD; mode < MODE_NUM_MODES; ++mode)
    {
        Mode currentMode = static_cast<Mode>(mode);

        // Check if this tab should have info for this mode
        bool shouldInit = false;

        switch (m_tabId)
        {
        case MotherPanel::TAB_QUESTLOG:
        case MotherPanel::TAB_MAP:
        case MotherPanel::TAB_JOURNAL:
            // These tabs only have info in field mode
            shouldInit = (currentMode == MODE_IN_FIELD);
            break;

        case MotherPanel::TAB_INVENTORY_VS_SHOP:
        case MotherPanel::TAB_CHARACTERISTIC_VS_WORKSHOP:
            // These tabs have info in both modes
            shouldInit = true;
            break;

        case MotherPanel::TAB_BAR:
        case MotherPanel::TAB_ADDITIONAL_BUILDING:
            // These tabs only have info in town mode
            shouldInit = (currentMode == MODE_IN_TOWN);
            break;

        default:
            // Other tabs don't have per-mode info
            break;
        }

        if (shouldInit)
        {
            // Build the texture name
            CStr baseName = "TabBtn_";
            baseName += MotherPanel::Tab2Str(m_tabId);
            baseName += "_";
            baseName += Mode2Str(currentMode);

            // Get textures from interface manager
            CStr selectedTexName = baseName;    // Selected texture
            CStr unselectedTexName = baseName;  // Unselected texture

            // Get textures (the original code seems to pass 0/1 as flags)
            auto selectedTex = M3D_APP->m_pInterfaceManager->GetIcoByName(selectedTexName, 0);

            auto unselectedTex = M3D_APP->m_pInterfaceManager->GetIcoByName(unselectedTexName, 1);

            // Create and store per-mode info
            m_info[mode] = new PerModeInfo(selectedTex, unselectedTex);
        }
        else
        {
            m_info[mode] = nullptr;
        }
    }
}

ai::Building const* MotherPanelTabButton::GetBuilding(Mode) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

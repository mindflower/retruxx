#include "optionswnd.h"

#include "controloptionswnd.h"
#include "gamemenu.h"
#include "videooptionswnd.h"
#include "core/kernel.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uiwindows/charwindows/motherpanel.h"

RT_CLASS_EXPORTS_BEGIN(OptionTabButton)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(OptionTabButton);

RT_CLASS_EXPORTS_BEGIN(OptionsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(OptionsWnd);

OptionsWnd::AuxInfo::AuxInfo()
{
}

m3d::Class* OptionsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(OptionsWnd);
}

int OptionsWnd::GameDataSetup()
{
    using namespace m3d::ui;
    auto res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        for (int i = 0; i < TAB_NUM_TABS; ++i)
        {
            auto child = dynamic_cast<Wnd*>(GetChildByName(m_aif.m_tabButtonNames[i]));
            if (child && child->IsKindOf(RT_CLASS_LOCAL(ButtonWnd)))
            {
                auto& tabButton = m_tabButtons[i];
                tabButton = dynamic_cast<OptionTabButton*>(M3D_KERNEL->New("OptionTabButton"));
                if (tabButton)
                {
                    if (!tabButton->CreateFromPattern(child, true))
                    {
                        M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_tabButtonNames[i] + " from pattern class");
                        res = 0;
                    }
                }
                else
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_tabButtonNames[i] + " - cannot find rtti class OptionTabButton");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_tabButtonNames[i] + " is not found or incorrect type");
                res = 0;
            }
            if (auto const& tabButton = m_tabButtons[i])
            {
                tabButton->SetupForTab(static_cast<Tab>(i));
            }
            auto wndId = -1;
            switch(i)
            {
            case 0:
            {
                wndId = 149;
                break;
            }
            case 1:
            {
                wndId = 150;
                break;
            }
            case 2:
            {
                wndId = 151;
                break;
            }
            case 3:
            {
                wndId = 152;
                break;
            }
            default:
                break;
            }
            if (auto const wnd = M3D_APP->m_pInterfaceManager->GetWindow(wndId))
            {
                m_optionWindows[i] = wnd;
            }
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
    M3D_LOG_INFO("OptionsWnd: error - fail to init because of a bad resource");
    return 0;
}

m3d::Class* OptionsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Object* OptionsWnd::CreateObject()
{
    return new OptionsWnd;
}

int OptionsWnd::OnBeforeAddToWndStation()
{
    auto const curTab = m_lastTabId != TAB_NUM_TABS ? m_lastTabId : TAB_VIDEO;
    if (curTab == m_curTabId
        || !ShowOptionWindowForTab(curTab))
    {
        return Wnd::OnBeforeAddToWndStation();
    }
    m_curTabId = curTab;
    if (curTab != TAB_NUM_TABS)
    {
        m_lastTabId = curTab;
    }
    SelectTabButton(curTab);
    return Wnd::OnBeforeAddToWndStation();
}

OptionsWnd::OptionsWnd() :
    m_tabButtons(4, nullptr),
    m_optionWindows(4, nullptr)
{

}

OptionsWnd::OptionsWnd(OptionsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void OptionsWnd::UpdateTabButtonsStates()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int OptionsWnd::SetCurTab(Tab tabId)
{
    if (tabId == this->m_curTabId)
        return 1;
    auto result = ShowOptionWindowForTab(tabId);
    if (!result)
        return result;
    m_curTabId = tabId;
    if (tabId != TAB_NUM_TABS)
        this->m_lastTabId = tabId;
    SelectTabButton(tabId);
    return 1;
}

void OptionsWnd::SelectTabButton(Tab tabId)
{
    if ((m_gameDataFlags & 1) != 0)
    {
        for (int i = 0; i < m_tabButtons.size(); ++i)
        {
            auto const& tabButton = m_tabButtons[i];
            tabButton->Select(i == tabId);
        }
    }
}

int OptionsWnd::ApplyTabChanges(Tab tabId)
{
    // TODO: generated code
    // Check if there are any game data changes that need to be applied
    if ((m_gameDataFlags & 1) != 0)
    {
        switch (tabId)
        {
        case TAB_VIDEO:
        {
            // Get the video options window
            ref_ptr<m3d::ui::Wnd>& videoWindowRef = m_optionWindows[0];
            if (videoWindowRef && videoWindowRef->IsKindOf(&VideoOptionsWnd::m_classVideoOptionsWnd))
            {
                VideoOptionsWnd* videoWnd = static_cast<VideoOptionsWnd*>(&*videoWindowRef);

                // Check if video settings have been modified
                if (videoWnd->IsChanged())
                {
                    // TOD: check this!
                    // Show warning dialog about potential performance impact
                    auto warningResult = videoWnd->RunChangeWarningDlg();

                    switch (warningResult)
                    {
                    case m3d::ui::MBX_RET_CANCEL:
                        // User canceled - don't apply changes
                        break;

                    case m3d::ui::MBX_RET_USER:
                    {
                        // User confirmed - apply all video changes
                        videoWnd->ApplyResolution();
                        videoWnd->ApplyGamma();
                        videoWnd->ApplyFarDistance();
                        videoWnd->ApplyGrass();
                        videoWnd->ApplyShadows();
                        videoWnd->ApplyWaterQuality();
                        videoWnd->ApplyAntialiasing();
                        videoWnd->ApplyFiltration();
                        videoWnd->ApplyBlum();

                        // Reset modified flag
                        videoWnd->m_bVideoOptionsChanged = false;
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
            break;
        }

        case TAB_CONTROL:
        {
            // Get the control options window
            ref_ptr<m3d::ui::Wnd> controlWindowRef = m_optionWindows[2];
            if (controlWindowRef && controlWindowRef->IsKindOf(&ControlOptionsWnd::m_classControlOptionsWnd))
            {
                ControlOptionsWnd* controlWnd = static_cast<ControlOptionsWnd*>(&*controlWindowRef);

                // Apply control changes (no confirmation needed)
                return controlWnd->ApplyChanges(false);
            }
            break;
        }

        default:
            // Other tabs (audio, game, etc.) - no special handling needed
            break;
        }
    }

    // Changes applied successfully or no changes to apply
    return 1;
}

int OptionsWnd::OnAfterRemoveFromWndStation()
{
    auto res = Wnd::OnAfterRemoveFromWndStation();
    if (m_curTabId != TAB_NUM_TABS && ShowOptionWindowForTab(TAB_NUM_TABS))
    {
        m_curTabId = TAB_NUM_TABS;
        SelectTabButton(TAB_NUM_TABS);
    }
    if (M3D_APP->GetCurGameMode())
    {
        return res;
    }
    M3D_APP->EnqueueMessage(65656, 0, 0, 0, 0, GameMenuWnd::ROOT_LEVEL_NAME, {});
    return res;
}

int OptionsWnd::ShowOptionWindowForTab(Tab tabId)
{
    if (m_curTabId != TAB_NUM_TABS)
    {
        if (auto const& wnd = m_optionWindows[m_curTabId])
        {
            if (IsDirectChild(wnd))
            {
                auto const res = ApplyTabChanges(m_curTabId);
                if (!res)
                {
                    return res;
                }
                RemoveChild(wnd);
            }
        }
    }
    if (tabId == TAB_NUM_TABS)
    {
        return 1;
    }
    auto const& wnd = m_optionWindows[tabId];
    if (!wnd)
    {
        return 1;
    }
    if (IsDirectChild(wnd))
    {
        return 1;
    }
    AddChild(wnd);
    MoveChildToFirstPosition(wnd);
    return 1;
}

int OptionsWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if (ModalWnd::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }
    if (id == 10000)
    {
        if (msg == 1)
        {
            M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
            return 1;
        }
    }
    else if (id == 10001 && msg == 1)
    {
        if (from->IsKindOf(RT_CLASS_LOCAL(OptionTabButton)))
        {
            auto const* tabButton = dynamic_cast<OptionTabButton*>(from);
            SetCurTab(tabButton->GetTabId());
        }
        return 1;
    }
    return 0;
}

int OptionsWnd::GetOptionWindowGuiIdByTabId(Tab) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int OptionsWnd::CanClose()
{
    if (m_curTabId == TAB_NUM_TABS)
    {
        return 1;
    }
    auto result = ShowOptionWindowForTab(TAB_NUM_TABS);
    if (!result)
    {
        return result;
    }
    m_curTabId = TAB_NUM_TABS;
    SelectTabButton(TAB_NUM_TABS);
    return 1;
}

OptionTabButton::AuxInfo::AuxInfo()
{
}

m3d::Class* OptionTabButton::GetClass() const
{
    return RT_CLASS_LOCAL(OptionTabButton);
}

m3d::Object* OptionTabButton::CreateObject()
{
    return new OptionTabButton;
}

OptionsWnd::Tab OptionTabButton::GetTabId() const
{
    return m_tabId;
}

int OptionTabButton::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
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
        M3D_LOG_INFO("OptionTabButton::CreateFromPattern error - cannot create window");
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
        SetImaged(buttonWnd->GetImageRegular(), buttonWnd->GetImageDown(), buttonWnd->GetImageIn(), buttonWnd->GetImageDisabled());
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

    M3D_LOG_INFO("OptionTabButton::CreateFromPattern error - null parent for paternWnd");
    return 0;
}

void OptionTabButton::Select(bool bSelect)
{
    m_bSelected = bSelect;
    SetPane(m_bSelected ? m_aif.m_paneNameSel : m_aif.m_paneNameUnsel);
}

int OptionTabButton::SetupForTab(OptionsWnd::Tab tab)
{
    if (tab == OptionsWnd::TAB_NUM_TABS)
    {
        return 0;
    }
    m_tabId = tab;
    SetPane(m_bSelected ? m_aif.m_paneNameSel : m_aif.m_paneNameUnsel);
    return 1;
}

m3d::Object* OptionTabButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

OptionTabButton::~OptionTabButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool OptionTabButton::IsSelected() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* OptionTabButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

OptionTabButton::OptionTabButton(OptionTabButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

OptionTabButton::OptionTabButton()
{
}

void OptionTabButton::UpdatePane()
{
    RETRUXX_NOT_IMPLEMENTED;
}

#include "cinemapanel.h"

#include "autoscrolltextwnd.h"
#include "config.h"
#include "game/m3dgame.h"
#include "core/log.h"
#include "core/timer.h"
#include "game/uimanager/uidefs.h"
#include "game/uimisc/guihelper.h"
#include "game/uiwindows/commonwindows/itemmodelwnd.h"
#include "ui/image.h"
#include "ui/modelwnd.h"
#include <server/objects/player.h>

RT_CLASS_EXPORT_METHOD_DEFINE(CinemaPanel, AddMessage)
{
    auto panel = (CinemaPanel*)context->asObject(0, "CinemaPanel");
    int msgId = context->asInt(1);
    float delay = context->asFloat(2);
    panel->AddMessage(msgId, delay);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(CinemaPanel, ClearMessages)
{
    auto panel = (CinemaPanel*)context->asObject(0, "CinemaPanel");
    panel->ClearMessages();
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(CinemaPanel)
RT_CLASS_EXPORT(CinemaPanel, m3d::METHOD, AddMessage, "", "", "")
RT_CLASS_EXPORT(CinemaPanel, m3d::METHOD, ClearMessages, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CinemaPanel);

CinemaPanel::MessageTimeInfo::MessageTimeInfo(int msgId, float delay)
{
    if (delay < 0.0)
    {
        delay = 0.0;
    }

    m_msgId = msgId;
    m_delay = delay;
}

CinemaPanel::AuxInfo::AuxInfo()
{
    m_wndPortraitName = "wndPortrait_";
    m_wndTextName = "wndText";
    m_wndPortraitOverlayName = "wndPortraitOverlay";
    m_wndUpPanelName = "UpPanel";
    m_wndDownPanelName = "DownPanel";
    m_wndScrollImageName = "wndScrollImage";
    m_wndScrollImageUpOverlayName = "wndScrollImageUpOverlay";
    m_wndScrollImageDownOverlayName = "wndScrollImageDownOverlay";
    m_wndScrollTextName = "wndScrollText";
}

m3d::Class* CinemaPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* CinemaPanel::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::AddMessage(int msgId, float delay)
{
    MessageTimeInfo info(msgId, delay);
    m_msgInfos.push_back(std::move(info));
}

unsigned CinemaPanel::GetTimeToTheEndOfMsg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool CinemaPanel::MessageQueueIsEmpty() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::OnHide()
{
    m_msgInfos.clear();
    M3D_APP->m_pInterfaceManager->ShowWindow(IW_WND_CINEMA_PANEL, false, false, false, false, nullptr);

    m_panelType = PANELTYPE_NORMAL;
    ShowControlsForPanelType(PANELTYPE_NORMAL);
    if (m_panelType)
    {
        if (m_panelType == PANELTYPE_SCROLL)
        {
            StopSound();
            m_bIsShowingMessage = false;
            m_curMessageEndTime = M3D_KERNEL->GetTimer().GetCurTime();
        }
    }
    else
    {
        ClearNormal();
    }
}

void CinemaPanel::SkipMessage()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool CinemaPanel::HasMsg()
{
    return !m_msgInfos.empty() || m_bIsShowingMessage;
}

void CinemaPanel::ClearMessages()
{
    m_msgInfos.clear();
}

m3d::Object* CinemaPanel::CreateObject()
{
    return new CinemaPanel;
}

CinemaPanel::~CinemaPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CinemaPanel::GetClass() const
{
    return RT_CLASS_LOCAL(CinemaPanel);
}

void CinemaPanel::Clear()
{
    if (m_panelType)
    {
        if (m_panelType == PANELTYPE_SCROLL)
        {
            StopSound();
            m_bIsShowingMessage = false;
            m_curMessageEndTime = M3D_KERNEL->GetTimer().GetCurTime();
        }
    }
    else
    {
        ClearNormal();
    }
}

void CinemaPanel::ClearNormal()
{
    StopSound();
    m_bIsShowingMessage = false;
    m_curMessageEndTime = M3D_KERNEL->GetTimer().GetCurTime();
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndText->SetText({});
        m_wndText->ShowWindow(false);
        for (auto* wndPortrait : m_wndsPortraits)
        {
            wndPortrait->SetModelByName({}, 0, 0);
            wndPortrait->ShowWindow(false);
        }
    }
}

int CinemaPanel::OnPaint(const m3d::ui::DrawInfo& di)
{
    // TODO: check this
    if (m_bSkipMessage)
    {
        Clear();
        m_bSkipMessage = false;
        if (m_msgInfos.empty())
        {
            m3d::AuxImpulseInfo info(3, true, M3D_APP->GetCurGameMode(), 0, 0);
            M3D_APP->OnChangeMode(info);
        }
        else
        {
            auto& frontMsg = m_msgInfos.front();
            frontMsg.m_delay = 0.0f;
        }
    }
    if (m_bIsShowingMessage)
    {
        const auto diff = M3D_KERNEL->GetTimer().GetCurTime() - m_curMessageStartTime;
        if (diff >= 1000 * m_minTimeToExists)
        {
            if (!M3D_APP->m_sound || m_soundChannelId == -1 || !M3D_APP->m_sound->IsChannelPlaying(m_soundChannelId))
            {
                Clear();
            }
        }
    }
    if (!m_msgInfos.empty() && !m_bIsShowingMessage)
    {
        const auto& nextMsg = m_msgInfos.front();
        const auto msgId = nextMsg.m_msgId;
        const auto delay = nextMsg.m_delay;
        const auto timeSinceLastMessage = M3D_KERNEL->GetTimer().GetCurTime() - m_curMessageEndTime;

        if (timeSinceLastMessage >= (delay * 1000.0f))
        {
            _SetMsg(msgId);
            m_msgInfos.pop_front();
        }
    }
    if (m_panelType == PANELTYPE_NORMAL)
    {
        UpdateAnimation();
    }
    return Wnd::OnPaint(di);
}

void CinemaPanel::SetPanelTypeForMsg(int msgId)
{
    m_panelType = PANELTYPE_NORMAL;
    auto* msgManager = M3D_APP->m_pInterfaceManager->GetMsgManager();
    const auto* msgInfo = msgManager->GetMsgInfo(msgId);
    if (msgInfo)
    {
        m_panelType = msgInfo->GetMsgType() == MsgInfo::MSGTYPE_SCROLL ? PANELTYPE_SCROLL : PANELTYPE_NORMAL;
    }
    ShowControlsForPanelType(m_panelType);
}

int CinemaPanel::_SetMsg(int msgId)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    SetPanelTypeForMsg(msgId);
    Clear();

    auto* msgManager = M3D_APP->m_pInterfaceManager->GetMsgManager();
    const auto* msgInfo = msgManager->GetMsgInfo(msgId);
    if (!msgInfo)
    {
        return 0;
    }

    if (m_panelType)
    {
        if (m_panelType == PANELTYPE_SCROLL)
        {
            InitControlsForMsgScroll(msgInfo);
        }
    }
    else
    {
        InitControlsForMsgNormal(msgInfo);
    }
    if (ai::thePlayer)
    {
        ai::thePlayer->CauseEvent(ai::GE_START_CINEMATIC_MSG, 0.0, msgId, {});
    }
    return 1;
}

void CinemaPanel::SetupPortrait(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::ClearBase()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::HideAllControls()
{
    retruxx::vector<Wnd*> controls;
    for (int i = PANELTYPE_NORMAL; i < PANELTYPE_NUM_PANEL_TYPES; ++i)
    {
        GetControlsByPanelType(static_cast<PanelType>(i), controls, true);
    }

    for (auto* obj : controls)
    {
        if (obj && IsDirectChild(obj))
        {
            RemoveChild(obj);
        }
    }
}

void CinemaPanel::GetControlsByPanelType(PanelType panelType, std::vector<m3d::ui::Wnd*>& controls, bool bAdd) const
{
    if (!bAdd)
    {
        controls.clear();
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        if (panelType)
        {
            if (panelType == PANELTYPE_SCROLL)
            {
                controls.push_back(m_wndScrollImageUpOverlay);
                controls.push_back(m_wndScrollImageDownOverlay);
                controls.push_back(m_wndScrollText);
                controls.push_back(m_wndScrollImage);
            }
        }
        else
        {
            controls.push_back(m_wndText);
            controls.push_back(m_wndPortraitOverlay);
            controls.push_back(m_wndsPortraits[0]);
            controls.push_back(m_wndsPortraits[1]);
            controls.push_back(m_wndUpPanel);
            controls.push_back(m_wndDownPanel);
        }
    }
}

void CinemaPanel::SetupTextScroll(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::GetAllControls(std::vector<m3d::ui::Wnd*>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupSound(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::DeleteAllControls()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::GameDataSetup()
{
    if ((m_gameDataFlags & 2) == 0)
    {
        bool success = true;

        auto child = GetChildByName(m_aif.m_wndTextName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndText = dynamic_cast<Wnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndTextName + " is not found or incorrect type");
            success = false;
        }

        child = GetChildByName(m_aif.m_wndUpPanelName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndUpPanel = dynamic_cast<Wnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndUpPanelName + " is not found or incorrect type");
            success = false;
        }

        child = GetChildByName(m_aif.m_wndDownPanelName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndDownPanel = dynamic_cast<Wnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndDownPanelName + " is not found or incorrect type");
            success = false;
        }

        child = GetChildByName(m_aif.m_wndScrollImageName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndScrollImage = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndScrollImageName + " is not found or incorrect type");
            success = false;
        }

        child = GetChildByName(m_aif.m_wndScrollImageUpOverlayName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndScrollImageUpOverlay = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndScrollImageUpOverlayName + " is not found or incorrect type");
            success = false;
        }

        child = GetChildByName(m_aif.m_wndScrollImageDownOverlayName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndScrollImageDownOverlay = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndScrollImageDownOverlayName + " is not found or incorrect type");
            success = false;
        }

        child = GetChildByName(m_aif.m_wndPortraitOverlayName);
        if (child && child->IsKindOf(&m3d::ui::ImageWnd::m_classWnd))
        {
            m_wndPortraitOverlay = dynamic_cast<m3d::ui::ImageWnd*>(child);
        }
        else
        {
            M3D_LOG_ERR("Get control error: control " + m_aif.m_wndPortraitOverlayName + " is not found or incorrect type");
            success = false;
        }

        child = GetChildByName(m_aif.m_wndScrollTextName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndScrollText = static_cast<AutoScrollTextWnd*>(M3D_KERNEL->New("AutoScrollTextWnd"));
            if (m_wndScrollText)
            {
                if (!m_wndScrollText->CreateFromPattern(dynamic_cast<m3d::ui::Wnd*>(child), true))
                {
                    M3D_LOG_ERR("Make control error: cannot create " + m_aif.m_wndScrollTextName + " from pattern class");
                    success = false;
                }
            }
            else
            {
                M3D_LOG_ERR("Make control error: cannot create " + m_aif.m_wndScrollTextName + " - cannot find rtti class AutoScrollTextWnd");
                success = false;
            }
        }
        else
        {
            M3D_LOG_ERR("Make control error: control " + m_aif.m_wndScrollTextName + " is not found or incorrect type");
            success = false;
        }

        for (int i = 0; i < 2; ++i)
        {
            const auto portraitName = m_aif.m_wndPortraitName + CStr(i);
            child = GetChildByName(portraitName);
            if (child && child->IsKindOf(&m3d::ui::ModelWnd::m_classModelWnd))
            {
                m_wndsPortraits[i] = static_cast<ItemModelWnd*>(M3D_KERNEL->New("ItemModelWnd"));
                if (m_wndsPortraits[i])
                {
                    if (!m_wndsPortraits[i]->CreateFromPattern(dynamic_cast<m3d::ui::Wnd*>(child), true))
                    {
                        M3D_LOG_ERR("Make control error: cannot create " + portraitName + " from pattern class");
                        success = false;
                    }
                }
                else
                {
                    M3D_LOG_ERR("Make control error: cannot create " + portraitName + " - cannot find rtti class ItemModelWnd");
                    success = false;
                }
            }
            else
            {
                M3D_LOG_ERR("Make control error: control " + portraitName + " is not found or incorrect type");
                success = false;
            }
        }

        SetCursorShow(false);
        if (success)
        {
            m_gameDataFlags |= 1u;
            m_panelType = PANELTYPE_NORMAL;
            ShowControlsForPanelType(PANELTYPE_NORMAL);
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_ERR("CinemaPanel: error - fail to init because of a bad resource");
    return 0;
}

void CinemaPanel::SetupTextNormal(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::CinemaPanel(CinemaPanel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::CinemaPanel()
{
    m_soundTableId = -1;
    m_soundChannelId = -1;
    m_curMessageStartTime = 0;
    m_curMessageEndTime = 0;
    m_bIsShowingMessage = 0;
    m_bSkipMessage = 0;
    m_wndText = 0;
    m_panelType = PANELTYPE_NORMAL;
    m_wndUpPanel = 0;
    m_wndDownPanel = 0;
    m_wndScrollImage = 0;
    m_wndScrollImageUpOverlay = 0;
    m_wndScrollImageDownOverlay = 0;
    m_wndPortraitOverlay = 0;
    m_minTimeToExists = 10;
    m_wndsPortraits[0] = 0;
    m_wndsPortraits[1] = 0;
}

void CinemaPanel::InitControlsForMsgScroll(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::InitControlsForMsgNormal(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::UpdateAnimation()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    // TODO: generated code CinemaPanel::UpdateAnimation
    for (int i = 0; i < 2; ++i)
    {
        auto* portrait = m_wndsPortraits[i];
        if (!portrait)
        {
            continue;
        }

        // Check if portrait is a child of the application's window station
        if (!portrait->IsChildOf(M3D_APP))
        {
            continue;
        }

        m3d::AnimatedModel* model = portrait->GetModel();
        m3d::AnimInfo* animInfo = portrait->Animation();

        if (!model || !animInfo)
        {
            continue;
        }

        // Check if we have a current animation
        if (!animInfo->GetCurAnimation())
        {
            continue;
        }

        // Get animation data
        auto* currentAnim = animInfo->GetCurAnimation();
        uint16_t totalFrames = currentAnim->m_numFrames;

        // Check if we're near the end of the animation (last 2 frames)
        // or if there are no frames at all
        if (totalFrames == 0 || animInfo->CurAnimFrame() >= (totalFrames - 2))
        {
            ActionType currentAction = currentAnim->m_action;
            if (currentAction != AT_NUMTYPES)
            {
                ActionType randomMove = help::GetRandomMoveAnimation(model);
                model->SetNextForAnimation(currentAction, randomMove);
            }
        }
    }
}

void CinemaPanel::SetupTime(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::ShowControlsForPanelType(PanelType panelType)
{
    HideAllControls();

    retruxx::vector<Wnd*> panelControls;
    GetControlsByPanelType(panelType, panelControls, false);
    for (auto* control : panelControls)
    {
        if (control && !control->GetParent())
        {
            AddChild(control);
        }
    }
}

void CinemaPanel::ClearScroll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CinemaPanel::PanelType CinemaPanel::GetPanelTypeByMsgType(MsgInfo::MsgType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::InitControlsForMsgBase(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CinemaPanel::SetupImagesScroll(MsgInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CinemaPanel::StopSound()
{
    int res = 1;
    if (m_soundTableId != -1)
    {
        if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
        {
            res = M3D_APP->m_sound->DeleteIdTableSound(m_soundTableId);
        }
        m_soundTableId = -1;
        m_soundChannelId = -1;
    }
    return res;
}

void CinemaPanel::SetPanelType(PanelType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

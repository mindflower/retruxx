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
#include <iface.h>
#include <math/matrix.h>
#include <math/quaternion.h>
#include <skelmodel.h>
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
    // The shipped game allocates a fresh CinemaPanel and runs the copy
    // constructor, which is an assert(0) - the panel is not copyable.
    return new CinemaPanel(*this);
}

void CinemaPanel::AddMessage(int msgId, float delay)
{
    MessageTimeInfo info(msgId, delay);
    m_msgInfos.push_back(std::move(info));
}

unsigned CinemaPanel::GetTimeToTheEndOfMsg()
{
    return m_curMessageStartTime + 1000 * m_minTimeToExists - M3D_KERNEL->GetTimer().GetCurTime();
}

bool CinemaPanel::MessageQueueIsEmpty() const
{
    return m_msgInfos.empty() && !m_bIsShowingMessage;
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
    m_bSkipMessage = true;
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
    DeleteAllControls();
    // m_aif, m_msgInfos and the m3d::ui::Wnd base destroy themselves.
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

void CinemaPanel::SetupPortrait(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (msgInfo->GetModelName().empty())
    {
        return;
    }

    int slot = msgInfo->GetModelSlot();
    if (slot < 0)
    {
        slot = 0;
    }
    else if (slot > 1)
    {
        slot = 1;
    }

    auto* portrait = m_wndsPortraits[slot];

    portrait->SetModelByName(msgInfo->GetModelName(), msgInfo->GetModelSkin(), msgInfo->GetModelCfg());

    if (portrait->GetModel() && portrait->Animation())
    {
        const ActionType randomMove = help::GetRandomMoveAnimation(portrait->GetModel());
        if (!portrait->Animation()->SetAnimation(randomMove))
        {
            portrait->Animation()->SetAnimation(AT_MOVE1);
        }
    }

    portrait->SetAutosized(msgInfo->IsModelAutosized());

    // Reset the model camera pitch: keep yaw/roll from the current orientation
    // but force pitch to the autosize default angle (or 0 when not autosized).
    Quaternion& rotation = portrait->Rotation();
    CMatrix m;
    m.rotTranslate(rotation, CVector(0.0f, 0.0f, 0.0f));

    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    m.getYPR(yaw, pitch, roll);

    const float newPitch = msgInfo->IsModelAutosized() ? portrait->GetDefaultRotationAngleX() : 0.0f;
    m.rotYPR(yaw, newPitch, roll);
    rotation.FromMatrix(m);

    portrait->ShowWindow(true);
}

void CinemaPanel::ClearBase()
{
    StopSound();
    m_bIsShowingMessage = false;
    m_curMessageEndTime = M3D_KERNEL->GetTimer().GetCurTime();
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

void CinemaPanel::SetupTextScroll(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    m_wndScrollText->SetText(msgInfo->GetMsg());
    m_wndScrollText->SetScrollSpeed(msgInfo->GetScrollSpeed());

    if (msgInfo->GetFontSize() == -1.0f)
    {
        m_wndScrollText->SetDefaultFont(0);
    }
    else
    {
        m_wndScrollText->SetDefaultFont(help::GetScaledFontId(0, msgInfo->GetFontSize()));
    }

    m_wndScrollText->m_bScrollingEnabled = true;
    const auto clientBounds = m_wndScrollText->GetClientBounds();
    m_wndScrollText->m_textBounds.y0 = clientBounds.y0 + clientBounds.height;
}

int CinemaPanel::GameDataClear(bool)
{
    Clear();
    return 1;
}

void CinemaPanel::GetAllControls(std::vector<m3d::ui::Wnd*>& controls) const
{
    controls.clear();
    for (int i = PANELTYPE_NORMAL; i < PANELTYPE_NUM_PANEL_TYPES; ++i)
    {
        GetControlsByPanelType(static_cast<PanelType>(i), controls, true);
    }
}

void CinemaPanel::SetupSound(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (msgInfo->GetSoundFileName().empty())
    {
        return;
    }

    if (!M3D_ENGINE_CFG.m_snd_Enable.GetB())
    {
        return;
    }

    m_soundTableId = M3D_APP->m_sound->AddSound(
        msgInfo->GetSoundFileName().c_str(), snd::SND_TYPE_2DSOUND, 1, 1, snd::SND_PRIORITY_EXTRAHIGH);
    if (m_soundTableId != -1 && m_soundChannelId == -1)
    {
        m_soundChannelId = M3D_APP->m_sound->PlaySound2D(m_soundTableId, false);
    }
}

void CinemaPanel::DeleteAllControls()
{
    retruxx::vector<Wnd*> controls;
    for (int i = PANELTYPE_NORMAL; i < PANELTYPE_NUM_PANEL_TYPES; ++i)
    {
        GetControlsByPanelType(static_cast<PanelType>(i), controls, true);
    }

    for (auto* control : controls)
    {
        if (control)
        {
            control->DecRef();
        }
    }
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

void CinemaPanel::SetupTextNormal(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (msgInfo->GetMsg().empty())
    {
        return;
    }

    m_wndText->SetText(msgInfo->GetMsg());
    m_wndText->ShowWindow(true);
}

CinemaPanel::CinemaPanel(CinemaPanel const&) : CinemaPanel()
{
    // Matches the shipped game: the copy constructor is an assert(0) - a
    // CinemaPanel cannot be duplicated.
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

void CinemaPanel::InitControlsForMsgScroll(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    SetupTime(msgInfo);
    SetupSound(msgInfo);
    m_bIsShowingMessage = true;
    SetupTextScroll(msgInfo);
    SetupImagesScroll(msgInfo);
}

void CinemaPanel::InitControlsForMsgNormal(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    SetupTime(msgInfo);
    SetupSound(msgInfo);
    m_bIsShowingMessage = true;
    SetupTextNormal(msgInfo);
    SetupPortrait(msgInfo);
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

void CinemaPanel::SetupTime(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    const int time = msgInfo->GetTime();
    m_minTimeToExists = (time == -1) ? 10 : time;
    m_curMessageStartTime = M3D_KERNEL->GetTimer().GetCurTime();
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
    StopSound();
    m_bIsShowingMessage = false;
    m_curMessageEndTime = M3D_KERNEL->GetTimer().GetCurTime();
}

CinemaPanel::PanelType CinemaPanel::GetPanelTypeByMsgType(MsgInfo::MsgType msgType) const
{
    return msgType == MsgInfo::MSGTYPE_SCROLL ? PANELTYPE_SCROLL : PANELTYPE_NORMAL;
}

void CinemaPanel::InitControlsForMsgBase(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    SetupTime(msgInfo);
    SetupSound(msgInfo);
    m_bIsShowingMessage = true;
}

void CinemaPanel::SetupImagesScroll(MsgInfo const* msgInfo)
{
    if (!msgInfo || (m_gameDataFlags & 1) == 0)
    {
        return;
    }

    m_wndScrollImage->SetImage(msgInfo->GetImage());
    m_wndScrollImageUpOverlay->SetImage(msgInfo->GetImageUpOverlay());
    m_wndScrollImageDownOverlay->SetImage(msgInfo->GetImageDownOverlay());
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

void CinemaPanel::SetPanelType(PanelType panelType)
{
    m_panelType = panelType;
    ShowControlsForPanelType(panelType);
}

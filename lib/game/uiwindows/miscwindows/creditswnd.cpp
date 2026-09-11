#include "creditswnd.h"

#include <cmath>

#include <config.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/timer.h>
#include <game/m3dgame.h>
#include <game/music/blockmusicmanager.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/guihelper.h>
#include <game/uiwindows/commonwindows/itemmodelwnd.h>
#include <iface.h>
#include <m3dapp.h>
#include <skelmodel.h>
#include <ui/modelwnd.h>
#include <ui/wndstation.h>

namespace
{
    // GameDataUpdate data type this window listens to.
    int const DATATYPE_NEW_FRAME = 89;

    // The window closes on Escape.
    unsigned char const KEY_ESCAPE = 1;
}  // namespace

RT_CLASS_EXPORTS_BEGIN(CreditsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CreditsWnd);

// ---------------------------------------------------------------------------
// CreditsWnd::AuxInfo / CreditsWnd::PageInfo
// ---------------------------------------------------------------------------

CreditsWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4B1120
    m_wndTextName = "wndText";
    m_wndPortraitName = "wndPortrait";
    m_texIdBg = "CreditsBg";
}

CreditsWnd::PageInfo::PageInfo()
{
    // RVA 0x4B1200
    m_fontSize = 12.0f;
    m_modelSkin = 0;
    m_modelCfg = 0;
    m_showTime = 3.0f;
}

// ---------------------------------------------------------------------------
// CreditsWnd::Pointer
// ---------------------------------------------------------------------------

CreditsWnd::Pointer::Pointer()
{
    // RVA 0x4B1240 - a clock hand that sweeps between two angles over the life of
    // one page.
    m_minAngle = -0.78539819f;
    m_maxAngle = 0.78539819f;
    m_curAngle = -0.78539819f;
    m_texSz.x = 32.0f;
    m_texSz.y = 256.0f;
    m_rotationCenter.x = 16.0f;
    m_rotationCenter.y = 129.0f;
    m_rotationScreenCenter.x = 186.0f;
    m_rotationScreenCenter.y = 290.0f;
    m_texId = "CreditsPointer";
    m_texture.SetInvalid();
    Init();
}

CreditsWnd::Pointer::~Pointer()
{
    // RVA 0x4B1360
    Clear();
}

void CreditsWnd::Pointer::Init()
{
    // RVA 0x4B13A0
    m_texture = M3D_APP->m_pInterfaceManager->GetIcoByName(m_texId, 0);
    M3D_RENDERER->ReferenceTexture(m_texture);
}

void CreditsWnd::Pointer::Clear()
{
    // RVA 0x4B13E0
    M3D_RENDERER->ReleaseTexture(m_texture);
}

void CreditsWnd::Pointer::Update(float startTime, float delayPeriod, bool bInversedDirection)
{
    // RVA 0x4B1400 - linear sweep from one end of the arc to the other over
    // delayPeriod seconds; a break runs the hand backwards.
    float const elapsed =
        static_cast<float>(static_cast<double>(M3D_KERNEL->GetTimer().GetCurTimeUnscaled()) * 0.001) - startTime;

    float const from = bInversedDirection ? m_maxAngle : m_minAngle;
    float const to = bInversedDirection ? m_minAngle : m_maxAngle;

    if (delayPeriod < 0.001f)
    {
        m_curAngle = from;
    }
    else
    {
        m_curAngle = (to - from) / delayPeriod * elapsed + from;
    }
}

void CreditsWnd::Pointer::Draw(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4B1480
    M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestInterface.GetI());
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_MODULATE);
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    M3D_RENDERER->SetStageState(1, m3d::rend::BM_COLOR, m3d::rend::TS_NONE);
    M3D_RENDERER->PushBlend(m3d::rend::BM_ALPHA);
    M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);

    if (!m_texture.IsValid())
    {
        M3D_RENDERER->SetWhiteTexture(0);
    }
    else
    {
        M3D_RENDERER->SetTexture(0, m_texture, -1.0);
    }

    // The hand pivots about m_rotationCenter, so the sprite centre is that offset
    // rotated by the current angle. NOTE: only the y component of
    // m_rotationCenter is used - the shipped code rotates the offset (0, -y).
    float const sinA = static_cast<float>(std::sin(m_curAngle));
    float const cosA = static_cast<float>(std::cos(m_curAngle));
    float const offsetY = 0.0f - m_rotationCenter.y;
    float const cx = di.m_originalRect.x0 + (m_rotationScreenCenter.x + (cosA * 0.0f - sinA * offsetY));
    float const cy = di.m_originalRect.y0 + (m_rotationScreenCenter.y + (cosA * offsetY + sinA * 0.0f));

    M3D_APP->PutSpriteRelRot(
        cx, cy, m_texSz.x * 0.5f, m_texSz.y * 0.5f, 0xFFFFFFFF, m_curAngle, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    M3D_RENDERER->SetAlphaTest(0);
    M3D_RENDERER->PopBlend();
    M3D_RENDERER->PopZbState();
}

// ---------------------------------------------------------------------------
// construction / RTTI
// ---------------------------------------------------------------------------

CreditsWnd::CreditsWnd()
{
    // RVA 0x4B1670
    m_wndText = nullptr;
    m_wndPortrait = nullptr;
    m_texBgShow.SetInvalid();
    m_texBgBreak.SetInvalid();
    m_breakTime = 0.5f;
    m_showSoundTableId = -1;
    m_breakSoundTableId = -1;
    m_ambientSoundTableId = -1;
    m_musicTableId = -1;
    m_switchSoundChannelId = -1;
    m_ambientSoundChannelId = -1;
    m_musicChannelId = -1;
    m_curPageId = -1;
    m_bInBreak = false;
    m_startTime = 0.0f;

    m_cvPathToPageInfo.Init(
        "pathToCredits", "data\\if\\dialogs\\credits.xml", m3d::CVar::CVAR_STRING, m3d::CVar::CVAR_ARCHIVE);
    M3D_ENGINE_CFG.m_console->RegisterCVar(&m_cvPathToPageInfo, nullptr);
}

CreditsWnd::CreditsWnd(CreditsWnd const&)
{
    // RVA 0x4B1860 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves every scalar member uninitialised and never
    // registers the console variable.
}

CreditsWnd::~CreditsWnd()
{
    // RVA 0x4B18F0
    M3D_RENDERER->ReleaseTexture(m_texBgShow);
    M3D_RENDERER->ReleaseTexture(m_texBgBreak);
    StopSwitchSound();
    StopAmbientSound();
    StopMusic();
    ClearPages();
    M3D_ENGINE_CFG.m_console->UnregisterCVar(&m_cvPathToPageInfo);
}

m3d::Object* CreditsWnd::Clone()
{
    // RVA 0x4B10A0
    return new CreditsWnd(*this);
}

m3d::Object* CreditsWnd::CreateObject()
{
    return new CreditsWnd;
}

m3d::Class* CreditsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Class* CreditsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CreditsWnd);
}

// ---------------------------------------------------------------------------
// resource setup
// ---------------------------------------------------------------------------

int CreditsWnd::GameDataSetup()
{
    // RVA 0x4B19F0
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        m3d::Object* wndText = GetChildByName(m_aif.m_wndTextName);
        if (wndText && wndText->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndText = static_cast<m3d::ui::Wnd*>(wndText);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndTextName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndPortrait = GetChildByName(m_aif.m_wndPortraitName);
        if (wndPortrait && wndPortrait->IsKindOf(&m3d::ui::ModelWnd::m_classModelWnd))
        {
            m_wndPortrait = static_cast<ItemModelWnd*>(M3D_KERNEL->New("ItemModelWnd"));
            if (m_wndPortrait)
            {
                if (!m_wndPortrait->CreateFromPattern(static_cast<m3d::ui::Wnd*>(wndPortrait), true))
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_wndPortraitName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndPortraitName + " - cannot find rtti class " +
                    CStr("ItemModelWnd"));
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndPortraitName + " is not found or incorrect type");
            res = 0;
        }

        LoadPageInfo();

        m_texBgShow = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdBg, 0);
        m_texBgBreak = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdBg, 1);
        M3D_RENDERER->ReferenceTexture(m_texBgShow);
        M3D_RENDERER->ReferenceTexture(m_texBgBreak);
        SetCursorShow(false);

        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("CreditsWnd: error - fail to init because of a bad resource");
    return 0;
}

void CreditsWnd::LoadPageInfo()
{
    // RVA 0x4B26A0 - reads the page list and the soundtrack out of the XML the
    // pathToCredits console variable points at.
    ClearPages();

    CStr const fileName(m_cvPathToPageInfo.GetS());
    CStr err;
    ref_ptr xmlFile = m3d::ReadXmlFile(fileName.c_str(), &err);
    if (!xmlFile)
    {
        M3D_LOG_DATA("CreditsWnd::LoadPageInfo error - cannot parse " + fileName + " (" + err + ") ");
        return;
    }

    ref_ptr rootNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    xmlFile->GetFirstChild(rootNode, "Credits");
    if (rootNode->IsEmpty())
    {
        M3D_LOG_DATA("CreditsWnd::LoadPageInfo error - cannot find root node \"Credits\"");
        return;
    }

    m3d::SafeFloatAttrib(m_breakTime, rootNode, "breakTime");

    if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
    {
        CStr breakSoundFileName;
        CStr showSoundFileName;
        CStr ambientSoundFileName;
        CStr musicFileName;
        m3d::SafeStrAttrib(breakSoundFileName, rootNode, "breakSoundFile");
        m3d::SafeStrAttrib(showSoundFileName, rootNode, "showSoundFile");
        m3d::SafeStrAttrib(ambientSoundFileName, rootNode, "ambientSoundFile");
        m3d::SafeStrAttrib(musicFileName, rootNode, "musicFile");

        m_breakSoundTableId = M3D_APP->m_sound->AddSound(
            breakSoundFileName.c_str(), snd::SND_TYPE_2DSOUND, 1, 1, snd::SND_PRIORITY_EXTRAHIGH);
        m_showSoundTableId = M3D_APP->m_sound->AddSound(
            showSoundFileName.c_str(), snd::SND_TYPE_2DSOUND, 1, 1, snd::SND_PRIORITY_EXTRAHIGH);
        m_ambientSoundTableId = M3D_APP->m_sound->AddSound(
            ambientSoundFileName.c_str(), snd::SND_TYPE_2DSOUND, 1, 1, snd::SND_PRIORITY_EXTRAHIGH);
        m_musicTableId =
            M3D_APP->m_sound->AddSound(musicFileName.c_str(), snd::SND_TYPE_MUSIC, 0, 1, snd::SND_PRIORITY_EXTRAHIGH);
    }

    ref_ptr pagesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    rootNode->GetFirstChild(pagesNode, "Pages");
    if (pagesNode->IsEmpty())
    {
        M3D_LOG_DATA("CreditsWnd::LoadPageInfo warning - no pages was specified");
        return;
    }

    ref_ptr pageNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    for (pagesNode->GetFirstChild(pageNode, "Page"); !pageNode->IsEmpty(); pageNode->GetNextSibling(pageNode, "Page"))
    {
        auto* pageInfo = new PageInfo;
        if (!pageInfo)
        {
            break;
        }

        CStr text;
        m3d::SafeStrAttrib(text, pageNode, "text");
        pageInfo->m_text = text;
        m3d::SafeStrAttrib(pageInfo->m_modelName, pageNode, "modelName");
        m3d::SafeUintAttrib(pageInfo->m_modelSkin, pageNode, "modelSkin");
        m3d::SafeUintAttrib(pageInfo->m_modelCfg, pageNode, "modelCfg");
        m3d::SafeFloatAttrib(pageInfo->m_showTime, pageNode, "showTime");
        m3d::SafeFloatAttrib(pageInfo->m_fontSize, pageNode, "fontSize");
        m_pageInfos.push_back(pageInfo);
    }

    if (m_pageInfos.empty())
    {
        M3D_LOG_DATA("CreditsWnd::LoadPageInfo warning - no pages was specified");
    }
}

void CreditsWnd::ClearPages()
{
    // RVA 0x4B3100
    for (auto* pageInfo : m_pageInfos)
    {
        delete pageInfo;
    }
    m_pageInfos.clear();
}

// ---------------------------------------------------------------------------
// page sequencing
// ---------------------------------------------------------------------------

int CreditsWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x4B1EC0
    if (dataType == DATATYPE_NEW_FRAME)
    {
        OnNewFrame();
    }
    return 1;
}

void CreditsWnd::OnNewFrame()
{
    // RVA 0x4B1EE0
    if (NeedExit())
    {
        Exit();
        return;
    }

    if (m_bInBreak)
    {
        CheckAndShowCurrentPage();
    }
    else
    {
        CheckAndShowNextPage();
        if (NeedExit())
        {
            Exit();
            return;
        }
    }

    UpdatePointer();
    UpdatePortraitAnimation();
}

void CreditsWnd::StartShowingPages()
{
    // RVA 0x4B25F0 - start on the first page, in the break that precedes it.
    m_curPageId = m_pageInfos.empty() ? -1 : 0;
    m_bInBreak = true;
}

void CreditsWnd::CheckAndShowCurrentPage()
{
    // RVA 0x4B1F50
    if (IsCurPageIdValid() && m_bInBreak && IsTimeEnded())
    {
        DoShowPage();
    }
}

void CreditsWnd::CheckAndShowNextPage()
{
    // RVA 0x4B1FB0
    if (!IsCurPageIdValid() || m_bInBreak || !IsTimeEnded())
    {
        return;
    }

    if (m_curPageId + 1 < static_cast<int>(m_pageInfos.size()))
    {
        m_curPageId = m_curPageId + 1;
        DoBreak();
    }
    else
    {
        m_curPageId = -1;
    }
}

void CreditsWnd::DoShowPage()
{
    // RVA 0x4B21D0
    m_bInBreak = false;
    UpdateStartTime();
    UpdateControls();
    PlaySwitchSound();
}

void CreditsWnd::DoBreak()
{
    // RVA 0x4B2220
    m_bInBreak = true;
    UpdateStartTime();
    UpdateControls();
    PlaySwitchSound();
}

void CreditsWnd::Exit()
{
    // RVA 0x4B20A0
    M3D_APP->m_pInterfaceManager->ShowWindow(m_guiId, false, false, false, false, nullptr);
}

bool CreditsWnd::IsCurPageIdValid() const
{
    // RVA 0x4B2180
    return m_curPageId >= 0 && m_curPageId < static_cast<int>(m_pageInfos.size()) && m_pageInfos[m_curPageId];
}

bool CreditsWnd::NeedExit() const
{
    // RVA 0x4B2040 - running off the end of the page list is the only way out.
    return !IsCurPageIdValid();
}

bool CreditsWnd::IsTimeEnded() const
{
    // RVA 0x4B20D0
    if (!IsCurPageIdValid())
    {
        return true;
    }
    float const period = m_bInBreak ? m_breakTime : m_pageInfos[m_curPageId]->m_showTime;
    float const now = static_cast<float>(static_cast<double>(M3D_KERNEL->GetTimer().GetCurTimeUnscaled()) * 0.001);
    return now - m_startTime >= period;
}

void CreditsWnd::UpdateStartTime()
{
    // RVA 0x4B2270
    if (IsCurPageIdValid())
    {
        m_startTime = static_cast<float>(static_cast<double>(M3D_KERNEL->GetTimer().GetCurTimeUnscaled()) * 0.001);
    }
    else
    {
        m_startTime = 0.0f;
    }
}

// ---------------------------------------------------------------------------
// controls
// ---------------------------------------------------------------------------

void CreditsWnd::UpdateControls()
{
    // RVA 0x4B22F0
    UpdateBg();
    UpdateText();
    UpdatePortrait();
    UpdatePointer();
}

void CreditsWnd::UpdateBg()
{
    // RVA 0x4B2330
    SetBackground(m_bInBreak ? m_texBgBreak : m_texBgShow);
}

void CreditsWnd::UpdateText()
{
    // RVA 0x4B2360
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!IsCurPageIdValid())
    {
        m_wndText->SetText(CStr());
        return;
    }

    m_wndText->SetDefaultFont(help::GetScaledFontId(m_defFont, m_pageInfos[m_curPageId]->m_fontSize));
    m_wndText->SetText(m_bInBreak ? CStr() : m_pageInfos[m_curPageId]->m_text);
}

void CreditsWnd::UpdatePortrait()
{
    // RVA 0x4B2440
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    if (!IsCurPageIdValid() || m_bInBreak)
    {
        m_wndPortrait->SetModelByName(CStr(), 0, 0);
        return;
    }

    PageInfo const* pageInfo = m_pageInfos[m_curPageId];
    m_wndPortrait->SetModelByName(pageInfo->m_modelName, pageInfo->m_modelSkin, pageInfo->m_modelCfg);
    if (m_wndPortrait->GetModel() && m_wndPortrait->Animation())
    {
        if (!m_wndPortrait->Animation()->SetAnimation(help::GetRandomStandAnimation(m_wndPortrait->GetModel())))
        {
            m_wndPortrait->Animation()->SetAnimation(AT_STAND1);
        }
    }
}

void CreditsWnd::UpdatePointer()
{
    // RVA 0x4B2550
    if (!IsCurPageIdValid())
    {
        m_pointer.Update(m_startTime, 0.0f, false);
        return;
    }
    float const period = m_bInBreak ? m_breakTime : m_pageInfos[m_curPageId]->m_showTime;
    m_pointer.Update(m_startTime, period, m_bInBreak);
}

void CreditsWnd::UpdatePortraitAnimation()
{
    // RVA 0x4B3730
    if ((m_gameDataFlags & 1) != 0 && !m_bInBreak)
    {
        if (m_wndPortrait->GetModel() && m_wndPortrait->Animation())
        {
            help::RandomizeCurAnimationOnFinish(m_wndPortrait->GetModel(), m_wndPortrait->Animation(), help::_AT_STAND);
        }
    }
}

// ---------------------------------------------------------------------------
// sound and music
// ---------------------------------------------------------------------------

int CreditsWnd::PlaySoundA(int soundTableId, bool bLooped)
{
    // RVA 0x4B3290
    if (M3D_ENGINE_CFG.m_snd_Enable.GetB() && soundTableId != -1)
    {
        return M3D_APP->m_sound->PlaySound2D(soundTableId, bLooped);
    }
    return -1;
}

void CreditsWnd::StopSound(int soundChannelId)
{
    // RVA 0x4B3460 - NOTE: the shipped code gates the sound-effect stops on the
    // *music* switch, not on m_snd_Enable.
    if (M3D_ENGINE_CFG.m_mus_Enable.GetB() && soundChannelId != -1)
    {
        M3D_APP->m_sound->StopChannel(soundChannelId);
    }
}

void CreditsWnd::PlaySwitchSound()
{
    // RVA 0x4B32E0 - the page-turn blip; a break and a page get different sounds.
    StopSound(m_switchSoundChannelId);
    m_switchSoundChannelId = PlaySoundA(m_bInBreak ? m_breakSoundTableId : m_showSoundTableId, false);
}

void CreditsWnd::PlayAmbientSound()
{
    // RVA 0x4B3390
    m_ambientSoundChannelId = PlaySoundA(m_ambientSoundTableId, true);
}

void CreditsWnd::StopSwitchSound()
{
    // RVA 0x4B34A0
    StopSound(m_switchSoundChannelId);
    m_switchSoundChannelId = -1;
}

void CreditsWnd::StopAmbientSound()
{
    // RVA 0x4B34F0
    StopSound(m_ambientSoundChannelId);
    m_ambientSoundChannelId = -1;
}

void CreditsWnd::PlayMusic()
{
    // RVA 0x4B33F0 - takes the music system off the game playlist for the
    // duration of the credits.
    if (!M3D_ENGINE_CFG.m_mus_Enable.GetB() || m_musicTableId == -1)
    {
        return;
    }
    m_musicChannelId = M3D_APP->m_sound->PlayMusic(m_musicTableId, true, true);
    if (M3D_APP->m_hackedMusicType != HACKMUSIC_CUSTOM)
    {
        M3D_APP->m_bMustStartNewMusic = true;
        M3D_APP->m_hackedMusicType = HACKMUSIC_CUSTOM;
    }
}

void CreditsWnd::StopMusic()
{
    // RVA 0x4B3540
    StopSound(m_musicChannelId);
    m_musicChannelId = -1;
    if (M3D_APP->m_hackedMusicType != HACKMUSIC_GAME)
    {
        M3D_APP->m_bMustStartNewMusic = true;
        M3D_APP->m_hackedMusicType = HACKMUSIC_GAME;
        M3D_APP->GetBlockMusicManager()->Reset();
    }
}

void CreditsWnd::StopAllSoundsAndMusic()
{
    // RVA 0x4B35C0
    StopSwitchSound();
    StopAmbientSound();
    StopMusic();
}

// ---------------------------------------------------------------------------
// window plumbing
// ---------------------------------------------------------------------------

int CreditsWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4B35E0
    StartShowingPages();
    PlayAmbientSound();
    PlayMusic();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int CreditsWnd::OnAfterRemoveFromWndStation()
{
    // RVA 0x4B3630
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    Reset();
    return res;
}

void CreditsWnd::Reset()
{
    // RVA 0x4B3650
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndText->SetText(CStr());
        m_wndPortrait->SetModelByName(CStr(), 0, 0);
    }
    m_curPageId = -1;
    m_bInBreak = false;
    m_startTime = 0.0f;
    m_pointer.Update(m_startTime, 0.0f, false);
    StopSwitchSound();
    StopAmbientSound();
    StopMusic();
}

int CreditsWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    // RVA 0x4B2620
    if (!state || static_cast<unsigned char>(key) != KEY_ESCAPE)
    {
        return ModalWnd::OnKey(key, scanCode, state);
    }
    Exit();
    return 1;
}

int CreditsWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4B2670
    ModalWnd::OnPaint(di);
    m_pointer.Draw(di);
    return 1;
}

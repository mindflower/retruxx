#include "creditswnd.h"

#include "config.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "game/m3dgame.h"
#include "ui/modelwnd.h"
#include "game/uiwindows/commonwindows/itemmodelwnd.h"

RT_CLASS_EXPORTS_BEGIN(CreditsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CreditsWnd);

CreditsWnd::PageInfo::PageInfo()
{
}

CreditsWnd::Pointer::Pointer()
{
    auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    m_texture = app->m_pInterfaceManager->GetIcoByName(m_texId, 0);
    app->m_renderer->ReferenceTexture(m_texture);
}

void CreditsWnd::Pointer::Update(float, float, bool)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Pointer::Draw(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::Pointer::~Pointer()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Pointer::Clear()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Pointer::Init()
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::AuxInfo::AuxInfo()
{

}

CreditsWnd::~CreditsWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CreditsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CreditsWnd);
}

m3d::Object* CreditsWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CreditsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

m3d::Object* CreditsWnd::CreateObject()
{
    return new CreditsWnd;
}

int CreditsWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StartShowingPages()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateStartTime()
{
    throw std::logic_error("Not implemented");
}

CreditsWnd::CreditsWnd()
{
    //TODO: check this
    m_cvPathToPageInfo.Set("pathToCredits");
    m_cvPathToPageInfo.Set("data\\if\\dialogs\\credits.xml");
    m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&m_cvPathToPageInfo, nullptr);
}

CreditsWnd::CreditsWnd(CreditsWnd const&)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateBg()
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::OnKey(unsigned short, unsigned char, unsigned)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdatePortrait()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Exit()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::DoShowPage()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::PlaySwitchSound()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::DoBreak()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopSwitchSound()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::CheckAndShowCurrentPage()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateText()
{
    throw std::logic_error("Not implemented");
}

bool CreditsWnd::IsCurPageIdValid() const
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::PlaySoundA(int, bool)
{
    throw std::logic_error("Not implemented");
}

bool CreditsWnd::IsTimeEnded() const
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopSound(int)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::LoadPageInfo()
{
    ClearPages();
    scoped_ptr fileStream = m3d::g_Kernel->GetFileServer().CreateFileStream();
    if (fileStream->Open(m_cvPathToPageInfo.GetS(), m3d::fs::IStream::OPEN_READ))
    {
        ref_ptr xmlFile = m3d::g_Kernel->CreateXmlFile();
        if (xmlFile->Read(*fileStream))
        {
            fileStream->Close();
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            xmlFile->GetFirstChild(node, "Credits");
            if (node->IsEmpty())
            {
                M3D_LOG_DATA("CreditsWnd::LoadPageInfo error - cannot find root node \"Credits\"");
                return;
            }
            m3d::SafeFloatAttrib(m_breakTime, node, "breakTime");
            if (m3d::g_Kernel->GetEngineCfg().m_snd_Enable.GetB())
            {
                CStr breakSoundFileName;
                CStr showSoundFileName;
                CStr ambientSoundFileName;
                CStr musicFile;
                m3d::SafeStrAttrib(breakSoundFileName, node, "breakSoundFile");
                m3d::SafeStrAttrib(showSoundFileName, node, "showSoundFile");
                m3d::SafeStrAttrib(ambientSoundFileName, node, "ambientSoundFile");
                m3d::SafeStrAttrib(musicFile, node, "musicFile");
                m_breakSoundTableId = m3d::Application::g_pApp->m_sound->AddSound(
                    breakSoundFileName.c_str(),
                    snd::SND_TYPE_2DSOUND,
                    1,
                    1,
                    snd::SND_PRIORITY_EXTRAHIGH
                );
                m_showSoundTableId = m3d::Application::g_pApp->m_sound->AddSound(
                    showSoundFileName.c_str(),
                    snd::SND_TYPE_2DSOUND,
                    1,
                    1,
                    snd::SND_PRIORITY_EXTRAHIGH
                );
                m_ambientSoundTableId = m3d::Application::g_pApp->m_sound->AddSound(
                    ambientSoundFileName.c_str(),
                    snd::SND_TYPE_2DSOUND,
                    1,
                    1,
                    snd::SND_PRIORITY_EXTRAHIGH
                );
                m_musicTableId = m3d::Application::g_pApp->m_sound->AddSound(
                    musicFile.c_str(),
                    snd::SND_TYPE_MUSIC,
                    0,
                    1,
                    snd::SND_PRIORITY_EXTRAHIGH
                );
            }
            ref_ptr pagesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            node->GetFirstChild(pagesNode, "Pages");
            if (pagesNode->IsEmpty())
            {
                M3D_LOG_DATA("CreditsWnd::LoadPageInfo warning - no pages was specified");
            }
            else
            {
                ref_ptr pageNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
                for (pagesNode->GetFirstChild(pageNode, "Page"); !pageNode->IsEmpty(); pageNode->GetNextSibling(pageNode, "Page"))
                {
                    auto info = new PageInfo;
                    m3d::SafeStrAttrib(info->m_text, pageNode, "text");
                    m3d::SafeStrAttrib(info->m_modelName, pageNode, "modelName");
                    m3d::SafeUintAttrib(info->m_modelSkin, pageNode, "modelSkin");
                    m3d::SafeUintAttrib(info->m_modelCfg, pageNode, "modelCfg");
                    m3d::SafeFloatAttrib(info->m_showTime, pageNode, "showTime");
                    m3d::SafeFloatAttrib(info->m_fontSize, pageNode, "fontSize");
                    m_pageInfos.push_back(info);
                }
                if (m_pageInfos.empty())
                {
                    M3D_LOG_DATA("CreditsWnd::LoadPageInfo warning - no pages was specified");
                }
            }
        }
        else
        {
            M3D_LOG_DATA("CreditsWnd::LoadPageInfo error - cannot parse " + CStr(xmlFile->GetError()));
        }
    }
    else
    {
        M3D_LOG_DATA("CreditsWnd::LoadPageInfo error - can't open file " + CStr(m_cvPathToPageInfo.GetS()) + " for read.");
    }
}

int CreditsWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopAllSoundsAndMusic()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdateControls()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::ClearPages()
{
    for (auto* page : m_pageInfos)
    {
        delete page;
    }
    m_pageInfos.clear();
}

void CreditsWnd::UpdatePointer()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::PlayMusic()
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopMusic()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::Reset()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::UpdatePortraitAnimation()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int CreditsWnd::GameDataSetup()
{
    using namespace m3d::ui;
    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto text = GetChildByName(m_aif.m_wndTextName);
        if (text && text->IsKindOf(RT_CLASS_LOCAL(Wnd)))
        {
            m_wndText = dynamic_cast<Wnd*>(text);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndTextName + " is not found or incorrect type");
            res = 0;
        }

        auto portrait = GetChildByName(m_aif.m_wndPortraitName);
        if (portrait && portrait->IsKindOf(RT_CLASS_LOCAL(ModelWnd)))
        {
            auto portraitWnd = dynamic_cast<ModelWnd*>(portrait);
            m_wndPortrait = dynamic_cast<ItemModelWnd*>(m3d::g_Kernel->New("ItemModelWnd"));
            if (m_wndPortrait)
            {
                if (!m_wndPortrait->CreateFromPattern(portraitWnd, true))
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndPortraitName + " from pattern class");
                    res = 0;
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndPortraitName + " - cannot find rtti class ItemModelWnd");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndPortraitName + " is not found or incorrect type");
            res = 0;
        }
        LoadPageInfo();
        auto app = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
        m_texBgShow = app->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdBg, 0);
        app->m_renderer->ReferenceTexture(m_texBgShow);
        SetCursorShow(false);
        if (res)
        {
            m_gameDataFlags |= 1u;
        }
        if ((m_gameDataFlags & 1) != 0)
            return 1;
        M3D_LOG_INFO("CreditsWnd: error - fail to init because of a bad resource");
        return 0;
    }
}

bool CreditsWnd::NeedExit() const
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::CheckAndShowNextPage()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::PlayAmbientSound()
{
    throw std::logic_error("Not implemented");
}

void CreditsWnd::StopAmbientSound()
{
    throw std::logic_error("Not implemented");
}

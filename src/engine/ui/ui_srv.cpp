#include <config.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/ini.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <ui/ui_srv.h>
#include <ui/frame.h>

namespace m3d
{
    ui::GfxServer::SoundInfo::SoundInfo()
    {
    }

    int ui::GfxServer::GetSliderHeight()
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::SetTexture(rend::TexHandle)
    {
        throw std::logic_error("Not implemented");
    }

    rend::TexHandle ui::GfxServer::GetTexture(ThemeTexture)
    {
        throw std::logic_error("Not implemented");
    }

    PointBase<float> ui::GfxServer::MeasureText(CStr const&, int, TextWrapFlags, float)
    {
        throw std::logic_error("Not implemented");
    }

    float ui::GfxServer::GetTabButtonHeight() const
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::SetColor(unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ui::GfxServer::GetColor(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddLineFlatAxialPane(DrawInfo const&, BoundsBase<float> const&, unsigned, int)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddRect(DrawInfo const&, BoundsBase<float> const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::ReadFrames()
    {
        CStr err;
        CStr filename = g_Kernel->GetEngineCfg().m_ui_pathToFrames.GetS();
        ref_ptr file = ReadXmlFile(filename.c_str(), &err);
        if (file)
        {
            ref_ptr nodeThemes = file->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            file->GetFirstChild_(nodeThemes, "Themes");
            if (nodeThemes->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }

            ref_ptr node = file->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            nodeThemes->GetFirstChild_(node, "Frames");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild_(node, "Item");
            while(!node->IsEmpty())
            {
                auto frame = new Frame;
                if (frame->ReadFromXmlNode(node))
                {
                    m_frames.push_back(frame);
                }
                else
                {
                    delete frame;
                }
                node->GetNextSibling_(node, "Item");
            }

            nodeThemes->GetFirstChild_(node, "Backgrounds");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild_(node, "Item");
            while (!node->IsEmpty())
            {
                auto background = new BackGround;
                if (background->ReadFromXmlNode(node))
                {
                    m_backgrounds.push_back(background);
                }
                else
                {
                    delete background;
                }
                node->GetNextSibling_(node, "Item");
            }

            nodeThemes->GetFirstChild_(node, "Panes");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild_(node, "Item");
            while (!node->IsEmpty())
            {
                auto pane = new Pane;
                if (pane->ReadFromXmlNode(node, m_backgrounds, m_frames))
                {
                    m_panes.add(pane->m_name, pane);
                    m_panesVector.push_back(pane);
                }
                else
                {
                    delete pane;
                }
                node->GetNextSibling_(node, "Item");
            }

            nodeThemes->GetFirstChild_(node, "Scrolls");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild_(node, "Item");
            while (!node->IsEmpty())
            {
                auto scroll = new ScrollPane;
                if (scroll->ReadFromXmlNode(node))
                {
                    m_scrollPanes.push_back(scroll);
                }
                else
                {
                    delete scroll;
                }
                node->GetNextSibling_(node, "Item");
            }

            nodeThemes->GetFirstChild_(node, "GlyphButtons");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild_(node, "Item");
            while (!node->IsEmpty())
            {
                CStr buttonName = node->GetAttribute("name");
                CStr buttonFile = node->GetAttribute("file");
                if (!buttonName.empty())
                {
                    m_glyphButtonTextures[buttonName] = Application::g_pApp->m_renderer->AddTexture(buttonFile, 4);
                    if (m_glyphButtonTextures[buttonName].IsValid())
                    {
                        Application::g_pApp->m_renderer->ReferenceTexture(m_glyphButtonTextures[buttonName]);
                    }
                }
                node->GetNextSibling_(node, "Item");
            }
            return 1;
        }
        M3D_LOG_INFO("cannot parse " + filename + " err: " + err);
        return 0;
    }

    ui::Font* ui::GfxServer::GetCurFont() const
    {
        throw std::logic_error("Not implemented");
    }

    ui::ScrollPane* ui::GfxServer::GetScrollPane(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    float ui::GfxServer::GetSliderNotchWidthRatio()
    {
        throw std::logic_error("Not implemented");
    }

    ui::GfxServer::~GfxServer()
    {
        if (m_created)
        {
            ReleaseSchema();
            delete m_fontManager;
            m_created = false;
        }
        //TODO: check additional
    }

    float ui::GfxServer::GetGlyphHeight()
    {
        throw std::logic_error("Not implemented");
    }

    float ui::GfxServer::GetTabButtonMaxWidth() const
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddTabWndPaneNormal(DrawInfo const&, BoundsBase<float> const&, unsigned, TabButtonInfo const&, std::vector<BoundsBase<float>> const&, int, int, CStr const&, PaneFlagBg)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddChkButtonFlatAxialPane(DrawInfo const&, BoundsBase<float> const&, unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    float ui::GfxServer::GetTabButtonSpace() const
    {
        throw std::logic_error("Not implemented");
    }

    ui::Pane* ui::GfxServer::GetPane(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::GetCornerSz() const
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::SetSchema(CStr const& name)
    {
        if (name.empty())
        {
            return 0;
        }
        CStr err;
        ref_ptr xmlFile = ReadXmlFile(name.c_str(), &err);
        if (!xmlFile)
        {
            M3D_LOG_INFO("SetSchema: " + err);
            return 0;
        }
        ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild_(node, "resource");
        if (node->IsEmpty())
        {
            M3D_LOG_INFO("GfxServer:: Create cannot find resources in " + name);
            return 0;
        }
        node->GetFirstChild_(node, "schema");
        if (node->IsEmpty())
        {
            M3D_LOG_INFO("GfxServer:: Create cannot find schema in " + name);
            return 0;
        }

        CStr sliderBody = node->GetAttribute("sliderBody");
        if (!sliderBody.empty())
        {
            m_texTheme[8] = Application::g_pApp->m_renderer->AddTexture(sliderBody, 4);
        }
        if (m_texTheme[8].IsValid())
        {
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[8], rend::TM_WRAP_S, 1);
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[8], rend::TM_WRAP_T, 1);
        }
        CStr sliderThumb = node->GetAttribute("sliderThumb");
        if (!sliderThumb.empty())
        {
            m_texTheme[9] = Application::g_pApp->m_renderer->AddTexture(sliderThumb, 4);
        }
        if (m_texTheme[9].IsValid())
        {
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[9], rend::TM_WRAP_S, 3);
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[9], rend::TM_WRAP_T, 3);
        }
        CStr sliderLeft = node->GetAttribute("sliderLeft");
        if (!sliderLeft.empty())
        {
            m_texTheme[10] = Application::g_pApp->m_renderer->AddTexture(sliderLeft, 4);
        }
        if (m_texTheme[10].IsValid())
        {
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[10], rend::TM_WRAP_S, 3);
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[10], rend::TM_WRAP_T, 3);
        }
        CStr sliderRight = node->GetAttribute("sliderRight");
        if (!sliderRight.empty())
        {
            m_texTheme[11] = Application::g_pApp->m_renderer->AddTexture(sliderRight, 4);
        }
        if (m_texTheme[11].IsValid())
        {
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[11], rend::TM_WRAP_S, 3);
            Application::g_pApp->m_renderer->SetTextureParameter(m_texTheme[11], rend::TM_WRAP_T, 3);
        }

        m_colors[0] = -1870626688;
        m_colors[1] = -1863257872;
        m_colors[2] = -1868521312;
        m_colors[3] = -2139062144;
        m_colors[4] = -8421505;
        m_colors[5] = -1;

        struct
        {
            const char* str = nullptr;
            unsigned int id = 0;
        } colors[6];
        colors[0].str = "clrDefault";
        colors[0].id = 0;
        colors[1].str = "clrSelected";
        colors[1].id = 1;
        colors[2].str = "clrMouseover";
        colors[2].id = 2;
        colors[3].str = "clrWndDisable";
        colors[3].id = 3;
        colors[4].str = "clrTextDisable";
        colors[4].id = 4;
        colors[5].str = "clrTextDefault";
        colors[5].id = 5;
        for (auto& color : colors)
        {
            unsigned clr = 0;
            SafeClrAttrib(clr, node, color.str);
            if (color.id < 0xFF)
            {
                m_colors[color.id] = clr;
            }
        }
        if (!m3d::SafeIntAttrib(m_cornerSz, node, "cornerSize"))
        {
            m_cornerSz = 32;
        }
        if (!m3d::SafeIntAttrib(m_btnWidth, node, "btnWidth"))
        {
            m_btnWidth = 50;
        }
        if (!m3d::SafeIntAttrib(m_btnHeight, node, "btnHeight"))
        {
            m_btnHeight = 140;
        }
        if (!m3d::SafeIntAttrib(m_sliderHeight, node, "sliderHeight"))
        {
            m_sliderHeight = 32;
        }
        if (!m3d::SafeFloatAttrib(m_sliderNotchWidthRatio, node, "sliderNotchWidthRatio"))
        {
            m_sliderNotchWidthRatio = 0.75;
        }

        char const* fontFaces[] = { "titleFontFace", "wndFontFace", "tooltipFontFace", "miscFontFace"};
        for (size_t i =0; i<4;++i)
        {
            if (!SafeStrAttrib(m_fontFaces[i], node, fontFaces[i]))
            {
                m_fontFaces[i] = "Arial";
            }
        }
        char const* fontSizes[] = { "titleFontSize", "wndFontSize", "tooltipFontSize", "miscFontSize" };
        for (size_t i = 0; i < 4; ++i)
        {
            if (!SafeUintAttrib(m_fontSizes[i], node, fontSizes[i]))
            {
                m_fontSizes[i] = 12;
            }
        }
        char const* fontFlags[] = { "titleFontFlag", "wndFontFlag", "tooltipFontFlag", "miscFontFlag" };
        CStr flag;
        for (size_t i = 0; i < 4; ++i)
        {
            //TODO: check tis
            if (!SafeStrAttrib(flag, node, fontFlags[i]))
            {
                m_fontFlags[i] = 0;
            }
            if (flag == "normal")
            {
                m_fontFlags[i] = 0;
            }
            else if (flag == "bold")
            {
                m_fontFlags[i] = 1;
            }
            else if (flag == "italic")
            {
                m_fontFlags[i] = 3;
            }
        }
        char const* fontTypes[] = { "titleFontType", "wndFontType", "tooltipFontType", "miscFontType" };
        for (size_t i = 0; i < 4; ++i)
        {
            if (!SafeEnumAttrib(m_fontTypes[i], node, fontTypes[i]))
            {
                m_fontTypes[i] = FONT_TYPE_WINDOWS;
            }
        }

        m_fontManager->Init();
        CStr font;
        for (size_t i = 0; i < 4; ++i)
        {
            //TODO: chcek this!!!
            FontParams params;
            if(m_fontTypes[i] == FONT_TYPE_SELFMAKING)
            {
                params.ttfParams.style = 1;
                params.ttfParams.codePage = 0;
            }
            else
            {
                params.ttfParams.style = m_fontFlags[i];
                params.ttfParams.codePage = Application::g_pApp->m_codePage.CodePage;
            }
            auto id = m_fontManager->GetFontId(m_fontFaces[i], m_fontSizes[i], m_fontTypes[i], params);
            if (id != i)
            {
                m_fontManager->RearrangeFonts(id, i);
            }
        }

        m_curFont = m_fontManager->GetFontById(0);
        m_hieroglyphicFontId = -1;
        auto frames = ReadFrames();
        if (Application::g_pApp->m_sound)
        {
            ref_ptr newNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            node->GetFirstChild_(newNode, nullptr);
            if (!newNode->IsEmpty())
            {
                LoadSoundsFromXml(xmlFile, newNode);
            }
        }
        if (!m3d::SafeFloatAttrib(m_glyphHeight, node, "glyphHeight"))
        {
            m_glyphHeight = 15.0;
        }
        if (!m3d::SafeFloatAttrib(m_tabButtonMinWidth, node, "tabBtnMinWidth"))
        {
            m_tabButtonMinWidth = 10.0;
        }
        if (!m3d::SafeFloatAttrib(m_tabButtonMaxWidth, node, "tabBtnMaxWidth"))
        {
            m_tabButtonMaxWidth = 50.0;
        }
        if (!m3d::SafeFloatAttrib(m_tabButtonHeight, node, "tabBtnHeight"))
        {
            m_tabButtonHeight = 30.0;
        }
        if (!m3d::SafeFloatAttrib(m_tabButtonSpace, node, "tabBtnSpace"))
        {
            m_tabButtonSpace = 5.0;
        }
        return 1;
    }

    void ui::GfxServer::AddButtonFlatAxialPane(DrawInfo const&, BoundsBase<float> const&, unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddFlatAxialPane0(DrawInfo const&, BoundsBase<float> const&, unsigned, int, CStr const&, PaneFlagBg)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddText(DrawInfo const&, PointBase<float> const&, CStr const&, int, TextWrapFlags, TextFormatFlags)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddImagedRect(DrawInfo const&, BoundsBase<float> const&, unsigned, rend::TexHandle)
    {
        throw std::logic_error("Not implemented");
    }

    rend::TexHandle ui::GfxServer::GetGlyph(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    ui::GfxServer::GfxServer()
    {
        m_fontManager = new FontManager;
        m_hieroglyphicFontId = -1;
    }

    int ui::GfxServer::PlayControlSound(CStr const&, bool*)
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::GetBtnHeight()
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::GetFontId(CStr const&, float, FontType, FontParams) const
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::GetBtnWidth()
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddFlatAxialQuad(DrawInfo const&, BoundsBase<float> const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::SetFont(int&)
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::SetFont(Font*)
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::SetFont(CStr const&, float, FontType, FontParams)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::AddImagedRectGeneral(DrawInfo const&, BoundsBase<float> const&, unsigned, rend::TexHandle, float, float, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::ClearFonts()
    {
        throw std::logic_error("Not implemented");
    }

    float ui::GfxServer::GetTabButtonMinWidth() const
    {
        throw std::logic_error("Not implemented");
    }

    ui::Font* ui::GfxServer::GetFontById(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::LoadSoundsFromXml(cmn::XmlFile* xmlFile, cmn::XmlNode const* xmlNode)
    {
        if (!xmlFile || !xmlNode)
        {
            return 0;
        }
        ClearSounds();
        ref_ptr soundNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        CStr soundName;
        CStr fileName;
        for (xmlNode->GetFirstChild_(soundNode, "SoundInfo"); !soundNode->IsEmpty(); soundNode->GetNextSibling_(soundNode, "SoundInfo"))
        {
            SafeStrAttrib(soundName, soundNode, "Name");
            if (soundName.empty())
            {
                M3D_LOG_WARN("GfxServer::LoadSoundsFromXml error - empty sound mame");
                continue;
            }
            if (m_controlSoundInfos.find(soundName) != cend(m_controlSoundInfos))
            {
                M3D_LOG_WARN("GfxServer::LoadSoundsFromXml error - sound with name " + soundName + " already exists. Second sound would be ignored");
                continue;
            }
            SafeStrAttrib(fileName, soundNode, "File");
            if (fileName.empty())
            {
                M3D_LOG_WARN("GfxServer::LoadSoundsFromXml error - sound file name is empty for sound " + soundName);
                continue;
            }
            auto const id = Application::g_pApp->m_sound->AddSound(fileName.c_str(), snd::SND_TYPE_2DSOUND, 1, 4, snd::SND_PRIORITY_HIGH);
            if (id == -1)
            {
                M3D_LOG_WARN("GfxServer::LoadSoundsFromXml error - cannot load sound from file " + soundName);
                continue;
            }
            auto soundInfo = new SoundInfo;
            soundInfo->m_soundName = soundName;
            soundInfo->m_soundFile = fileName;
            SafeBoolAttrib(soundInfo->m_bSoundLooped, soundNode, "Looped");
            soundInfo->m_soundTableId = id;
            m_controlSoundInfos[soundName] = soundInfo;
        }
        return 1;
    }

    void ui::GfxServer::ReleaseSchema()
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::ClearSounds()
    {
        for (auto&[name, sound] : m_controlSoundInfos)
        {
            if (Application::g_pApp->m_sound && sound)
            {
                Application::g_pApp->m_sound->DeleteIdTableSound(sound->m_soundTableId);
            }
            delete sound;
        }
        m_controlSoundInfos.clear();
    }

    void ui::GfxServer::AddFlatAxialQuad(DrawInfo const&, BoundsBase<float> const&, unsigned, float, float, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::FlushWindow(Wnd*)
    {
        throw std::logic_error("Not implemented");
    }

    int ui::GfxServer::Create()
    {
        m_created = true;
        return 1;
    }

    int ui::GfxServer::Done()
    {
        ReleaseSchema();
        if (m_fontManager)
        {
            delete m_fontManager;
        }
        m_created = false;
        return 1;
    }
}

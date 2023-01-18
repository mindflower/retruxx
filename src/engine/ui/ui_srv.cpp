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
    ui::FormattedLine::FormattedLine()
    {
    }

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

    rend::TexHandle ui::GfxServer::GetTexture(ThemeTexture tex)
    {
        return m_texTheme[tex];
    }

    PointBase<float> ui::GfxServer::MeasureText(CStr const& text, int uiFont, TextWrapFlags tw, float maxWidth)
    {
        //TODO: implement GfxServer::MeasureText
        PointBase<float> result;
        if (m_fontManager->ValidateFontId(uiFont))
        {
            if (auto font = m_fontManager->GetFontById(uiFont))
            {
                m_curFont = font;
            }
        }
        if (tw != TW_NOWRAP)
        {
            Application::g_pApp->GetTextFit(text, result, maxWidth, tw);
        }
        else
        {
            Application::g_pApp->GetTextExtent(text, result, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        return result;
    }

    float ui::GfxServer::GetTabButtonHeight() const
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::SetColor(unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ui::GfxServer::GetColor(unsigned c) const
    {
        auto res = c;
        if ((c & 0xFF000000) == 0 && c < 0xFF)
        {
            res = m_colors[c];
        }
        return res;
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
        return m_curFont;
    }

    ui::ScrollPane* ui::GfxServer::GetScrollPane(CStr const& paneName)
    {
        for (auto pane : m_scrollPanes)
        {
            if (pane->m_name == paneName)
            {
                return pane;
            }
        }
        for (auto pane : m_scrollPanes)
        {
            if (pane->m_name == "Scroll1")
            {
                return pane;
            }
        }
        return nullptr;
    }

    float ui::GfxServer::GetSliderNotchWidthRatio()
    {
        return m_sliderNotchWidthRatio;
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

    ui::Pane* ui::GfxServer::GetPane(CStr const& paneName)
    {
        Pane* pane = nullptr;
        m_panes.get(paneName, pane);
        return pane;
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

        m_colors[0] = 0x90808080;
        m_colors[1] = 0x90F0F0F0;
        m_colors[2] = 0x90A0A0A0;
        m_colors[3] = 0x80808080;
        m_colors[4] = 0xFF7F7F7F;
        m_colors[5] = 0xFFFFFFFF;

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

    void ui::GfxServer::AddFlatAxialPane0(DrawInfo const& di, BoundsBase<float> const& rect, unsigned clr, int drawFlags, CStr const& paneName, PaneFlagBg bgFlags)
    {
        //TODO: check and refactor all this shit!!!
        Pane* pane = nullptr;
        m_panes.get(paneName, pane);
        if (!pane)
        {
            m_panes.get("defaultWnd", pane);
        }
        if (pane)
        {
            M3D_APP->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            M3D_APP->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
            M3D_APP->m_renderer->PushBlend(rend::BM_ALPHA);
            M3D_APP->m_renderer->SetAlphaTest(g_Kernel->GetEngineCfg().m_alphaTestInterface.GetI());
            M3D_APP->m_renderer->PushZbState(rend::ZB_DISABLE);
            auto frame = pane->m_frame[bgFlags];
            auto cornerSize = 0;
            if (frame)
            {
                cornerSize = frame->m_cornerSize;
            }
            else
            {
                if (pane->m_frame[0])
                {
                    cornerSize = pane->m_frame[0]->m_cornerSize;
                }
            }
            int drawComplexCorners = 0;
            if (rect.width > cornerSize && rect.height > cornerSize && (drawFlags & 4) != 0)
            {
                drawComplexCorners = 1;
            }
            else
            {
                drawComplexCorners = 0;
                cornerSize = 0;
            }
            if ((drawFlags & 1) != 0)
            {
                rend::TexHandle texture;
                auto flag = bgFlags;
                if (pane->m_bg[bgFlags] && pane->m_bg[bgFlags]->m_texture.IsValid())
                {
                    texture = pane->m_bg[bgFlags]->m_texture;
                }
                else
                {
                    flag = PANE_FLAG_BG_OUT;
                }
                if (!texture.IsValid() && pane->m_bg[PANE_FLAG_BG_OUT])
                {
                    texture = pane->m_bg[PANE_FLAG_BG_OUT]->m_texture;
                }
                if (texture.IsValid())
                {
                    //TODO: check this!!!!!!!!!
                    int sx = 0;
                    int sy = 0;
                    M3D_APP->m_renderer->GetDims(texture, sx, sy);
                    float fsx = sx;
                    float fsy = sy;
                    M3D_APP->m_renderer->AbsToRel(fsx, fsy);

                    auto tu1 = 1.0;
                    auto tv1 = 1.0;
                    if (pane->m_bg[flag]->m_repeatU)
                    {
                        tu1 = rect.width / fsx;
                    }
                    if (pane->m_bg[flag]->m_repeatV)
                    {
                        tv1 = rect.height / fsy;
                    }
                    auto bgRect = rect;
                    if (pane->m_bInnerBg && (drawFlags & 6) != 0)
                    {
                        auto barTexWidth = 0.0;
                        if (pane->m_frame[flag])
                        {
                            barTexWidth = pane->m_frame[flag]->m_barTexWidth;
                        }
                        bgRect.x0 = rect.x0 - (0.0 - barTexWidth);
                        bgRect.y0 = rect.y0 - (0.0 - barTexWidth);
                        bgRect.width = ((0.0 - barTexWidth) * 2.0) + rect.width;
                        bgRect.height = ((0.0 - barTexWidth) * 2.0) + rect.height;
                    }
                    M3D_APP->m_renderer->SetTexture(0, texture, -1.0);
                    AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, tu1, tv1);
                }
            }

            if (pane->m_frame[bgFlags] || pane->m_frame[PANE_FLAG_BG_OUT])
            {
                auto flag = bgFlags;
                if (!pane->m_frame[bgFlags])
                {
                    flag = PANE_FLAG_BG_OUT;
                }
                if ((drawFlags & 2) != 0)
                {
                    if (pane->m_frame[flag]->m_textures[0].IsValid())
                    {
                        auto bgRect = rect;
                        bgRect.x0 = rect.x0;
                        bgRect.width = (bgRect.x0 + pane->m_frame[flag]->m_barTexWidth) - bgRect.x0;
                        bgRect.y0 = rect.y0 + cornerSize;
                        bgRect.height = (rect.y0 + rect.height - cornerSize) - bgRect.y0;

                        auto tv1 = rect.height;
                        auto scale = rect.height;
                        if (pane->m_frame[flag]->m_barRepeat)
                        {
                            throw std::logic_error("Not implemented");
                            int sx = 0;
                            int sy = 0;
                            M3D_APP->m_renderer->GetDims(pane->m_frame[flag]->m_textures[0], sx, sy);
                            float fsx = sx;
                            float fsy = sy;
                            M3D_APP->m_renderer->AbsToRel(fsx, fsy);
                            scale = fsy;
                        }
                        M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[0], -1.0);
                        AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, 1.0, tv1 / scale);


                        bgRect.x0 = rect.x0 + rect.width - cornerSize;
                        bgRect.height = (rect.y0 + rect.height - cornerSize) - (rect.y0 + cornerSize);
                        bgRect.y0 = rect.y0 + cornerSize;
                        bgRect.width = cornerSize;
                        if (pane->m_frame[flag]->m_textures[2].IsValid())
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[2], -1.0);

                            if (pane->m_frame[flag]->m_barRepeat)
                            {
                                throw std::logic_error("Not implemented");
                            }
                            AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, 1.0, tv1 / scale);
                        }
                        else
                        {
                            throw std::logic_error("Not implemented");
                            AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, 1.0, tv1 / scale);
                        }
                    }

                    if (pane->m_frame[flag]->m_textures[1].IsValid())
                    {
                        auto bgRect = rect;
                        bgRect.x0 = rect.x0 + cornerSize;
                        bgRect.width = (rect.x0 + rect.width) - cornerSize - bgRect.x0;
                        bgRect.height = cornerSize;
                        bgRect.y0 = rect.y0;

                        auto tv0 = rect.width;
                        auto scale = rect.width;
                        if (pane->m_frame[flag]->m_barRepeat)
                        {
                            throw std::logic_error("Not implemented");
                            int sx = 0;
                            int sy = 0;
                            M3D_APP->m_renderer->GetDims(pane->m_frame[flag]->m_textures[0], sx, sy);
                            float fsx = sx;
                            float fsy = sy;
                            M3D_APP->m_renderer->AbsToRel(fsx, fsy);
                            scale = fsy;
                        }
                        M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[1], -1.0);
                        AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, tv0 / scale, 1.0);


                        bgRect.x0 = rect.x0 + cornerSize;
                        bgRect.height = cornerSize;
                        bgRect.y0 = rect.height + rect.y0 - cornerSize;
                        bgRect.width = (rect.x0 + rect.width - cornerSize) - bgRect.x0;
                        if (pane->m_frame[flag]->m_textures[3].IsValid())
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[3], -1.0);

                            if (pane->m_frame[flag]->m_barRepeat)
                            {
                                throw std::logic_error("Not implemented");
                            }
                            AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, tv0 / scale, 1.0);
                        }
                        else
                        {
                            throw std::logic_error("Not implemented");
                            AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, 1.0, tv0 / scale);
                        }
                    }
                }
                if (drawComplexCorners)
                {
                    if (pane->m_frame[flag]->m_textures[4].IsValid())
                    {
                        //TODO: check this!!
                        M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[4], -1.0);
                        auto bgRect = rect;
                        bgRect.width = cornerSize;
                        bgRect.height = cornerSize;
                        AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, 1.0, 1.0);
                        float tu0 = 0.0;
                        float tv0 = 0.0;
                        float tu1 = 0.0;
                        float tv1 = 0.0;
                        if (!pane->m_frame[flag]->m_textures[7].IsValid())
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[4], -1.0);
                            tu0 = 1.0;
                            tv0 = 1.0;
                        }
                        else
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[7], -1.0);
                            tu1 = 1.0;
                            tv1 = 1.0;
                        }
                        bgRect.x0 = rect.x0 + rect.width - cornerSize;
                        bgRect.y0 = rect.y0 + rect.height - cornerSize;
                        AddFlatAxialQuad(di, bgRect, clr, tu0, tv0, tu1, tv1);

                        tu0 = 0.0;
                        tv0 = 0.0;
                        tu1 = 0.0;
                        tv1 = 0.0;
                        if (!pane->m_frame[flag]->m_textures[6].IsValid())
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[4], -1.0);
                            tv0 = 1.0;
                        }
                        else
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[6], -1.0);
                            tv1 = 1.0;
                        }

                        bgRect.x0 = rect.x0;
                        bgRect.y0 = rect.y0 + rect.height - cornerSize;
                        bgRect.width = cornerSize;
                        bgRect.height = cornerSize;
                        AddFlatAxialQuad(di, bgRect, clr, 0.0, tv0, 1.0, tv1);

                        if (!pane->m_frame[flag]->m_textures[5].IsValid())
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[6], -1.0);
                            tu0 = 1.0;
                        }
                        else
                        {
                            M3D_APP->m_renderer->SetTexture(0, pane->m_frame[flag]->m_textures[5], -1.0);
                            tu1 = 1.0;
                        }

                        bgRect.x0 = rect.x0 + rect.width - cornerSize;
                        bgRect.y0 = rect.y0;
                        bgRect.width = cornerSize;
                        bgRect.height = cornerSize;
                        AddFlatAxialQuad(di, bgRect, clr, tu0, 0.0, tu1, 1.0);
                    }
                }
            }
            m3d::Application::g_pApp->m_renderer->PopBlend();
            m3d::Application::g_pApp->m_renderer->PopZbState();
            m3d::Application::g_pApp->m_renderer->SetAlphaTest(0);
        }
    }

    void ui::GfxServer::AddText(DrawInfo const& di, PointBase<float> const& at, CStr const& text, int uiFont, TextWrapFlags tw, TextFormatFlags tf)
    {
        if (m_fontManager->ValidateFontId(uiFont))
        {
            auto const font = m_fontManager->GetFontById(uiFont);
            if (font)
                m_curFont = font;
        }
        PointBase<float> point;
        point.x = at.x + di.m_clientRect.x0;
        point.y = at.y + di.m_clientRect.y0;
        M3D_APP->DrawTextRelClip(point, text, di, tw, tf);
    }

    void ui::GfxServer::AddImagedRect(DrawInfo const& di, BoundsBase<float> const& rect, unsigned clr, rend::TexHandle tex)
    {
        AddImagedRectGeneral(di, rect, clr, tex, 0.0, 0.0, 1.0, 1.0);
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

    int ui::GfxServer::PlayControlSound(CStr const& soundName, bool* pbLooped)
    {
        if (!M3D_APP->m_sound || !M3D_KERNEL->GetEngineCfg().m_snd_Enable.GetB())
        {
            return -1;
        }
        auto const it = m_controlSoundInfos.find(soundName);
        if (it == m_controlSoundInfos.cend())
        {
            return -1;
        }
        auto const info = it->second;
        if (info == nullptr || info->m_soundTableId == -1)
        {
            return -1;
        }
        bool looped = pbLooped ? *pbLooped : info->m_bSoundLooped;
        return M3D_APP->m_sound->PlaySound2D(info->m_soundTableId, looped);
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

    void ui::GfxServer::AddImagedRectGeneral(DrawInfo const& di, BoundsBase<float> const& rect, unsigned clr, rend::TexHandle tex, float u0, float v0, float u1, float v1)
    {
        m3d::Application::g_pApp->m_renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        m3d::Application::g_pApp->m_renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        m3d::Application::g_pApp->m_renderer->PushBlend(rend::BM_ALPHA);
        m3d::Application::g_pApp->m_renderer->SetAlphaTest(g_Kernel->GetEngineCfg().m_alphaTestInterface.GetI());
        m3d::Application::g_pApp->m_renderer->PushZbState(rend::ZB_DISABLE);
        m3d::Application::g_pApp->m_renderer->SetTexture(0, tex, -1.0);
        auto sx = 0;
        auto sy = 0;
        m3d::Application::g_pApp->m_renderer->GetDims( tex, sx, sy);
        PointBase<float> s;
        if (u1 >= 0.0)
            s.x = u1;
        else
            s.x = 0.0 - (u1 / sx);
        if (v1 >= 0.0)
            s.y = v1;
        else
            s.y = 0.0 - (v1 / sy);
        AddFlatAxialQuad(di, rect, clr, u0, v0, s.x, s.y);
        m3d::Application::g_pApp->m_renderer->PopBlend();
        m3d::Application::g_pApp->m_renderer->PopZbState();
        m3d::Application::g_pApp->m_renderer->SetAlphaTest(0);
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

    void ui::GfxServer::AddFlatAxialQuad(DrawInfo const& di, BoundsBase<float> const& rc, unsigned clr, float tu0, float tv0, float tu1, float tv1)
    {
        //TODO: check this and recator
        auto v8 = rc.width;
        auto v9 = rc.height;
        auto actual_4 = di.m_originalRect.y0 + rc.y0;
        auto v10 = di.m_originalRect.x0 + rc.x0;
        auto v11 = di.m_clippedRect.x0;
        auto v12 = v8 + v10;
        auto actual_8 = v8;
        auto actual_12 = v9;
        auto v13 = di.m_clippedRect.width + v11;
        auto v14 = v9 + actual_4;

        auto v15 = 0.0;
        auto clipped_4 = 0.0;
        auto v17 = 0.0;
        auto clipped_12 = 0.0;
        if (v11 > (v8 + v10)
            || v10 > v13
            || di.m_clippedRect.y0 > (v9 + actual_4)
            || actual_4 > (di.m_clippedRect.height + di.m_clippedRect.y0))
        {
            v15 = 0.0;
            clipped_4 = 0.0;
            v17 = 0.0;
            clipped_12 = 0.0;
        }
        else
        {
            v15 = di.m_clippedRect.x0;
            if (v15 <= v10)
                v15 = di.m_originalRect.x0 + rc.x0;
            if (v12 <= v13)
                v13 = v8 + v10;
            auto v16 = di.m_originalRect.y0 + rc.y0;
            if (di.m_clippedRect.y0 > actual_4)
                v16 = di.m_clippedRect.y0;
            if (v14 > (di.m_clippedRect.height + di.m_clippedRect.y0))
                v14 = di.m_clippedRect.height + di.m_clippedRect.y0;
            clipped_4 = v16;
            v17 = v13 - v15;
            clipped_12 = v14 - v16;
        }
        if ((v12 - v10) != 0.0 || (actual_4 - (actual_12 + actual_4)) != 0.0)
        {
            auto v18 = tu0;
            auto v19 = tv1;
            auto u0 = tu0;
            auto tu0a = tv0;
            auto tu2 = tu1;
            auto u1 = tv1;
            if (v15 > v10)
            {
                v19 = tv1;
                u0 = (((tu1 - v18) / actual_8) * (v15 - v10)) + v18;
            }
            auto v20 = v17 + v15;
            if (v12 > v20)
                tu2 = (((v18 - tu1) / actual_8) * (v12 - v20)) + tu1;
            if (clipped_4 > actual_4)
                tu0a = (((v19 - tv0) / actual_12) * (clipped_4 - actual_4)) + tv0;
            if ((actual_12 + actual_4) > (clipped_12 + clipped_4))
                u1 = (((tv0 - v19) / actual_12) * ((actual_12 + actual_4) - (clipped_12 + clipped_4))) + v19;
            auto v21 = clr;
            if ((clr & 0xFF000000) == 0 && clr < 0xFF)
                v21 = this->m_colors[clr];
            m3d::Application::g_pApp->PutSprite2Rel(
                v15,
                clipped_4,
                u0,
                tu0a,
                v20,
                clipped_12 + clipped_4,
                tu2,
                u1,
                v21);
        }
    }

    void ui::GfxServer::FlushWindow(Wnd* wnd)
    {
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

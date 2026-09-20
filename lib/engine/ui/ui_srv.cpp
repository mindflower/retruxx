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
    ui::FormattedLine::FormattedLine(CStr text)
    {
        m_text = text;
        m_color = 0;
        m_origin.x = 0.0f;
        m_origin.y = 0.0f;
        m_format = TF_LEFT;
        m_isHieroglyphic = Application::g_pApp->IsTextHieroglyphic(m_text);
    }

    ui::FormattedLine::FormattedLine()
    {
        this->m_color = 0;
        this->m_origin.x = 0.0;
        this->m_origin.y = 0.0;
        this->m_format = TF_LEFT;
        this->m_isHieroglyphic = 0;
    }

    ui::GfxServer::SoundInfo::SoundInfo()
    {
        this->m_soundTableId = -1;
        this->m_bSoundLooped = 0;
    }

    int ui::GfxServer::GetSliderHeight()
    {
        return m_sliderHeight;
    }

    void ui::GfxServer::SetTexture(rend::TexHandle tex)
    {
        M3D_RENDERER->SetTexture(0, tex, -1.0);
    }

    rend::TexHandle ui::GfxServer::GetTexture(ThemeTexture tex)
    {
        return m_texTheme[tex];
    }

    PointBase<float> ui::GfxServer::MeasureText(CStr const& text, int uiFont, TextWrapFlags tw, float maxWidth)
    {
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
        return m_tabButtonHeight;
    }

    void ui::GfxServer::SetColor(unsigned clrEnum, unsigned clr)
    {
        if (clrEnum < 0xFF)
        {
            m_colors[clrEnum] = clr;
        }
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

    void ui::GfxServer::AddLineFlatAxialPane(DrawInfo const& di, BoundsBase<float> const& rect, unsigned clr, int dir)
    {
        rend::TexHandle tex = m_texTheme[dir != 0 ? TEX_LINE_VERT : TEX_LINE_HORZ];
        M3D_RENDERER->SetTexture(0, tex, -1.0);

        int sx = 0;
        int sy = 0;
        M3D_RENDERER->GetDims(tex, sx, sy);
        float fsx = static_cast<float>(sx);
        float fsy = static_cast<float>(sy);
        M3D_RENDERER->AbsToRel(fsx, fsy);

        float tu1 = 1.0f;
        float tv1 = 1.0f;
        if (dir == 0)
        {
            tu1 = rect.width / fsx;
        }
        else
        {
            tv1 = rect.height / fsy;
        }
        AddFlatAxialQuad(di, rect, clr, 0.0, 0.0, tu1, tv1);
    }

    void ui::GfxServer::AddRect(DrawInfo const& di, BoundsBase<float> const& rect, unsigned clr)
    {
        float const ax0 = di.m_originalRect.x0 + rect.x0;
        float const ay0 = di.m_originalRect.y0 + rect.y0;
        float const ax1 = ax0 + rect.width;
        float const ay1 = ay0 + rect.height;

        float const cx0 = di.m_clippedRect.x0;
        float const cx1 = di.m_clippedRect.x0 + di.m_clippedRect.width;
        float const cy0 = di.m_clippedRect.y0;
        float const cy1 = di.m_clippedRect.y0 + di.m_clippedRect.height;

        BoundsBase<float> clipped;
        clipped.x0 = 0.0f;
        clipped.y0 = 0.0f;
        clipped.width = 0.0f;
        clipped.height = 0.0f;
        if (!(cx0 > ax1 || ax0 > cx1 || cy0 > ay1 || ay0 > cy1))
        {
            float const x0 = (cx0 <= ax0) ? ax0 : cx0;
            float const x1 = (ax1 <= cx1) ? ax1 : cx1;
            float const y0 = (cy0 <= ay0) ? ay0 : cy0;
            float const y1 = (ay1 > cy1) ? cy1 : ay1;
            clipped.x0 = x0;
            clipped.y0 = y0;
            clipped.width = x1 - x0;
            clipped.height = y1 - y0;
        }

        if ((ax1 - ax0) != 0.0f || (ay0 - ay1) != 0.0f)
        {
            M3D_RENDERER->SetWhiteTexture(0);
            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
            M3D_RENDERER->DisableTextureStages(1);

            unsigned c = clr;
            if ((clr & 0xFF000000) == 0 && clr < 0xFF)
            {
                c = m_colors[clr];
            }

            CVector2 const from(clipped.x0, clipped.y0);
            CVector2 const to(clipped.x0 + clipped.width, clipped.y0 + clipped.height);
            Application::g_pApp->DrawWireRectRel(from, to, c);
        }
    }

    int ui::GfxServer::ReadFrames()
    {
        CStr err;
        CStr filename = g_Kernel->GetEngineCfg().m_ui_pathToFrames.GetS();
        ref_ptr file = ReadXmlFile(filename.c_str(), &err);
        if (file)
        {
            ref_ptr nodeThemes = file->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            file->GetFirstChild(nodeThemes, "Themes");
            if (nodeThemes->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }

            ref_ptr node = file->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            nodeThemes->GetFirstChild(node, "Frames");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild(node, "Item");
            while (!node->IsEmpty())
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
                node->GetNextSibling(node, "Item");
            }

            nodeThemes->GetFirstChild(node, "Backgrounds");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild(node, "Item");
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
                node->GetNextSibling(node, "Item");
            }

            nodeThemes->GetFirstChild(node, "Panes");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild(node, "Item");
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
                node->GetNextSibling(node, "Item");
            }

            nodeThemes->GetFirstChild(node, "Scrolls");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild(node, "Item");
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
                node->GetNextSibling(node, "Item");
            }

            nodeThemes->GetFirstChild(node, "GlyphButtons");
            if (node->IsEmpty())
            {
                M3D_LOG_INFO("cannot parse " + filename);
                return 0;
            }
            node->GetFirstChild(node, "Item");
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
                node->GetNextSibling(node, "Item");
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
        return m_glyphHeight;
    }

    float ui::GfxServer::GetTabButtonMaxWidth() const
    {
        return m_tabButtonMaxWidth;
    }

    void ui::GfxServer::AddTabWndPaneNormal(
        DrawInfo const&,
        BoundsBase<float> const&,
        unsigned,
        TabButtonInfo const&,
        retruxx::vector<BoundsBase<float>> const&,
        int,
        int,
        CStr const&,
        PaneFlagBg)
    {
        // RVA 0x680D60 - the tab strip: the same pane drawing as
        // AddFlatAxialPane0, but with the top edge broken open around the tab
        // buttons and each button framed in its own right.
        //
        // Structure recovered from the disassembly (not yet written out):
        //  * pane lookup falls back to "defaultTab", not "defaultWnd";
        //    stage state is TS_TEXTURE here where AddFlatAxialPane0 uses
        //    TS_MODULATE.
        //  * cornerSz / cornerRoundSz / usedBarW come from m_frame[bgFlags],
        //    else m_frame[0]; both corner sizes are forced to 0 when
        //    (drawFlags & 4) == 0.
        //  * background (drawFlags & 1) is not one quad but a list of rects:
        //    the body below the tab row, the strip directly under the buttons,
        //    one rect per button spanning cornerRoundSz..btnHeight, and (with
        //    complex corners) each button's rounded top inset by cornerRoundSz.
        //    Rects belonging to a button other than selButton are drawn from
        //    m_bg[1]'s texture, or from the normal one in colour 3 if the pane
        //    has no second background.
        //  * frame (drawFlags & 2): left bar m_textures[0] and right bar
        //    m_textures[2] (or [0] with U flipped) span
        //    rect.y0 + btnHeight + cornerRoundSz .. rect.y0 + rect.height - cornerSz;
        //    the top bar m_textures[1] is drawn as two segments either side of
        //    the selected button, cut at (cornerSz - usedBarW) from its edges;
        //    the bottom bar is m_textures[3], or [1] with V flipped.
        //  * each button then gets its own top / left / right bars, taking the
        //    texture from m_frame[1] when the button is not selected.
        //  * corners (drawFlags & 4): the window's own from m_textures[4..7],
        //    and each button's rounded corners from m_textures[8..11] - the
        //    tabbtn_corner_* set that Frame::ReadFromXmlNode loads.
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ui::GfxServer::AddTabWndPaneIzvrat(
        DrawInfo const&,
        BoundsBase<float> const&,
        unsigned,
        TabButtonInfo const&,
        retruxx::vector<BoundsBase<float>> const&,
        retruxx::vector<rend::TexHandle> const&,
        int,
        int,
        CStr const&,
        PaneFlagBg)
    {
        // RVA 0x67F3C0 - the "izvrat" tab strip draws each tab as its own glyph
        // image (the buttonImages vector) rather than a pane-framed button.
        // Same shape as AddTabWndPaneNormal above, which should be written
        // first; not reconstructed yet. TabWnd::OnNcPaint dispatches here when
        // the tab info asks for DRAWSTYLE_IZVRAT.
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ui::GfxServer::AddChkButtonFlatAxialPane(
        DrawInfo const& di,
        BoundsBase<float> const& rect,
        unsigned clr,
        bool down)
    {
        rend::TexHandle const tex = down ? m_texTheme[TEX_CHK_CHECKED] : m_texTheme[TEX_CHK_UNCHECKED];
        BoundsBase<float> rc;
        rc.x0 = rect.x0;
        rc.y0 = rect.y0;
        rc.height = rect.height;
        rc.width = rc.height;
        AddImagedRectGeneral(di, rc, clr, tex, 0.0, 0.0, 1.0, 1.0);
    }

    float ui::GfxServer::GetTabButtonSpace() const
    {
        return m_tabButtonSpace;
    }

    ui::Pane* ui::GfxServer::GetPane(CStr const& paneName)
    {
        Pane* pane = nullptr;
        m_panes.get(paneName, pane);
        return pane;
    }

    int ui::GfxServer::GetCornerSz() const
    {
        return m_cornerSz;
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
        xmlFile->GetFirstChild(node, "resource");
        if (node->IsEmpty())
        {
            M3D_LOG_INFO("GfxServer:: Create cannot find resources in " + name);
            return 0;
        }
        node->GetFirstChild(node, "schema");
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
            char const* str = nullptr;
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

        char const* fontFaces[] = {"titleFontFace", "wndFontFace", "tooltipFontFace", "miscFontFace"};
        for (size_t i = 0; i < 4; ++i)
        {
            if (!SafeStrAttrib(m_fontFaces[i], node, fontFaces[i]))
            {
                m_fontFaces[i] = "Arial";
            }
        }
        char const* fontSizes[] = {"titleFontSize", "wndFontSize", "tooltipFontSize", "miscFontSize"};
        for (size_t i = 0; i < 4; ++i)
        {
            if (!SafeUintAttrib(m_fontSizes[i], node, fontSizes[i]))
            {
                m_fontSizes[i] = 12;
            }
        }
        char const* fontFlags[] = {"titleFontFlag", "wndFontFlag", "tooltipFontFlag", "miscFontFlag"};
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
        char const* fontTypes[] = {"titleFontType", "wndFontType", "tooltipFontType", "miscFontType"};
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
            if (m_fontTypes[i] == FONT_TYPE_SELFMAKING)
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
            node->GetFirstChild(newNode, nullptr);
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

    void ui::GfxServer::AddButtonFlatAxialPane(
        DrawInfo const& di,
        BoundsBase<float> const& rect,
        unsigned clr,
        bool down)
    {
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->SetAlphaTest(g_Kernel->GetEngineCfg().m_alphaTestInterface.GetI());
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);

        rend::TexHandle const texC = down ? m_texTheme[TEX_BTN_C_1] : m_texTheme[TEX_BTN_C_0];
        rend::TexHandle const texR = down ? m_texTheme[TEX_BTN_R_1] : m_texTheme[TEX_BTN_R_0];
        rend::TexHandle const texL = down ? m_texTheme[TEX_BTN_L_1] : m_texTheme[TEX_BTN_L_0];

        // Left cap: a square of side rect.height.
        BoundsBase<float> rc;
        rc.x0 = rect.x0;
        rc.y0 = rect.y0;
        rc.height = rect.height;
        rc.width = rc.height;
        M3D_RENDERER->SetTexture(0, texL, -1.0);
        AddFlatAxialQuad(di, rc, clr, 0.0, 0.0, 1.0, 1.0);

        // Right cap.
        rc.x0 = rect.width - rect.height;
        rc.y0 = rect.y0;
        rc.height = rect.height;
        rc.width = rc.height;
        M3D_RENDERER->SetTexture(0, texR, -1.0);
        AddFlatAxialQuad(di, rc, clr, 0.0, 0.0, 1.0, 1.0);

        // Centre: horizontally tiled between the two caps.
        int sx = 0;
        int sy = 0;
        M3D_RENDERER->GetDims(texC, sx, sy);
        float ssx = static_cast<float>(sx);
        float ssy = static_cast<float>(sy);
        M3D_RENDERER->AbsToRel(ssx, ssy);

        rc.x0 = rect.height;
        rc.y0 = rect.y0;
        rc.height = rect.height;
        rc.width = rect.width - rect.height * 2.0f;
        M3D_RENDERER->SetTexture(0, texC, -1.0);
        AddFlatAxialQuad(di, rc, clr, 0.0, 0.0, rect.width / ssx, 1.0);

        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->SetAlphaTest(0);
    }

    void ui::GfxServer::AddFlatAxialPane0(
        DrawInfo const& di,
        BoundsBase<float> const& rect,
        unsigned clr,
        int drawFlags,
        CStr const& paneName,
        PaneFlagBg bgFlags)
    {
        // RVA 0x67E690 - draws a window frame as a nine-slice: a background quad, four edge
        // bars and four corners. A skin only has to supply a left bar, a top bar and the
        // top-left corner; the opposite pieces are drawn from those with the texture
        // coordinates reversed, so the art mirrors rather than repeating.
        Pane* pane = nullptr;
        m_panes.get(paneName, pane);
        if (!pane)
        {
            m_panes.get("defaultWnd", pane);
        }
        if (!pane)
        {
            return;
        }

        auto* renderer = M3D_APP->m_renderer;
        renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        renderer->PushBlend(rend::BM_ALPHA);
        renderer->SetAlphaTest(g_Kernel->GetEngineCfg().m_alphaTestInterface.GetI());
        renderer->PushZbState(rend::ZB_DISABLE);

        // The corner size comes from the frame for the requested state, falling back to the
        // default one.
        int cornerSize = 0;
        {
            Frame const* cornerFrame = pane->m_frame[bgFlags];
            if (!cornerFrame)
            {
                cornerFrame = pane->m_frame[PANE_FLAG_BG_OUT];
            }
            if (cornerFrame)
            {
                cornerSize = cornerFrame->m_cornerSize;
            }
        }

        // Corners are only drawn when the pane is bigger than they are; otherwise the edges run
        // the whole way and nothing is held back for them.
        bool const drawComplexCorners =
            rect.width > cornerSize && rect.height > cornerSize && (drawFlags & 4) != 0;
        if (!drawComplexCorners)
        {
            cornerSize = 0;
        }

        if ((drawFlags & 1) != 0)
        {
            // The background of the requested state, or the default one if that state has none.
            PaneFlagBg bgIndex = bgFlags;
            rend::TexHandle texture;
            if (pane->m_bg[bgFlags])
            {
                texture = pane->m_bg[bgFlags]->m_texture;
            }
            if (!texture.IsValid())
            {
                bgIndex = PANE_FLAG_BG_OUT;
                if (pane->m_bg[PANE_FLAG_BG_OUT])
                {
                    texture = pane->m_bg[PANE_FLAG_BG_OUT]->m_texture;
                }
            }

            if (texture.IsValid())
            {
                int sx = 0;
                int sy = 0;
                renderer->GetDims(texture, sx, sy);
                float texWidth = static_cast<float>(sx);
                float texHeight = static_cast<float>(sy);
                renderer->AbsToRel(texWidth, texHeight);

                // A background that does not repeat is stretched over the pane instead.
                BackGround const* bg = pane->m_bg[bgIndex];
                float tu = 1.0f;
                float tv = 1.0f;
                if (bg->m_repeatU)
                {
                    tu = rect.width / texWidth;
                }
                if (bg->m_repeatV)
                {
                    tv = rect.height / texHeight;
                }

                auto bgRect = rect;
                if (pane->m_bInnerBg && (drawFlags & 6) != 0)
                {
                    // Inset by the bar width so the background does not show through under the
                    // border. The frame is chosen independently of whichever background won.
                    Frame const* insetFrame = pane->m_frame[bgFlags];
                    if (!insetFrame)
                    {
                        insetFrame = pane->m_frame[PANE_FLAG_BG_OUT];
                    }

                    float const inset = insetFrame ? static_cast<float>(insetFrame->m_barTexWidth) : 0.0f;
                    bgRect.x0 = rect.x0 + inset;
                    bgRect.y0 = rect.y0 + inset;
                    bgRect.width = rect.width - inset * 2.0f;
                    bgRect.height = rect.height - inset * 2.0f;
                }

                renderer->SetTexture(0, texture, -1.0);
                AddFlatAxialQuad(di, bgRect, clr, 0.0, 0.0, tu, tv);
            }
        }

        Frame const* frame = pane->m_frame[bgFlags];
        if (!frame)
        {
            frame = pane->m_frame[PANE_FLAG_BG_OUT];
        }

        if (frame)
        {
            // Every edge is the same thickness: the width of the bar artwork. cornerSize only
            // says where along an edge the bar starts and stops.
            float const barW = static_cast<float>(frame->m_barTexWidth);

            // A bar that repeats is tiled by its own size; one that does not is stretched, which
            // is the same as a single span of the texture.
            auto repeatAlong = [&](rend::TexHandle tex, float span, bool vertical) -> float
            {
                if (!frame->m_barRepeat)
                {
                    return 1.0f;
                }

                int sx = 0;
                int sy = 0;
                renderer->GetDims(tex, sx, sy);
                float texWidth = static_cast<float>(sx);
                float texHeight = static_cast<float>(sy);
                renderer->AbsToRel(texWidth, texHeight);
                return span / (vertical ? texHeight : texWidth);
            };

            if ((drawFlags & 2) != 0)
            {
                if (frame->m_textures[0].IsValid())
                {
                    float const barY = rect.y0 + cornerSize;
                    float const barH = (rect.y0 + rect.height - cornerSize) - barY;

                    renderer->SetTexture(0, frame->m_textures[0], -1.0);

                    BoundsBase<float> bar;
                    bar.x0 = rect.x0;
                    bar.y0 = barY;
                    bar.width = barW;
                    bar.height = barH;
                    AddFlatAxialQuad(
                        di, bar, clr, 0.0, 0.0, 1.0, repeatAlong(frame->m_textures[0], barH, true));

                    bar.x0 = (rect.x0 + rect.width) - barW;
                    if (!frame->m_textures[2].IsValid())
                    {
                        // No right bar of its own, so the left one - still bound - is drawn with
                        // U running backwards.
                        AddFlatAxialQuad(
                            di, bar, clr, 1.0, 0.0, 0.0, repeatAlong(frame->m_textures[0], barH, true));
                    }
                    else
                    {
                        renderer->SetTexture(0, frame->m_textures[2], -1.0);
                        AddFlatAxialQuad(
                            di, bar, clr, 0.0, 0.0, 1.0, repeatAlong(frame->m_textures[2], barH, true));
                    }
                }

                if (frame->m_textures[1].IsValid())
                {
                    float const barX = rect.x0 + cornerSize;
                    float const barWidth = (rect.x0 + rect.width - cornerSize) - barX;

                    renderer->SetTexture(0, frame->m_textures[1], -1.0);

                    BoundsBase<float> bar;
                    bar.x0 = barX;
                    bar.y0 = rect.y0;
                    bar.width = barWidth;
                    bar.height = barW;
                    AddFlatAxialQuad(
                        di, bar, clr, 0.0, 0.0, repeatAlong(frame->m_textures[1], barWidth, false), 1.0);

                    bar.y0 = (rect.y0 + rect.height) - barW;
                    if (!frame->m_textures[3].IsValid())
                    {
                        // Likewise the top bar drawn with V running backwards.
                        AddFlatAxialQuad(
                            di, bar, clr, 0.0, 1.0, repeatAlong(frame->m_textures[1], barWidth, false), 0.0);
                    }
                    else
                    {
                        renderer->SetTexture(0, frame->m_textures[3], -1.0);
                        AddFlatAxialQuad(
                            di, bar, clr, 0.0, 0.0, repeatAlong(frame->m_textures[3], barWidth, false), 1.0);
                    }
                }
            }

            // The top-left corner is the one a skin must supply; the other three fall back to it
            // mirrored into place.
            if (drawComplexCorners && frame->m_textures[4].IsValid())
            {
                float const cs = static_cast<float>(cornerSize);

                BoundsBase<float> corner;
                corner.x0 = rect.x0;
                corner.y0 = rect.y0;
                corner.width = cs;
                corner.height = cs;

                renderer->SetTexture(0, frame->m_textures[4], -1.0);
                AddFlatAxialQuad(di, corner, clr, 0.0, 0.0, 1.0, 1.0);

                float tu0 = 0.0f;
                float tv0 = 0.0f;
                float tu1 = 1.0f;
                float tv1 = 1.0f;
                if (!frame->m_textures[7].IsValid())
                {
                    // Bottom right: the top-left corner turned through both axes.
                    renderer->SetTexture(0, frame->m_textures[4], -1.0);
                    tu0 = 1.0f;
                    tv0 = 1.0f;
                    tu1 = 0.0f;
                    tv1 = 0.0f;
                }
                else
                {
                    renderer->SetTexture(0, frame->m_textures[7], -1.0);
                }
                corner.x0 = (rect.x0 + rect.width) - cs;
                corner.y0 = (rect.y0 + rect.height) - cs;
                AddFlatAxialQuad(di, corner, clr, tu0, tv0, tu1, tv1);

                if (!frame->m_textures[6].IsValid())
                {
                    // Bottom left: flipped vertically.
                    renderer->SetTexture(0, frame->m_textures[4], -1.0);
                    tv0 = 1.0f;
                    tv1 = 0.0f;
                }
                else
                {
                    renderer->SetTexture(0, frame->m_textures[6], -1.0);
                    tv0 = 0.0f;
                    tv1 = 1.0f;
                }
                corner.x0 = rect.x0;
                corner.y0 = (rect.y0 + rect.height) - cs;
                AddFlatAxialQuad(di, corner, clr, 0.0, tv0, 1.0, tv1);

                if (!frame->m_textures[5].IsValid())
                {
                    // Top right: flipped horizontally.
                    renderer->SetTexture(0, frame->m_textures[4], -1.0);
                    tu0 = 1.0f;
                    tu1 = 0.0f;
                }
                else
                {
                    renderer->SetTexture(0, frame->m_textures[5], -1.0);
                    tu0 = 0.0f;
                    tu1 = 1.0f;
                }
                corner.x0 = (rect.x0 + rect.width) - cs;
                corner.y0 = rect.y0;
                AddFlatAxialQuad(di, corner, clr, tu0, 0.0, tu1, 1.0);
            }
        }

        renderer->PopBlend();
        renderer->PopZbState();
        renderer->SetAlphaTest(0);
    }

    void ui::GfxServer::AddText(
        DrawInfo const& di,
        PointBase<float> const& at,
        CStr const& text,
        int uiFont,
        TextWrapFlags tw,
        TextFormatFlags tf)
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

    void ui::GfxServer::AddImagedRect(
        DrawInfo const& di,
        BoundsBase<float> const& rect,
        unsigned clr,
        rend::TexHandle tex)
    {
        AddImagedRectGeneral(di, rect, clr, tex, 0.0, 0.0, 1.0, 1.0);
    }

    rend::TexHandle ui::GfxServer::GetGlyph(CStr const& name)
    {
        auto const it = m_glyphButtonTextures.find(name);
        if (it == m_glyphButtonTextures.end())
        {
            return rend::TexHandle{};
        }
        return it->second;
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
        if (it == m_controlSoundInfos.end())
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
        return m_btnHeight;
    }

    int ui::GfxServer::GetFontId(CStr const& name, float height, FontType type, FontParams params) const
    {
        return m_fontManager->GetFontId(name, height, type, params);
    }

    int ui::GfxServer::GetBtnWidth()
    {
        return m_btnWidth;
    }

    // A value with no alpha byte that fits in a byte is a palette index rather than a colour.
    unsigned ui::GfxServer::_ResolveColor(unsigned clr) const
    {
        if ((clr & 0xFF000000) == 0 && clr < 0xFF)
        {
            return m_colors[clr];
        }
        return clr;
    }

    void ui::GfxServer::AddFlatAxialQuad(DrawInfo const& di, BoundsBase<float> const& rect, unsigned clr)
    {
        // RVA 0x67B830 - an untextured fill: a white texture modulated by the colour, with the
        // depth buffer off so it always lands on top.
        auto* renderer = M3D_RENDERER;
        renderer->PushBlend();
        renderer->PushZbState(rend::ZB_DISABLE);
        renderer->SetWhiteTexture(0);
        renderer->DisableTextureStages(1);
        renderer->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        renderer->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);

        // Blending is only worth paying for when the colour actually has an alpha channel.
        renderer->SetBlend((_ResolveColor(clr) & 0xFF000000) != 0 ? rend::BM_ALPHA : rend::BM_NONE, 0);

        // NOTE: the raw colour is passed on, not the resolved one - the overload below resolves
        // it again for itself.
        AddFlatAxialQuad(di, rect, clr, 0.0, 0.0, 1.0, 1.0);

        renderer->PopZbState();
        renderer->PopBlend();
    }

    int ui::GfxServer::SetFont(int& fontId)
    {
        if (!m_fontManager->ValidateFontId(fontId))
        {
            return 0;
        }

        auto* fontById = m_fontManager->GetFontById(fontId);
        if (!fontById)
        {
            return 0;
        }

        m_curFont = fontById;
        return 1;
    }

    int ui::GfxServer::SetFont(Font* fnt)
    {
        m_curFont = fnt;
        return fnt != nullptr;
    }

    int ui::GfxServer::SetFont(CStr const& name, float height, FontType type, FontParams params)
    {
        int id = m_fontManager->GetFontId(name, height, type, params);
        if (id == -1)
        {
            return 0;
        }
        if (!m_fontManager->ValidateFontId(id))
        {
            return 0;
        }
        auto* font = m_fontManager->GetFontById(id);
        if (!font)
        {
            return 0;
        }
        m_curFont = font;
        return 1;
    }

    void ui::GfxServer::AddImagedRectGeneral(
        DrawInfo const& di,
        BoundsBase<float> const& rect,
        unsigned clr,
        rend::TexHandle tex,
        float u0,
        float v0,
        float u1,
        float v1)
    {
        M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_MODULATE);
        M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_MODULATE);
        M3D_RENDERER->PushBlend(rend::BM_ALPHA);
        M3D_RENDERER->SetAlphaTest(g_Kernel->GetEngineCfg().m_alphaTestInterface.GetI());
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
        M3D_RENDERER->SetTexture(0, tex, -1.0);
        auto sx = 0;
        auto sy = 0;
        M3D_RENDERER->GetDims(tex, sx, sy);
        PointBase<float> s;
        if (u1 >= 0.0)
            s.x = u1;
        else
            s.x = 0.0 - (u1 / (float)sx);
        if (v1 >= 0.0)
            s.y = v1;
        else
            s.y = 0.0 - (v1 / (float)sy);
        AddFlatAxialQuad(di, rect, clr, u0, v0, s.x, s.y);
        M3D_RENDERER->PopBlend();
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->SetAlphaTest(0);
    }

    void ui::GfxServer::ClearFonts()
    {
        m_fontManager->Clear();
        m_curFont = nullptr;
        m_hieroglyphicFontId = -1;
        m_curFontTexture.SetInvalid();
    }

    float ui::GfxServer::GetTabButtonMinWidth() const
    {
        return m_tabButtonMinWidth;
    }

    ui::Font* ui::GfxServer::GetFontById(unsigned id) const
    {
        return m_fontManager->GetFontById(id);
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
        for (xmlNode->GetFirstChild(soundNode, "SoundInfo"); !soundNode->IsEmpty();
             soundNode->GetNextSibling(soundNode, "SoundInfo"))
        {
            SafeStrAttrib(soundName, soundNode, "Name");
            if (soundName.empty())
            {
                M3D_LOG_WARN("GfxServer::LoadSoundsFromXml error - empty sound mame");
                continue;
            }
            if (m_controlSoundInfos.find(soundName) != m_controlSoundInfos.end())
            {
                M3D_LOG_WARN(
                    "GfxServer::LoadSoundsFromXml error - sound with name " + soundName +
                    " already exists. Second sound would be ignored");
                continue;
            }
            SafeStrAttrib(fileName, soundNode, "File");
            if (fileName.empty())
            {
                M3D_LOG_WARN("GfxServer::LoadSoundsFromXml error - sound file name is empty for sound " + soundName);
                continue;
            }
            auto const id = Application::g_pApp->m_sound->AddSound(
                fileName.c_str(), snd::SND_TYPE_2DSOUND, 1, 4, snd::SND_PRIORITY_HIGH);
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
        for (auto& tex : m_texTheme)
        {
            M3D_RENDERER->ReleaseTexture(tex);
        }
        for (auto* frame : m_frames)
        {
            delete frame;
        }
        for (auto* background : m_backgrounds)
        {
            delete background;
        }
        for (auto* pane : m_panesVector)
        {
            delete pane;
        }
        for (auto* scroll : m_scrollPanes)
        {
            delete scroll;
        }
        for (auto& [name, tex] : m_glyphButtonTextures)
        {
            M3D_RENDERER->ReleaseTexture(tex);
        }

        m_frames.clear();
        m_backgrounds.clear();
        m_panes.clear();
        m_panesVector.clear();
        m_scrollPanes.clear();
        m_glyphButtonTextures.clear();

        ClearSounds();
    }

    void ui::GfxServer::ClearSounds()
    {
        for (auto& [name, sound] : m_controlSoundInfos)
        {
            if (Application::g_pApp->m_sound && sound)
            {
                Application::g_pApp->m_sound->DeleteIdTableSound(sound->m_soundTableId);
            }
            delete sound;
        }
        m_controlSoundInfos.clear();
    }

    void ui::GfxServer::AddFlatAxialQuad(
        DrawInfo const& di,
        BoundsBase<float> const& rc,
        unsigned clr,
        float tu0,
        float tv0,
        float tu1,
        float tv1)
    {
        // RVA 0x67B510 - draws one axis-aligned quad of a widget, clipped to whatever part of
        // it is still visible. Cutting a quad down also has to cut its texture coordinates
        // down, or the artwork would slide as the clip moves.

        // Where the quad lands, in the window's own space.
        float const left = di.m_originalRect.x0 + rc.x0;
        float const top = di.m_originalRect.y0 + rc.y0;
        float const right = left + rc.width;
        float const bottom = top + rc.height;

        float const clipLeft = di.m_clippedRect.x0;
        float const clipTop = di.m_clippedRect.y0;
        float const clipRight = clipLeft + di.m_clippedRect.width;
        float const clipBottom = clipTop + di.m_clippedRect.height;

        float visibleX = 0.0f;
        float visibleY = 0.0f;
        float visibleWidth = 0.0f;
        float visibleHeight = 0.0f;

        // NOTE: nothing visible leaves the quad collapsed at the origin rather than skipping
        // it, so a fully clipped widget still submits a degenerate sprite at (0, 0).
        if (clipLeft <= right && left <= clipRight && clipTop <= bottom && top <= clipBottom)
        {
            visibleX = clipLeft > left ? clipLeft : left;
            visibleY = clipTop > top ? clipTop : top;
            visibleWidth = (right <= clipRight ? right : clipRight) - visibleX;
            visibleHeight = (bottom <= clipBottom ? bottom : clipBottom) - visibleY;
        }

        // NOTE: a quad is dropped only when both dimensions are zero, so one that is flat
        // in a single direction is still handed to the renderer.
        if (rc.width == 0.0f && rc.height == 0.0f)
        {
            return;
        }

        float const visibleRight = visibleX + visibleWidth;
        float const visibleBottom = visibleY + visibleHeight;

        // Each edge that was cut moves its texture coordinate in by the same fraction, so the
        // texture stays pinned to the untrimmed corners.
        float u0 = tu0;
        float u1 = tu1;
        float v0 = tv0;
        float v1 = tv1;

        if (visibleX > left)
        {
            u0 = tu0 + (tu1 - tu0) / rc.width * (visibleX - left);
        }
        if (right > visibleRight)
        {
            u1 = tu1 + (tu0 - tu1) / rc.width * (right - visibleRight);
        }
        if (visibleY > top)
        {
            v0 = tv0 + (tv1 - tv0) / rc.height * (visibleY - top);
        }
        if (bottom > visibleBottom)
        {
            v1 = tv1 + (tv0 - tv1) / rc.height * (bottom - visibleBottom);
        }

        M3D_APP->PutSprite2Rel(
            visibleX, visibleY, u0, v0, visibleRight, visibleBottom, u1, v1, _ResolveColor(clr));
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
}  // namespace m3d

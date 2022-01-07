#include <stdexcept>
#include <ui/ui_srv.h>
#include <ui/frame.h>

namespace m3d
{
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
        throw std::logic_error("Not implemented");
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

    int ui::GfxServer::SetSchema(CStr const&)
    {
        throw std::logic_error("Not implemented");
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

    int ui::GfxServer::LoadSoundsFromXml(cmn::XmlFile*, cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::ReleaseSchema()
    {
        throw std::logic_error("Not implemented");
    }

    void ui::GfxServer::ClearSounds()
    {
        throw std::logic_error("Not implemented");
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

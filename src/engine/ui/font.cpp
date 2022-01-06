#include <stdexcept>
#include <ui/font.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace ui
    {
        int Font::SaveToXml()
        {
            throw std::logic_error("Not implemented");
        }

        int Font::SaveToTga()
        {
            throw std::logic_error("Not implemented");
        }

        CStr Font::GetFileNameForReadableInfo() const
        {
            throw std::logic_error("Not implemented");
        }

        int Font::CreateFromPrototype(Font*, float)
        {
            throw std::logic_error("Not implemented");
        }

        FontType Font::GetType() const
        {
            throw std::logic_error("Not implemented");
        }

        int Font::Save()
        {
            throw std::logic_error("Not implemented");
        }

        CStr Font::GetBaseFileName() const
        {
            throw std::logic_error("Not implemented");
        }

        float Font::GetScale() const
        {
            throw std::logic_error("Not implemented");
        }

        int Font::CreateFromXmlNode(cmn::XmlFile*, cmn::XmlNode const*)
        {
            throw std::logic_error("Not implemented");
        }

        int Font::CreateFromTtf(CStr const&, float, unsigned, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        CStr Font::CreateNameFull(CStr const&, unsigned, unsigned) const
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Font::CalcGlyphSz(unsigned char) const
        {
            throw std::logic_error("Not implemented");
        }

        unsigned Font::GetStyle() const
        {
            throw std::logic_error("Not implemented");
        }

        int Font::SaveGrid()
        {
            throw std::logic_error("Not implemented");
        }

        CStr Font::GetFileNameForTexture(int) const
        {
            throw std::logic_error("Not implemented");
        }

        std::vector<rend::TexHandle> const& Font::GetTextures() const
        {
            throw std::logic_error("Not implemented");
        }

        Font::~Font()
        {
            throw std::logic_error("Not implemented");
        }

        void Font::Clear()
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> Font::GetGlyphSz(unsigned char) const
        {
            throw std::logic_error("Not implemented");
        }

        Font::TextureCoordinates Font::GetTexCoord(unsigned char) const
        {
            throw std::logic_error("Not implemented");
        }

        CStr const& Font::GetName() const
        {
            throw std::logic_error("Not implemented");
        }

        float Font::CalcCharWidthAdvanced(unsigned char) const
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<int> Font::GetTexSz() const
        {
            throw std::logic_error("Not implemented");
        }

        void Font::PrecalcSymbolsSizes()
        {
            throw std::logic_error("Not implemented");
        }

        Font::Font()
        {
            throw std::logic_error("Not implemented");
        }

        CStr Font::GetFileNameForGrid(int) const
        {
            throw std::logic_error("Not implemented");
        }

        float Font::GetCharWidthAdvanced(unsigned char) const
        {
            throw std::logic_error("Not implemented");
        }

        Font::FontABC Font::GetAbcWidth(unsigned char) const
        {
            throw std::logic_error("Not implemented");
        }

        bool FontManager::NeedCharSetWChars(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::ValidateFontId(int&)
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::Init()
        {
            throw std::logic_error("Not implemented");
        }

        void FontManager::Clear()
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::GetFontId(CStr const&, float, FontType, FontParams)
        {
            throw std::logic_error("Not implemented");
        }

        FontManager::~FontManager()
        {
            throw std::logic_error("Not implemented");
        }

        FontManager::FontManager()
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::ReadFontsFromXml()
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::GetNumFonts() const
        {
            throw std::logic_error("Not implemented");
        }

        void FontManager::RearrangeFonts(int, int)
        {
            throw std::logic_error("Not implemented");
        }

        TCharDictionary const& FontManager::GetTCharDictionary()
        {
            throw std::logic_error("Not implemented");
        }

        unsigned FontManager::GetCodePageByCharset(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Font* FontManager::GetFontById(int) const
        {
            throw std::logic_error("Not implemented");
        }

        unsigned FontManager::GetCharsetByCodePage(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        float FontManager::GetScaledHeight(float) const
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::FindMatchFont(CStr const&, float, bool, bool)
        {
            throw std::logic_error("Not implemented");
        }
    }
}

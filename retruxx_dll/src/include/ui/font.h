#pragma once
#include <vector>
#include <core/stringm3d.h>
#include <math/point2d.h>

namespace m3d
{
    namespace rend
    {
        class TexHandle;
    }

    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }

    namespace ui
    {
        class TCharDictionary
        {
        public:
            int GetNumOfTChars() const;
            TCharDictionary();
            CStr const& GetSrcFile() const;
            CStr const& GetCharSetName() const;
            CStr const& GetCodePageName() const;
            bool IsTCharPresent(unsigned char) const;
            ~TCharDictionary();
            unsigned char GetTCharAtPos(int) const;
            CStr const& GetTChars() const;
            int CreateFromXml(CStr const&);

        private:
            void InitDefault();

        private:
            CStr m_tChars;
            CStr m_charSetName;
            CStr m_codePageName;
            CStr m_srcFile;
        };

        enum FontType
        {
            FONT_TYPE_WINDOWS = 0x0,
            FONT_TYPE_SELFMAKING = 0x1,
        };

        enum FontMatch
        {
            FONT_MATCH_STRICT_FULL = 0x0,
            FONT_MATCH_STRICT_NAME = 0x1,
            FONT_MATCH_NOSTRICT = 0x2,
        };

        union FontParams
        {
            FontMatch match;
            struct
            {
                unsigned int style;
                unsigned int codePage;
            } ttfParams;
            //$7A16E786D20B30359EF3D15CD24517DA ttfParams;
        };


        class Font
        {
        public:
            class FontABC
            {
            public:
                FontABC();

            public:
                float m_A = 0.0;
                float m_B = 0.0;
                float m_C = 0.0;
            };

            class TextureCoordinates
            {
            public:
                TextureCoordinates();

            public:
                int m_texId = -1;
                float m_coordinates[4] = {0};
            };

            class SymbolInfo
            {
            public:
                SymbolInfo();

            public:
                char m_symbol = 0;
                FontABC m_abc;
                TextureCoordinates m_tcs;
                PointBase<float> m_precalcedGlyphSz{0.0, 0.0};
                float m_precalcedABCWidth = 0.0;
            };

        public:
            int SaveToXml();
            int SaveToTga();
            CStr GetFileNameForReadableInfo() const;
            int CreateFromPrototype(Font*, float);
            FontType GetType() const;
            int Save();
            CStr GetBaseFileName() const;
            float GetScale() const;
            int CreateFromXmlNode(cmn::XmlFile*, cmn::XmlNode const*);
            int CreateFromTtf(CStr const&, float, unsigned int, unsigned int);
            CStr CreateNameFull(CStr const&, unsigned int, unsigned int) const;
            PointBase<float> CalcGlyphSz(unsigned char) const;
            unsigned int GetStyle() const;
            int SaveGrid();
            CStr GetFileNameForTexture(int) const;
            std::vector<rend::TexHandle> const& GetTextures() const;
            ~Font();
            void Clear();
            PointBase<float> GetGlyphSz(unsigned char) const;
            TextureCoordinates GetTexCoord(unsigned char) const;
            CStr const& GetName() const;
            float CalcCharWidthAdvanced(unsigned char) const;
            class PointBase<int> GetTexSz() const;
            void PrecalcSymbolsSizes();
            Font();
            CStr GetFileNameForGrid(int) const;
            float GetCharWidthAdvanced(unsigned char) const;
            FontABC GetAbcWidth(unsigned char) const;

        public:
            CStr m_nameShort;
            CStr m_nameFull;
            float m_heightScaled = 0.0;
            float m_heightUnscaled = 0.0;
            float m_scaleTex = 0.0;
            FontType m_type = FONT_TYPE_SELFMAKING;
            unsigned int m_style = 0;
            std::vector<rend::TexHandle> m_textures;
            std::vector<SymbolInfo*> m_symbols;
        };

        class FontManager
        {
        public:
            static bool __fastcall NeedCharSetWChars(unsigned int);
            int ValidateFontId(int&);
            int Init();
            void Clear();
            int GetFontId(CStr const&, float, FontType, union FontParams);
            ~FontManager();
            int ReadFontsFromXml();
            int GetNumFonts() const;
            void RearrangeFonts(int, int);
            static TCharDictionary const& GetTCharDictionary();
            static unsigned int GetCodePageByCharset(unsigned int);
            Font* GetFontById(int) const;
            static unsigned int GetCharsetByCodePage(unsigned int);

        private:
            float GetScaledHeight(float) const;
            int FindMatchFont(CStr const&, float, bool, bool);

        private:
            static inline TCharDictionary m_tCharDictionary;
            std::vector<Font*> m_fonts;
        };
    }
}

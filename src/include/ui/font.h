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
            //$7A16E786D20B30359EF3D15CD24517DA ttfParams;
        };


        class Font
        {
        public:
            class FontABC
            {
            public:
                FontABC();

            private:
                float m_A;
                float m_B;
                float m_C;
            };

            class TextureCoordinates
            {
            public:
                TextureCoordinates();
            protected:
            private:
                int m_texId;
                float m_coordinates[4];
            };

            class SymbolInfo
            {
            public:
                SymbolInfo();
            protected:
            private:
                char m_symbol;
                FontABC m_abc;
                TextureCoordinates m_tcs;
                PointBase<float> m_precalcedGlyphSz;
                float m_precalcedABCWidth;
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

        private:
            //std::_Uninit_copy<SymbolInfo*, SymbolInfo*>(SymbolInfo**, SymbolInfo**, SymbolInfo**, std::allocator<SymbolInfo*>&, std::_Scalar_ptr_iterator_tag);
            //std::fill<SymbolInfo**, SymbolInfo*>(SymbolInfo**, SymbolInfo**, SymbolInfo* const&);
            //std::allocator<SymbolInfo*>::allocator<SymbolInfo*>();
            //std::allocator<SymbolInfo*>::allocator<SymbolInfo*>();
            //std::copy_backward<SymbolInfo**, SymbolInfo**>(SymbolInfo**, SymbolInfo**, SymbolInfo**);
            //std::_Allocate<SymbolInfo*>(uint, SymbolInfo**);
            //std::_Ptr_cat<SymbolInfo>(SymbolInfo**, SymbolInfo**);
            //std::_Copy_backward_opt<SymbolInfo**, SymbolInfo**>(SymbolInfo**, SymbolInfo**, SymbolInfo**, std::_Scalar_ptr_iterator_tag);
            //std::copy<SymbolInfo**, SymbolInfo**>(SymbolInfo**, SymbolInfo**, SymbolInfo**);
            //std::_Copy_opt<SymbolInfo**, SymbolInfo**>(SymbolInfo**, SymbolInfo**, SymbolInfo**, std::_Scalar_ptr_iterator_tag);
            CStr m_nameShort;
            CStr m_nameFull;
            float m_heightScaled;
            float m_heightUnscaled;
            float m_scaleTex;
            FontType m_type;
            unsigned int m_style;
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
            FontManager();
            int ReadFontsFromXml();
            int GetNumFonts() const;
            void RearrangeFonts(int, int);
            static TCharDictionary const& __fastcall GetTCharDictionary();
            static unsigned int __fastcall GetCodePageByCharset(unsigned int);
            Font* GetFontById(int) const;
            static unsigned int __fastcall GetCharsetByCodePage(unsigned int);

        private:
            float GetScaledHeight(float) const;
            int FindMatchFont(CStr const&, float, bool, bool);

        private:
            std::vector<Font*> m_fonts;
        };
    }
}

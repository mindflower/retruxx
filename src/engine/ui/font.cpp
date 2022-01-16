#include <config.h>
#include <m3dapp.h>
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <ui/font.h>
#include <renderer/i_renderer.h>
#include <server/utils.h>

namespace m3d
{
    namespace ui
    {
        int TCharDictionary::GetNumOfTChars() const
        {
            return m_tChars.length();
        }

        TCharDictionary::TCharDictionary()
        {
            InitDefault();
        }

        CStr const& TCharDictionary::GetSrcFile() const
        {
            throw std::logic_error("Not implemented");
        }

        CStr const& TCharDictionary::GetCharSetName() const
        {
            throw std::logic_error("Not implemented");
        }

        CStr const& TCharDictionary::GetCodePageName() const
        {
            throw std::logic_error("Not implemented");
        }

        bool TCharDictionary::IsTCharPresent(unsigned char tChar) const
        {
            assert(!m_tChars.empty());
            return m_tChars.find(tChar) != CStr_npos;
        }

        TCharDictionary::~TCharDictionary()
        {
        }

        unsigned char TCharDictionary::GetTCharAtPos(int pos) const
        {
            if (pos < 0 || pos >=m_tChars.length())
            {
                return 0;
            }
            return m_tChars[pos];
        }

        CStr const& TCharDictionary::GetTChars() const
        {
            return m_tChars;
        }

        int TCharDictionary::CreateFromXml(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        void TCharDictionary::InitDefault()
        {
            m_charSetName = "ANSI_CHARSET";
            m_codePageName = "CP_ACP";
            m_tChars = "";
            for (int i = 32; i<256;++i)
            {
                m_tChars += CStr(static_cast<char>(i), 1);
            }
        }

        Font::FontABC::FontABC()
        {
        }

        Font::TextureCoordinates::TextureCoordinates()
        {
        }

        Font::SymbolInfo::SymbolInfo()
        {
        }

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

        int Font::CreateFromPrototype(Font* prototype, float heightUnscaled)
        {
            if (!prototype || heightUnscaled <= 0.0)
            {
                return 0;
            }
            if (prototype->m_nameShort.empty() || prototype->m_nameFull.empty() || prototype->m_heightUnscaled <= 0.0)
            {
                return 0;
            }
            Clear();
            m_nameShort = prototype->m_nameShort;
            m_nameFull = prototype->m_nameFull;
            m_heightUnscaled = heightUnscaled;
            auto vieport = Application::g_pApp->m_renderer->GetViewport();
            m_heightScaled = (vieport.m_width * heightUnscaled) * 0.0009765625;
            m_scaleTex = (prototype->m_scaleTex / prototype->m_heightScaled) * m_heightScaled;
            m_textures = prototype->m_textures;
            for (auto& tex : m_textures)
            {
                if (tex.IsValid())
                {
                    Application::g_pApp->m_renderer->ReferenceTexture(tex);
                }
            }
            for (int i = 0; i<FontManager::GetTCharDictionary().GetNumOfTChars(); ++i)
            {
                auto idx = FontManager::GetTCharDictionary().GetTCharAtPos(i);
                delete m_symbols[idx];
                m_symbols[idx] = nullptr;
                if (prototype->m_symbols[idx])
                {
                    m_symbols[idx] = new SymbolInfo;
                    *m_symbols[idx] = *prototype->m_symbols[idx];
                    m_symbols[idx]->m_abc.m_A = (m_heightScaled / prototype->m_heightScaled) * prototype->m_symbols[idx]->m_abc.m_A;
                    m_symbols[idx]->m_abc.m_B = (m_heightScaled / prototype->m_heightScaled) * prototype->m_symbols[idx]->m_abc.m_B;
                    m_symbols[idx]->m_abc.m_C = (m_heightScaled / prototype->m_heightScaled) * prototype->m_symbols[idx]->m_abc.m_C;
                }
            }
            PrecalcSymbolsSizes();
            return 1;
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

        int Font::CreateFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode const* xmlNode)
        {
            Clear();
            if (!xmlNode)
            {
                M3D_LOG_ERR("Font unknown: fail to create from xml - invalid node");
                return 0;
            }
            m_nameShort = xmlNode->GetAttribute("name");
            if (m_nameShort.empty())
            {
                M3D_LOG_ERR("Font unknown: fail to create from xml - name was not specified");
                return 0;
            }
            m_nameFull = CreateNameFull(m_nameShort, 0, 0);
            SafeFloatAttrib(m_heightScaled, xmlNode, "height");
            if (m_heightScaled <= 0.0)
            {
                M3D_LOG_ERR("Font " + m_nameShort + ": fail to create from xml - height is not valid");
                return 0;
            }
            SafeFloatAttrib(m_heightUnscaled, xmlNode, "heightVirtual");
            std::vector<CStr> filesVector;
            ai::StrToStringVector(xmlNode->GetAttribute("file"), filesVector);
            if (filesVector.empty())
            {
                M3D_LOG_ERR("Font " + m_nameShort + ": fail to create from xml - texture files were not specified");
                return 0;
            }
            for (auto const& file : filesVector)
            {
                auto tex = Application::g_pApp->m_renderer->AddTexture(file, 4);
                if (!tex.IsValid())
                {
                    M3D_LOG_ERR("Font " + m_nameShort + ": fail to create from xml - invalid texture");
                    return 0;
                }
                Application::g_pApp->m_renderer->SetTextureParameter(tex, rend::TM_TEX_FILTER, 2);
                m_textures.push_back(tex);
            }
            ref_ptr node = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            xmlNode->GetFirstChild_(node, "Symbol");
            while (!node->IsEmpty())
            {
                CStr symbolValue;
                SafeStrAttrib(symbolValue, node, "value");
                if (symbolValue.empty())
                {
                    M3D_LOG_ERR("Font::CreateFromXmlNode error: invalid symbol name for font " + m_nameShort);
                    return 0;
                }
                if (!FontManager::GetTCharDictionary().IsTCharPresent(symbolValue[0]))
                {
                    M3D_LOG_WARN("Font::CreateFromXmlNode warning: symbol " + symbolValue + " not found in dictionary");
                }
                auto symbolInfo = new SymbolInfo;
                symbolInfo->m_symbol = symbolValue[0];

                CStr strAbc;
                SafeStrAttrib(strAbc, node, "abc");
                std::vector<float> abc;
                ai::StrToFloatVector(strAbc, abc);
                if (abc.size() != 3)
                {
                    M3D_LOG_ERR("Font::CreateFromXmlNode error: invalid size of ABC structure for symbol " + symbolValue + " for font " + m_nameShort);
                    return 0;
                }
                symbolInfo->m_abc.m_A = abc[0];
                symbolInfo->m_abc.m_B = abc[1];
                symbolInfo->m_abc.m_C = abc[2];

                CStr strTcs;
                SafeStrAttrib(strTcs, node, "tcs");
                std::vector<float> tcs;
                ai::StrToFloatVector(strTcs, tcs);
                if (tcs.size() != 5)
                {
                    M3D_LOG_ERR("Font::CreateFromXmlNode error: invalid size of TCS structure for symbol " + symbolValue + " for font " + m_nameShort);
                    return 0;
                }
                symbolInfo->m_tcs.m_texId = static_cast<int>(tcs[0]);
                symbolInfo->m_tcs.m_coordinates[0] = tcs[1];
                symbolInfo->m_tcs.m_coordinates[1] = tcs[2];
                symbolInfo->m_tcs.m_coordinates[2] = tcs[3];
                symbolInfo->m_tcs.m_coordinates[3] = tcs[4];

                auto symbolIdx = static_cast<unsigned char>(symbolValue[0]);
                delete m_symbols[symbolIdx];
                m_symbols[symbolIdx] = symbolInfo;

                node->GetNextSibling_(node, "Symbol");
            }
            m_type = FONT_TYPE_SELFMAKING;
            m_scaleTex = 1.0;
            PrecalcSymbolsSizes();
            return 1;
        }

        int Font::CreateFromTtf(CStr const& name, float heightUnscaled, unsigned style,  unsigned charset)
        {
            //TODO: implement this!!!!!!!!!!!
            return 0;
            //Clear();
            //m_type = FONT_TYPE_WINDOWS;
            //m_style = style;
            //auto viewport = Application::g_pApp->m_renderer->GetViewport();
            //m_nameShort = name;
            //m_nameFull = CreateNameFull(name, style, FontManager::GetCodePageByCharset(charset));
            //auto y = (viewport.m_width * heightUnscaled) * 0.0009765625;
            //PointBase<int> texSz;
            //if (FontManager::NeedCharSetWChars(charset))
            //{
            //    texSz.x = 512;
            //    texSz.y = 512;
            //}
            //else if (y <= 19.0)
            //{
            //    if (y <= 13.0)
            //    {
            //        texSz.x = 256;
            //        if (y <= 8.0)
            //        {
            //            texSz.y = 128;
            //        }
            //        else
            //        {
            //            texSz.y = 256;
            //        }
            //    }
            //    else
            //    {
            //        texSz.x = 512;
            //        texSz.y = 256;
            //    }
            //}
            //else
            //{
            //    texSz.y = 512;
            //    texSz.x = 512;
            //}
            //m_heightScaled = y;
            //m_heightUnscaled = heightUnscaled;
            //m_scaleTex = 1.0;
            //auto hDc = ::CreateCompatibleDC(NULL);
            //::SetMapMode(hDc, 1);
            ////TODO: check this
            //auto height = static_cast<int>((::GetDeviceCaps(hDc, 90)* y) * 0.013888889);
            //auto hFont = CreateFontA(
            //    height,
            //    0,
            //    0,
            //    0,
            //    (style & 1) != 0 ? 700 : 400,
            //    (style >> 1) & 1,
            //    0,
            //    0,
            //    charset,
            //    0,
            //    0,
            //    0,
            //    2,
            //    name.c_str()
            //);
            //if (hFont)
            //{
            //    ::SelectObject(hDc, hFont);
            //    ::SetTextColor(hDc, 0xFFFFFF);
            //    ::SetBkColor(hDc, 0);
            //    BITMAPINFO bitmapInfo{};
            //    bitmapInfo.bmiHeader.biSize = 40;
            //    bitmapInfo.bmiHeader.biWidth = texSz.x;
            //    bitmapInfo.bmiHeader.biHeight = -texSz.y;
            //    bitmapInfo.bmiHeader.biPlanes = 1;
            //    bitmapInfo.bmiHeader.biBitCount = 24;
            //
            //    unsigned x = 0;
            //    for (int i = 0; i < FontManager::GetTCharDictionary().GetNumOfTChars(); ++i)
            //    {
            //        unsigned bits = 0;
            //        auto hBmp = ::CreateDIBSection(hDc, &bitmapInfo, 0, (void**)&bits, NULL, 0);
            //        ::SelectObject(hDc, hBmp);
            //
            //        SIZE ssize{};
            //        ::GetTextExtentPoint32A(hDc, FontManager::GetTCharDictionary().GetTChars().c_str() + i, 1, &ssize);
            //
            //        ABC abc{};
            //        ::GetCharABCWidthsA(hDc, FontManager::GetTCharDictionary().GetTCharAtPos(i), FontManager::GetTCharDictionary().GetTCharAtPos(i), &abc);
            //
            //        auto width = abc.abcA + abc.abcB + 2;
            //    }
            //}
            throw std::logic_error("Not implemented");
        }

        CStr Font::CreateNameFull(CStr const& name, unsigned style, unsigned codePage) const
        {
            if (m_type == FONT_TYPE_SELFMAKING)
            {
                return m_nameShort;
            }
            //TODO: check this
            return name +"_" + CStr(style) + "_" + CStr(codePage);
        }

        PointBase<float> Font::CalcGlyphSz(unsigned char c) const
        {
            //TODO: check this
            if (m_symbols[c])
            {
                PointBase<float> res;
                auto texSize = GetTexSz();
                res.x = ((m_symbols[c]->m_tcs.m_coordinates[2] - m_symbols[c]->m_tcs.m_coordinates[0]) * texSize.x) * m_scaleTex;
                res.y = ((m_symbols[c]->m_tcs.m_coordinates[3] - m_symbols[c]->m_tcs.m_coordinates[1]) * texSize.y) * m_scaleTex;
                return res;
            }
            return { 0.0, 0.0 };
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
            Clear();
            for (auto sym: m_symbols)
            {
                delete sym;
            }
        }

        void Font::Clear()
        {
            for (auto& texture : m_textures)
            {
                if (texture.IsValid())
                {
                    Application::g_pApp->m_renderer->ReleaseTexture(texture);
                }
            }
            m_textures.clear();
            auto& tChars = FontManager::GetTCharDictionary().GetTChars();
            if (!tChars.empty())
            {
                for (int i=0; i < tChars.length(); ++i)
                {
                    auto const idx = static_cast<unsigned char>(tChars[i]);
                    delete m_symbols[idx];
                    m_symbols[idx] = nullptr;
                }
            }
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
            if (!m_textures.empty())
            {
                PointBase<int> res;
                Application::g_pApp->m_renderer->GetDims(m_textures.front(), res.x, res.y);
                return res;
            }
            return { 0, 0 };
        }

        void Font::PrecalcSymbolsSizes()
        {
            //TODO: check this
            for (int i = 0; i< FontManager::GetTCharDictionary().GetNumOfTChars(); ++i)
            {
                auto sym = FontManager::GetTCharDictionary().GetTCharAtPos(i);
                if (m_symbols[sym])
                {
                    m_symbols[sym]->m_precalcedGlyphSz = CalcGlyphSz(sym);
                    m_symbols[sym]->m_precalcedABCWidth = m_symbols[sym]->m_abc.m_A + m_symbols[sym]->m_abc.m_B + m_symbols[sym]->m_abc.m_C;
                }
            }
        }

        Font::Font()
        {
            m_symbols.resize(0x100, nullptr);
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

        TCharDictionary FontManager::m_tCharDictionary;

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
            return ReadFontsFromXml() & 1;
        }

        void FontManager::Clear()
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::GetFontId(CStr const& name, float heightUnscaled, FontType type, FontParams params)
        {
            if (name.empty() || heightUnscaled <= 0.0)
            {
                return -1;
            }
            CStr nameToFind;
            if (type == FONT_TYPE_SELFMAKING)
            {
                nameToFind = name;
            }
            else
            {
                nameToFind = name + "_" + CStr(params.ttfParams.style) + "_" + CStr(params.ttfParams.codePage);
            }
            auto matchFont = FindMatchFont(nameToFind, heightUnscaled, true, true);
            if (matchFont == -1)
            {
                if (type == FONT_TYPE_SELFMAKING && params.ttfParams.style)
                {
                    matchFont = FindMatchFont(name, heightUnscaled, true, false);
                    if (matchFont == -1)
                    {
                        matchFont = FindMatchFont(name, heightUnscaled, false, false);
                    }
                }
                else
                {
                    auto charset = GetCharsetByCodePage(params.ttfParams.codePage);
                    if (charset == 1 && params.ttfParams.codePage)
                    {
                        return -1;
                    }
                    auto font = new Font;
                    if (font->CreateFromTtf(name, heightUnscaled, params.ttfParams.style, params.ttfParams.codePage))
                    {
                        m_fonts.push_back(font);
                        matchFont = m_fonts.size() - 1;
                    }
                    else
                    {
                        delete font;
                    }
                }
            }
            return matchFont;
        }

        FontManager::~FontManager()
        {
            Clear();
        }

        int FontManager::ReadFontsFromXml()
        {
            CStr err;
            ref_ptr file = ReadXmlFile(g_Kernel->GetEngineCfg().m_ui_pathToFonts.GetS(), &err);
            if (file)
            {
                ref_ptr fontsNode = file->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                file->GetFirstChild_(fontsNode, "Fonts");
                if (fontsNode->IsEmpty())
                {
                    M3D_LOG_ERR("FontManager: error while loading fonts");
                    return 0;
                }
                ref_ptr itemNode = file->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
                fontsNode->GetFirstChild_(itemNode, "Item");
                while(!itemNode->IsEmpty())
                {
                    auto font = new Font;
                    if (font->CreateFromXmlNode(file, itemNode))
                    {
                        m_fonts.push_back(font);
                    }
                    else
                    {
                        delete font;
                    }
                    itemNode->GetNextSibling_(itemNode, "Item");
                }
                M3D_LOG_INFO("FontManager: fonts are loaded successfully");
                return 1;
            }
            M3D_LOG_ERR("cannot load " + CStr(g_Kernel->GetEngineCfg().m_ui_pathToFonts.GetS()) + " err: " + err);
            return 0;
        }

        int FontManager::GetNumFonts() const
        {
            throw std::logic_error("Not implemented");
        }

        void FontManager::RearrangeFonts(int id1, int id2)
        {
            //TODO: check this
            if (id1>=0 && id1<m_fonts.size() && id1 !=id2)
            {
                if (id1 <=id2)
                {
                    do
                    {
                        auto font = new Font;
                        font->CreateFromPrototype(m_fonts[id1], m_fonts[id1]->m_heightUnscaled);
                        m_fonts.push_back(font);
                    } while (m_fonts.size() <= id2);
                    auto temp = m_fonts.back();
                    m_fonts.back() = m_fonts[id2];
                    m_fonts[id2] = temp;
                }
                else
                {
                    auto temp = m_fonts[id1];
                    m_fonts[id1] = m_fonts[id2];
                    m_fonts[id2] = temp;
                }
            }
        }

        TCharDictionary const& FontManager::GetTCharDictionary()
        {
            return m_tCharDictionary;
        }

        unsigned FontManager::GetCodePageByCharset(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Font* FontManager::GetFontById(int id) const
        {
            if (id >=0 && m_fonts.size() > id)
            {
                return m_fonts[id];
            }
            return 0;
        }

        unsigned FontManager::GetCharsetByCodePage(unsigned codePage)
        {
            CHARSETINFO info{};
            if (::TranslateCharsetInfo((DWORD*)codePage, &info, 2))
            {
                return info.ciCharset;
            }
            M3D_LOG_INFO("FontManager::GetCharsetByCodePage error: cannot find charset for code page " + CStr(codePage));
            return 1;
        }

        float FontManager::GetScaledHeight(float) const
        {
            throw std::logic_error("Not implemented");
        }

        int FontManager::FindMatchFont(CStr const& name, float heightUnscaled, bool strictName, bool strictHeight)
        {
            if (name.empty() || heightUnscaled <= 0.0)
            {
                return -1;
            }
            auto fontId = -1;
            Font* font = nullptr;
            auto viewport = Application::g_pApp->m_renderer->GetViewport();
            //TODO: check this and recreate logic with foundProtoEqualUSize
            auto heightScaled = (viewport.m_width * heightUnscaled) * 0.0009765625;
            for (int i = 0; i<m_fonts.size();++i)
            {
                if (!m_fonts[i] || m_fonts[i]->m_nameFull != name && strictName)
                {
                    continue;
                }
                auto heightThreshold = fabs(m_fonts[i]->m_heightScaled - heightScaled);
                if (heightThreshold > 0.001 && strictHeight)
                {
                    continue;
                }
                if (heightThreshold > 0.001)
                {
                    font = m_fonts[i];
                }
                else
                {
                    return i;
                }
            }
            if (!font)
            {
                return fontId;
            }
            auto newFont = new Font;
            if (newFont->CreateFromPrototype(font, heightUnscaled))
            {
                m_fonts.push_back(newFont);
                return m_fonts.size() - 1;
            }
            delete newFont;
            return fontId;
        }
    }
}

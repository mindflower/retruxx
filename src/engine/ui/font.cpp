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

        unsigned char TCharDictionary::GetTCharAtPos(int) const
        {
            throw std::logic_error("Not implemented");
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

                node->GetNextSibling_(node, nullptr);
            }
            m_type = FONT_TYPE_SELFMAKING;
            m_scaleTex = 1.0;
            PrecalcSymbolsSizes();
            return 1;
        }

        int Font::CreateFromTtf(CStr const&, float, unsigned, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        CStr Font::CreateNameFull(CStr const& name, unsigned style, unsigned codePage) const
        {
            if (m_type == FONT_TYPE_SELFMAKING)
            {
                return m_nameShort;
            }
            //TODO: check this
            return name + CStr(style) + CStr(codePage);
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
            throw std::logic_error("Not implemented");
        }

        void Font::PrecalcSymbolsSizes()
        {
            throw std::logic_error("Not implemented");
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

        int FontManager::GetFontId(CStr const&, float, FontType, FontParams)
        {
            throw std::logic_error("Not implemented");
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

        void FontManager::RearrangeFonts(int, int)
        {
            throw std::logic_error("Not implemented");
        }

        TCharDictionary const& FontManager::GetTCharDictionary()
        {
            return m_tCharDictionary;
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

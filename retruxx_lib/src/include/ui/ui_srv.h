#pragma once
#include "font.h"
#include "ui.h"
#include <core/containers.h>
#include <math/bounds2d.h>
#include <math/point2d.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace ui
    {
        class TabButtonInfo;
        class Frame;
        class BackGround;
        class Pane;
        class ScrollPane;
        class Wnd;

        enum ThemeTexture
        {
            TEX_BTN_C_0 = 0x0,
            TEX_BTN_C_1 = 0x1,
            TEX_BTN_R_0 = 0x2,
            TEX_BTN_R_1 = 0x3,
            TEX_BTN_L_0 = 0x4,
            TEX_BTN_L_1 = 0x5,
            TEX_CHK_UNCHECKED = 0x6,
            TEX_CHK_CHECKED = 0x7,
            TEX_SLIDER_BODY = 0x8,
            TEX_SLIDER_THUMB = 0x9,
            TEX_SLIDER_LEFT = 0xA,
            TEX_SLIDER_RIGHT = 0xB,
            TEX_LINE_HORZ = 0xC,
            TEX_LINE_VERT = 0xD,
            TEX_NUM_TEXS = 0xE,
        };

        class FormattedLine
        {
        public:
            FormattedLine();
            FormattedLine(FormattedLine const&);

        public:
            unsigned int m_color = 0;
            CStr m_text;
            PointBase<float> m_origin = {0.0, 0.0};
            m3d::TextFormatFlags m_format = TF_LEFT;
            bool m_isHieroglyphic = false;
        };

        class GfxServer
        {
            friend class WndStation;
        public:
            class SoundInfo
            {
            public:
                SoundInfo();

            public:
                CStr m_soundName;
                CStr m_soundFile;
                int m_soundTableId = -1;
                bool m_bSoundLooped = false;
            };

        public:
            int GetSliderHeight();
            void SetTexture(rend::TexHandle);
            rend::TexHandle GetTexture(ThemeTexture);
            PointBase<float> MeasureText(CStr const&, int, TextWrapFlags, float);
            float GetTabButtonHeight() const;
            void SetColor(unsigned int, unsigned int);
            unsigned int GetColor(unsigned int) const;
            void AddLineFlatAxialPane(DrawInfo const&, BoundsBase<float> const&, unsigned int, int);
            void AddRect(DrawInfo const&, BoundsBase<float> const&, unsigned int);
            int ReadFrames();
            Font* GetCurFont() const;
            ScrollPane* GetScrollPane(CStr const&);
            float GetSliderNotchWidthRatio();
            ~GfxServer();
            float GetGlyphHeight();
            float GetTabButtonMaxWidth() const;
            void AddTabWndPaneNormal(DrawInfo const&, BoundsBase<float> const&, unsigned int, TabButtonInfo const&, retruxx::vector<BoundsBase<float>> const&, int, int, CStr const&, PaneFlagBg);
            void AddChkButtonFlatAxialPane(DrawInfo const&, BoundsBase<float> const&, unsigned int, bool);
            float GetTabButtonSpace() const;
            int Create();
            Pane* GetPane(CStr const&);
            int GetCornerSz() const;
            int SetSchema(CStr const&);
            void AddButtonFlatAxialPane(DrawInfo const&, BoundsBase<float> const&, unsigned int, bool);
            void AddFlatAxialPane0(DrawInfo const&, BoundsBase<float> const&, unsigned int, int, CStr const&, PaneFlagBg);
            void AddText(DrawInfo const&, PointBase<float> const&, CStr const&, int, TextWrapFlags, TextFormatFlags);
            void AddImagedRect(DrawInfo const&, BoundsBase<float> const&, unsigned int, rend::TexHandle);
            int Done();
            rend::TexHandle GetGlyph(CStr const&);
            GfxServer();
            int PlayControlSound(CStr const&, bool*);
            int GetBtnHeight();
            int GetFontId(CStr const&, float, FontType, union FontParams) const;
            int GetBtnWidth();
            void AddFlatAxialQuad(DrawInfo const&, BoundsBase<float> const&, unsigned int);
            int SetFont(int&);
            int SetFont(Font*);
            int SetFont(CStr const&, float, FontType, union FontParams);
            void AddImagedRectGeneral(DrawInfo const&, BoundsBase<float> const&, unsigned int, rend::TexHandle, float, float, float, float);
            void ClearFonts();
            float GetTabButtonMinWidth() const;
            Font* GetFontById(unsigned int) const;

        protected:
            int LoadSoundsFromXml(cmn::XmlFile*, cmn::XmlNode const*);
            void ReleaseSchema();
            void ClearSounds();
            void AddFlatAxialQuad(DrawInfo const&, BoundsBase<float> const&, unsigned int, float, float, float, float);

        private:
            void FlushWindow(Wnd*);

        public:
            int m_hieroglyphicFontId;
            rend::TexHandle m_curFontTexture;

        private:
            unsigned int m_colors[255];
            rend::TexHandle m_texTheme[14];
            retruxx::vector<Pane> m_paneTexTheme;
            retruxx::vector<BackGround*> m_backgrounds;
            retruxx::vector<Frame*> m_frames;
            CStrHash<Pane*> m_panes;
            retruxx::vector<Pane*> m_panesVector;
            retruxx::vector<ScrollPane*> m_scrollPanes;
            retruxx::map<CStr, rend::TexHandle> m_glyphButtonTextures;
            bool m_created = false;
            rend::TexHandle m_texBumpmap;
            FontManager* m_fontManager = nullptr;
            Font* m_curFont = nullptr;
            CStr m_fontFaces[4];
            unsigned int m_fontSizes[4];
            unsigned int m_fontFlags[4];
            FontType m_fontTypes[4];
            int m_barWidth;
            int m_cornerSz;
            int m_lineWidth;
            int m_btnWidth;
            int m_btnHeight;
            int m_checkSize;
            int m_sliderHeight;
            float m_sliderNotchWidthRatio;
            float m_glyphHeight;
            float m_tabButtonMinWidth;
            float m_tabButtonMaxWidth;
            float m_tabButtonHeight;
            float m_tabButtonSpace;
            retruxx::map<CStr, SoundInfo*> m_controlSoundInfos;
        };
    }
}

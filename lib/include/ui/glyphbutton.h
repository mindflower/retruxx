#pragma once
#include "button.h"

namespace m3d
{
    namespace ui
    {
        class GlyphButton : public ButtonWnd
        {
        protected:
            GlyphButton();
            GlyphButton(m3d::ui::GlyphButton const& rhs);

        public:
            virtual ~GlyphButton() override /* 0x00 */;
            virtual Object* Clone() override /* 0x00 */;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const override /* 0x00 */;
            RT_CLASS_DECLARE(GlyphButton);

            enum GlyphButtonType
            {
                GBTYPE_OK = 0,
                GBTYPE_CANCEL = 1,
                GBTYPE_YES = 2,
                GBTYPE_NO = 3,
                GBTYPE_USER = 4,
            };

            virtual int CreateGlyphButton(BoundsBase<float> const& rc, GlyphButtonType type) /* 0x124 */;
            virtual int CreateGlyphButton(unsigned int style, BoundsBase<float> const& rc, GlyphButtonType type, CStr const& caption, unsigned int id, rend::TexHandle glyph, rend::TexHandle glyphDisabled) /* 0x124 */;
            virtual int CreateGlyphButton(unsigned int style, BoundsBase<float> const& rc, GlyphButtonType type, CStr const& caption, unsigned int id, CStr glyphFile, CStr glyphFileDisabled) /* 0x124 */;
            float GetGlyphHeight();
            void SetGlyphHeight(float height);
            void SetGlyphTexture(rend::TexHandle tex, rend::TexHandle texDisabled);
            int SetGlyphTexture(CStr const& fileName, CStr const& fileNameDisabled);
            void AdjustBoundsToFit(float maxW);
            rend::TexHandle GetGlyphTexture() const;
            rend::TexHandle GetGlyphTextureDisabled() const;

        protected:
            virtual int OnPaint(DrawInfo const& di) override /* 0x00 */;

            /* 0x023c */ rend::TexHandle m_glyphTexture;
            /* 0x0240 */ rend::TexHandle m_glyphTextureDisabled;
            /* 0x0244 */ float m_glyphHeight = 0.0f;
            /* 0x0248 */ float m_space = 0.0f;
        }; /* size: 0x024c */
    }
}

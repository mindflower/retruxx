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
            virtual ~GlyphButton() override;
            virtual Object* Clone() override;
            static Object* CreateObject();
            static Class* GetBaseClass();
            virtual Class* GetClass() const override;
            RT_CLASS_DECLARE(GlyphButton);

            enum GlyphButtonType
            {
                GBTYPE_OK = 0,
                GBTYPE_CANCEL = 1,
                GBTYPE_YES = 2,
                GBTYPE_NO = 3,
                GBTYPE_USER = 4,
            };

            virtual int CreateGlyphButton(BoundsBase<float> const& rc, GlyphButtonType type);
            virtual int CreateGlyphButton(
                unsigned int style,
                BoundsBase<float> const& rc,
                GlyphButtonType type,
                CStr const& caption,
                unsigned int id,
                rend::TexHandle glyph,
                rend::TexHandle glyphDisabled);
            virtual int CreateGlyphButton(
                unsigned int style,
                BoundsBase<float> const& rc,
                GlyphButtonType type,
                CStr const& caption,
                unsigned int id,
                CStr glyphFile,
                CStr glyphFileDisabled);
            float GetGlyphHeight();
            void SetGlyphHeight(float height);
            void SetGlyphTexture(rend::TexHandle tex, rend::TexHandle texDisabled);
            int SetGlyphTexture(CStr const& fileName, CStr const& fileNameDisabled);
            void AdjustBoundsToFit(float maxW);
            rend::TexHandle GetGlyphTexture() const;
            rend::TexHandle GetGlyphTextureDisabled() const;

        protected:
            virtual int OnPaint(DrawInfo const& di) override;

            rend::TexHandle m_glyphTexture;
            rend::TexHandle m_glyphTextureDisabled;
            float m_glyphHeight = 0.0f;
            float m_space = 0.0f;
        };
    }  // namespace ui
}  // namespace m3d

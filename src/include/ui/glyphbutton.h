#pragma once
#include "button.h"

namespace m3d
{
    namespace ui
    {
        class GlyphButton : public ButtonWnd
        {
        public:
            enum GlyphButtonType
            {
                GBTYPE_OK = 0x0,
                GBTYPE_CANCEL = 0x1,
                GBTYPE_YES = 0x2,
                GBTYPE_NO = 0x3,
                GBTYPE_USER = 0x4,
            };

        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            void SetGlyphHeight(float);
            virtual Class* GetClass() const;
            float GetGlyphHeight();
            void AdjustBoundsToFit(float);
            int SetGlyphTexture(CStr const&, CStr const&);
            void SetGlyphTexture(rend::TexHandle, rend::TexHandle);
            virtual ~GlyphButton();
            rend::TexHandle GetGlyphTexture() const;
            rend::TexHandle GetGlyphTextureDisabled() const;
            virtual int CreateGlyphButton(BoundsBase<float> const&, GlyphButtonType);
            virtual int CreateGlyphButton(unsigned int, BoundsBase<float> const&, GlyphButtonType, CStr const&, unsigned int, CStr, CStr);
            virtual int CreateGlyphButton(unsigned int, BoundsBase<float> const&, GlyphButtonType, CStr const&, unsigned int, rend::TexHandle, rend::TexHandle);
            virtual Object* Clone();

        protected:
            virtual int OnPaint(DrawInfo const&);
            GlyphButton(GlyphButton const&);
            GlyphButton();

        private:
            rend::TexHandle m_glyphTexture;
            rend::TexHandle m_glyphTextureDisabled;
            float m_glyphHeight;
            float m_space;
        };
    }
}

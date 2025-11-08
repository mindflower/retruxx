#include <ui/glyphbutton.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(GlyphButton)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(GlyphButton);

        Object* GlyphButton::CreateObject()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* GlyphButton::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ButtonWnd);
        }

        void GlyphButton::SetGlyphHeight(float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* GlyphButton::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        float GlyphButton::GetGlyphHeight()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void GlyphButton::AdjustBoundsToFit(float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int GlyphButton::SetGlyphTexture(CStr const&, CStr const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void GlyphButton::SetGlyphTexture(rend::TexHandle, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        GlyphButton::~GlyphButton()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        rend::TexHandle GlyphButton::GetGlyphTexture() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        rend::TexHandle GlyphButton::GetGlyphTextureDisabled() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int GlyphButton::CreateGlyphButton(BoundsBase<float> const&, GlyphButtonType)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int GlyphButton::CreateGlyphButton(unsigned, BoundsBase<float> const&, GlyphButtonType, CStr const&, unsigned, CStr, CStr)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int GlyphButton::CreateGlyphButton(unsigned, BoundsBase<float> const&, GlyphButtonType, CStr const&, unsigned, rend::TexHandle, rend::TexHandle)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* GlyphButton::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int GlyphButton::OnPaint(DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        GlyphButton::GlyphButton(GlyphButton const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        GlyphButton::GlyphButton()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

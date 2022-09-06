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
            throw std::logic_error("Not implemented");
        }

        Class* GlyphButton::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ButtonWnd);
        }

        void GlyphButton::SetGlyphHeight(float)
        {
            throw std::logic_error("Not implemented");
        }

        Class* GlyphButton::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        float GlyphButton::GetGlyphHeight()
        {
            throw std::logic_error("Not implemented");
        }

        void GlyphButton::AdjustBoundsToFit(float)
        {
            throw std::logic_error("Not implemented");
        }

        int GlyphButton::SetGlyphTexture(CStr const&, CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        void GlyphButton::SetGlyphTexture(rend::TexHandle, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        GlyphButton::~GlyphButton()
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle GlyphButton::GetGlyphTexture() const
        {
            throw std::logic_error("Not implemented");
        }

        rend::TexHandle GlyphButton::GetGlyphTextureDisabled() const
        {
            throw std::logic_error("Not implemented");
        }

        int GlyphButton::CreateGlyphButton(BoundsBase<float> const&, GlyphButtonType)
        {
            throw std::logic_error("Not implemented");
        }

        int GlyphButton::CreateGlyphButton(unsigned, BoundsBase<float> const&, GlyphButtonType, CStr const&, unsigned, CStr, CStr)
        {
            throw std::logic_error("Not implemented");
        }

        int GlyphButton::CreateGlyphButton(unsigned, BoundsBase<float> const&, GlyphButtonType, CStr const&, unsigned, rend::TexHandle, rend::TexHandle)
        {
            throw std::logic_error("Not implemented");
        }

        Object* GlyphButton::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int GlyphButton::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        GlyphButton::GlyphButton(GlyphButton const&)
        {
            throw std::logic_error("Not implemented");
        }

        GlyphButton::GlyphButton()
        {
            throw std::logic_error("Not implemented");
        }
    }
}

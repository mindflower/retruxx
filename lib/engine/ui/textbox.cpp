#include <ui/textbox.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(TextBoxWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(TextBoxWnd);

        Class* TextBoxWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(FormattedStringsListBoxWnd);
        }

        Object* TextBoxWnd::CreateObject()
        {
            return new TextBoxWnd;
        }

        int TextBoxWnd::SetText(CStr const& caption)
        {
            auto res = Wnd::SetText(caption);
            RecalcLayout();
            return res;
        }

        CStr TextBoxWnd::GetText() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* TextBoxWnd::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TextBoxWnd::SetVScrollToPos(unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* TextBoxWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(TextBoxWnd);
        }

        int TextBoxWnd::Create(CStr const& capt, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            if (!ListBoxWnd<m3d::ui::FormattedLine>::Create(rc, style, id))
                return 0;
            m_drawFlags |= 5u;
            RecalcLayout();
            SetText(capt);
            return 1;
        }

        int TextBoxWnd::Create(BoundsBase<float> const& rc, unsigned flags, int id)
        {
            auto result = ListBoxWnd<m3d::ui::FormattedLine>::Create(rc, flags, id);
            if (result)
            {
                m_drawFlags |= 5u;
                RecalcLayout();
                return 1;
            }
            return result;
        }

        TextBoxWnd::~TextBoxWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TextBoxWnd::RecalcLayout()
        {
            // TODO: implement TextBoxWnd::RecalcLayout
            //RETRUXX_NOT_IMPLEMENTED;
        }

        int TextBoxWnd::OnPaint(DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TextBoxWnd::TextBoxWnd(TextBoxWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TextBoxWnd::TextBoxWnd()
        {
            m_textFormat = TF_FULL;
        }
    }
}

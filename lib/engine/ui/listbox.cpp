#include <ui/listbox.h>
#include <core/clazz.h>
#include <cstring>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(StringsListBoxWnd)
        RT_CLASS_EXPORTS_END;

        RT_CLASS_DEFINE(StringsListBoxWnd);

        RT_CLASS_EXPORTS_BEGIN(FormattedStringsListBoxWnd)
        RT_CLASS_EXPORTS_END;

        RT_CLASS_DEFINE(FormattedStringsListBoxWnd);

        Class* StringsListBoxWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Object* StringsListBoxWnd::CreateObject()
        {
            return new StringsListBoxWnd;
        }

        Object* StringsListBoxWnd::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        StringsListBoxWnd::~StringsListBoxWnd()
        {
        }

        Class* StringsListBoxWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(StringsListBoxWnd);
        }

        int StringsListBoxWnd::RenderItem(int, PointBase<float> const&, DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int StringsListBoxWnd::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
        {
            auto point = GetGfxServer()->MeasureText(m_items[itemIdx].m_item, m_defFont, TW_NOWRAP, 10000.0);
            //TODO: check this!!!
            bounds.x0 = 0.0;
            bounds.y0 = 0.0;
            bounds.width = point.y;
            bounds.height = point.x;
            return 1;
        }

        int StringsListBoxWnd::DeleteItem(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        StringsListBoxWnd::StringsListBoxWnd()
        {
            m_drawFlags = 0;
            m_curSel = -1;
            m_clientEdges[2] = 2.0;
            m_defFont = 1;
        }

        StringsListBoxWnd::StringsListBoxWnd(StringsListBoxWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int StringsListBoxWnd::CompareItem(int, int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* FormattedStringsListBoxWnd::CreateObject()
        {
            return new FormattedStringsListBoxWnd;
        }

        Class* FormattedStringsListBoxWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        FormattedStringsListBoxWnd::~FormattedStringsListBoxWnd() = default;

        Class* FormattedStringsListBoxWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(FormattedStringsListBoxWnd);
        }

        void FormattedStringsListBoxWnd::SetTextColor(unsigned textColor)
        {
            Wnd::SetTextColor(textColor);
            for (auto& item : m_items)
            {
                item.m_item.m_color = textColor;
            }
        }

        Object* FormattedStringsListBoxWnd::Clone()
        {
            return new FormattedStringsListBoxWnd(*this);
        }

        void FormattedStringsListBoxWnd::SetFormatMode(TextFormatFlags format)
        {
            m_textFormat = format;
            for (auto& item : m_items)
            {
                item.m_item.m_format = format;
            }
        }

        int FormattedStringsListBoxWnd::CompareItem(int itemIdx0, int itemIdx1)
        {
            return strcmp(m_items[itemIdx0].m_item.m_text.c_str(), m_items[itemIdx1].m_item.m_text.c_str());
        }

        int FormattedStringsListBoxWnd::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
        {
            auto const point =
                GetGfxServer()->MeasureText(m_items[itemIdx].m_item.m_text, m_defFont, TW_NOWRAP, 10000.0);
            bounds.x0 = 0.0f;
            bounds.y0 = 0.0f;
            bounds.width = point.y;
            bounds.height = point.x;
            return 1;
        }

        int FormattedStringsListBoxWnd::DeleteItem(int)
        {
            return 1;
        }

        int FormattedStringsListBoxWnd::RenderItem(int itemIdx, PointBase<float> const& pt, DrawInfo const& di)
        {
            FormattedLine const& line = m_items[itemIdx].m_item;

            float const width = di.m_clientRect.width;

            // Measured for side effects only, matching the original (the result is not used further).
            GetGfxServer()->MeasureText(line.m_text, m_defFont, m_textWrap, width);

            PointBase<float> origin = pt;
            if (line.m_format == TF_CENTER)
            {
                float x = width * 0.5f;
                if (x < 0.0f)
                {
                    x = 0.0f;
                }
                origin.x = x > width ? width : x;
            }
            else if (line.m_format == TF_RIGHT)
            {
                origin.x = width;
            }

            CStr const text = CStr::format_("@%x", line.m_color) + line.m_text;
            GetGfxServer()->AddText(di, origin, text, m_defFont, TW_NOWRAP, line.m_format);
            return 1;
        }

        FormattedStringsListBoxWnd::FormattedStringsListBoxWnd()
        {
            this->m_drawFlags = 0;
            this->m_curSel = -1;
            this->m_clientEdges[2] = 2.0;
            this->m_defFont = 1;
        }

        FormattedStringsListBoxWnd::FormattedStringsListBoxWnd(FormattedStringsListBoxWnd const& w) :
            ListBoxWnd<FormattedLine>(w)
        {
        }
    }  // namespace ui
}  // namespace m3d

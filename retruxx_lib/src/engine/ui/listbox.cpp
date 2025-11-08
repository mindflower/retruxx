#include <ui/listbox.h>
#include <core/clazz.h>

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
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* FormattedStringsListBoxWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        FormattedStringsListBoxWnd::~FormattedStringsListBoxWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* FormattedStringsListBoxWnd::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
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
            RETRUXX_NOT_IMPLEMENTED;
        }

        void FormattedStringsListBoxWnd::SetFormatMode(TextFormatFlags)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FormattedStringsListBoxWnd::CompareItem(int, int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FormattedStringsListBoxWnd::MeasureItem(int, BoundsBase<float>&) const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FormattedStringsListBoxWnd::DeleteItem(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int FormattedStringsListBoxWnd::RenderItem(int, PointBase<float> const&, DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        FormattedStringsListBoxWnd::FormattedStringsListBoxWnd()
        {
            this->m_drawFlags = 0;
            this->m_curSel = -1;
            this->m_clientEdges[2] = 2.0;
            this->m_defFont = 1;
        }

        FormattedStringsListBoxWnd::FormattedStringsListBoxWnd(FormattedStringsListBoxWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

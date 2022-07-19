#include <ui/listbox.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_DEFINE(StringsListBoxWnd);
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
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }

        int StringsListBoxWnd::MeasureItem(int, BoundsBase<float>&) const
        {
            throw std::logic_error("Not implemented");
        }

        int StringsListBoxWnd::DeleteItem(int)
        {
            throw std::logic_error("Not implemented");
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
            throw std::logic_error("Not implemented");
        }

        int StringsListBoxWnd::CompareItem(int, int)
        {
            throw std::logic_error("Not implemented");
        }

        Object* FormattedStringsListBoxWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        Class* FormattedStringsListBoxWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        FormattedStringsListBoxWnd::~FormattedStringsListBoxWnd()
        {
            throw std::logic_error("Not implemented");
        }

        Class* FormattedStringsListBoxWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        void FormattedStringsListBoxWnd::SetTextColor(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Object* FormattedStringsListBoxWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        void FormattedStringsListBoxWnd::SetFormatMode(TextFormatFlags)
        {
            throw std::logic_error("Not implemented");
        }

        int FormattedStringsListBoxWnd::CompareItem(int, int)
        {
            throw std::logic_error("Not implemented");
        }

        int FormattedStringsListBoxWnd::MeasureItem(int, BoundsBase<float>&) const
        {
            throw std::logic_error("Not implemented");
        }

        int FormattedStringsListBoxWnd::DeleteItem(int)
        {
            throw std::logic_error("Not implemented");
        }

        int FormattedStringsListBoxWnd::RenderItem(int, PointBase<float> const&, DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        FormattedStringsListBoxWnd::FormattedStringsListBoxWnd()
        {
            throw std::logic_error("Not implemented");
        }

        FormattedStringsListBoxWnd::FormattedStringsListBoxWnd(FormattedStringsListBoxWnd const&)
        {
            throw std::logic_error("Not implemented");
        }
    }
}

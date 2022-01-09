#include <ui/tabwnd.h>

namespace m3d
{
    namespace ui
    {
        TabButtonInfo::TabButtonInfo()
        {
            throw std::logic_error("Not implemented");
        }

        void TabButtonInfo::SetIzvrat(float, float, float, Alignment)
        {
            throw std::logic_error("Not implemented");
        }

        void TabButtonInfo::SetNormal(float, float, float, float, Alignment, float)
        {
            throw std::logic_error("Not implemented");
        }

        TabWnd::AuxInfo::AuxInfo()
        {
            throw std::logic_error("Not implemented");
        }

        Object* TabWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::SetCurSel(int)
        {
            throw std::logic_error("Not implemented");
        }

        TabWnd::~TabWnd()
        {
            throw std::logic_error("Not implemented");
        }

        Class* TabWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::AddItem(ref_ptr<Wnd>, TabItemInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        TabItem const* TabWnd::GetItem(int) const
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::GetCurSel() const
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        TabButtonInfo const& TabWnd::GetTabButtonInfo() const
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::RemoveAllItems()
        {
            throw std::logic_error("Not implemented");
        }

        Class* TabWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::SetClientEdges(std::vector<float, std::allocator<float>> const&)
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::SetClientEdges(float, float, float, float)
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::SetTabButtonInfo(TabButtonInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        Object* TabWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::GetItemsCount() const
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::AdjustWorkWindowRect(Wnd*, TabItemInfo const&) const
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::_SetCurSel(int, SelectionType)
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> TabWnd::RecalcButtonSize(int)
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::DrawWndText(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        std::vector<float, std::allocator<float>> TabWnd::_GetClientEdges() const
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::OnNcPaint(DrawInfo const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::OnWndNotify(Wnd*, unsigned, unsigned, AIParam const&)
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::OnAfterChangeCurSel(int, SelectionType)
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::AddButton(int)
        {
            throw std::logic_error("Not implemented");
        }

        bool TabWnd::IsItemIdValid(int) const
        {
            throw std::logic_error("Not implemented");
        }

        PointBase<float> TabWnd::RecalcButtonPos(int)
        {
            throw std::logic_error("Not implemented");
        }

        int TabWnd::OnKey(unsigned short, unsigned char, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        void TabWnd::OnBeforeChangeCurSel(int, SelectionType)
        {
            throw std::logic_error("Not implemented");
        }

        TabWnd::TabWnd()
        {
            throw std::logic_error("Not implemented");
        }

        TabWnd::TabWnd(TabWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        float TabWnd::_GetTopEdgeDiff() const
        {
            throw std::logic_error("Not implemented");
        }
    }
}

#include <ui/tabwnd.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(TabWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(TabWnd);

        TabButtonInfo::TabButtonInfo()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabButtonInfo::SetIzvrat(float, float, float, Alignment)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabButtonInfo::SetNormal(float, float, float, float, Alignment, float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TabWnd::AuxInfo::AuxInfo()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* TabWnd::CreateObject()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::SetCurSel(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TabWnd::~TabWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* TabWnd::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::AddItem(ref_ptr<Wnd>, TabItemInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TabItem const* TabWnd::GetItem(int) const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::GetCurSel() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TabButtonInfo const& TabWnd::GetTabButtonInfo() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::RemoveAllItems()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* TabWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        void TabWnd::SetClientEdges(std::vector<float, std::allocator<float>> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::SetClientEdges(float, float, float, float)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::SetTabButtonInfo(TabButtonInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* TabWnd::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::GetItemsCount() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::AdjustWorkWindowRect(Wnd*, TabItemInfo const&) const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::_SetCurSel(int, SelectionType)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        PointBase<float> TabWnd::RecalcButtonSize(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::DrawWndText(DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        std::vector<float, std::allocator<float>> TabWnd::_GetClientEdges() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::OnNcPaint(DrawInfo const&, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::OnWndNotify(Wnd*, unsigned, unsigned, AIParam const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::OnAfterChangeCurSel(int, SelectionType)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::AddButton(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        bool TabWnd::IsItemIdValid(int) const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        PointBase<float> TabWnd::RecalcButtonPos(int)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int TabWnd::OnKey(unsigned short, unsigned char, unsigned)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::OnBeforeChangeCurSel(int, SelectionType)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TabWnd::TabWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        TabWnd::TabWnd(TabWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        float TabWnd::_GetTopEdgeDiff() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

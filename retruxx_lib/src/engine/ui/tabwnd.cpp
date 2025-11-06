#include <ui/tabwnd.h>

#include "ui/ui_srv.h"

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(TabWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(TabWnd);

        TabButtonInfo::TabButtonInfo()
        {
            m_minWidth = 0.0;
            m_maxWidth = 0.0;
            m_height = 0.0;
            m_spaceBetweenButtons = 0.0;
            m_glyphSz = 0.0;
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
            m_dizSpace = 5.0;
            m_clientEdge = 10.0;
            m_btnClientEdge = 5.0;
        }

        Object* TabWnd::CreateObject()
        {
            return new TabWnd;
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
            return RT_CLASS_LOCAL(TabWnd);
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
            // TODO: implement TabWnd::Create
            // RETRUXX_NOT_IMPLEMENTED;
            return 1;
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
            // TODO: implement TabWnd::SetClientEdges
            // RETRUXX_NOT_IMPLEMENTED;
        }

        void TabWnd::SetTabButtonInfo(TabButtonInfo const&)
        {
            // TODO: implement TabWnd::SetTabButtonInfo
            // RETRUXX_NOT_IMPLEMENTED;
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
            this->m_curSel = -1;

            TabButtonInfo info;
            info.m_minWidth = m3d::ui::Wnd::m_gfx->GetTabButtonMinWidth();
            info.m_maxWidth = m3d::ui::Wnd::m_gfx->GetTabButtonMaxWidth();
            info.m_height = m3d::ui::Wnd::m_gfx->GetTabButtonHeight();
            info.m_spaceBetweenButtons = m3d::ui::Wnd::m_gfx->GetTabButtonSpace();
            info.m_drawStyle = TabButtonInfo::DRAWSTYLE_NORMAL;
            info.m_alignment = TabButtonInfo::ALIGNMENT_LEFT;
            info.m_glyphSz = m_gfx->GetGlyphHeight();
            SetTabButtonInfo(info);
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
